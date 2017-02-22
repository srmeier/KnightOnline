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
	
#if IsWinDef
	std::recursive_mutex m_lock;
#endif
#if IsUnixDef
	pthread_mutex_t * m_lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
#endif
	
	ZoneItemArray	m_RegionItemArray;
	ZoneUserArray	m_RegionUserArray;
	ZoneNpcArray	m_RegionNpcArray;

	void Add(CUser * pUser);
	void Remove(CUser * pUser);
	void Add(CNpc * pNpc);
	void Remove(CNpc * pNpc);
};
