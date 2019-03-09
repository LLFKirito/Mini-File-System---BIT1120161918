/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/OptCommand.h"

bool OptCommand::Accept(const std::string & str) const
{
	if (str == "opt")
		return true;
	else
		return false;
}

bool OptCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
		mfs->optimization();
		point->LogLine("\n �����������!");
		return true;
	}
	else
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}
	return true;
}

void OptCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" opt ", " ����ռ���Ƭ", "opt");
}

void OptCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("opt", "opt", "�� opt �����Ż���ǰ�ռ䣬���ļ��������������", "opt ����ռ���Ƭ");
}

OptCommand::OptCommand()
{
}
