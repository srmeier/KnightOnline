#pragma once

extern bool	g_bNpcExit;

struct _MAKE_WEAPON
{
	uint8	byIndex;
	uint16	sClass[MAX_UPGRADE_WEAPON];
	_MAKE_WEAPON() { memset(&sClass, 0, sizeof(sClass)); }
};

struct _MAKE_ITEM_GRADE_CODE
{
	uint8	byItemIndex;		// item grade
	uint16	sGrade[9];
};	

struct _MAKE_ITEM_LARE_CODE
{
	uint8	byItemLevel;
	uint16	sLareItem;
	uint16	sMagicItem;
	uint16	sGeneralItem;
};

struct _K_MONSTER_ITEM
{
	uint16 sIndex;
	uint32 iItem[5];
	uint16 sPercent[5];

	_K_MONSTER_ITEM()
	{
		memset(&iItem, 0, sizeof(iItem));
		memset(&sPercent, 0, sizeof(sPercent));
	}
};

struct _MAKE_ITEM_GROUP
{
	uint32	iItemGroupNum;
	std::vector<uint32> iItems;
};

struct _NPC_LIVE_TIME
{
	uint16 nIndex;
	int16 SocketID;
	uint16 Nid;
	uint16 Duration;
	int32 SpawnedTime;

};

#include "../shared/database/structs.h"