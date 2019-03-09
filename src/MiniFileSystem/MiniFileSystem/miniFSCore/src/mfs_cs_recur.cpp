/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../include/mfs_coresystem.h"
#include "../include/mfs_algorithm.h"
#include <iostream>
#include <regex>


///	<summary> �ݹ��ӡ��Ŀ¼�µ������ļ� </summary>
/// <comment> �ݹ麯��: ����������ȷ��Ŀ¼�ļ���� </comment>
void MiniFileSystem::treeRecur(uint_32 dir_entrance, std::vector<int>& path)
{
	Directory dir;
	dir = readDirectory(dir_entrance);

	for (uint_32 i = 0; i < dir.header.file_num; i++) {
		if (dir.fcb[i].delete_flag == 1)
			continue;

		// ��������ļ��� 
		for (int blank = 0; blank < (int)path.size(); blank++) {
			if (path[blank] == 1)
				printf("��  ");
			else
				printf("    ");
		}
		bool end_fcb = true;
		for (uint_32 j = i + 1; j < dir.header.file_num; j++) {
			if (dir.fcb[j].delete_flag == 0)
				end_fcb = false;
		}
		if (end_fcb == true)
			printf("��- %s\n", dir.fcb[i].name);
		else
			printf("��- %s\n", dir.fcb[i].name);
		// �ļ��еݹ���� 
		if (dir.fcb[i].mark == 0) {
			path.push_back(!end_fcb);
			treeRecur(dir.fcb[i].data_entrance, path);
			path.erase(path.end() - 1);
		}
	}

	free(dir.fcb);
}


///	<summary> �ݹ�ƥ���Ŀ¼�Լ���Ŀ¼�µ������ļ� </summary>
/// <comment> �ݹ麯��: ����������ȷ��Ŀ¼�ļ���� </comment>
void MiniFileSystem::findRecur(const char expression[], uint_32 dir_entrance, std::vector<std::string>& path, bool &empty)
{
	Directory dir;
	dir = readDirectory(dir_entrance);
	path.push_back(dir.header.name);
	std::regex reg_pattern(expression);

	// ����Ŀ¼��������ƥ��
	for (uint_32 i = 0; i < dir.header.file_num; i++) {
		if (std::regex_match(dir.fcb[i].name, reg_pattern) && dir.fcb[i].delete_flag == 0) {
			empty = false;
			printf(" %-24s Mini-FS:", dir.fcb[i].name);
			for (uint_32 pi = 0; pi < path.size(); pi++) {
				std::cout << "\\" << path[pi];
			}
			puts("\\");
		}
		if (dir.fcb[i].mark == 0 && dir.fcb[i].delete_flag == 0)
			findRecur(expression, dir.fcb[i].data_entrance, path, empty);
	}

	path.erase(path.end() - 1);
	free(dir.fcb);
}


///	<summary> �ݹ鳹�׸�Ŀ¼�Լ���Ŀ¼�µ������ļ� </summary>
/// <comment> �ݹ麯��: ����������ȷ��Ŀ¼�ļ���� </comment>
void MiniFileSystem::delRecur(uint_32 dir_entrance)
{
	Directory dir;
	dir = readDirectory(dir_entrance);

	// ��ո�Ŀ¼�������ļ���CAB��FAT�� 
	for (uint_32 i = 0; i < dir.header.file_num; i++) {
		// �ļ��еݹ���� 
		if (dir.fcb[i].mark == 0) {
			delRecur(dir.fcb[i].data_entrance);
		}

		// �������ݵݹ����
		uint_32 cur_cluster = dir.fcb[i].data_entrance;
		while (cur_cluster != ECOF) {
			uint_32 last_cluster = cur_cluster;
			MfsAlg::BitReset(CAB, mbr.cluster_num, cur_cluster);
			cur_cluster = FAT[cur_cluster];
			FAT[last_cluster] = 0;
		}

	}

	free(dir.fcb);
}


///	<summary> �ݹ���ʾ��Ŀ¼�Լ���Ŀ¼�µ����б�ɾ���ļ� </summary>
/// <comment> �ݹ麯��: ����������ȷ��Ŀ¼�ļ���� </comment>
void MiniFileSystem::showRBRecur(uint_32 dir_entrance, std::vector<std::string>& path, bool &empty, int mode)
{
	Directory dir;
	dir = readDirectory(dir_entrance);
	path.push_back(dir.header.name);

	// �ݹ���ʾ
	for (uint_32 i = 0; i < dir.header.file_num; i++) {
		if (dir.fcb[i].delete_flag == 1 || mode) {
			empty = false;
			printf(" %-24s Mini-FS:", dir.fcb[i].name);
			for (uint_32 pi = 0; pi < path.size(); pi++) {
				std::cout << "\\" << path[pi];
			}
			puts("\\");
			if (dir.fcb[i].mark == 0)
				showRBRecur(dir.fcb[i].data_entrance, path, empty, 1);
		}
		if (dir.fcb[i].delete_flag == 0 && dir.fcb[i].mark == 0)
			showRBRecur(dir.fcb[i].data_entrance, path, empty, 0);
	}

	path.erase(path.end() - 1);
	free(dir.fcb);
}


///	<summary> �ݹ���ջ���վ </summary>
/// <comment> �ݹ麯��: �ⲿ������Ŀ¼��ڴغ� </comment>
void MiniFileSystem::emptyRBRecur(uint_32 dir_entrance)
{
	Directory dir;
	dir = readDirectory(dir_entrance);

	for (uint_32 i = 0; i < dir.header.file_num; i++) {
		if (dir.fcb[i].delete_flag == 1) {
			// ɾ���ļ�����Ϣ
			if (dir.fcb[i].mark == 0) {
				delRecur(dir.fcb[i].data_entrance);
				mbr.free_cluster_num += dir.fcb[i].occupy_cluster_num;
				dir.fcb[i] = dir.fcb[dir.header.file_num - 1];
				dir.header.file_num--;
				i--;
			}
			// ɾ���ļ���Ϣ
			else {
				uint_32 cur_cluster = dir.fcb[i].data_entrance;
				while (cur_cluster != ECOF) {
					uint_32 last_cluster = cur_cluster;
					MfsAlg::BitReset(CAB, mbr.cluster_num, cur_cluster);
					cur_cluster = FAT[cur_cluster];
					FAT[last_cluster] = 0;
				}
				mbr.free_cluster_num += dir.fcb[i].occupy_cluster_num;
				dir.header.folder_size -= dir.fcb[i].file_size;
				dir.fcb[i] = dir.fcb[dir.header.file_num - 1];
				dir.header.file_num--;
				i--;
			}
		}
		else if (dir.fcb[i].mark == 0)
			emptyRBRecur(dir.fcb[i].data_entrance);
	}
	rewriteDirectory(dir);

	free(dir.fcb);
}

