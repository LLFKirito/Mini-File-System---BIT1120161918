/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#pragma once
#ifndef __MFS_ALGORITHM_H__
#define __MFS_ALGORITHM_H__


#include "mfs_define.h"
#include <vector>


/// ΢���ļ�ϵͳ�㷨��
namespace MfsAlg
{
	/// λ����
			// λ����: ��λ1
	bool	BitSet(uint_8 table[], uint_32 size, uint_32 bit_No);
			// λ����: ����
	bool	BitReset(uint_8 table[], uint_32 size, uint_32 bit_No);
			// λ����: ����0ֵλ��
	uint_32	BitFindRoom(uint_8 table[], uint_32 size);
			// λ����: ���ҵ�iλ�Ƿ�Ϊ1
	uint_32 BitCheck(uint_8 table[], uint_32 size, uint_32 bit_No);


	/// �ָ�·��
	void	cutPath(const char name[], std::vector<std::string>& path);
}


#endif
