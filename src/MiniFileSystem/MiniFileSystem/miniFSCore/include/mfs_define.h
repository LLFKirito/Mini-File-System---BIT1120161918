/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#pragma once
#ifndef __MFS_DEFINE_H__
#define __MFS_DEFINE_H__


#include <cassert>
#include <ctime>


typedef signed char			int_8;
typedef signed short		int_16;
typedef signed int			int_32;
typedef signed long long	int_64;
typedef unsigned char		uint_8;
typedef unsigned short		uint_16;
typedef unsigned int		uint_32;
typedef unsigned long long	uint_64;




///	主引导记录 (Main Boot Record)											[64B]
struct MBR {
	char			space_name[24];			// 空间名字						[24B]
	uint_32			space_size;				// 空间容量 (单位: MB)			[4B]
	uint_32			cluster_size;			// 单簇大小 (单位: KB)			[4B]
	uint_32			cluster_num;			// 空间簇数目 					[4B]
	uint_32			CAB_entrance;			// 簇分配位图 入口簇号 			[4B]
	uint_32			FAT_entrance;			// 文件分配图 入口簇号 			[4B]
	uint_32			RDF_entrance;			// 根目录文件 入口簇号 			[4B]
	time_t			create_time;			// 空间创建时间					[8B]
	uint_32			free_cluster_num;		// 空闲簇数目 					[4B]
	uint_8			MBR_reserved[4];		// 保留4字节         			[4B]
};
static_assert(sizeof(MBR) == 64, "MBR占用内存大小出错: 注意边界对齐;");




/// FAT表文件终止标记 (End Cluster Of File)
#define ECOF		(0xffffffff)




/// 文件属性定义
#define ATT_ARCHIVE	(1)				// 存档
#define ATT_READ	(2)				// 只读
#define ATT_HIDE	(4)				// 隐藏
#define ATT_SYSTEM	(8)				// 系统

///	文件控制块 (File Control Block)											[64B]
struct FCB {
	char			name[24];				// 文件名						[24B]
	uint_8			mark;					// 标志域: 0:文件夹 1:文件		[1B]
	uint_8			attribute;				// 文件属性						[1B]
	uint_8			delete_flag;			// 删除标记: 0:存在 1:删除		[1B]
	uint_8			FCB_reserved[5];		// 保留5字节空数据 				[5B]
	uint_32			data_entrance;			// 文件数据 入口簇号   			[4B]
	uint_32			occupy_cluster_num;		// 文件占用簇数目  				[4B]
	uint_64			file_size;				// 文件大小 单位B  				[8B]
	time_t			create_time;			// 文件创建时间					[8B]
	time_t			modify_time;			// 文件最后一次修改时间			[8B]
};
static_assert(sizeof(FCB) == 64, "FCB占用内存大小出错: 注意边界对齐;");




///	目录文件头 (Directory File Header)										[64B]
struct DFH {
	char			name[24];				// 当前目录文件名字				[24B]
	uint_32			occupy_cluster_num;		// 文件夹占用簇数目				[4B]
	uint_32			current_dir_entrance;	// 当前目录文件入口簇号			[4B]
	uint_32			parent_dir_entrance;	// 上一级目录文件入口簇号  		[4B]
	uint_32			file_num;				// 该目录下文件数目				[4B]
	time_t			create_time;			// 文件夹 创建时间				[8B]
	time_t			modify_time;			// 文件夹最后一次 修改时间		[8B]
	uint_64			folder_size;			// 文件夹大小 单位B				[8B]
};
static_assert(sizeof(DFH) == 64, "DFH占用内存大小出错: 注意边界对齐;");




///	目录文件簇 (Directory File Cluster)										[64KB]
union DFC {
	struct {
		DFH			header;
		FCB			fcb[1023];
	}	firstclu;							// 目录文件第一簇内容  			[64KB]

	struct {
		FCB			fcb[1024];
	}	otherclu;							// 目录文件其它簇内容  			[64KB]
};
static_assert(sizeof(DFC) == 64 * 1024, "DFC占用内存大小出错: 注意边界对齐;");




///	文件目录 (Directory)
struct Directory {
	DFH				header;					// 目录文件头
	FCB *			fcb;					// 目录文件信息
};




#endif
