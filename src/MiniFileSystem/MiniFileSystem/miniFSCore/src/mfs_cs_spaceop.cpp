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


bool MiniFileSystem::isLoadSpace(void)
{
	return mount_flag;
}


uint_32 MiniFileSystem::getFreeClusterNum()
{
	uint_32 num = 0;
	for (uint_32 i = 0; i < mbr.cluster_num; i++)
		if (MfsAlg::BitCheck(CAB, mbr.cluster_num, i) == 0)
			num++;
	return num;
}


uint_32 MiniFileSystem::getFreeSpaceSize()
{
	return mbr.cluster_size * getFreeClusterNum();
}


/// <summary> �����ռ� </summary>
/// <return> -1:�ռ�·������; -2:�ռ������ȳ���; -3:�ռ����ظ�; 1:�����ɹ� </return>
int MiniFileSystem::createSpace(const char name[], uint_32 space_size, uint_32 cluster_size)
{
	std::vector<std::string> path;
	std::string spacedir = "";

	MfsAlg::cutPath(name, path);
	for (int i = 0; i < (int)path.size() - 1; i++)
		spacedir += path[i] + "\\";

	// ���·���Ƿ���ȷ
	if (spacedir != ""){
		struct stat buf;
		const char* dirname = spacedir.data();
		int result = stat(dirname, &buf);
		if ((_S_IFDIR & buf.st_mode) != _S_IFDIR)
			return -1;
	}

	// ���ռ�����·�����ռ�������
	const char* spacename = path[path.size() - 1].data();
	if (strlen(spacename) >= 24)
		return -2;

	// �жϴ�λ���Ƿ���ͬ���ļ�
	FILE * fp = fopen(name, "r");
	if (fp != NULL) {
		fclose(fp);
		return -3;
	}

	// �����ռ�
	space_fp = fopen(name, "wb+");
	_fseeki64(space_fp, space_size * 1024 * 1024 - 1, SEEK_SET);
	fwrite("\0", 1, 1, space_fp);
	fclose(space_fp);

	// ��ʼ���ռ���Ϣ
	space_fp = fopen(name, "rb+");

	// д��mbr
	strcpy(mbr.space_name, spacename);
	mbr.space_size = space_size;
	mbr.cluster_size = cluster_size;
	mbr.cluster_num = space_size * 1024 / cluster_size;
	mbr.CAB_entrance = 1;
	mbr.FAT_entrance = mbr.CAB_entrance + (uint_32)ceil(mbr.cluster_num / (8192.0 * mbr.cluster_size));
	mbr.RDF_entrance = mbr.FAT_entrance + (uint_32)ceil(mbr.cluster_num / (256.0 * mbr.cluster_size));
	mbr.create_time = time(NULL);
	mbr.free_cluster_num = mbr.cluster_num - mbr.RDF_entrance - 1;
	// MBRд��Ӳ��
	writeMBR();

	// ���¿���CAB�ڴ�ռ䲢д��
	CAB_occupu_byte = (uint_32)(ceil(mbr.cluster_num / 8.0));
	CAB = (uint_8 *)calloc(CAB_occupu_byte, sizeof(uint_8));
	for (uint_32 i = 0; i <= mbr.RDF_entrance; i++) {
		MfsAlg::BitSet(CAB, mbr.cluster_num, i);
	}
	writeCAB();
	free(CAB);

	// ���¿���FAT�ڴ�ռ䲢д��
	FAT = (uint_32 *)calloc(mbr.cluster_num, sizeof(uint_32));
	FAT[0] = ECOF;
	for (uint_32 i = mbr.CAB_entrance; i < mbr.FAT_entrance - 1; i++) {
		FAT[i] = i + 1;
	}
	FAT[mbr.FAT_entrance - 1] = ECOF;
	for (uint_32 i = mbr.FAT_entrance; i < mbr.RDF_entrance - 1; i++) {
		FAT[i] = i + 1;
	}
	FAT[mbr.RDF_entrance - 1] = ECOF;
	FAT[mbr.RDF_entrance] = ECOF;
	writeFAT();
	free(FAT);

	// �½���Ŀ¼�ļ���д��Ӳ��
	Directory create_directory;
	strcpy(create_directory.header.name, mbr.space_name);
	create_directory.header.occupy_cluster_num = 1;
	create_directory.header.current_dir_entrance = mbr.RDF_entrance;
	create_directory.header.parent_dir_entrance = mbr.RDF_entrance;
	create_directory.header.file_num = 0;
	create_directory.header.create_time = mbr.create_time;
	create_directory.header.modify_time = mbr.create_time;
	create_directory.header.folder_size = 0;
	create_directory.fcb = (FCB *)calloc(1, sizeof(FCB));
	newWriteDirectory(create_directory);
	free(create_directory.fcb);

	fclose(space_fp);
	space_fp = NULL;
	return 1;
}


