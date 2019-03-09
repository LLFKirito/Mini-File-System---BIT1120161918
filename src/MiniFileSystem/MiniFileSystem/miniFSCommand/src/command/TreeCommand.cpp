/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/TreeCommand.h"

bool TreeCommand::Accept(const std::string & str) const
{
	if (str == "tree")
		return true;
	else
		return false;
}

bool TreeCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
{
	MFSConsole * point = MFSConsole::getInstance();
	if (!mfs->isLoadSpace())
	{
		point->LogLine("��ǰδ���ؿռ�!");
		return true;
	}
	if (mfs->isLoadFile())
	{
		point->LogLine("��ǰ�Ѿ���ĳ�����ļ�, ��ʹ��closeָ��رո��ļ���������!");
		return true;
	}

	if (argv.size() == 1)
	{
		mfs->treeDirectory();
		return true;
	}
	else
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}
	return true;
}

void TreeCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" tree ", " ��ʾĿ¼��", "tree ");
}

void TreeCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("tree", "tree", "�� tree ������ʾĿ¼��", "tree ��ʾĿ¼��");
}

TreeCommand::TreeCommand()
{
}
