/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../include/mfs_coresystem.h"
#include "../include/mfs_algorithm.h"


/// <summary> 删除文件 </summary>
/// <return> -1:删除失败; 1:删除成功 </return>
int MiniFileSystem::deleteFile(const char filename[])
{
	Directory &current_directory = directory[directory.size() - 1];

	current_directory.header.modify_time = time(NULL);

	// 遍历目录
	uint_32 file_num = current_directory.header.file_num;
	uint_64 file_size;
	for (uint_32 i = 0; i < file_num; i++) {
		if (strcmp(filename, current_directory.fcb[i].name) == 0) {
			if (current_directory.fcb[i].delete_flag == 1)
				return -1;
			else {
				file_size = current_directory.fcb[i].file_size;
				current_directory.fcb[i].delete_flag = 1;
				break;
			}
		}
	}

	// 向上到根更新目录信息
	std::vector<Directory>::reverse_iterator iter;
	uint_32 last_cluster = current_directory.header.current_dir_entrance;
	for (iter = directory.rbegin(); iter != directory.rend(); iter++) {
		Directory	&cur_dir = (*iter);
		cur_dir.header.modify_time = current_directory.header.modify_time;
		cur_dir.header.folder_size -= file_size;
		uint_32 file_num = cur_dir.header.file_num;
		for (uint_32 i = 0; i < file_num; i++) {
			if (cur_dir.fcb[i].data_entrance == last_cluster) {
				cur_dir.fcb[i].modify_time = current_directory.header.modify_time;
				break;
			}
		}
		last_cluster = cur_dir.header.current_dir_entrance;
		rewriteDirectory(cur_dir);
	}

	return 1;
}


/// <summary> 恢复文件 </summary>
/// <return> -1:查找文件失败; 1:恢复成功 </return>
int	MiniFileSystem::recoverFile(const char filename[]) {
	Directory *current_directory = &readDirectory(mbr.RDF_entrance);
	std::vector<std::string> path;

	MfsAlg::cutPath(filename, path);
	if (path[0] == "")
		path.erase(path.begin());
	for (int i = 1; i < (int)path.size() - 1; i++) {
		uint_32 file_num = current_directory->header.file_num;
		bool find = false;
		for (uint_32 j = 0; j < current_directory->header.file_num; j++) {
			if (current_directory->fcb[j].name == path[i] && current_directory->fcb[j].delete_flag == 0 && current_directory->fcb[j].mark == 0) {
				current_directory = &readDirectory(current_directory->fcb[j].data_entrance);
				find = true;
				break;
			}
		}
		if (find == false)
			return -1;
	}
	for (int i = 0; i < (int)directory.size(); i++) {
		if (current_directory->header.current_dir_entrance == directory[i].header.current_dir_entrance)
			current_directory = &directory[i];
	}
	for (uint_32 i = 0; i < current_directory->header.file_num; i++) {
		if (current_directory->fcb[i].name == path[path.size() - 1] && current_directory->fcb[i].delete_flag == 1) {
			current_directory->fcb[i].delete_flag = 0;
			current_directory->header.modify_time = time(NULL);
			current_directory->header.folder_size += current_directory->fcb[i].file_size;
			rewriteDirectory(*current_directory);
			break;
		}
	}

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

	return 1;
}


///	<summary> 显示回收站内容 </summary>
void MiniFileSystem::showRecycleBin(void)
{
	std::vector<std::string> path;
	bool is_empty = true;
	path.clear();
	puts("");
	showRBRecur(mbr.RDF_entrance, path, is_empty);
	if (is_empty == true)
		puts(" 回收站无文件");
	path.clear();
}


///	<summary> 清空回收站 </summary>
void MiniFileSystem::emptyRecycleBin(void)
{
	emptyRBRecur(mbr.RDF_entrance);
	// 更新路径信息
	for (int i = 0; i < (int)directory.size(); i++) {
		Directory cur_dir = readDirectory(directory[0].header.current_dir_entrance);
		directory.erase(directory.begin());
		directory.push_back(cur_dir);
	}
}

