/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/CpCommand.h"

bool CpCommand::Accept(const std::string & str) const
{
	if (str == "cp")
		return true;
	else
		return false;
}

bool CpCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
	if (argv.size() != 3)
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}

	int exe_res = mfs->copyFile(argv[1].data(), argv[2].data());

	if (exe_res == -1)
	{
		point->LogLine("·��������!");
		return true;
	}
	else if (exe_res == 1)
	{
		point->LogLine("�ɹ������ļ�!");
		return true;
	}
	return true;
}

void CpCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" cp ", " �����ļ�", "cp <name_old> <name_new>");
}

void CpCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("cp", "cp <name1> <name2>", "�� cp ������ļ�",
		"cp wFName sFName ���ļ��� Windows �ռ��п����� Mini-FS �ռ��� \n\t   cp sFName wFName ���ļ��� Mini-FS �ռ��п����� Windows �ռ��� \n\t   cp sFName1 sFName2 ���ļ��� Mini-FS �ռ��п����� Mini-FS �ռ���");
}

CpCommand::CpCommand()
{
}
