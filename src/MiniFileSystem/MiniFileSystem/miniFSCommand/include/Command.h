/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#pragma once
#ifndef __MFS_COMMANDS_H__
#define __MFS_COMMANDS_H__


#include <string>
#include <vector>
#include "../../miniFSCore/include/mfs_coresystem.h"
#include "../include/io/mfs_Console.h"


class MFSCommand
{
public:
	virtual bool Accept(const std::string & str) const = 0;
	virtual bool Action(MiniFileSystem * MFS, const std::vector<std::string> & argv) const = 0;
	virtual void OutlineHelp() const = 0;
	virtual void DetailHelp() const = 0;

protected:
	MFSCommand() = default;
};


#endif
