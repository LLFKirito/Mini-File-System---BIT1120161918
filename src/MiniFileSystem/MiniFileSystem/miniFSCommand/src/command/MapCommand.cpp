/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/MapCommand.h"

bool MapCommand::Accept(const std::string & str) const
{
	if (str == "map")
		return true;
	else
		return false;
}

bool MapCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
		int exe_res = mfs->occupyCluster(argv[1].data());

		if (exe_res == -1)
		{
			point->LogLine("查找文件失败!");
		}
		return true;
	}
	else
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}
	return true;
}

void MapCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" map ", " 显示文件占用块号", "map <filename>");
}

void MapCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("map", "map <filename>", "用 map 命令显示文件使用块号", "map filename 显示名为 filename 文件的占用的所有块号");
}

MapCommand::MapCommand()
{
}
