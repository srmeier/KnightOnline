#pragma once

#include "../shared/globals.h"

#define CONF_LOGIN_SERVER	"./LogInServer.ini"

struct _SERVER_INFO
{
	std::string strServerIP;
	std::string strLanIP;
	std::string strServerName;
	short sUserCount;
	short sServerID;
	short sGroupID;
	short sPlayerCap;
	short sFreePlayerCap;
	std::string strKarusKingName;
	std::string strKarusNotice;
	std::string strElMoradKingName;
	std::string strElMoradNotice;

	_SERVER_INFO()
	{
		sUserCount = sServerID = sGroupID = sPlayerCap = sFreePlayerCap = 0;
	}
};

struct News
{
	uint8 Content[4096];
	size_t Size;
};

struct _VERSION_INFO
{
	uint16 sVersion;
	uint16 sHistoryVersion;
	std::string strFilename;
};