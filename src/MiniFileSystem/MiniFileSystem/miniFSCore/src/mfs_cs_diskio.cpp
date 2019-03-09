/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../include/mfs_coresystem.h"
#include "../include/mfs_algorithm.h"


void MiniFileSystem::fileSeekCluster(const uint_32 cluster)
{
	int_64 loc = (int_64)cluster * mbr.cluster_size * 1024;
	_fseeki64(space_fp, loc, SEEK_SET);
}


/// <summary> ��Ӳ���е�MBR��Ϣ�����ڴ�mbr�� </summary>
void MiniFileSystem::readMBR(void)
{
	_fseeki64(space_fp, 0LL, SEEK_SET);
	fread(&mbr, sizeof(MBR), 1, space_fp);
}


/// <summary> ���ڴ��е�mbr��Ϣд��Ӳ�� </summary>
void MiniFileSystem::writeMBR(void) const
{
	_fseeki64(space_fp, 0LL, SEEK_SET);
	fwrite(&mbr, sizeof(MBR), 1, space_fp);
}


/// <summary> ��Ӳ���е�CAB��Ϣ�����ڴ�CAB�� </summary>
void MiniFileSystem::readCAB(void)
{
	_fseeki64(space_fp, mbr.CAB_entrance*mbr.cluster_size * 1024, SEEK_SET);
	fread(CAB, sizeof(uint_8), CAB_occupu_byte, space_fp);
}


/// <summary> ���ڴ��е�CAB��Ϣд��Ӳ�� </summary>
void MiniFileSystem::writeCAB(void)
{
	_fseeki64(space_fp, mbr.CAB_entrance*mbr.cluster_size * 1024, SEEK_SET);
	fwrite(CAB, sizeof(uint_8), CAB_occupu_byte, space_fp);
}


/// <summary> ��Ӳ���е�FAT��Ϣ�����ڴ�FAT�� </summary>
void MiniFileSystem::readFAT(void)
{
	_fseeki64(space_fp, mbr.FAT_entrance*mbr.cluster_size * 1024, SEEK_SET);
	fread(FAT, sizeof(uint_32), mbr.cluster_num, space_fp);
}


/// <summary> ���ڴ��е�FAT��Ϣд��Ӳ�� </summary>
void MiniFileSystem::writeFAT(void)
{
	_fseeki64(space_fp, mbr.FAT_entrance*mbr.cluster_size * 1024, SEEK_SET);
	fwrite(FAT, sizeof(uint_32), mbr.cluster_num, space_fp);
}


/// <summary> ��Ӳ����ָ���غŵ���Ϣ����buffer </summary>
/// <param name="cluster"> ָ���غ� </param>
void MiniFileSystem::readCluster(const uint_32 cluster)
{
	_fseeki64(space_fp, cluster*mbr.cluster_size * 1024, SEEK_SET);
	fread(buffer, mbr.cluster_size * 1024, 1, space_fp);
}


/// <summary> ��buffer�е�����д��Ӳ��ָ���غ� </summary>
void MiniFileSystem::writeCluster(const uint_32 cluster)
{
	_fseeki64(space_fp, cluster*mbr.cluster_size * 1024, SEEK_SET);
	fwrite(buffer, mbr.cluster_size * 1024, 1, space_fp);
}


/// <summary> ��ȡӲ��Ŀ¼�ļ������ص��ڴ��� </summary>
/// <param name="dir_entrance"> ָ��Ŀ¼�ļ�Ӳ����ڴغ� </param>
/// <return> �ڴ�Ŀ¼�ṹ </return>
Directory MiniFileSystem::readDirectory(uint_32 dir_entrance) const
{
	Directory	cur_dir;							// ��ǰ��ȡ���ļ�Ŀ¼
	DFC			dir_buffer;							// Ŀ¼�ļ�������
	uint_32		cur_cluster = dir_entrance;			// ��ǰ����Ĵغ�
	uint_32		block_num = mbr.cluster_size * 16;	// ���ؿɴ�fcb����
	uint_32		remain_file;						// ��ǰ�ļ���δ��ȡFCB�ļ���
	uint_32		remain_block;						// ��ǰ��δ������

	_fseeki64(space_fp, dir_entrance * mbr.cluster_size * 1024, SEEK_SET);
	fread(&dir_buffer, mbr.cluster_size * 1024, 1, space_fp);

	cur_dir.header = dir_buffer.firstclu.header;
	cur_dir.fcb = (FCB *)malloc(cur_dir.header.file_num * sizeof(FCB));

	remain_file = cur_dir.header.file_num;
	remain_block = block_num - 1;

	uint_32 idx_m = 0;
	uint_32 idx_d = 0;
	while (remain_block > 0 && remain_file > 0)
	{
		cur_dir.fcb[idx_m] = dir_buffer.firstclu.fcb[idx_d];
		remain_block--;
		remain_file--;
		idx_m++;
		idx_d++;
	}
	while (remain_file > 0) {
		if (remain_block == 0) {
			cur_cluster = FAT[cur_cluster];
			_fseeki64(space_fp, cur_cluster * mbr.cluster_size * 1024, SEEK_SET);
			fread(&dir_buffer, mbr.cluster_size * 1024, 1, space_fp);
			remain_block = block_num;
			idx_d = 0;
		}
		cur_dir.fcb[idx_m] = dir_buffer.otherclu.fcb[idx_d];
		remain_block--;
		remain_file--;
		idx_m++;
		idx_d++;
	}

	return cur_dir;
}


