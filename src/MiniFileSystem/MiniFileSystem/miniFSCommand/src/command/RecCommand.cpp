/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/RecCommand.h"

bool RecCommand::Accept(const std::string & str) const
{
	if (str == "rec")
		return true;
	else
		return false;
}

bool RecCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
		int exe_res = mfs->recoverFile(argv[1].data());

		if (exe_res != 1)
		{
			point->LogLine("无此文件!");
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

void RecCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" rec ", " 恢复文件(夹)", "rec <name>");
}

void RecCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("rec", "rec <name>", "用 rec 命令恢复文件(夹)", "rec name 恢复 name 文件(夹)");
}

RecCommand::RecCommand()
{
}
