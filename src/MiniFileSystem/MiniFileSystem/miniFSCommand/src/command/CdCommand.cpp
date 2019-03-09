/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/CdCommand.h"

bool CdCommand::Accept(const std::string & str) const
{
	if (str == "cd")
		return true;
	else
		return false;
}

bool CdCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
		point->LogLine(argv[0]," 指令参数错误!");
		return true;
	}

	int exe_res = mfs->changeDirectory(argv[1].data());

	if (exe_res == -1)
	{
		point->LogLine("当前文件夹下不存在 ", argv[1], " 文件(夹)!");
		return true;
	}
	return true;
}

void CdCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" cd ", " 目录转移", "cd <dirName>");
}

void CdCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("cd", "cd <dirName>", "用 cd 命令进行位置转移", "cd dirName 移动到 dirName 目录下");
}

CdCommand::CdCommand()
{
}
