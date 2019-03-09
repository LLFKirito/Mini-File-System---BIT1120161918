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
		point->LogLine("当前未挂载空间!");
		return true;
	}
	if (mfs->isLoadFile())
	{
		point->LogLine("当前已经打开某数据文件, 请使用close指令关闭该文件再做操作!");
		return true;
	}

	if (argv.size() == 1)
	{
		mfs->formatSpace();
		point->LogLine("格式化成功!");
	}
	else if (argv.size() == 2)
	{
		std::regex reg_pattern_size("^\\+?[1-9][0-9]*$"); // 非0整数
		if (!std::regex_match(argv[1], reg_pattern_size))
		{
			point->LogLine(argv[1], " 不是非零整数!");
			return true;
		}

		int cluster_size = atoi(argv[1].data());
		if ((cluster_size & (cluster_size - 1)) || cluster_size > 64)
		{
			point->LogLine(argv[1], " 不是合法参数!(应{x|1<=x<=64 且 x为2的幂次方})!");
			return true;
		}

		mfs->formatSpace((uint_32)cluster_size);
		point->LogLine("格式化成功!");
	}
	else
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}
	return true;
}

void FmtCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" fmt ", " 格式化空间", "fmt [size(可选)]");
}

void FmtCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("fmt", "fmt [size(可选)]", "用 fmt 命令格式化当前的空间",
		"fmt 初始化当前打开的 SName 空间，簇大小设置为 4KB \n\t  fmt 8 初始化当前打开的 SName 空间，簇大小设置为 8KB");
}

FmtCommand::FmtCommand()
{
}