/// <summary> �򿪿ռ� </summary>
/// <return> -1:��ʧ��; 1:�򿪳ɹ� </return>
int MiniFileSystem::mountSpace(const char name[])
{
	FILE * fp = fopen(name, "r");
	if (fp == NULL) {
		return -1;
	}
	fclose(fp);

	space_fp = fopen(name, "rb+");
	mount_flag = true;

	readMBR();
	CAB_occupu_byte = (uint_32)(ceil(mbr.cluster_num / 8.0));
	CAB = (uint_8 *)calloc(CAB_occupu_byte, sizeof(uint_8));
	readCAB();
	FAT = (uint_32 *)calloc(mbr.cluster_num, sizeof(uint_32));
	readFAT();
	Directory current_directory = readDirectory(mbr.RDF_entrance);
	directory.push_back(current_directory);

	// ��д�ļ���������С
	buffer = (char *)calloc(mbr.cluster_size, 1024);

	return 1;
}


/// <summary> ��ʽ���ռ� </summary>
/// <param name="cluster_size"> �ļ�ϵͳ���ش�С </param>
/// <return> 1:��ʽ���ɹ�; -1:��ʽ��ʧ�� </return>
int MiniFileSystem::formatSpace(uint_32 cluster_size)
{
	// ��дMBR
	/*	CAB�ļ�ռ�ô��� Ϊ �� ���� / ���ش�С(KB) / 8192 ��
		FAT�ļ�ռ�ô��� Ϊ �� ���� / ���ش�С(KB) / 256 ��	 */
	mbr.cluster_size = cluster_size;
	mbr.cluster_num = mbr.space_size * 1024 / mbr.cluster_size;
	mbr.CAB_entrance = 1;
	mbr.FAT_entrance = mbr.CAB_entrance + (uint_32)ceil(mbr.cluster_num / (8192.0*mbr.cluster_size));
	mbr.RDF_entrance = mbr.FAT_entrance + (uint_32)ceil(mbr.cluster_num / (256.0*mbr.cluster_size));
	mbr.free_cluster_num = mbr.cluster_num - mbr.RDF_entrance - 1;

	// MBRд��Ӳ��
	writeMBR();

	// ���¿���CAB�ڴ�ռ䲢д��
	CAB_occupu_byte = (uint_32)(ceil(mbr.cluster_num / 8.0));
	free(CAB);
	CAB = (uint_8 *)calloc(CAB_occupu_byte, sizeof(uint_8));
	for (uint_32 i = 0; i <= mbr.RDF_entrance; i++) {
		MfsAlg::BitSet(CAB, mbr.cluster_num, i);
	}
	writeCAB();

	// ���¿���FAT�ڴ�ռ�
	free(FAT);
	FAT = (uint_32 *)calloc(mbr.cluster_num, sizeof(uint_32));
	FAT[mbr.RDF_entrance] = ECOF;
	writeFAT();

	// ����ļ�Ŀ¼������Ŀ¼�ļ�д��
	std::vector<Directory>::iterator iter;
	Directory cur_dir;
	for (iter = directory.begin(); iter != directory.end(); iter++) {
		cur_dir = (*iter);
		free(cur_dir.fcb);
	}
	directory.clear();
	Directory format_directory;
	strcpy(format_directory.header.name, mbr.space_name);
	format_directory.header.occupy_cluster_num = 1;
	format_directory.header.current_dir_entrance = mbr.RDF_entrance;
	format_directory.header.parent_dir_entrance = mbr.RDF_entrance;
	format_directory.header.file_num = 0;
	format_directory.header.create_time = mbr.create_time;
	format_directory.header.modify_time = time(NULL);
	format_directory.header.folder_size = 0;
	format_directory.fcb = (FCB *)calloc(1, sizeof(FCB));
	newWriteDirectory(format_directory);
	directory.push_back(format_directory);

	// ��д�ļ���������С
	buffer = (char *)realloc(buffer, mbr.cluster_size * 1024);

	return 1;
}


