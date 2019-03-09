/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/NewCommand.h"
#include <regex>

bool NewCommand::Accept(const std::string & str) const
{
	if (str == "new")
		return true;
	else
		return false;
}

bool NewCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
{
	MFSConsole * point = MFSConsole::getInstance();
	int exe_res;

	if (argv.size() == 2)
	{
		exe_res = mfs->createSpace(argv[1].data(), (uint_32)1024, (uint_32)4);
	}
	else if (argv.size() == 3)
	{
		std::regex reg_pattern_space_size("^\\+?[1-9][0-9]*$"); // �ռ��С����Ϊ��0����
		if (!std::regex_match(argv[2], reg_pattern_space_size))
		{
			point->LogLine(argv[2], " ���Ƿ�������!");
			return true;
		}
		int space_size = atoi(argv[2].data());
		if (space_size < 16 || space_size > 4194304)
		{
			point->LogLine(argv[2], " ����С�� 16 ����� 4194304������!");
			return true;
		}
		exe_res = mfs->createSpace(argv[1].data(), (uint_32)space_size, (uint_32)4);
	}
	else if (argv.size() == 4)
	{
		int f_pos = 0, f_size_pos = 0;
		for (int i = 1; i < 4; i++)
		{
			if (argv[i] == "-f")
			{
				f_pos = i;
				f_size_pos = i + 1;
				break;
			}
		}

		if (f_pos == f_size_pos || f_pos == 3) // -f������������Ҳ����������һ��
		{
			point->LogLine(argv[0], " ָ���������!");
			return true;
		}

		std::regex reg_pattern_cluster_size("^\\+?[1-9][0-9]*$");
		if (!std::regex_match(argv[f_size_pos], reg_pattern_cluster_size))
		{
			point->LogLine(argv[f_size_pos], " ���Ƿ�������!");
			return true;
		}

		int cluster_size = atoi(argv[f_size_pos].data());
		if ((cluster_size & (cluster_size - 1)) || cluster_size > 64)
		{
			point->LogLine(argv[f_size_pos], " ���ǽ������!(Ӧ{x|1<=x<=64 �� xΪ2���ݴη�})");
			return true;
		}

		int file_pos = 1 ^ 2 ^ 3 ^ f_pos ^ f_size_pos;
		exe_res = mfs->createSpace(argv[file_pos].data(), (uint_32)1024, (uint_32)cluster_size);
	}
	else if (argv.size() == 5)
	{
		int f_pos = 0, f_size_pos = 0;
		for (int i = 1; i < 5; i++)
		{
			if (argv[i] == "-f")
			{
				f_pos = i;
				f_size_pos = i + 1;
				break;
			}
		}

		if (f_pos == 1 || f_pos == 4 || f_pos == f_size_pos) // 5�����£��ռ���Ĭ���ڵڶ�λ
		{
			point->LogLine(argv[0], " ָ���������!");
			return true;
		}

		std::regex reg_pattern_cluster_size("^\\+?[1-9][0-9]*$");
		if (!std::regex_match(argv[f_size_pos], reg_pattern_cluster_size))
		{
			point->LogLine(argv[f_size_pos], " ���Ƿ�������!");
			return true;
		}

		int cluster_size = atoi(argv[f_size_pos].data());
		if ((cluster_size & (cluster_size - 1)) || cluster_size > 64)
		{
			point->LogLine(argv[f_size_pos], " ���ǽ������!(Ӧ{x|1<=x<=64 �� xΪ2���ݴη�})");
			return true;
		}

		int space_size_pos = 2 ^ 3 ^ 4 ^ f_pos ^ f_size_pos;
		std::regex reg_pattern_space_size("^\\+?[1-9][0-9]*$"); // �ռ��С����Ϊ��0����
		if (!std::regex_match(argv[space_size_pos], reg_pattern_space_size))
		{
			point->LogLine(argv[space_size_pos], " ���Ƿ�������!");
			return true;
		}

		int space_size = atoi(argv[space_size_pos].data());
		if (space_size < 64 || space_size > 4194304)
		{
			point->LogLine(argv[space_size_pos], " ����С�� 64 ����� 4194304����!");
			return true;
		}

		exe_res = mfs->createSpace(argv[1].data(), (uint_32)space_size, (uint_32)cluster_size);
	}
	else
	{
		point->LogLine(argv[0], " ָ���������!");
		return true;
	}

	switch (exe_res)
	{
	case 1:
		point->LogLine("�ռ䴴���ɹ�!");
		break;
	case -1:
		point->LogLine("�ռ�·������!");
		break;
	case -2:
		point->LogLine("�ռ������ȳ���!");
		break;
	case -3:
		point->LogLine("�ռ����ظ�!");
		break;
	default:
		point->LogLine("�ռ䴴��ʧ��!");
		break;
	}
	return true;
}

void NewCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" new ", " �����ռ�", "new <name> [size(��ѡ)]");
}

void NewCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("new", "new <name> [size(��ѡ)]",
		"�� new �����һ���µ� Mini-FS �ռ�",
		"new SName ����һ����Ϊ SName ��СΪ 1G �� Windows �ļ���Ϊ�洢�ռ� \n\t  new SName 2048 ����һ����Ϊ SName ��СΪ 2G �� Windows �ļ���Ϊ�洢�ռ�");
}

NewCommand::NewCommand()
{
}
