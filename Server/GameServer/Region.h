#pragma once

#include <set>
#include "Define.h"
#include "GameDefine.h"
#include "../shared/STLMap.h"

typedef CSTLMap <_LOOT_BUNDLE>	ZoneItemArray;
typedef std::set<uint16>    ZoneUserArray;
typedef std::set<uint16>    ZoneNpcArray;

class CNpc;
class CUser;

class CRegion  
{
public:
	std::recursive_mutex		m_lock;
	ZoneItemArray	m_RegionItemArray;
	ZoneUserArray	m_RegionUserArray;
	ZoneNpcArray	m_RegionNpcArray;

	void Add(CUser * pUser);
	void Remove(CUser * pUser);
	void Add(CNpc * pNpc);
	void Remove(CNpc * pNpc);
};
