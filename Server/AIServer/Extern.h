#pragma once

extern bool	g_bNpcExit;

struct _MAKE_WEAPON
{
	uint8_t	byIndex;
	uint16_t	sClass[MAX_UPGRADE_WEAPON];
	_MAKE_WEAPON() { memset(&sClass, 0, sizeof(sClass)); }
};

struct _MAKE_ITEM_GRADE_CODE
{
	uint8_t	byItemIndex;		// item grade
	uint16_t	sGrade[9];
};	

struct _MAKE_ITEM_LARE_CODE
{
	uint8_t	byItemLevel;
	uint16_t	sLareItem;
	uint16_t	sMagicItem;
	uint16_t	sGeneralItem;
};

struct _K_MONSTER_ITEM
{
	uint16_t sIndex;
	uint32_t iItem[5];
	uint16_t sPercent[5];

	_K_MONSTER_ITEM()
	{
		memset(&iItem, 0, sizeof(iItem));
		memset(&sPercent, 0, sizeof(sPercent));
	}
};

struct _MAKE_ITEM_GROUP
{
	uint32_t	iItemGroupNum;
	std::vector<uint32_t> iItems;
};

struct _NPC_LIVE_TIME
{
	uint16_t nIndex;
	int16_t SocketID;
	uint16_t Nid;
	uint16_t Duration;
	int32_t SpawnedTime;

};

#include "../shared/database/structs.h"