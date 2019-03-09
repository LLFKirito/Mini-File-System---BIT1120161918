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


/// <summary> 显示当前文件目录 </summary>
int MiniFileSystem::showDirectory(void)
{
	Directory current_directory = directory[directory.size() - 1];
	struct tm* time;
	uint_32 dir_num = 0, file_num = 0;

	// 输出当前路径
	putchar('\n');
	printf(" Mini-FS:");
	std::vector<Directory>::iterator iter;
	for (iter = directory.begin(); iter != directory.end(); iter++)
		printf("\\%s", (*iter).header.name);
	printf(" 的目录\n\n");

	// 当前不是根目录则加上.和..目录
	if (directory.size() > 1) {
		Directory &parent_directory = directory[directory.size() - 2];
		dir_num += 2;
		time = gmtime(&current_directory.header.modify_time);
        if ((8 + time->tm_hour) % 24 != 8 + time->tm_hour)
            time->tm_mday++;
		printf("%04d/%02d/%02d  %02d:%02d", 1900 + time->tm_year, 1 + time->tm_mon, time->tm_mday, (8 + time->tm_hour)%24, time->tm_min);
		printf("    <DIR>          .\n");
		time = gmtime(&parent_directory.header.modify_time);
        if ((8 + time->tm_hour) % 24 != 8 + time->tm_hour)
            time->tm_mday++;
		printf("%04d/%02d/%02d  %02d:%02d", 1900 + time->tm_year, 1 + time->tm_mon, time->tm_mday, (8 + time->tm_hour)%24, time->tm_min);
		printf("    <DIR>          ..\n");
	}

	// 遍历输出目录下文件
	for (uint_32 i = 0; i < current_directory.header.file_num; i++){
		if (current_directory.fcb[i].delete_flag == 1)
			continue;
		time = gmtime(&current_directory.fcb[i].modify_time);
        if ((8 + time->tm_hour) % 24 != 8 + time->tm_hour)
            time->tm_mday++;
		printf("%04d/%02d/%02d  %02d:%02d", 1900 + time->tm_year, 1 + time->tm_mon, time->tm_mday, (8 + time->tm_hour)%24, time->tm_min);
		if (current_directory.fcb[i].mark == 1) {
			printf("%18lld ", current_directory.fcb[i].file_size);
			file_num++;
		}
		else if (current_directory.fcb[i].mark == 0) {
			printf("    <DIR>          ");
			dir_num++;
		}
		printf("%s\n", current_directory.fcb[i].name);
	}
	printf("            %d 个文件      %d 个目录\n", file_num, dir_num);

	return 1;
}


/// <summary> 树形打印当前文件夹内文件目录 </summary>
void MiniFileSystem::treeDirectory(void)
{
	uint_32 cur_dir_entrance = directory[directory.size() - 1].header.current_dir_entrance;
	std::vector<int> path;

	// 当前目录下无文件
	puts("");
	if (directory[directory.size() - 1].header.file_num == 0){
		puts(" 没有子文件夹");
		return;
	}

	// 递归遍历子目录
	treeRecur(cur_dir_entrance, path);
	path.clear();
}


/// <summary> 输出文件占用簇号 </summary>
/// <return> -1:查找文件失败; 1:输出成功 </return>
int MiniFileSystem::occupyCluster(const char filename[]){
	Directory current_directory = directory[directory.size() - 1];

	// 遍历查找文件
	uint_32 file_num = current_directory.header.file_num;
	for (uint_32 i = 0; i < file_num; i++) {
		if (strcmp(filename, current_directory.fcb[i].name) == 0 && current_directory.fcb[i].delete_flag == 0) {
			uint_32 cur_cluster = current_directory.fcb[i].data_entrance;
			int count_cluster = 0;
			printf("\n %s 占用的全部块号为:\n", filename);

			// 根据FAT表查找占用块
			while (cur_cluster != ECOF){
				count_cluster++;
				if (count_cluster % 20 == 0)
					puts("");
				printf(" %d%c", cur_cluster, FAT[cur_cluster] == ECOF ? '\n' : ',');
				cur_cluster = FAT[cur_cluster];
			}
			return 1;
		}
	}

	return -1;
}


