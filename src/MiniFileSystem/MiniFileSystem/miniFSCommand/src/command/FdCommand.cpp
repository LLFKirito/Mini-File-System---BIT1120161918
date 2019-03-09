/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/FdCommand.h"

bool FdCommand::Accept(const std::string & str) const
{
	if (str == "fd")
		return true;
	else
		return false;
}

bool FdCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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

	if (argv.size() == 2)
	{
		int exe_res = mfs->findFile(argv[1].data());

		if (exe_res != 1)
		{
			point->LogLine(argv[1], "������!");
			return true;
		}
	}
	else
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}
	return true;
}

void FdCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" fd ", " �����ļ�(��)", "fd <name>");
}

void FdCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("fd", "fd <name>", "�� fd ��������ļ�(��)", "fd name ���� name �ļ�(��)");
}

FdCommand::FdCommand()
{
}
