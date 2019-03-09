/**
 *	main.cpp
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
 *	This is master control program of Mini File System.
 *
 ********************************************************************************
 *	Mini File System Project:
 *		Author:     Linfeng Li.
 *		Tutor:      Hong Song.
 *		Company:    School of Computer Science, Beijing Institute of Technology.
 *		Version:    3.2.13 (Release).
 *		Submit:     December 6th, 2018.
 ********************************************************************************
**/
#include "MiniFileSystem.h"

int main(int argc, char *argv[])
{
	system("title Mini File System (Linfeng Li)");
	MiniFileSystem SimulatedDisk;
	while (SimulatedDisk.cmd());
	return 0;
}

