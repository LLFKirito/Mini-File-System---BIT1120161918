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


/// <summary> 将硬盘中的MBR信息读入内存mbr中 </summary>
void MiniFileSystem::readMBR(void)
{
	_fseeki64(space_fp, 0LL, SEEK_SET);
	fread(&mbr, sizeof(MBR), 1, space_fp);
}


/// <summary> 将内存中的mbr信息写回硬盘 </summary>
void MiniFileSystem::writeMBR(void) const
{
	_fseeki64(space_fp, 0LL, SEEK_SET);
	fwrite(&mbr, sizeof(MBR), 1, space_fp);
}


/// <summary> 将硬盘中的CAB信息读入内存CAB中 </summary>
void MiniFileSystem::readCAB(void)
{
	_fseeki64(space_fp, mbr.CAB_entrance*mbr.cluster_size * 1024, SEEK_SET);
	fread(CAB, sizeof(uint_8), CAB_occupu_byte, space_fp);
}


/// <summary> 将内存中的CAB信息写回硬盘 </summary>
void MiniFileSystem::writeCAB(void)
{
	_fseeki64(space_fp, mbr.CAB_entrance*mbr.cluster_size * 1024, SEEK_SET);
	fwrite(CAB, sizeof(uint_8), CAB_occupu_byte, space_fp);
}


/// <summary> 将硬盘中的FAT信息读入内存FAT中 </summary>
void MiniFileSystem::readFAT(void)
{
	_fseeki64(space_fp, mbr.FAT_entrance*mbr.cluster_size * 1024, SEEK_SET);
	fread(FAT, sizeof(uint_32), mbr.cluster_num, space_fp);
}


/// <summary> 将内存中的FAT信息写回硬盘 </summary>
void MiniFileSystem::writeFAT(void)
{
	_fseeki64(space_fp, mbr.FAT_entrance*mbr.cluster_size * 1024, SEEK_SET);
	fwrite(FAT, sizeof(uint_32), mbr.cluster_num, space_fp);
}


/// <summary> 将硬盘中指定簇号的信息读入buffer </summary>
/// <param name="cluster"> 指定簇号 </param>
void MiniFileSystem::readCluster(const uint_32 cluster)
{
	_fseeki64(space_fp, cluster*mbr.cluster_size * 1024, SEEK_SET);
	fread(buffer, mbr.cluster_size * 1024, 1, space_fp);
}


/// <summary> 将buffer中的内容写回硬盘指定簇号 </summary>
void MiniFileSystem::writeCluster(const uint_32 cluster)
{
	_fseeki64(space_fp, cluster*mbr.cluster_size * 1024, SEEK_SET);
	fwrite(buffer, mbr.cluster_size * 1024, 1, space_fp);
}


/// <summary> 读取硬盘目录文件并加载到内存中 </summary>
/// <param name="dir_entrance"> 指定目录文件硬盘入口簇号 </param>
/// <return> 内存目录结构 </return>
Directory MiniFileSystem::readDirectory(uint_32 dir_entrance) const
{
	Directory	cur_dir;							// 当前读取的文件目录
	DFC			dir_buffer;							// 目录文件缓冲区
	uint_32		cur_cluster = dir_entrance;			// 当前处理的簇号
	uint_32		block_num = mbr.cluster_size * 16;	// 单簇可存fcb块数
	uint_32		remain_file;						// 当前文件夹未读取FCB文件数
	uint_32		remain_block;						// 当前簇未读块数

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


/// <summary> 将目录文件重写回硬盘 </summary>
/// <param name="dir"> 指定目录文件 </param>
/// <comment> 必须原目录文件存在, 才能使用此函数写回 </comment>
void MiniFileSystem::rewriteDirectory(const Directory dir)
{
	uint_32		cur_cluster;						// 当前处理簇号
	uint_32		remain_file;						// 当前文件夹未读取FCB文件数
	uint_32		remain_block;						// 当前簇剩余空块数
	uint_32		block_num = mbr.cluster_size * 16;	// 单簇可存fcb块数
	DFC			dir_buffer;							// 目录文件缓冲区

	// 处理FAT表
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


/// <summary> 将新建的空目录文件写进硬盘 </summary>
/// <param name="dir"> 目录信息 </param>
/// <comment> 目录文件一定是新建且为空的, 并且入口簇号已经被分配好才使用本函数 </comment>
void MiniFileSystem::newWriteDirectory(const Directory dir) const
{
	uint_32		cur_cluster = dir.header.current_dir_entrance;  // 当前处理簇号
	DFC			dir_buffer;							            // 目录文件缓冲区

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

