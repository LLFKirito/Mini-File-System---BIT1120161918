/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/LrbCommand.h"

bool LrbCommand::Accept(const std::string & str) const
{
	if (str == "lrb")
		return true;
	else
		return false;
}

bool LrbCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
		point->LogLine("回收站:");
		mfs->showRecycleBin();
		return true;
	}
	else
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}
	return true;
}

void LrbCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" lrb ", " 显示回收站", "lrb");
}

void LrbCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("lrb", "lrb", "用 lrb 命令显示回收站内容", "lrb 显示回收站内容");
}

LrbCommand::LrbCommand()
{
}
