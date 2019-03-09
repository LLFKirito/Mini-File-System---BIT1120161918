/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/LsCommand.h"

bool LsCommand::Accept(const std::string & str) const
{
	if (str == "ls" || str == "dr")
		return true;
	else
		return false;
}

bool LsCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
	if (argv.size() != 1)
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}
	
	mfs->showDirectory();
	return true;
}

void LsCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" ls ", " ��ʾĿ¼", "ls");
}

void LsCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("ls", "ls", "ls ��ʾ��ǰĿ¼�µ��ļ�Ŀ¼", "ls ��ʾ��ǰĿ¼�������ļ����ļ���");
}

LsCommand::LsCommand()
{
}
