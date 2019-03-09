/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/

#include "../../include/command/CreateCommand.h"

bool CreateCommand::Accept(const std::string & str) const
{
	if (str == "create" || str == "touch")
		return true;
	else
		return false;
}

bool CreateCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
	if (argv[1].size() >= 24)
	{
		point->LogLine("���� ", argv[1], " ʧ��, �ļ�������23�ֽ�!");
		return true;
	}

	int exe_res = mfs->createFile(argv[1].data());

	if (exe_res == -1)
	{
		point->LogLine("���� ", argv[1], " ʧ��, ��ǰ�ļ�������ͬ���ļ�!");
		return true;
	}
	return true;
}

void CreateCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" create ", " �½��ļ�", "create <filename>");
}

void CreateCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("create", "create <fileName>", "�� create �����½��ļ�",
		"create fileName �½���Ϊ fileName ���ļ�");
}

CreateCommand::CreateCommand()
{
}
