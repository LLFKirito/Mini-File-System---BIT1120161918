/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/WriteCommand.h"

bool WriteCommand::Accept(const std::string & str) const
{
	if (str == "write")
		return true;
	else
		return false;
}

bool WriteCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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

	int exe_res = mfs->writeFile();

	if (exe_res == -1)
	{
		point->LogLine("��ǰδ�������ļ�, ���ȴ������ļ���������!");
		return true;
	}
	return true;
}

void WriteCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" write ", " ���ļ�д������", "write");
}

void WriteCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("write", "write", "�� write ָ����ǰ�Ѿ��򿪵��ļ�д������", "write ���Ѿ��򿪵��ļ�д������");
}

WriteCommand::WriteCommand()
{
}
