/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/ReadCommand.h"

bool ReadCommand::Accept(const std::string & str) const
{
	if (str == "read")
		return true;
	else
		return false;
}

bool ReadCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
{
	MFSConsole * point = MFSConsole::getInstance();
	if (!mfs->isLoadSpace())
	{
		point->LogLine("当前未挂载空间!");
		return true;
	}
	if (!mfs->isLoadFile())
	{
		point->LogLine("当前未打开数据文件, 请先打开数据文件再做操作!");
		return true;
	}
	if (argv.size() != 1)
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}

	int exe_res = mfs->readFile();

	if (exe_res == -1)
	{
		point->LogLine("当前未打开数据文件, 请先打开数据文件再做操作!");
		return true;
	}
	return true;
}

void ReadCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" read ", " 读文件", "read");
}

void ReadCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("read", "read", "用 read 命令读取已经打开的文件", "read 读取已经打开的文件");
}

ReadCommand::ReadCommand()
{
}
