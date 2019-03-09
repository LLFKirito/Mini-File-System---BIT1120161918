/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/TreeCommand.h"

bool TreeCommand::Accept(const std::string & str) const
{
	if (str == "tree")
		return true;
	else
		return false;
}

bool TreeCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
		mfs->treeDirectory();
		return true;
	}
	else
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}
	return true;
}

void TreeCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" tree ", " 显示目录树", "tree ");
}

void TreeCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("tree", "tree", "用 tree 命令显示目录树", "tree 显示目录树");
}

TreeCommand::TreeCommand()
{
}
