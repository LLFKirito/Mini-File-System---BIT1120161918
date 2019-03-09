/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/CdCommand.h"

bool CdCommand::Accept(const std::string & str) const
{
	if (str == "cd")
		return true;
	else
		return false;
}

bool CdCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
	if (argv.size() != 2)
	{
		point->LogLine(argv[0]," ָ���������!");
		return true;
	}

	int exe_res = mfs->changeDirectory(argv[1].data());

	if (exe_res == -1)
	{
		point->LogLine("��ǰ�ļ����²����� ", argv[1], " �ļ�(��)!");
		return true;
	}
	return true;
}

void CdCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" cd ", " Ŀ¼ת��", "cd <dirName>");
}

void CdCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("cd", "cd <dirName>", "�� cd �������λ��ת��", "cd dirName �ƶ��� dirName Ŀ¼��");
}

CdCommand::CdCommand()
{
}
