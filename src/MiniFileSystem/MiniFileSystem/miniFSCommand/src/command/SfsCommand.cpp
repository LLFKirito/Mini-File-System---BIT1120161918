/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/SfsCommand.h"
#include <fstream>

bool SfsCommand::Accept(const std::string & str) const
{
	if (str == "sfs")
		return true;
	else
		return false;
}

bool SfsCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
{
	MFSConsole * point = MFSConsole::getInstance();
	if (mfs->isLoadSpace())
	{
		point->LogLine("当前已经挂载了空间, 请卸载后再加载!");
		return true;
	}
	if (argv.size() != 2)
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}

	std::ifstream ifs(argv[1].c_str());
	if (ifs)
	{
		int exe_res = mfs->mountSpace(argv[1].data());

		if (exe_res == -1)
		{
			point->LogLine(argv[1], " 打开失败!");
			return true;
		}
		else if (exe_res == 1)
		{
			point->LogLine(argv[1], " 打开成功!");
			return true;
		}
	}
	else
	{
		point->LogLine(argv[1], " 不存在此路径!");
		return true;
	}
	
	return true;
}

void SfsCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" sfs ", " 安装空间", "sfs <name>");
}

void SfsCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("sfs", "sfs <name>", "用 sfs 命令安装要操作的空间",
		"sfs SName 在 Mini-FS 中安装(打开)空间 SName，为后续操作进行准备");
}

SfsCommand::SfsCommand()
{
}
