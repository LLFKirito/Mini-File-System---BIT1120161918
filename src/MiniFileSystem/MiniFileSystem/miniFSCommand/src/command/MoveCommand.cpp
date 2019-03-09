/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/MoveCommand.h"

bool MoveCommand::Accept(const std::string & str) const
{
	if (str == "move")
		return true;
	else
		return false;
}

bool MoveCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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

	if (argv.size() == 3)
	{
		int exe_res = mfs->moveFile(argv[1].data(), argv[2].data());

		switch (exe_res)
		{
		case 1:
			point->LogLine("�ƶ��ɹ�!");
			return true;
		case -1:
			point->LogLine("��ǰ�ڱ��ƶ��ļ���!");
			return true;
		case -2:
			point->LogLine("�ƶ�ʧ��!");
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

void MoveCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" move ", " �ƶ��ļ�", "more <name1> <name2>");
}

void MoveCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("move", "move <name1> <name2>", "�� move �����ƶ��ļ�",
		"move name1 name2 �� name1 �ļ��ƶ��� name2");
}

MoveCommand::MoveCommand()
{
}
