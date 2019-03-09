/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/DeleteCommand.h"

bool DeleteCommand::Accept(const std::string & str) const
{
	if (str == "delete" || str == "dl")
		return true;
	else
		return false;
}

bool DeleteCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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

void DeleteCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" delete ", " 删除文件", "delete <name>");
}

void DeleteCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("delete", "delete <name>", "用 delete 命令删除空间中的文件",
		"delete sFName 将文件 sFName 从空间中删除");
}

DeleteCommand::DeleteCommand()
{
}
