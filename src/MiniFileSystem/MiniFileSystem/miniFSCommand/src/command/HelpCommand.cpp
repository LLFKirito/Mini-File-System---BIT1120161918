/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/HelpCommand.h"
#include "../../include/mfs_ResponsibilityChain.h"

bool HelpCommand::Accept(const std::string & str) const
{
	if (str == "help")
		return true;
	else
		return false;
}

bool HelpCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
{
	MFSConsole * point = MFSConsole::getInstance();
	if (argv.size() == 1)
	{
		CommandChain * list = CommandChain::getInstance();
		for (CommandList::iterator it = list->begin(); it != list->end(); it++)
		{
			(*it)->OutlineHelp();
		}
		return true;
	}
	else if (argv.size() == 2)
	{
		CommandChain * list = CommandChain::getInstance();
		for (CommandList::iterator it = list->begin(); it != list->end(); it++)
		{
			if ((*it)->Accept(argv[1]))
			{
				(*it)->DetailHelp();
				return true;
			}
		}
		point->LogLine(argv[1], " 指令名称错误!");
		return true;
	}
	else
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}
	return true;
}

void HelpCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" help ", " 帮助信息", "help [命令名(可选)]");
}

void HelpCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("help", "help [命令名(可选)]", "用 help 命令获取帮助信息",
		"help 显示系统名称及含义 \n\t   help create 显示 create 的指令格式等详细信息");
}

HelpCommand::HelpCommand()
{
}
