/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#pragma once
#ifndef __MFS_CONSOLE_H__
#define __MFS_CONSOLE_H__


#include <string>


enum struct MFSColors : unsigned short
{
	Black = 0x0000,
	Blue = 0x0001,
	Green = 0x0002,
	Red = 0x0004,
	Cyan = Blue | Green,
	Yellow = Red | Green,
	Purple = Blue | Red,
	White = Blue | Green | Red,

	Intensity = 0x0008,
	BlueI = Blue | Intensity,
	GreenI = Green | Intensity,
	RedI = Red | Intensity,
	CyanI = Cyan | Intensity,
	YellowI = Yellow | Intensity,
	PurpleI = Purple | Intensity,
	WhiteI = White | Intensity
};



class MFSConsole
{
public:
	static MFSConsole * getInstance();


	void setColor(MFSColors color);

	void Log(const std::string str1);
	void Log(const std::string str1, const std::string str2);
	void LogLine(const std::string str1);
	void LogLine(const std::string str1, const std::string str2);
	void LogLine(const std::string str1, const std::string str2,
							const std::string str3);
	void LogOutLineInfo(const std::string ins, const std::string content, 
							const std::string layout);
	void LogDetailInfo(const std::string ins, const std::string layout,
							const std::string content, const std::string example);


private:
	MFSConsole();
	~MFSConsole();

	static MFSConsole * uniqueInstance;
};




#endif
