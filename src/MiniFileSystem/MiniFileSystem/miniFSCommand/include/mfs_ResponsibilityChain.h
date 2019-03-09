/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#pragma once
#include "Command.h"
#include "../include/command/CdCommand.h"
#include <list>


typedef std::list<MFSCommand *> CommandList;


class CommandChain
{
public:
	static CommandChain * getInstance();
	CommandList::iterator begin();
	CommandList::iterator end();

private:
	CommandChain();
	~CommandChain();

	static CommandChain * uniqueInstance;
	CommandList responsibilityChain;
};



