/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/CloseCommand.h"

bool CloseCommand::Accept(const std::string & str) const
{
	if (str == "close")
		return true;
	else
		return false;
}

bool CloseCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
{
	MFSConsole * point = MFSConsole::getInstance();
	if (!mfs->isLoadSpace())
	{
		point->LogLine("��ǰδ���ؿռ�!");
		return true;
	}
	if (!mfs->isLoadFile())
	{
		point->LogLine("��ǰδ��ĳ�����ļ�!");
		return true;
	}
	if (argv.size() != 1)
	{
		point->LogLine(argv[0]," ָ���������!");
		return true;
	}

	int exe_res = mfs->closeFile();

	if (exe_res == -1)
	{
		point->LogLine("�ļ�δ����!");
		return true;
	}
	return true;
}

void CloseCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" close ", " �ر��Ѿ��򿪵��ļ�", "close");
}

void CloseCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("close", "close", "�� close ����ر��Ѿ��򿪵��ļ�",
		"close �رյ�ǰ�Ѿ��򿪵��ļ�");
}

CloseCommand::CloseCommand()
{
}
