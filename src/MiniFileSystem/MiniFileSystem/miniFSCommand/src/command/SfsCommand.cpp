/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/SfsCommand.h"
#include <fstream>

bool SfsCommand::Accept(const std::string & str) const
{
	if (str == "sfs")
		return true;
	else
		return false;
}

bool SfsCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
{
	MFSConsole * point = MFSConsole::getInstance();
	if (mfs->isLoadSpace())
	{
		point->LogLine("��ǰ�Ѿ������˿ռ�, ��ж�غ��ټ���!");
		return true;
	}
	if (argv.size() != 2)
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}

	std::ifstream ifs(argv[1].c_str());
	if (ifs)
	{
		int exe_res = mfs->mountSpace(argv[1].data());

		if (exe_res == -1)
		{
			point->LogLine(argv[1], " ��ʧ��!");
			return true;
		}
		else if (exe_res == 1)
		{
			point->LogLine(argv[1], " �򿪳ɹ�!");
			return true;
		}
	}
	else
	{
		point->LogLine(argv[1], " �����ڴ�·��!");
		return true;
	}
	
	return true;
}

void SfsCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" sfs ", " ��װ�ռ�", "sfs <name>");
}

void SfsCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("sfs", "sfs <name>", "�� sfs ���װҪ�����Ŀռ�",
		"sfs SName �� Mini-FS �а�װ(��)�ռ� SName��Ϊ������������׼��");
}

SfsCommand::SfsCommand()
{
}
