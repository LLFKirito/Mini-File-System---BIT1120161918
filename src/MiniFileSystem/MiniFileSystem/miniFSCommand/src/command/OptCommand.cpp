/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/OptCommand.h"

bool OptCommand::Accept(const std::string & str) const
{
	if (str == "opt")
		return true;
	else
		return false;
}

bool OptCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
		mfs->optimization();
		point->LogLine("\n 磁盘整理完成!");
		return true;
	}
	else
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}
	return true;
}

void OptCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" opt ", " 整理空间碎片", "opt");
}

void OptCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("opt", "opt", "用 opt 命令优化当前空间，将文件尽可能连续存放", "opt 整理空间碎片");
}

OptCommand::OptCommand()
{
}
