/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/LsCommand.h"

bool LsCommand::Accept(const std::string & str) const
{
	if (str == "ls" || str == "dr")
		return true;
	else
		return false;
}

bool LsCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
	if (argv.size() != 1)
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}
	
	mfs->showDirectory();
	return true;
}

void LsCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" ls ", " 显示目录", "ls");
}

void LsCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("ls", "ls", "ls 显示当前目录下的文件目录", "ls 显示当前目录下所有文件及文件夹");
}

LsCommand::LsCommand()
{
}
