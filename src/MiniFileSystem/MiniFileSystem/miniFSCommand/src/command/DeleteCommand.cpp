/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/DeleteCommand.h"

bool DeleteCommand::Accept(const std::string & str) const
{
	if (str == "delete" || str == "dl")
		return true;
	else
		return false;
}

bool DeleteCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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

void DeleteCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" delete ", " ɾ���ļ�", "delete <name>");
}

void DeleteCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("delete", "delete <name>", "�� delete ����ɾ���ռ��е��ļ�",
		"delete sFName ���ļ� sFName �ӿռ���ɾ��");
}

DeleteCommand::DeleteCommand()
{
}
