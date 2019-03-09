/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../include/mfs_algorithm.h"


/// ���ޱ��ļ�ʹ��
static const uint_8 set_mask[8]   = {128,  64,  32,  16,   8,   4,   2,   1};
static const uint_8 reset_mask[8] = {127, 191, 223, 239, 247, 251, 253, 254};


/// <summary> λ����: ��λ1 </summary>
/// <param name="table"> ����λ���� </param>
/// <param name="size"> ������Чλ��Ŀ </param>
/// <param name="bit_no"> ��1��λ�� </param>
/// <return> true:��λ�ɹ�; false:��λʧ�� </return>
bool MfsAlg::BitSet(uint_8 table[], uint_32 size, uint_32 bit_No)
{
	if (bit_No >= size) {
		return false;
	}
	table[bit_No >> 3] |= set_mask[bit_No & 7];
	return true;
}


/// <summary> λ����: ���� </summary>
/// <return> true:����ɹ�; false:����ʧ�� </return>
bool MfsAlg::BitReset(uint_8 table[], uint_32 size, uint_32 bit_No)
{
	if (bit_No >= size) {
		return false;
	}
	table[bit_No >> 3] &= reset_mask[bit_No & 7];
	return true;
}


/// <summary> λ����: ���ĳλȡֵ </summary>
/// <return> 0:��λΪ0; 1:��λΪ1; 2:����Խ�� </return>
uint_32 MfsAlg::BitCheck(uint_8 table[], uint_32 size, uint_32 bit_No)
{
	if (bit_No >= size) {
		return 2;
	}
	if ((table[bit_No >> 3] & set_mask[bit_No & 7]) != 0){
		return 1;
	}
	else return 0;
}


/// <summary> λ����: ����0ֵλ�� </summary>
/// <return> true:����ɹ�; 0xffffffff:�޿�λ�� </return>
uint_32	MfsAlg::BitFindRoom(uint_8 table[], uint_32 size)
{
	bool flag = false;
	uint_32 bit_No = 0xffffffff;

	uint_32 byte_num = (uint_32)ceil(size / 8.0);
	for (uint_32 i = 0; i < byte_num; i++) {
		if (table[i] == 255) continue;
		for (uint_32 j = 0; j < 8; j++) {
			if ((table[i] & set_mask[j]) == 0) {
				bit_No = i * 8 + j;
				flag = true;
				break;
			}
		}
		if (flag) break;
	}

	if (bit_No >= size) {
		bit_No = 0xffffffff;
	}
	return bit_No;
}


/// <summary> �ָ�·�� </summary>
/// <return> ��������·��name[], ���ηָ��path���� </return>
void MfsAlg::cutPath(const char name[], std::vector<std::string> &path) {
	std::string cur_path = "";
	int len = strlen(name);
	for (int i = 0; i < len; i++) {
		if (name[i] == '\\' || name[i] == '/') {
			path.push_back(cur_path);
			cur_path.clear();
		}
		else {
			cur_path += name[i];
		}
	}
	if (cur_path.length())
		path.push_back(cur_path);
}

