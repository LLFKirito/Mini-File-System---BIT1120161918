/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/ReadCommand.h"

bool ReadCommand::Accept(const std::string & str) const
{
	if (str == "read")
		return true;
	else
		return false;
}

bool ReadCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
{
	MFSConsole * point = MFSConsole::getInstance();
	if (!mfs->isLoadSpace())
	{
		point->LogLine("��ǰδ���ؿռ�!");
		return true;
	}
	if (!mfs->isLoadFile())
	{
		point->LogLine("��ǰδ�������ļ�, ���ȴ������ļ���������!");
		return true;
	}
	if (argv.size() != 1)
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}

	int exe_res = mfs->readFile();

	if (exe_res == -1)
	{
		point->LogLine("��ǰδ�������ļ�, ���ȴ������ļ���������!");
		return true;
	}
	return true;
}

void ReadCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" read ", " ���ļ�", "read");
}

void ReadCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("read", "read", "�� read �����ȡ�Ѿ��򿪵��ļ�", "read ��ȡ�Ѿ��򿪵��ļ�");
}

ReadCommand::ReadCommand()
{
}
