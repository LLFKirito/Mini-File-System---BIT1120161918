/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/RmdirCommand.h"

bool RmdirCommand::Accept(const std::string & str) const
{
	if (str == "rmdir")
		return true;
	else
		return false;
}

bool RmdirCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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

	int exe_res = mfs->deleteFile(argv[1].data());

	if (exe_res == -1)
	{
		point->LogLine("删除 ", argv[1], " 文件(夹)失败!");
		return true;
	}
	if (exe_res == 1)
	{
		point->LogLine("删除 ", argv[1], " 文件(夹)成功!");
		return true;
	}
	return true;
}

void RmdirCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" rmdir ", " 删除文件夹", "rmdir <dirName>");
}

void RmdirCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("rmdir", "rmdir <dirName>", "用 rmdir 命令进行位置转移", "rmdir dirName 删除 dirName 目录下所有文件");
}

RmdirCommand::RmdirCommand()
{
}
