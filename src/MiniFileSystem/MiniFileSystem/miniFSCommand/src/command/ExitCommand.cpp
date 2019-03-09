/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/ExitCommand.h"

bool ExitCommand::Accept(const std::string & str) const
{
	if (str == "exit")
		return true;
	else
		return false;
}

bool ExitCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
{
	MFSConsole * point = MFSConsole::getInstance();
	if (argv.size() != 1)
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}

	if (mfs->isLoadSpace())
	{
		mfs->closeSpace();
	}
	return false;
}

void ExitCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" exit ", " �˳�����", "exit");
}

void ExitCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("exit", "exit", "�� exit �����˳�����", "exit �˳�����");
}

ExitCommand::ExitCommand()
{
}
