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
#include <cstring>
#include <iostream>


bool MiniFileSystem::isLoadFile()
{
	return this->file_load_flag;
}


int MiniFileSystem::openFile(const char * filename)
{
	if (file_load_flag == true)
		return -2;
	Directory &current_directory = directory[directory.size() - 1];
	uint_64 no_file = -1;
	for (uint_32 i = 0; i < current_directory.header.file_num; i++)
	{
		if (strcmp(filename, current_directory.fcb[i].name) == 0 && current_directory.fcb[i].mark == 1 && current_directory.fcb[i].delete_flag == 0)
		{
			no_file = i;
		}
	}
	if (no_file == -1)	// 当前目录下没有该文件
		return -3;
	else
		now_fcb = &current_directory.fcb[no_file];
	file_load_flag = true;
	return 1;
}


int MiniFileSystem::closeFile(void)
{
	if (file_load_flag == false)
		return -1;
	writeBackDirectory();
	now_fcb = NULL;
	file_load_flag = false;
	return 1;
}


int MiniFileSystem::readFile(void)
{
	if (file_load_flag == false)
		return -1;
	std::string file_data;
	char * tmp = (char *)malloc(mbr.cluster_size * 1024 + 5);
	for (uint_32 cur_cluster = now_fcb->data_entrance; cur_cluster != ECOF; cur_cluster = FAT[cur_cluster])
	{
		readCluster(cur_cluster);
		strncpy(tmp, buffer, mbr.cluster_size * 1024);
		tmp[mbr.cluster_size * 1024] = 0;
		file_data.append(tmp);
	}
	free(tmp);
	char * data = (char *)calloc(file_data.size() + 5, sizeof(char));
	strncpy(data, file_data.data(), (size_t)now_fcb->file_size);
	std::cout << data;
	free(data);
	return 1;
}


int MiniFileSystem::writeFile(void)
{
	if (file_load_flag == false)
		return -1;

	std::cout << " 请输入内容, 单行输入字符串\"@@@\"结束输入. " << std::endl;

	uint_64 text_size;
	std::string text;
	std::string tmp;
	while (std::getline(std::cin, tmp))
	{
		if (tmp == "@@@")
			break;
		text.append(tmp);
		text.append("\n");
	}
	text_size = text.size();

	// 输入文本超过内存限制，拒绝 
	if (text_size >= 1024 * getFreeSpaceSize())
	{
		return -2;
	}

	// 处理CAB、FAT表
	uint_32 cur_cluster = now_fcb->data_entrance;
	while (cur_cluster != ECOF) {
		uint_32 last_cluster = cur_cluster;
		MfsAlg::BitReset(CAB, mbr.cluster_num, cur_cluster);
		cur_cluster = FAT[cur_cluster];
		FAT[last_cluster] = 0;
	}

	// 写回磁盘
	uint_32 occupy_cluster = 0;	// 占用数目 
	cur_cluster = now_fcb->data_entrance;
	MfsAlg::BitSet(CAB, mbr.cluster_num, cur_cluster);
	occupy_cluster++;
	FAT[cur_cluster] = ECOF;
	if (text.size() <= mbr.cluster_size * 1024)
	{
		strcpy(buffer, text.data());
		text.clear();
		writeCluster(cur_cluster);
	}
	else
	{
		strncpy(buffer, text.data(), mbr.cluster_size * 1024);
		text.substr(0, mbr.cluster_size * 1024);
		writeCluster(cur_cluster);
	}

	while (!text.empty())
	{
		uint_32 last_cluster = cur_cluster;

		cur_cluster = MfsAlg::BitFindRoom(CAB, mbr.cluster_num);
		MfsAlg::BitSet(CAB, mbr.cluster_num, cur_cluster);
		FAT[last_cluster] = cur_cluster;
		FAT[cur_cluster] = ECOF;

		occupy_cluster++;
		if (text.size() <= mbr.cluster_size * 1024)
		{
			strcpy(buffer, text.data());
			text.clear();
			writeCluster(cur_cluster);
		}
		else
		{
			strncpy(buffer, text.data(), mbr.cluster_size * 1024);
			text.substr(0, mbr.cluster_size * 1024);
			writeCluster(cur_cluster);
		}
	}

	uint_64 size_dif = text_size - now_fcb->file_size;
	uint_32 occupy_cluster_dif = occupy_cluster - now_fcb->occupy_cluster_num;

	now_fcb->file_size = text_size;
	now_fcb->modify_time = time(NULL);
	now_fcb->occupy_cluster_num = occupy_cluster;

	Directory &current_directory = directory[directory.size() - 1];
	std::vector<Directory>::reverse_iterator iter;
	uint_32 last_cluster = current_directory.header.current_dir_entrance;
	for (iter = directory.rbegin(); iter != directory.rend(); iter++)
	{
		Directory &cur_dir = (*iter);
		cur_dir.header.modify_time = now_fcb->modify_time;
		cur_dir.header.folder_size += size_dif;
		cur_dir.header.occupy_cluster_num += occupy_cluster_dif;
		uint_32 file_num = cur_dir.header.file_num;
		for (uint_32 i = 0; i < file_num; i++)
		{
			if (cur_dir.fcb[i].data_entrance == last_cluster)
			{
				cur_dir.fcb[i].modify_time = now_fcb->modify_time;
				cur_dir.fcb[i].file_size += size_dif;
				cur_dir.fcb[i].occupy_cluster_num += occupy_cluster_dif;
				break;
			}
		}
		last_cluster = cur_dir.header.current_dir_entrance;
	}
	writeBackDirectory();
	return 1;
}

