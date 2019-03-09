/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/OpenCommand.h"

bool OpenCommand::Accept(const std::string & str) const
{
	if (str == "open")
		return true;
	else
		return false;
}

bool OpenCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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

	int exe_res = mfs->openFile(argv[1].data());

	if (exe_res == -1)
	{
		point->LogLine("�� ", argv[1], " ʧ��!");
		return true;
	}
	if (exe_res == -2)
	{
		point->LogLine("�� ", argv[1], " ʧ��, ��ǰ�Ѿ���ĳ�����ļ�!");
		return true;
	}
	if (exe_res == -3)
	{
		point->LogLine("�� ", argv[1], " ʧ��, ��ǰĿ¼��û�и��ļ�!");
		return true;
	}
	if (exe_res == 1)
	{
		point->LogLine("�� ", argv[1], " �ɹ�!");
		return true;
	}
	return true;
}

void OpenCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" open ", " ���ļ�", "open <dirName>");
}

void OpenCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("open", "open <name>", "�� open ������ļ�", "open Name ��Name�ļ�");
}

OpenCommand::OpenCommand()
{
}
