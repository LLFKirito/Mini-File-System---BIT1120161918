/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/AttCommand.h"

bool AttCommand::Accept(const std::string & str) const
{
	if (str == "att")
		return true;
	else
		return false;
}

bool AttCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
		int exe_res = mfs->showAttribute(".");

		if (exe_res == -1)
		{
			point->LogLine("无此文件!");
		}
	}
	else if (argv.size() == 2)
	{
		int exe_res = mfs->showAttribute(argv[1].data());

		if (exe_res == -1)
		{
			point->LogLine("无此文件!");
		}
	}
	else
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}
	return true;
}

void AttCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" att ", " 显示文件属性", "att <name>");
}

void AttCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("att", "att <name>", "用 att 命令显示文本属性",
		"att sFName 显示名为 sFName 的文件的文件属性(文件名、拓展名、创建时间、文件大小)");
}

AttCommand::AttCommand()
{
}
