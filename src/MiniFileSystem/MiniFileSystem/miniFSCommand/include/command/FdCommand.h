/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#pragma once
#include "../Command.h"


class FdCommand :virtual public MFSCommand
{
public:
	bool Accept(const std::string & str) const;
	bool Action(MiniFileSystem * mfs, const std::vector<std::string> & argv) const;
	void OutlineHelp() const;
	void DetailHelp() const;

	FdCommand();
};

