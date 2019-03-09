/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/MapCommand.h"

bool MapCommand::Accept(const std::string & str) const
{
	if (str == "map")
		return true;
	else
		return false;
}

bool MapCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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

	if (argv.size() == 2)
	{
		int exe_res = mfs->occupyCluster(argv[1].data());

		if (exe_res == -1)
		{
			point->LogLine("�����ļ�ʧ��!");
		}
		return true;
	}
	else
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}
	return true;
}

void MapCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" map ", " ��ʾ�ļ�ռ�ÿ��", "map <filename>");
}

void MapCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("map", "map <filename>", "�� map ������ʾ�ļ�ʹ�ÿ��", "map filename ��ʾ��Ϊ filename �ļ���ռ�õ����п��");
}

MapCommand::MapCommand()
{
}
