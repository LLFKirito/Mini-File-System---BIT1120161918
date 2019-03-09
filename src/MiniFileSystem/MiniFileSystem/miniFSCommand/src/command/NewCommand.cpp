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
		std::regex reg_pattern_space_size("^\\+?[1-9][0-9]*$"); // 空间大小必须为非0整数
		if (!std::regex_match(argv[2], reg_pattern_space_size))
		{
			point->LogLine(argv[2], " 不是非零整数!");
			return true;
		}
		int space_size = atoi(argv[2].data());
		if (space_size < 16 || space_size > 4194304)
		{
			point->LogLine(argv[2], " 不是小于 16 或大于 4194304的整数!");
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

		if (f_pos == f_size_pos || f_pos == 3) // -f参数必须出现且不可能是最后一个
		{
			point->LogLine(argv[0], " 指令参数错误!");
			return true;
		}

		std::regex reg_pattern_cluster_size("^\\+?[1-9][0-9]*$");
		if (!std::regex_match(argv[f_size_pos], reg_pattern_cluster_size))
		{
			point->LogLine(argv[f_size_pos], " 不是非零整数!");
			return true;
		}

		int cluster_size = atoi(argv[f_size_pos].data());
		if ((cluster_size & (cluster_size - 1)) || cluster_size > 64)
		{
			point->LogLine(argv[f_size_pos], " 不是建议参数!(应{x|1<=x<=64 且 x为2的幂次方})");
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

		if (f_pos == 1 || f_pos == 4 || f_pos == f_size_pos) // 5参数下，空间名默认在第二位
		{
			point->LogLine(argv[0], " 指令参数错误!");
			return true;
		}

		std::regex reg_pattern_cluster_size("^\\+?[1-9][0-9]*$");
		if (!std::regex_match(argv[f_size_pos], reg_pattern_cluster_size))
		{
			point->LogLine(argv[f_size_pos], " 不是非零整数!");
			return true;
		}

		int cluster_size = atoi(argv[f_size_pos].data());
		if ((cluster_size & (cluster_size - 1)) || cluster_size > 64)
		{
			point->LogLine(argv[f_size_pos], " 不是建议参数!(应{x|1<=x<=64 且 x为2的幂次方})");
			return true;
		}

		int space_size_pos = 2 ^ 3 ^ 4 ^ f_pos ^ f_size_pos;
		std::regex reg_pattern_space_size("^\\+?[1-9][0-9]*$"); // 空间大小必须为非0整数
		if (!std::regex_match(argv[space_size_pos], reg_pattern_space_size))
		{
			point->LogLine(argv[space_size_pos], " 不是非零整数!");
			return true;
		}

		int space_size = atoi(argv[space_size_pos].data());
		if (space_size < 64 || space_size > 4194304)
		{
			point->LogLine(argv[space_size_pos], " 不是小于 64 或大于 4194304的数!");
			return true;
		}

		exe_res = mfs->createSpace(argv[1].data(), (uint_32)space_size, (uint_32)cluster_size);
	}
	else
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}

	switch (exe_res)
	{
	case 1:
		point->LogLine("空间创建成功!");
		break;
	case -1:
		point->LogLine("空间路径错误!");
		break;
	case -2:
		point->LogLine("空间名长度超限!");
		break;
	case -3:
		point->LogLine("空间名重复!");
		break;
	default:
		point->LogLine("空间创建失败!");
		break;
	}
	return true;
}

void NewCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" new ", " 创建空间", "new <name> [size(可选)]");
}

void NewCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("new", "new <name> [size(可选)]",
		"用 new 命令建立一个新的 Mini-FS 空间",
		"new SName 建立一个名为 SName 大小为 1G 的 Windows 文件作为存储空间 \n\t  new SName 2048 建立一个名为 SName 大小为 2G 的 Windows 文件作为存储空间");
}

NewCommand::NewCommand()
{
}
