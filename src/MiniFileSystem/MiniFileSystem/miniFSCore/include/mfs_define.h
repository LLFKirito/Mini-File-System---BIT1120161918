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




///	��������¼ (Main Boot Record)											[64B]
struct MBR {
	char			space_name[24];			// �ռ�����						[24B]
	uint_32			space_size;				// �ռ����� (��λ: MB)			[4B]
	uint_32			cluster_size;			// ���ش�С (��λ: KB)			[4B]
	uint_32			cluster_num;			// �ռ����Ŀ 					[4B]
	uint_32			CAB_entrance;			// �ط���λͼ ��ڴغ� 			[4B]
	uint_32			FAT_entrance;			// �ļ�����ͼ ��ڴغ� 			[4B]
	uint_32			RDF_entrance;			// ��Ŀ¼�ļ� ��ڴغ� 			[4B]
	time_t			create_time;			// �ռ䴴��ʱ��					[8B]
	uint_32			free_cluster_num;		// ���д���Ŀ 					[4B]
	uint_8			MBR_reserved[4];		// ����4�ֽ�         			[4B]
};
static_assert(sizeof(MBR) == 64, "MBRռ���ڴ��С����: ע��߽����;");




/// FAT���ļ���ֹ��� (End Cluster Of File)
#define ECOF		(0xffffffff)




/// �ļ����Զ���
#define ATT_ARCHIVE	(1)				// �浵
#define ATT_READ	(2)				// ֻ��
#define ATT_HIDE	(4)				// ����
#define ATT_SYSTEM	(8)				// ϵͳ

///	�ļ����ƿ� (File Control Block)											[64B]
struct FCB {
	char			name[24];				// �ļ���						[24B]
	uint_8			mark;					// ��־��: 0:�ļ��� 1:�ļ�		[1B]
	uint_8			attribute;				// �ļ�����						[1B]
	uint_8			delete_flag;			// ɾ�����: 0:���� 1:ɾ��		[1B]
	uint_8			FCB_reserved[5];		// ����5�ֽڿ����� 				[5B]
	uint_32			data_entrance;			// �ļ����� ��ڴغ�   			[4B]
	uint_32			occupy_cluster_num;		// �ļ�ռ�ô���Ŀ  				[4B]
	uint_64			file_size;				// �ļ���С ��λB  				[8B]
	time_t			create_time;			// �ļ�����ʱ��					[8B]
	time_t			modify_time;			// �ļ����һ���޸�ʱ��			[8B]
};
static_assert(sizeof(FCB) == 64, "FCBռ���ڴ��С����: ע��߽����;");




///	Ŀ¼�ļ�ͷ (Directory File Header)										[64B]
struct DFH {
	char			name[24];				// ��ǰĿ¼�ļ�����				[24B]
	uint_32			occupy_cluster_num;		// �ļ���ռ�ô���Ŀ				[4B]
	uint_32			current_dir_entrance;	// ��ǰĿ¼�ļ���ڴغ�			[4B]
	uint_32			parent_dir_entrance;	// ��һ��Ŀ¼�ļ���ڴغ�  		[4B]
	uint_32			file_num;				// ��Ŀ¼���ļ���Ŀ				[4B]
	time_t			create_time;			// �ļ��� ����ʱ��				[8B]
	time_t			modify_time;			// �ļ������һ�� �޸�ʱ��		[8B]
	uint_64			folder_size;			// �ļ��д�С ��λB				[8B]
};
static_assert(sizeof(DFH) == 64, "DFHռ���ڴ��С����: ע��߽����;");




///	Ŀ¼�ļ��� (Directory File Cluster)										[64KB]
union DFC {
	struct {
		DFH			header;
		FCB			fcb[1023];
	}	firstclu;							// Ŀ¼�ļ���һ������  			[64KB]

	struct {
		FCB			fcb[1024];
	}	otherclu;							// Ŀ¼�ļ�����������  			[64KB]
};
static_assert(sizeof(DFC) == 64 * 1024, "DFCռ���ڴ��С����: ע��߽����;");




///	�ļ�Ŀ¼ (Directory)
struct Directory {
	DFH				header;					// Ŀ¼�ļ�ͷ
	FCB *			fcb;					// Ŀ¼�ļ���Ϣ
};




#endif
