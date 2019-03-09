/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/CloseCommand.h"

bool CloseCommand::Accept(const std::string & str) const
{
	if (str == "close")
		return true;
	else
		return false;
}

bool CloseCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
{
	MFSConsole * point = MFSConsole::getInstance();
	if (!mfs->isLoadSpace())
	{
		point->LogLine("当前未挂载空间!");
		return true;
	}
	if (!mfs->isLoadFile())
	{
		point->LogLine("当前未打开某数据文件!");
		return true;
	}
	if (argv.size() != 1)
	{
		point->LogLine(argv[0]," 指令参数错误!");
		return true;
	}

	int exe_res = mfs->closeFile();

	if (exe_res == -1)
	{
		point->LogLine("文件未被打开!");
		return true;
	}
	return true;
}

void CloseCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" close ", " 关闭已经打开的文件", "close");
}

void CloseCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("close", "close", "用 close 命令关闭已经打开的文件",
		"close 关闭当前已经打开的文件");
}

CloseCommand::CloseCommand()
{
}
