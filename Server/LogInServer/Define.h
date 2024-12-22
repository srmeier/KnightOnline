#pragma once

#include "../shared/globals.h"

#define CONF_LOGIN_SERVER	"./LogInServer.ini"

struct _SERVER_INFO
{
	std::string strServerIP;
	std::string strServerName;
	int16_t sUserCount;
	int16_t sServerID;
	int16_t sPlayerCap;

	_SERVER_INFO()
	{
		sUserCount = sServerID = sPlayerCap = 0;
	}
};

struct News
{
	uint8_t Content[4096];
	size_t Size;
};

struct _VERSION_INFO
{
	uint16_t sVersion;
	uint16_t sHistoryVersion;
	std::string strFilename;
};
