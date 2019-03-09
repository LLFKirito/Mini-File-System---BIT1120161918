/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/RmdirCommand.h"

bool RmdirCommand::Accept(const std::string & str) const
{
	if (str == "rmdir")
		return true;
	else
		return false;
}

bool RmdirCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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

	int exe_res = mfs->deleteFile(argv[1].data());

	if (exe_res == -1)
	{
		point->LogLine("ɾ�� ", argv[1], " �ļ�(��)ʧ��!");
		return true;
	}
	if (exe_res == 1)
	{
		point->LogLine("ɾ�� ", argv[1], " �ļ�(��)�ɹ�!");
		return true;
	}
	return true;
}

void RmdirCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" rmdir ", " ɾ���ļ���", "rmdir <dirName>");
}

void RmdirCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("rmdir", "rmdir <dirName>", "�� rmdir �������λ��ת��", "rmdir dirName ɾ�� dirName Ŀ¼�������ļ�");
}

RmdirCommand::RmdirCommand()
{
}
