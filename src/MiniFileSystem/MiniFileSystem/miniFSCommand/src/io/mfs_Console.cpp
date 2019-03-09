/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/io/mfs_Console.h"
#include <iostream>
#include <Windows.h>


MFSConsole * MFSConsole::uniqueInstance = NULL;

MFSConsole::MFSConsole()
{
}

MFSConsole::~MFSConsole()
{
	if (uniqueInstance == NULL)
	{
		return;
	}
	delete uniqueInstance;
	uniqueInstance = NULL;
}

MFSConsole * MFSConsole::getInstance()
{
	if (uniqueInstance == NULL)
	{
		uniqueInstance = new MFSConsole;
	}
	return uniqueInstance;
}

void MFSConsole::setColor(MFSColors color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color);
}

void MFSConsole::Log(std::string str1)
{
	std::cout << str1;
}

void MFSConsole::Log(const std::string str1, const std::string str2)
{
	std::cout << str1 << str2;
}

void MFSConsole::LogLine(std::string str1)
{
	std::cout << " " << str1 << std::endl;
}

void MFSConsole::LogLine(const std::string str1, const std::string str2)
{
	std::cout << " " << str1 << str2 << std::endl;
}

void MFSConsole::LogLine(const std::string str1, const std::string str2,
						const std::string str3)
{
	std::cout << " " << str1 << str2 << str3 << std::endl;
}

void MFSConsole::LogOutLineInfo(const std::string ins, const std::string content,
						const std::string layout)
{
	std::string name = ins + " ";
	std::cout << " ";
	std::cout.setf(std::ios::left);
	std::cout.fill('.');
	std::cout.width(12);
	std::cout << name << " ";
	std::cout.fill(' ');
	std::cout.width(22);
	std::cout << content;
	std::cout << "指令格式: ";
	std::cout.fill(' ');
	std::cout << layout << std::endl;
	std::cout.unsetf(std::ios::left);
}

void MFSConsole::LogDetailInfo(const std::string ins, const std::string layout,
						const std::string content, const std::string example)
{
	std::cout << " " << ins << std::endl;
	std::cout << " 指令格式: " << layout << std::endl;
	std::cout << " 指令含义: " << content << std::endl;
	std::cout << " 指令示例: " << example << std::endl;
}




