/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/AttCommand.h"

bool AttCommand::Accept(const std::string & str) const
{
	if (str == "att")
		return true;
	else
		return false;
}

bool AttCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
		int exe_res = mfs->showAttribute(".");

		if (exe_res == -1)
		{
			point->LogLine("�޴��ļ�!");
		}
	}
	else if (argv.size() == 2)
	{
		int exe_res = mfs->showAttribute(argv[1].data());

		if (exe_res == -1)
		{
			point->LogLine("�޴��ļ�!");
		}
	}
	else
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}
	return true;
}

void AttCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" att ", " ��ʾ�ļ�����", "att <name>");
}

void AttCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("att", "att <name>", "�� att ������ʾ�ı�����",
		"att sFName ��ʾ��Ϊ sFName ���ļ����ļ�����(�ļ�������չ��������ʱ�䡢�ļ���С)");
}

AttCommand::AttCommand()
{
}
