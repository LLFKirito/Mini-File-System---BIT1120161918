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


/// 微型文件系统算法库
namespace MfsAlg
{
	/// 位操作
			// 位操作: 置位1
	bool	BitSet(uint_8 table[], uint_32 size, uint_32 bit_No);
			// 位操作: 置零
	bool	BitReset(uint_8 table[], uint_32 size, uint_32 bit_No);
			// 位操作: 查找0值位号
	uint_32	BitFindRoom(uint_8 table[], uint_32 size);
			// 位操作: 查找第i位是否为1
	uint_32 BitCheck(uint_8 table[], uint_32 size, uint_32 bit_No);


	/// 分割路径
	void	cutPath(const char name[], std::vector<std::string>& path);
}


#endif
