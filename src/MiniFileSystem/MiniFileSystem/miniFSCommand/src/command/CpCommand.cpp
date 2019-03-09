/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "../../include/command/CpCommand.h"

bool CpCommand::Accept(const std::string & str) const
{
	if (str == "cp")
		return true;
	else
		return false;
}

bool CpCommand::Action(MiniFileSystem * mfs, const std::vector<std::string>& argv) const
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
	if (argv.size() != 3)
	{
		point->LogLine(argv[0], " 指令参数错误!");
		return true;
	}

	int exe_res = mfs->copyFile(argv[1].data(), argv[2].data());

	if (exe_res == -1)
	{
		point->LogLine("路径不存在!");
		return true;
	}
	else if (exe_res == 1)
	{
		point->LogLine("成功拷贝文件!");
		return true;
	}
	return true;
}

void CpCommand::OutlineHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogOutLineInfo(" cp ", " 拷贝文件", "cp <name_old> <name_new>");
}

void CpCommand::DetailHelp() const
{
	MFSConsole * point = MFSConsole::getInstance();
	point->LogDetailInfo("cp", "cp <name1> <name2>", "用 cp 命令拷贝文件",
		"cp wFName sFName 将文件从 Windows 空间中拷贝至 Mini-FS 空间中 \n\t   cp sFName wFName 将文件从 Mini-FS 空间中拷贝至 Windows 空间中 \n\t   cp sFName1 sFName2 将文件从 Mini-FS 空间中拷贝至 Mini-FS 空间中");
}

CpCommand::CpCommand()
{
}
