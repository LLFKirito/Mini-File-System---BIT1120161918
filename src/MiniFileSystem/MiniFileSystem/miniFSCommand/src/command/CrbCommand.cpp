/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/CrbCommand.h"

bool CrbCommand::Accept(const std::string & str) const
{
	if (str == "crb")
		return true;
	else
		return false;
}

bool CrbCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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

	if (argv.size() == 1)
	{
		mfs->emptyRecycleBin();
		point->LogLine("回收站清空完毕!");
		return true;
	}
	else
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}
	return true;
}

void CrbCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" crb ", " 清空回收站", "crb");
}

void CrbCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("crb", "crb", "用 crb 命令清空回收站", "crb 清空回收站");
}

CrbCommand::CrbCommand()
{
}