/// <summary> ��Ŀ¼�ļ���д��Ӳ�� </summary>
/// <param name="dir"> ָ��Ŀ¼�ļ� </param>
/// <comment> ����ԭĿ¼�ļ�����, ����ʹ�ô˺���д�� </comment>
void MiniFileSystem::rewriteDirectory(const Directory dir)
{
	uint_32		cur_cluster;						// ��ǰ����غ�
	uint_32		remain_file;						// ��ǰ�ļ���δ��ȡFCB�ļ���
	uint_32		remain_block;						// ��ǰ��ʣ��տ���
	uint_32		block_num = mbr.cluster_size * 16;	// ���ؿɴ�fcb����
	DFC			dir_buffer;							// Ŀ¼�ļ�������

	// ����FAT��
	cur_cluster = dir.header.current_dir_entrance;
	while (cur_cluster != ECOF) {
		uint_32 last_cluster = cur_cluster;
		MfsAlg::BitReset(CAB, mbr.cluster_num, cur_cluster);
		cur_cluster = FAT[cur_cluster];
		FAT[last_cluster] = 0;
	}

	cur_cluster = dir.header.current_dir_entrance;
	MfsAlg::BitSet(CAB, mbr.cluster_num, cur_cluster);

	dir_buffer.firstclu.header = dir.header;
	remain_block = block_num - 1;
	remain_file = dir.header.file_num;
	uint_32 idx_m = 0;
	uint_32 idx_d = 0;
	while (remain_block > 0 && remain_file > 0) {
		dir_buffer.firstclu.fcb[idx_d] = dir.fcb[idx_m];
		remain_block--;
		remain_file--;
		idx_m++;
		idx_d++;
	}
	_fseeki64(space_fp, cur_cluster*mbr.cluster_size * 1024, SEEK_SET);
	fwrite(&dir_buffer, mbr.cluster_size * 1024, 1, space_fp);
	mbr.free_cluster_num -= 1;

	remain_block = block_num;
	idx_d = 0;
	while (remain_file > 0) {
		dir_buffer.otherclu.fcb[idx_d] = dir.fcb[idx_m];
		remain_block--;
		remain_file--;
		idx_m++;
		idx_d++;
		if (remain_block == 0) {
			uint_32 last_curster = cur_cluster;
			cur_cluster = MfsAlg::BitFindRoom(CAB, mbr.cluster_num);
			MfsAlg::BitSet(CAB, mbr.cluster_num, cur_cluster);
			FAT[last_curster] = cur_cluster;
			_fseeki64(space_fp, cur_cluster * mbr.cluster_size * 1024, SEEK_SET);
			fwrite(&dir_buffer, mbr.cluster_size * 1024, 1, space_fp);
			mbr.free_cluster_num -= 1;
			remain_block = block_num;
			idx_d = 0;
		}
	}
	if (remain_block != block_num) {
		uint_32 last_curster = cur_cluster;
		cur_cluster = MfsAlg::BitFindRoom(CAB, mbr.cluster_num);
		MfsAlg::BitSet(CAB, mbr.cluster_num, cur_cluster);
		FAT[last_curster] = cur_cluster;
		_fseeki64(space_fp, cur_cluster * mbr.cluster_size * 1024, SEEK_SET);
		fwrite(&dir_buffer, mbr.cluster_size * 1024, 1, space_fp);
		mbr.free_cluster_num -= 1;
	}

	FAT[cur_cluster] = ECOF;
}


/// <summary> ���½��Ŀ�Ŀ¼�ļ�д��Ӳ�� </summary>
/// <param name="dir"> Ŀ¼��Ϣ </param>
/// <comment> Ŀ¼�ļ�һ�����½���Ϊ�յ�, ������ڴغ��Ѿ�������ò�ʹ�ñ����� </comment>
void MiniFileSystem::newWriteDirectory(const Directory dir) const
{
	uint_32		cur_cluster = dir.header.current_dir_entrance;  // ��ǰ����غ�
	DFC			dir_buffer;							            // Ŀ¼�ļ�������

	dir_buffer.firstclu.header = dir.header;
	_fseeki64(space_fp, cur_cluster * mbr.cluster_size * 1024, SEEK_SET);
	fwrite(&dir_buffer, mbr.cluster_size * 1024, 1, space_fp);
}


bool MiniFileSystem::writeBackDirectory(void)
{
	if (!mount_flag)
		return false;
	std::vector<Directory>::reverse_iterator iter;
	for (iter = directory.rbegin(); iter != directory.rend(); iter++)
	{
		rewriteDirectory(*iter);
	}
	return true;
}

