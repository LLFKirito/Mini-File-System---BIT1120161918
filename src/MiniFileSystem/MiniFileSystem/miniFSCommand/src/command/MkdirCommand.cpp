/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/MkdirCommand.h"

bool MkdirCommand::Accept(const std::string & str) const
{
	if (str == "mkdir")
		return true;
	else
		return false;
}

bool MkdirCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
	if (argv[1].size() >= 24)
	{
		point->LogLine("创建 ", argv[1], " 失败, 文件夹名超过23字节!");
		return true;
	}
	
	int exe_res = mfs->makeDir(argv[1].data());

	if (exe_res == -1)
	{
		point->LogLine("创建 ", argv[1], " 失败, 当前文件夹下有同名文件夹!");
		return true;
	}
	return true;
}

void MkdirCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" mkdir ", " 新建文件夹", "mkdir <dirName>");
}

void MkdirCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("mkdir", "mkdir <dirName>", "用 mkdir 命令新建文件夹",
		"mkdir Name 新建名为 Name 的文件夹");
}

MkdirCommand::MkdirCommand()
{
}
