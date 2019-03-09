/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#include "..\include\mfs_ResponsibilityChain.h"
#include "..\include\mfs_Commands.h"


CommandChain * CommandChain::uniqueInstance = NULL;

CommandChain::CommandChain()
{
	responsibilityChain.clear();

	// ÷∏¡Ó‘»Œ¡¥ 
	responsibilityChain.push_back(new AttCommand);
	responsibilityChain.push_back(new CdCommand);
	responsibilityChain.push_back(new CloseCommand);
	responsibilityChain.push_back(new CpCommand);
	responsibilityChain.push_back(new CrbCommand);
	responsibilityChain.push_back(new CreateCommand);
	responsibilityChain.push_back(new DeleteCommand);
	responsibilityChain.push_back(new ExitCommand);
	responsibilityChain.push_back(new FdCommand);
	responsibilityChain.push_back(new FmtCommand);
	responsibilityChain.push_back(new HelpCommand);
	responsibilityChain.push_back(new LrbCommand);
	responsibilityChain.push_back(new LsCommand);
	responsibilityChain.push_back(new MapCommand);
	responsibilityChain.push_back(new MkdirCommand);
	responsibilityChain.push_back(new MoveCommand);
	responsibilityChain.push_back(new NewCommand);
	responsibilityChain.push_back(new OpenCommand);
	responsibilityChain.push_back(new OptCommand);
	responsibilityChain.push_back(new ReadCommand);
	responsibilityChain.push_back(new RecCommand);
	responsibilityChain.push_back(new RmdirCommand);
	responsibilityChain.push_back(new SfsCommand);
	responsibilityChain.push_back(new TreeCommand);
	responsibilityChain.push_back(new UnsfsCommand);
	responsibilityChain.push_back(new WriteCommand);
}

CommandChain::~CommandChain()
{
	if (uniqueInstance == NULL)
	{
		return;
	}
	for (CommandList::iterator it = begin(); it != end(); it++)
	{
		delete (*it);
	}
	responsibilityChain.clear();
	delete uniqueInstance;
	uniqueInstance = NULL;
}

CommandChain * CommandChain::getInstance()
{
	if (uniqueInstance == NULL)
	{
		uniqueInstance = new CommandChain;
	}
	return uniqueInstance;
}

CommandList::iterator CommandChain::begin()
{
	return responsibilityChain.begin();
}

CommandList::iterator CommandChain::end()
{
	return responsibilityChain.end();
}

