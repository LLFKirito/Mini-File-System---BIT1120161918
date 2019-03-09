/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/FdCommand.h"

bool FdCommand::Accept(const std::string & str) const
{
	if (str == "fd")
		return true;
	else
		return false;
}

bool FdCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
{
	MFSConsole * point = MFSConsole::getInstance();
	if (!mfs->isLoadSpace())
	{
		point->LogLine("当前未挂载空间!");
		return true;
	}
	if (mfs->isLoadFile())
	{
		point->LogLine("当前已经打开某数据文件, 请使用close指令关闭该文件再做操作!");
		return true;
	}

	if (argv.size() == 2)
	{
		int exe_res = mfs->findFile(argv[1].data());

		if (exe_res != 1)
		{
			point->LogLine(argv[1], "不存在!");
			return true;
		}
	}
	else
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}
	return true;
}

void FdCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" fd ", " 查找文件(夹)", "fd <name>");
}

void FdCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("fd", "fd <name>", "用 fd 命令查找文件(夹)", "fd name 查找 name 文件(夹)");
}

FdCommand::FdCommand()
{
}
