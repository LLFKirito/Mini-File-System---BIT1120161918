/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#pragma once
#ifndef __MFS_CORESYSTEM_H__
#define __MFS_CORESYSTEM_H__


#include "mfs_define.h"
#include <vector>
#include <string>


///	΢���ļ�ϵͳ (Mini File System)
class MiniFileSystem
{
public:
	MiniFileSystem() :
		mount_flag(false), space_fp(NULL), CAB(NULL), FAT(NULL), buffer(NULL), file_load_flag(false), now_fcb(NULL)
	{};
	~MiniFileSystem() {};


	bool cmd(void);

	bool isLoadSpace(void);
	uint_32 getFreeClusterNum(void);
	uint_32 getFreeSpaceSize(void);
	int createSpace(const char name[], uint_32 space_size = 1024, uint_32 cluster_size = 4);
	int mountSpace(const char name[]);
	int formatSpace(uint_32 cluster_size = 4);
	int closeSpace(void);
	void optimization(void);

	int showDirectory(void);
	void treeDirectory(void);
	int occupyCluster(const char filename[]);
	int changeDirectory(const char filename[]);
	int makeDir(const char filename[]);
	int createFile(const char filename[]);
	int findFile(const char expression[]);
	int moveFile(const char filename_1[], const char filename_2[]);
	int showAttribute(const char filename[]);

	bool isLoadFile(void);
	int openFile(const char * filename);
	int closeFile(void);
	int readFile(void);
	int writeFile(void);

	int copyFile(const char filename_1[], const char filename_2[]);

	int deleteFile(const char filename[]);
	int recoverFile(const char filename[]);
	void showRecycleBin(void);
	void emptyRecycleBin(void);


private:
	void fileSeekCluster(const uint_32 cluster);
	void readMBR(void);
	void writeMBR(void) const;
	void readCAB(void);
	void writeCAB(void);
	void readFAT(void);
	void writeFAT(void);
	void readCluster(const uint_32 cluster);
	void writeCluster(const uint_32 cluster);
	Directory readDirectory(const uint_32 dir_entrance) const;
	void rewriteDirectory(const Directory dir);
	void newWriteDirectory(const Directory dir) const;
	bool writeBackDirectory(void);

	void treeRecur(uint_32 dir_entrance, std::vector<int>& path);
	void findRecur(const char expression[], uint_32 dir_entrance, std::vector<std::string>& path, bool &empty);
	void delRecur(uint_32 dir_entrance);
	void showRBRecur(uint_32 dir_entrance, std::vector<std::string>& path, bool &empty, int mode = 0);
	void emptyRBRecur(uint_32 dir_entrance);


private:
	bool mount_flag;                    // �ռ�򿪱�� 
	MBR mbr;                            // ��������¼ 
	FILE * space_fp;                    // ��ǰ�ռ�(�ļ�)ָ�� 
	uint_8 * CAB;                       // �ط���λͼ 
	uint_32 CAB_occupu_byte;            // CABռ���ֽ��� 
	uint_32 * FAT;                      // �ļ������ 
	std::vector<Directory> directory;   // �ļ�Ŀ¼ 
	char * buffer;                      // �ռ�������������� 
	bool file_load_flag;
	FCB * now_fcb;
};


#endif
