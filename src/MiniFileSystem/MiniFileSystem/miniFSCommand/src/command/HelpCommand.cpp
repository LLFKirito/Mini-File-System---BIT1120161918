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
		point->LogLine(argv[1], " ָ�����ƴ���!");
		return true;
	}
	else
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}
	return true;
}

void HelpCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" help ", " ������Ϣ", "help [������(��ѡ)]");
}

void HelpCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("help", "help [������(��ѡ)]", "�� help �����ȡ������Ϣ",
		"help ��ʾϵͳ���Ƽ����� \n\t   help create ��ʾ create ��ָ���ʽ����ϸ��Ϣ");
}

HelpCommand::HelpCommand()
{
}
