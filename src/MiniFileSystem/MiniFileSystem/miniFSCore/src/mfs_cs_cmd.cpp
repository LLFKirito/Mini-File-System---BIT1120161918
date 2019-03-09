/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../include/mfs_coresystem.h"
#include "../../miniFSCommand/include/io/mfs_Console.h"
#include "../../miniFSCommand/include/mfs_ResponsibilityChain.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <Windows.h>


bool MiniFileSystem::cmd()
{
	MFSConsole * point = MFSConsole::getInstance();

	// 打印路径
	point->setColor(MFSColors::PurpleI);
	point->Log("\n Mini-FS:");
	point->setColor(MFSColors::YellowI);
	for (std::vector<Directory>::iterator it = directory.begin(); it != directory.end(); it++)
		point->Log("\\", (*it).header.name);
	point->setColor(MFSColors::PurpleI);
	point->Log(">");

	// 指令输入与切分
	std::vector<std::string> argv;

	std::string input;
	point->setColor(MFSColors::BlueI);
	std::getline(std::cin, input);
	std::istringstream tmp(input);
	while (tmp >> input)
	{
		argv.push_back(input);
	}

	if (argv.size() == 0)
	{
		return true;
	}

	point->setColor(MFSColors::WhiteI);
	CommandChain * list = CommandChain::getInstance();
	for (CommandList::iterator it = list->begin(); it != list->end(); it++)
	{
		std::string str(argv[0]);
		transform(str.begin(), str.end(), str.begin(), ::tolower);
		if ((*it)->Accept(str))
		{
			return (*it)->Action(this, argv);
		}
	}
	point->LogLine("指令名称错误!");
	return true;
}

