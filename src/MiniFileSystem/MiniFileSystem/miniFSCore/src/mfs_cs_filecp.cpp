/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#pragma warning (disable:4996)
#include "../include/mfs_coresystem.h"
#include "../include/mfs_algorithm.h"
#include <io.h>
#include <direct.h>


/// <summary> 复制文件 </summary>
/// <return> -1:路径不存在; 1:复制成功 </return>
int MiniFileSystem::copyFile(const char filename_1[], const char filename_2[])
{
	int mode = 0;
	struct stat buf;
	std::vector<std::string> path_1, path_2;
	Directory* current_directory = &readDirectory(mbr.RDF_entrance);

	// 判断复制方式，mode=1由Windows到MiniFileSystem；mode=2由MiniFileSystem到Windows；mode=3由MiniFileSystem到MiniFileSystem
	if (filename_1[1] == ':'&&filename_2[1] != ':')
		mode = 1;
	else if (filename_2[1] == ':'&&filename_1[1] != ':')
		mode = 2;
	else if (filename_1[1] != ':'&&filename_2[1] != ':')
		mode = 3;
	else
		return -1;

	// 分割路径
	MfsAlg::cutPath(filename_1, path_1);
	MfsAlg::cutPath(filename_2, path_2);

	if (mode == 1) {
		// 判断路径1是否合法
		stat(filename_1, &buf);
		if (buf.st_mode == 0)
			return -1;

		// 判断路径2是否合法
		if (path_2[0] == "")
			path_2.erase(path_2.begin());
		for (int i = 1; i < (int)path_2.size(); i++) {
			uint_32 file_num = current_directory->header.file_num;
			bool find = 0;
			for (uint_32 j = 0; j < current_directory->header.file_num; j++) {
				if (current_directory->fcb[j].name == path_2[i] && current_directory->fcb[j].delete_flag == 0 && current_directory->fcb[j].mark == 0) {
					current_directory = &readDirectory(current_directory->fcb[j].data_entrance);
					find = 1;
					break;
				}
			}
			if (find == 0)
				return -1;
		}

		// 判断目标位置是否在当前路径中
		bool in_path = false;
		for (uint_32 i = 0; i < directory.size(); i++) {
			if (current_directory->header.current_dir_entrance == directory[i].header.current_dir_entrance) {
				current_directory = &directory[i];
				in_path = true;
				break;
			}
		}

		// 复制文件
		if ((S_IFDIR & buf.st_mode) != S_IFDIR) {
			const char* name = path_1[path_1.size() - 1].data();

			// 判断是否有同名文件
			for (uint_32 i = 0; i < current_directory->header.file_num; i++) {
				if (strcmp(name, current_directory->fcb[i].name) == 0 && current_directory->fcb[i].delete_flag == 0 && current_directory->fcb[i].mark == 1)
					return 1;
			}

			// 更新目录文件头
			current_directory->header.modify_time = time(NULL);
			current_directory->header.file_num++;
			current_directory->header.occupy_cluster_num = current_directory->header.file_num / 1024 + 1;
			current_directory->header.folder_size += buf.st_size;

			// 新添目录项信息
			uint_32 file_num = current_directory->header.file_num;
			current_directory->fcb = (FCB *)realloc(current_directory->fcb, file_num * sizeof(FCB));
			strcpy(current_directory->fcb[file_num - 1].name, name);
			current_directory->fcb[file_num - 1].mark = 1;
			current_directory->fcb[file_num - 1].attribute = 0;
			if ((S_IREAD&buf.st_mode) == S_IREAD)
				current_directory->fcb[file_num - 1].attribute |= ATT_READ;
			if ((S_IWRITE&buf.st_mode) == S_IWRITE)
				current_directory->fcb[file_num - 1].attribute |= ATT_ARCHIVE;
			current_directory->fcb[file_num - 1].delete_flag = 0;
			current_directory->fcb[file_num - 1].occupy_cluster_num = (uint_32)ceil(buf.st_size / mbr.cluster_size);
			mbr.free_cluster_num -= current_directory->fcb[file_num - 1].occupy_cluster_num;
			current_directory->fcb[file_num - 1].file_size = buf.st_size;
			current_directory->fcb[file_num - 1].create_time = buf.st_atime;
			current_directory->fcb[file_num - 1].modify_time = buf.st_mtime;
			uint_32 empty_room = MfsAlg::BitFindRoom(CAB, mbr.cluster_num);
			MfsAlg::BitSet(CAB, mbr.cluster_num, empty_room);
			current_directory->fcb[file_num - 1].data_entrance = empty_room;
			rewriteDirectory(*current_directory);
			if (in_path == false)
				free(current_directory->fcb);

			// 将文件数据写入系统
			FILE* fp = fopen(filename_1, "rb");
			uint_32 cur_cluster = empty_room;
			uint_64 file_size = buf.st_size;
			while (file_size > 0) {
				if (file_size <= mbr.cluster_size * 1024) {
					fread(buffer, (long)file_size, 1, fp);
					writeCluster(cur_cluster);
					file_size = 0;
					break;
				}
				fread(buffer, mbr.cluster_size * 1024, 1, fp);
				writeCluster(cur_cluster);
				file_size -= mbr.cluster_size * 1024;
				uint_32 next_room = MfsAlg::BitFindRoom(CAB, mbr.cluster_num);
				MfsAlg::BitSet(CAB, mbr.cluster_num, next_room);
				FAT[cur_cluster] = next_room;
				cur_cluster = next_room;
			}
			FAT[cur_cluster] = ECOF;

			// 向上到根更新目录信息
			Directory* parent_directory = &readDirectory(current_directory->header.parent_dir_entrance);
			Directory son_directory = readDirectory(current_directory->header.current_dir_entrance);
			while (son_directory.header.current_dir_entrance != mbr.RDF_entrance) {
				for (uint_32 i = 0; i < directory.size(); i++) {
					if (parent_directory->header.current_dir_entrance == directory[i].header.current_dir_entrance) {
						parent_directory = &directory[i];
						break;
					}
				}
				for (uint_32 i = 0; i < parent_directory->header.file_num; i++) {
					if (parent_directory->fcb[i].data_entrance == son_directory.header.current_dir_entrance) {
						parent_directory->header.folder_size -= parent_directory->fcb[i].file_size;
						parent_directory->fcb[i].file_size = son_directory.header.folder_size;
						parent_directory->header.folder_size += parent_directory->fcb[i].file_size;
						break;
					}
				}
				rewriteDirectory(*parent_directory);
				if (parent_directory->header.current_dir_entrance == mbr.RDF_entrance)
					break;
				son_directory = *parent_directory;
				parent_directory = &readDirectory(parent_directory->header.parent_dir_entrance);
			}
		}
		// 复制文件夹
		else if ((S_IFDIR & buf.st_mode) == S_IFDIR) {
			const char* name = path_1[path_1.size() - 1].data();

			// 判断是否有同名文件夹
			for (uint_32 i = 0; i < current_directory->header.file_num; i++) {
				if (strcmp(name, current_directory->fcb[i].name) == 0 && current_directory->fcb[i].delete_flag == 0 && current_directory->fcb[i].mark == 0)
					return 1;
			}

			// 更新目录文件头
			current_directory->header.modify_time = time(NULL);
			current_directory->header.file_num++;
			current_directory->header.occupy_cluster_num = current_directory->header.file_num / 1024 + 1;

			// 新添目录项信息
			uint_32 file_num = current_directory->header.file_num;
			current_directory->fcb = (FCB *)realloc(current_directory->fcb, file_num * sizeof(FCB));
			strcpy(current_directory->fcb[file_num - 1].name, name);
			current_directory->fcb[file_num - 1].mark = 0;
			current_directory->fcb[file_num - 1].attribute = ATT_ARCHIVE;
			current_directory->fcb[file_num - 1].delete_flag = 0;
			current_directory->fcb[file_num - 1].occupy_cluster_num = (uint_32)ceil(buf.st_size / mbr.cluster_size);
			current_directory->fcb[file_num - 1].file_size = 0;
			current_directory->fcb[file_num - 1].create_time = buf.st_atime;
			current_directory->fcb[file_num - 1].modify_time = buf.st_mtime;
			uint_32 empty_room = MfsAlg::BitFindRoom(CAB, mbr.cluster_num);
			MfsAlg::BitSet(CAB, mbr.cluster_num, empty_room);
			current_directory->fcb[file_num - 1].data_entrance = empty_room;
			FAT[empty_room] = ECOF;
			rewriteDirectory(*current_directory);

			// 新建目录文件头
			Directory dir_directory;
			strcpy(dir_directory.header.name, name);
			dir_directory.header.occupy_cluster_num = 1;
			dir_directory.header.current_dir_entrance = empty_room;
			dir_directory.header.parent_dir_entrance = current_directory->header.current_dir_entrance;
			dir_directory.header.file_num = 0;
			dir_directory.header.create_time = current_directory->header.modify_time;
			dir_directory.header.modify_time = current_directory->header.modify_time;
			dir_directory.header.folder_size = 0;
			dir_directory.fcb = (FCB *)calloc(1, sizeof(FCB));
			newWriteDirectory(dir_directory);
			free(dir_directory.fcb);

			// 递归复制文件夹下目录
			char next_path[200];
			char next_fspath[200];
			strcpy(next_path, filename_1);
			if (next_path[strlen(next_path) - 1] != '\\')
				strcat(next_path, "\\");
			strcat(next_path, "*.*");
			intptr_t handle;
			_finddata_t find_data;
			handle = _findfirst(next_path, &find_data);
			if (handle == -1)
				return 1;
			do {
				if ((find_data.attrib & _A_SUBDIR) == _A_SUBDIR && strcmp(find_data.name, ".") == 0 || strcmp(find_data.name, "..") == 0)
					continue;
				else {
					strcpy(next_fspath, filename_2);
					if (next_fspath[strlen(next_fspath) - 1] != '\\')
						strcat(next_fspath, "\\");
					strcat(next_fspath, name);
					strcpy(next_path, filename_1);
					if (next_path[strlen(next_path) - 1] != '\\')
						strcat(next_path, "\\");
					strcat(next_path, find_data.name);
					copyFile(next_path, next_fspath);
				}
			} while (_findnext(handle, &find_data) == 0);
		}
	}
	else if (mode == 2) {
		// 判断路径2是否合法
		stat(filename_2, &buf);
		if (buf.st_mode == 0 || ((S_IFDIR & buf.st_mode) != S_IFDIR))
			return -1;

		// 判断路径1是否合法
		if (path_1[0] == "")
			path_1.erase(path_1.begin());
		for (int i = 1; i < (int)path_1.size() - 1; i++) {
			uint_32 file_num = current_directory->header.file_num;
			bool find = 0;
			for (uint_32 j = 0; j < current_directory->header.file_num; j++) {
				if (current_directory->fcb[j].name == path_1[i] && current_directory->fcb[j].delete_flag == 0 && current_directory->fcb[j].mark == 0) {
					current_directory = &readDirectory(current_directory->fcb[j].data_entrance);
					find = 1;
					break;
				}
			}
			if (find == 0)
				return -1;
		}

		// 复制文件
		uint_32 file = ECOF;
		for (uint_32 i = 0; i < current_directory->header.file_num; i++) {
			if (current_directory->fcb[i].name == path_1[path_1.size() - 1] && current_directory->fcb[i].delete_flag == 0 && current_directory->fcb[i].mark == 1) {
				file = i;
				break;
			}
		}
		if (file != ECOF) {
			uint_32 cur_cluster = current_directory->fcb[file].data_entrance;
			uint_64 file_size = current_directory->fcb[file].file_size;
			char file_name[200];
			strcpy(file_name, filename_2);
			if (file_name[strlen(file_name) - 1] != '\\')
				strcat(file_name, "\\");
			strcat(file_name, current_directory->fcb[file].name);

			FILE * fp = fopen(file_name, "r");
			if (fp != NULL) {
				fclose(fp);
				return -1;
			}

			fp = fopen(file_name, "wb+");
			while (file_size > 0) {
				if (file_size <= mbr.cluster_size * 1024) {
					readCluster(cur_cluster);
					fwrite(buffer, (long)file_size, 1, fp);
					file_size = 0;
					break;
				}
				readCluster(cur_cluster);
				fwrite(buffer, mbr.cluster_size * 1024, 1, fp);
				file_size -= mbr.cluster_size * 1024;
				cur_cluster = FAT[cur_cluster];
			}
			fclose(fp);
		}
		// 递归复制文件夹
		else {
			uint_32 dir = ECOF;
			for (uint_32 i = 0; i < current_directory->header.file_num; i++) {
				if (current_directory->fcb[i].name == path_1[path_1.size() - 1] && current_directory->fcb[i].delete_flag == 0 && current_directory->fcb[i].mark == 0) {
					current_directory = &readDirectory(current_directory->fcb[i].data_entrance);
					dir = i;
					break;
				}
			}
			if (dir == ECOF && current_directory->header.current_dir_entrance != mbr.RDF_entrance)
				return -1;
			char dir_name[200];
			char file_name[200];
			strcpy(dir_name, filename_2);
			if (dir_name[strlen(dir_name) - 1] != '\\')
				strcat(dir_name, "\\");
			strcat(dir_name, current_directory->header.name);
			_mkdir(dir_name);
			strcpy(file_name, filename_1);
			if (file_name[strlen(file_name) - 1] != '\\')
				strcat(file_name, "\\");
			for (uint_32 i = 0; i < current_directory->header.file_num; i++) {
				strcpy(file_name, filename_1);
				if (file_name[strlen(file_name) - 1] != '\\')
					strcat(file_name, "\\");
				strcat(file_name, current_directory->fcb[i].name);
				copyFile(file_name, dir_name);
			}
		}
	}
	else if (mode == 3) {
		// 判断路径是否合法
		Directory* fir_directory = &readDirectory(mbr.RDF_entrance);
		Directory* sec_directory = &readDirectory(mbr.RDF_entrance);
		if (path_1[0] == "")
			path_1.erase(path_1.begin());
		if (path_2[0] == "")
			path_2.erase(path_2.begin());
		for (int i = 1; i < (int)path_1.size() - 1; i++) {
			uint_32 file_num = fir_directory->header.file_num;
			bool find = 0;
			for (uint_32 j = 0; j < fir_directory->header.file_num; j++) {
				if (fir_directory->fcb[j].name == path_1[i] && fir_directory->fcb[j].delete_flag == 0 && fir_directory->fcb[j].mark == 0) {
					fir_directory = &readDirectory(fir_directory->fcb[j].data_entrance);
					find = 1;
					break;
				}
			}
			if (find == 0)
				return -1;
		}
		for (int i = 1; i < (int)path_2.size(); i++) {
			uint_32 file_num = sec_directory->header.file_num;
			bool find = 0;
			for (uint_32 j = 0; j < sec_directory->header.file_num; j++) {
				if (sec_directory->fcb[j].name == path_2[i] && sec_directory->fcb[j].delete_flag == 0 && sec_directory->fcb[j].mark == 0) {
					sec_directory = &readDirectory(sec_directory->fcb[j].data_entrance);
					find = 1;
					break;
				}
			}
			if (find == 0)
				return -1;
		}

		// 判断判断目标位置是否在当前目录中
		bool in_path = false;
		for (uint_32 i = 0; i < directory.size(); i++) {
			if (sec_directory->header.current_dir_entrance == directory[i].header.current_dir_entrance) {
				sec_directory = &directory[i];
				in_path = true;
				break;
			}
		}

		// 复制文件
		uint_32 file = ECOF;
		for (uint_32 i = 0; i < fir_directory->header.file_num; i++) {
			if (fir_directory->fcb[i].name == path_1[path_1.size() - 1] && fir_directory->fcb[i].delete_flag == 0 && fir_directory->fcb[i].mark == 1) {
				file = i;
				break;
			}
		}
		if (file != ECOF) {
			for (uint_32 i = 0; i < sec_directory->header.file_num; i++) {
				if (strcmp(fir_directory->fcb[file].name, sec_directory->fcb[i].name) == 0 && sec_directory->fcb[i].delete_flag == 0 && sec_directory->fcb[i].mark == 1) {
					return -1;
				}
			}
			uint_32 fir_cluster = fir_directory->fcb[file].data_entrance;
			uint_64 file_size = fir_directory->fcb[file].file_size;

			sec_directory->header.modify_time = time(NULL);
			sec_directory->header.file_num++;
			sec_directory->header.occupy_cluster_num = current_directory->header.file_num / 1024 + 1;
			sec_directory->header.folder_size += file_size;

			uint_32 file_num = sec_directory->header.file_num;
			sec_directory->fcb = (FCB *)realloc(sec_directory->fcb, file_num * sizeof(FCB));
			strcpy(sec_directory->fcb[file_num - 1].name, fir_directory->fcb[file].name);
			sec_directory->fcb[file_num - 1].mark = 1;
			sec_directory->fcb[file_num - 1].attribute = fir_directory->fcb[file].attribute;
			sec_directory->fcb[file_num - 1].delete_flag = 0;
			sec_directory->fcb[file_num - 1].occupy_cluster_num = (uint_32)ceil(file_size / mbr.cluster_size);
			mbr.free_cluster_num -= current_directory->fcb[file_num - 1].occupy_cluster_num;
			sec_directory->fcb[file_num - 1].file_size = file_size;
			sec_directory->fcb[file_num - 1].create_time = fir_directory->fcb[file].create_time;
			sec_directory->fcb[file_num - 1].modify_time = fir_directory->fcb[file].modify_time;
			uint_32 empty_room = MfsAlg::BitFindRoom(CAB, mbr.cluster_num);
			MfsAlg::BitSet(CAB, mbr.cluster_num, empty_room);
			sec_directory->fcb[file_num - 1].data_entrance = empty_room;
			rewriteDirectory(*sec_directory);
			if (in_path == false)
				free(sec_directory->fcb);

			uint_32 cur_cluster = empty_room;
			while (file_size > 0) {
				if (file_size <= mbr.cluster_size * 1024) {
					readCluster(fir_cluster);
					writeCluster(cur_cluster);
					file_size = 0;
					break;
				}
				readCluster(fir_cluster);
				writeCluster(cur_cluster);
				file_size -= mbr.cluster_size * 1024;
				uint_32 next_room = MfsAlg::BitFindRoom(CAB, mbr.cluster_num);
				MfsAlg::BitSet(CAB, mbr.cluster_num, next_room);
				FAT[cur_cluster] = next_room;
				cur_cluster = next_room;
				fir_cluster = FAT[fir_cluster];
			}
			FAT[cur_cluster] = ECOF;

			Directory* parent_directory = &readDirectory(sec_directory->header.parent_dir_entrance);
			Directory son_directory = readDirectory(sec_directory->header.current_dir_entrance);
			while (son_directory.header.current_dir_entrance != mbr.RDF_entrance) {
				for (uint_32 i = 0; i < directory.size(); i++) {
					if (parent_directory->header.current_dir_entrance == directory[i].header.current_dir_entrance) {
						parent_directory = &directory[i];
						break;
					}
				}
				for (uint_32 i = 0; i < parent_directory->header.file_num; i++) {
					if (parent_directory->fcb[i].data_entrance == son_directory.header.current_dir_entrance) {
						parent_directory->header.folder_size -= parent_directory->fcb[i].file_size;
						parent_directory->fcb[i].file_size = son_directory.header.folder_size;
						parent_directory->header.folder_size += parent_directory->fcb[i].file_size;
						break;
					}
				}
				rewriteDirectory(*parent_directory);
				if (parent_directory->header.current_dir_entrance == mbr.RDF_entrance)
					break;
				son_directory = *parent_directory;
				parent_directory = &readDirectory(parent_directory->header.parent_dir_entrance);
			}
		}
		// 递归复制文件夹
		else {
			uint_32 dir = ECOF;
			for (uint_32 i = 0; i < fir_directory->header.file_num; i++) {
				if (fir_directory->fcb[i].name == path_1[path_1.size() - 1] && fir_directory->fcb[i].delete_flag == 0 && fir_directory->fcb[i].mark == 0) {
					dir = i;
					break;
				}
			}
			if (dir == ECOF)
				return -1;

			for (uint_32 i = 0; i < sec_directory->header.file_num; i++) {
				if (strcmp(fir_directory->fcb[dir].name, sec_directory->fcb[i].name) == 0 && sec_directory->fcb[i].delete_flag == 0 && sec_directory->fcb[i].mark == 0) {
					return -1;
				}
			}

			uint_64 file_size = fir_directory->fcb[dir].file_size;
			sec_directory->header.modify_time = time(NULL);
			sec_directory->header.file_num++;
			sec_directory->header.occupy_cluster_num = current_directory->header.file_num / 1024 + 1;

			uint_32 file_num = sec_directory->header.file_num;
			sec_directory->fcb = (FCB *)realloc(sec_directory->fcb, file_num * sizeof(FCB));
			strcpy(sec_directory->fcb[file_num - 1].name, fir_directory->fcb[dir].name);
			sec_directory->fcb[file_num - 1].mark = 0;
			sec_directory->fcb[file_num - 1].attribute = fir_directory->fcb[dir].attribute;
			sec_directory->fcb[file_num - 1].delete_flag = 0;
			sec_directory->fcb[file_num - 1].occupy_cluster_num = (uint_32)ceil(file_size / mbr.cluster_size);
			sec_directory->fcb[file_num - 1].file_size = 0;
			sec_directory->fcb[file_num - 1].create_time = fir_directory->fcb[dir].create_time;
			sec_directory->fcb[file_num - 1].modify_time = fir_directory->fcb[dir].modify_time;
			uint_32 empty_room = MfsAlg::BitFindRoom(CAB, mbr.cluster_num);
			MfsAlg::BitSet(CAB, mbr.cluster_num, empty_room);
			sec_directory->fcb[file_num - 1].data_entrance = empty_room;
			FAT[empty_room] = ECOF;
			rewriteDirectory(*sec_directory);

			Directory dir_directory;
			strcpy(dir_directory.header.name, fir_directory->fcb[dir].name);
			dir_directory.header.occupy_cluster_num = 1;
			dir_directory.header.current_dir_entrance = empty_room;
			dir_directory.header.parent_dir_entrance = sec_directory->header.current_dir_entrance;
			dir_directory.header.file_num = 0;
			dir_directory.header.create_time = sec_directory->header.modify_time;
			dir_directory.header.modify_time = sec_directory->header.modify_time;
			dir_directory.header.folder_size = 0;
			dir_directory.fcb = (FCB *)calloc(1, sizeof(FCB));
			newWriteDirectory(dir_directory);
			free(dir_directory.fcb);

			char fir_path[200];
			char next_path[200];
			fir_directory = &readDirectory(fir_directory->fcb[dir].data_entrance);
			for (uint_32 i = 0; i < fir_directory->header.file_num; i++) {
				strcpy(fir_path, filename_1);
				if (fir_path[strlen(fir_path) - 1] != '\\')
					strcat(fir_path, "\\");
				strcat(fir_path, fir_directory->fcb[i].name);
				strcpy(next_path, filename_2);
				if (next_path[strlen(next_path) - 1] != '\\')
					strcat(next_path, "\\");
				strcat(next_path, fir_directory->header.name);
				copyFile(fir_path, next_path);
			}
		}
	}

	return 1;
}

