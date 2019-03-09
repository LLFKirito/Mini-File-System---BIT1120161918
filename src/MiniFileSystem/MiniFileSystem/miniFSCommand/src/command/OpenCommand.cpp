/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/OpenCommand.h"

bool OpenCommand::Accept(const std::string & str) const
{
	if (str == "open")
		return true;
	else
		return false;
}

bool OpenCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
	if (argv.size() != 2)
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}

	int exe_res = mfs->openFile(argv[1].data());

	if (exe_res == -1)
	{
		point->LogLine("打开 ", argv[1], " 失败!");
		return true;
	}
	if (exe_res == -2)
	{
		point->LogLine("打开 ", argv[1], " 失败, 当前已经打开某数据文件!");
		return true;
	}
	if (exe_res == -3)
	{
		point->LogLine("打开 ", argv[1], " 失败, 当前目录下没有该文件!");
		return true;
	}
	if (exe_res == 1)
	{
		point->LogLine("打开 ", argv[1], " 成功!");
		return true;
	}
	return true;
}

void OpenCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" open ", " 打开文件", "open <dirName>");
}

void OpenCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("open", "open <name>", "用 open 命令打开文件", "open Name 打开Name文件");
}

OpenCommand::OpenCommand()
{
}
