/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/WriteCommand.h"

bool WriteCommand::Accept(const std::string & str) const
{
	if (str == "write")
		return true;
	else
		return false;
}

bool WriteCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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

	int exe_res = mfs->writeFile();

	if (exe_res == -1)
	{
		point->LogLine("当前未打开数据文件, 请先打开数据文件再做操作!");
		return true;
	}
	return true;
}

void WriteCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" write ", " 向文件写入内容", "write");
}

void WriteCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("write", "write", "用 write 指令向当前已经打开的文件写入内容", "write 向已经打开的文件写入内容");
}

WriteCommand::WriteCommand()
{
}
