/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/RecCommand.h"

bool RecCommand::Accept(const std::string & str) const
{
	if (str == "rec")
		return true;
	else
		return false;
}

bool RecCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
		int exe_res = mfs->recoverFile(argv[1].data());

		if (exe_res != 1)
		{
			point->LogLine("�޴��ļ�!");
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

void RecCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" rec ", " �ָ��ļ�(��)", "rec <name>");
}

void RecCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("rec", "rec <name>", "�� rec ����ָ��ļ�(��)", "rec name �ָ� name �ļ�(��)");
}

RecCommand::RecCommand()
{
}
