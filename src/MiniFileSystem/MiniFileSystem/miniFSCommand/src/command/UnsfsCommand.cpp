/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/UnsfsCommand.h"

bool UnsfsCommand::Accept(const std::string & str) const
{
	if (str == "unsfs")
		return true;
	else
		return false;
}

bool UnsfsCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
{
	MFSConsole * point = MFSConsole::getInstance();
	if (!mfs->isLoadSpace())
	{
		point->LogLine("当前未挂载空间!");
		return true;
	}
	if (argv.size() != 1)
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}

	int exe_res = mfs->closeSpace();

	if (exe_res == -1)
	{
		point->LogLine("空间未被打开!");
		return true;
	}
	return true;
}

void UnsfsCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" unsfs ", " 退出空间", "unsfs");
}

void UnsfsCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("unsfs", "unsfs", "用 unsfs 命令退出 Mini-FS", "unsfs 退出当前空间");
}

UnsfsCommand::UnsfsCommand()
{
}