/// <summary> 改变当前所处文件夹位置 </summary>
/// <return> -1:改变失败; 1:改变成功 </return>
int MiniFileSystem::changeDirectory(const char filename[])
{
	Directory current_directory = directory[directory.size() - 1];

	// 进入上级目录
	if (strcmp(filename, "..") == 0) {
		if (directory.size() <= 1)
			return 1;
		free(current_directory.fcb);
		directory.erase(directory.end() - 1);
		return 1;
	}
	// 进入当前目录
	else if (strcmp(filename, ".") == 0) {
		return 1;
	}

	// 遍历目录寻找下级目录
	uint_32 file_num = current_directory.header.file_num;
	for (uint_32 i = 0; i < file_num; i++) {
		if (strcmp(filename, current_directory.fcb[i].name) == 0 && current_directory.fcb[i].delete_flag == 0) {
			if (current_directory.fcb[i].mark == 1)
				return -1;
			else {
				Directory next_directory = readDirectory(current_directory.fcb[i].data_entrance);
				directory.push_back(next_directory);
				return 1;
			}
		}
	}
	return -1;
}


/// <summary> 创建文件夹 </summary>
int MiniFileSystem::makeDir(const char filename[])
{
	Directory &current_directory = directory[directory.size() - 1];

	// 查找是否存在重名文件夹
	for (uint_32 i = 0; i < current_directory.header.file_num; i++) {
		if (strcmp(filename, current_directory.fcb[i].name) == 0 && current_directory.fcb[i].mark == 0) {
			return -1;
		}
	}

	// 更改目录文件头
	current_directory.header.modify_time = time(NULL);
	current_directory.header.file_num++;
	current_directory.header.occupy_cluster_num = current_directory.header.file_num / 1024 + 1;

	// 新添目录信息
	uint_32 file_num = current_directory.header.file_num;
	current_directory.fcb = (FCB *)realloc(current_directory.fcb, file_num * sizeof(FCB));
	strcpy(current_directory.fcb[file_num - 1].name, filename);
	current_directory.fcb[file_num - 1].mark = 0;
	current_directory.fcb[file_num - 1].attribute = ATT_ARCHIVE;
	current_directory.fcb[file_num - 1].delete_flag = 0;
	current_directory.fcb[file_num - 1].occupy_cluster_num = 1;
	current_directory.fcb[file_num - 1].file_size = 0;
	current_directory.fcb[file_num - 1].create_time = current_directory.header.modify_time;
	current_directory.fcb[file_num - 1].modify_time = current_directory.header.modify_time;
	uint_32 empty_room = MfsAlg::BitFindRoom(CAB, mbr.cluster_num);
	MfsAlg::BitSet(CAB, mbr.cluster_num, empty_room);
	FAT[empty_room] = ECOF;
	mbr.free_cluster_num -= 1;
	current_directory.fcb[file_num - 1].data_entrance = empty_room;

	// 由当前目录到根目录向上更新
	std::vector<Directory>::reverse_iterator iter;
	uint_32 last_cluster = current_directory.header.current_dir_entrance;
	for (iter = directory.rbegin(); iter != directory.rend(); iter++) {
		Directory &cur_dir = (*iter);
		cur_dir.header.modify_time = current_directory.header.modify_time;
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

	// 创建新文件头
	Directory dir_directory;
	strcpy(dir_directory.header.name, filename);
	dir_directory.header.occupy_cluster_num = 1;
	dir_directory.header.current_dir_entrance = empty_room;
	dir_directory.header.parent_dir_entrance = current_directory.header.current_dir_entrance;
	dir_directory.header.file_num = 0;
	dir_directory.header.create_time = current_directory.header.modify_time;
	dir_directory.header.modify_time = current_directory.header.modify_time;
	dir_directory.header.folder_size = 0;
	dir_directory.fcb = (FCB *)calloc(1, sizeof(FCB));
	newWriteDirectory(dir_directory);
	free(dir_directory.fcb);

	return 1;
}


/// <summary> 创建文件 </summary>
/// <return> -1:创建失败; 1:创建成功 </return>
int MiniFileSystem::createFile(const char filename[])
{
	Directory &current_directory = directory[directory.size() - 1];

	// 遍历查找是否有重名文件
	for (uint_32 i = 0; i < current_directory.header.file_num; i++) {
		if (strcmp(filename, current_directory.fcb[i].name) == 0 && current_directory.fcb[i].mark == 1 && current_directory.fcb[i].delete_flag == 0) {
			return -1;
		}
	}

	// 更新目录文件头
	current_directory.header.modify_time = time(NULL);
	current_directory.header.file_num++;
	current_directory.header.occupy_cluster_num = current_directory.header.file_num / 1024 + 1;

	// 新添目录信息
	uint_32 file_num = current_directory.header.file_num;
	current_directory.fcb = (FCB *)realloc(current_directory.fcb, file_num * sizeof(FCB));
	strcpy(current_directory.fcb[file_num - 1].name, filename);
	current_directory.fcb[file_num - 1].mark = 1;
	current_directory.fcb[file_num - 1].attribute = ATT_ARCHIVE;
	current_directory.fcb[file_num - 1].delete_flag = 0;
	current_directory.fcb[file_num - 1].occupy_cluster_num = 1;
	current_directory.fcb[file_num - 1].file_size = 0;
	current_directory.fcb[file_num - 1].create_time = current_directory.header.modify_time;
	current_directory.fcb[file_num - 1].modify_time = current_directory.header.modify_time;
	uint_32 empty_room = MfsAlg::BitFindRoom(CAB, mbr.cluster_num);
	MfsAlg::BitSet(CAB, mbr.cluster_num, empty_room);
	FAT[empty_room] = ECOF;
	mbr.free_cluster_num -= 1;
	current_directory.fcb[file_num - 1].data_entrance = empty_room;

	// 由当前目录到根目录向上更新
	std::vector<Directory>::reverse_iterator iter;
	uint_32 last_cluster = current_directory.header.current_dir_entrance;
	for (iter = directory.rbegin(); iter != directory.rend(); iter++) {
		Directory &cur_dir = (*iter);
		cur_dir.header.modify_time = current_directory.header.modify_time;
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


/// <summary> 正则匹配查找文件 </summary>
int MiniFileSystem::findFile(const char expression[]){
	Directory cur_directory = directory[directory.size() - 1];
	std::vector<std::string> path;

	// 记录当前路径
	for (int i = 0; i < (int)directory.size() - 1; i++)
		path.push_back(directory[i].header.name);

	// 递归匹配子目录
	bool empty = true;
	puts("");
	findRecur(expression, cur_directory.header.current_dir_entrance, path, empty);
	if (empty == true)
		puts(" 无匹配文件");

	return 1;
}


/// <summary> 移动文件 </summary>
/// <return> -1:当前在被移动文件内; -2:移动失败; 1:移动成功 </return>
int MiniFileSystem::moveFile(const char filename_1[], const char filename_2[]){
	Directory *current_directory = &readDirectory(mbr.RDF_entrance);
	std::vector<std::string> path;

	// 分割路径
	MfsAlg::cutPath(filename_1, path);
	if (path[0] == "")
		path.erase(path.begin());

	// 寻找目标文件
	for (int i = 1; i < (int)path.size() - 1; i++) {
		uint_32 file_num = current_directory->header.file_num;
		for (uint_32 j = 0; j < current_directory->header.file_num; j++) {
			if (current_directory->fcb[j].name == path[i] && current_directory->fcb[j].delete_flag == 0 && current_directory->fcb[j].mark == 0) {
				current_directory = &readDirectory(current_directory->fcb[j].data_entrance);
				break;
			}
		}
	}

	// 判断当前目录是否在被移动目录中
	for (uint_32 i = 0; i < directory.size() - 1; i++){
		if (current_directory->header.current_dir_entrance == directory[i].header.current_dir_entrance){
			return -1;
		}
	}
	if (current_directory->header.current_dir_entrance == directory[directory.size() - 1].header.current_dir_entrance)
		current_directory = &directory[directory.size() - 1];

	// 路径合法则复制，非法则报错
	if (copyFile(filename_1, filename_2) == -1)
		return -2;

	// 删除被移动路径
	for (uint_32 i = 0; i < current_directory->header.file_num; i++){
		if (current_directory->fcb[i].name == path[path.size() - 1] && current_directory->fcb[i].delete_flag == 0) {
			current_directory->header.folder_size -= current_directory->fcb[i].file_size;
			if (current_directory->fcb[i].mark == 0)
				delRecur(current_directory->fcb[i].data_entrance);
			else{
				uint_32 cur_cluster = current_directory->fcb[i].data_entrance;
				while (cur_cluster != ECOF){
					MfsAlg::BitReset(CAB, mbr.cluster_num, cur_cluster);
					cur_cluster = FAT[cur_cluster];
				}
			}
			current_directory->fcb[i] = current_directory->fcb[current_directory->header.file_num - 1];
			current_directory->header.file_num -= 1;
			rewriteDirectory(*current_directory);
			break;
		}
	}

	return 1;
}


/// <summary> 显示文件属性 </summary>
/// <return> -1:查找文件失败; 1:显示成功 </return>
int MiniFileSystem::showAttribute(const char filename[])
{
	Directory current_directory = directory[directory.size() - 1];
	struct tm* time;

	puts("");

	// 显示当前目录下所有文件属性
	if (strcmp(filename, ".") == 0) {
		printf(" %s 文件夹已存放 %lld 字节数据\n",
			current_directory.header.name,
			current_directory.header.folder_size
			);

		for (uint_32 i = 0; i < current_directory.header.file_num; i++) {
			if (current_directory.fcb[i].delete_flag == 1)
				continue;
			printf("  %-25s ", current_directory.fcb[i].name);
			if ((current_directory.fcb[i].attribute & ATT_ARCHIVE) == ATT_ARCHIVE)		printf("存档\n");
			else if ((current_directory.fcb[i].attribute & ATT_READ) == ATT_READ)	printf("只读\n");
			else if ((current_directory.fcb[i].attribute & ATT_HIDE) == ATT_HIDE)	printf("隐藏\n");
			else if ((current_directory.fcb[i].attribute & ATT_SYSTEM) == ATT_SYSTEM)	printf("系统\n");
			else printf("\n");
		}
		return 1;
	}

	// 显示单个文件属性
	for (uint_32 i = 0; i < current_directory.header.file_num; i++) {
		if (current_directory.fcb[i].delete_flag == 1)
			continue;
		if (strcmp(filename, current_directory.fcb[i].name) == 0 && current_directory.fcb[i].delete_flag == 0) {
			if (current_directory.fcb[i].mark == 0)
				printf(" 文件夹名:          %s\n", current_directory.fcb[i].name);
			else
				printf(" 文件名:            %s\n", current_directory.fcb[i].name);

			printf(" 权限:              ");
			if ((current_directory.fcb[i].attribute & ATT_ARCHIVE) == ATT_ARCHIVE)		printf("存档\n");
			else if ((current_directory.fcb[i].attribute & ATT_READ) == ATT_READ)	printf("只读\n");
			else if ((current_directory.fcb[i].attribute & ATT_HIDE) == ATT_HIDE)	printf("隐藏\n");
			else if ((current_directory.fcb[i].attribute & ATT_SYSTEM) == ATT_SYSTEM)	printf("系统\n");
			else printf("\n");

			time = gmtime(&current_directory.fcb[i].create_time);
			printf(" 创建时间:          %04d/%02d/%02d  %02d:%02d\n", 1900 + time->tm_year, 1 + time->tm_mon, time->tm_mday, 8 + time->tm_hour, time->tm_min);
			time = gmtime(&current_directory.fcb[i].modify_time);
			printf(" 修改时间:          %04d/%02d/%02d  %02d:%02d\n", 1900 + time->tm_year, 1 + time->tm_mon, time->tm_mday, 8 + time->tm_hour, time->tm_min);
			printf(" 文件大小:          %llu\n", current_directory.fcb[i].file_size);
			return 1;
		}
	}

	return -1;
}

