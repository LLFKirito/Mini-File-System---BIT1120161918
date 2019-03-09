/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/MkdirCommand.h"

bool MkdirCommand::Accept(const std::string & str) const
{
	if (str == "mkdir")
		return true;
	else
		return false;
}

bool MkdirCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}
	if (argv[1].size() >= 24)
	{
		point->LogLine("���� ", argv[1], " ʧ��, �ļ���������23�ֽ�!");
		return true;
	}
	
	int exe_res = mfs->makeDir(argv[1].data());

	if (exe_res == -1)
	{
		point->LogLine("���� ", argv[1], " ʧ��, ��ǰ�ļ�������ͬ���ļ���!");
		return true;
	}
	return true;
}

void MkdirCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" mkdir ", " �½��ļ���", "mkdir <dirName>");
}

void MkdirCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("mkdir", "mkdir <dirName>", "�� mkdir �����½��ļ���",
		"mkdir Name �½���Ϊ Name ���ļ���");
}

MkdirCommand::MkdirCommand()
{
}
