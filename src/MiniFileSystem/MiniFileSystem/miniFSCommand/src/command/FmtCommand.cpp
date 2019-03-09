/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/FmtCommand.h"
#include <regex>

bool FmtCommand::Accept(const std::string & str) const
{
	if (str == "fmt")
		return true;
	else
		return false;
}

bool FmtCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
		mfs->formatSpace();
		point->LogLine("��ʽ���ɹ�!");
	}
	else if (argv.size() == 2)
	{
		std::regex reg_pattern_size("^\\+?[1-9][0-9]*$"); // ��0����
		if (!std::regex_match(argv[1], reg_pattern_size))
		{
			point->LogLine(argv[1], " ���Ƿ�������!");
			return true;
		}

		int cluster_size = atoi(argv[1].data());
		if ((cluster_size & (cluster_size - 1)) || cluster_size > 64)
		{
			point->LogLine(argv[1], " ���ǺϷ�����!(Ӧ{x|1<=x<=64 �� xΪ2���ݴη�})!");
			return true;
		}

		mfs->formatSpace((uint_32)cluster_size);
		point->LogLine("��ʽ���ɹ�!");
	}
	else
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}
	return true;
}

void FmtCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" fmt ", " ��ʽ���ռ�", "fmt [size(��ѡ)]");
}

void FmtCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("fmt", "fmt [size(��ѡ)]", "�� fmt �����ʽ����ǰ�Ŀռ�",
		"fmt ��ʼ����ǰ�򿪵� SName �ռ䣬�ش�С����Ϊ 4KB \n\t  fmt 8 ��ʼ����ǰ�򿪵� SName �ռ䣬�ش�С����Ϊ 8KB");
}

FmtCommand::FmtCommand()
{
}