/// <summary> �رյ�ǰ�ռ� </summary>
/// <return> -1:�ر�ʧ��,��ǰδ���κοռ�; 1:�رյ�ǰ�ռ�ɹ� </return>
int MiniFileSystem::closeSpace(void)
{
	if (!mount_flag) return -1;

	writeMBR();
	writeCAB();
	free(CAB);
	writeFAT();
	free(FAT);
	free(buffer);

	std::vector<Directory>::iterator iter;
	Directory cur_dir;
	for (iter = directory.begin(); iter != directory.end(); iter++) {
		cur_dir = (*iter);
		free(cur_dir.fcb);
	}
	directory.clear();

	fclose(space_fp);
	space_fp = NULL;
	mount_flag = false;
	return 1;
}


void MiniFileSystem::optimization(void) {
	for (uint_32 i = mbr.RDF_entrance; i < mbr.cluster_num - 1; i++) {
		if (MfsAlg::BitCheck(CAB, mbr.cluster_num, i) == 1) {
			if (FAT[i] != i + 1 && FAT[i] != ECOF) {
				uint_32 cur_cluster = i;
				while (cur_cluster != ECOF) {
					uint_32 next_cluster = FAT[cur_cluster];
					if (next_cluster == ECOF) {
						FAT[cur_cluster] = ECOF;
						i = cur_cluster;
						break;
					}
					if (MfsAlg::BitCheck(CAB, mbr.cluster_num, cur_cluster + 1) == 1) {
						char *tmp_buffer = (char *)calloc(mbr.cluster_size, 1024);
						_fseeki64(space_fp, (cur_cluster + 1)*mbr.cluster_size * 1024, SEEK_SET);
						fread(tmp_buffer, mbr.cluster_size * 1024, 1, space_fp);
						readCluster(next_cluster);
						_fseeki64(space_fp, next_cluster*mbr.cluster_size * 1024, SEEK_SET);
						fwrite(tmp_buffer, mbr.cluster_size * 1024, 1, space_fp);
						writeCluster(cur_cluster + 1);
						for (uint_32 j = cur_cluster; j < mbr.cluster_num - 1; j++) {
							if (FAT[j] == cur_cluster + 1) {
								FAT[j] = FAT[cur_cluster];
								break;
							}
						}
						FAT[cur_cluster] = cur_cluster + 1;
						uint_32 tmp = FAT[next_cluster];
						FAT[next_cluster] = FAT[cur_cluster + 1];
						FAT[cur_cluster + 1] = tmp;
						cur_cluster = FAT[cur_cluster];
						free(tmp_buffer);
					}
					else {
						readCluster(next_cluster);
						writeCluster(cur_cluster + 1);
						MfsAlg::BitReset(CAB, mbr.cluster_num, next_cluster);
						MfsAlg::BitSet(CAB, mbr.cluster_num, cur_cluster + 1);
						FAT[cur_cluster] = cur_cluster + 1;
						FAT[cur_cluster + 1] = FAT[next_cluster];
						cur_cluster = FAT[cur_cluster];
					}
				}
			}
		}
	}
}

