/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/UnsfsCommand.h"

bool UnsfsCommand::Accept(const std::string & str) const
{
	if (str == "unsfs")
		return true;
	else
		return false;
}

bool UnsfsCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
{
	MFSConsole * point = MFSConsole::getInstance();
	if (!mfs->isLoadSpace())
	{
		point->LogLine("��ǰδ���ؿռ�!");
		return true;
	}
	if (argv.size() != 1)
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}

	int exe_res = mfs->closeSpace();

	if (exe_res == -1)
	{
		point->LogLine("�ռ�δ����!");
		return true;
	}
	return true;
}

void UnsfsCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" unsfs ", " �˳��ռ�", "unsfs");
}

void UnsfsCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("unsfs", "unsfs", "�� unsfs �����˳� Mini-FS", "unsfs �˳���ǰ�ռ�");
}

UnsfsCommand::UnsfsCommand()
{
}
