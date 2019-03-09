/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/LrbCommand.h"

bool LrbCommand::Accept(const std::string & str) const
{
	if (str == "lrb")
		return true;
	else
		return false;
}

bool LrbCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
		point->LogLine("����վ:");
		mfs->showRecycleBin();
		return true;
	}
	else
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}
	return true;
}

void LrbCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" lrb ", " ��ʾ����վ", "lrb");
}

void LrbCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("lrb", "lrb", "�� lrb ������ʾ����վ����", "lrb ��ʾ����վ����");
}

LrbCommand::LrbCommand()
{
}
