/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/MoveCommand.h"

bool MoveCommand::Accept(const std::string & str) const
{
	if (str == "move")
		return true;
	else
		return false;
}

bool MoveCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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

	if (argv.size() == 3)
	{
		int exe_res = mfs->moveFile(argv[1].data(), argv[2].data());

		switch (exe_res)
		{
		case 1:
			point->LogLine("移动成功!");
			return true;
		case -1:
			point->LogLine("当前在被移动文件内!");
			return true;
		case -2:
			point->LogLine("移动失败!");
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

void MoveCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" move ", " 移动文件", "more <name1> <name2>");
}

void MoveCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("move", "move <name1> <name2>", "用 move 命令移动文件",
		"move name1 name2 将 name1 文件移动到 name2");
}

MoveCommand::MoveCommand()
{
}
