#pragma once

#include "../shared/STLMap.h"

typedef CSTLMap <int>			ZoneUserArray;
typedef CSTLMap <int>			ZoneNpcArray;

class CRegion  
{
public:
	CRegion() : m_byMoving(0) {}
	ZoneUserArray	m_RegionUserArray;
	ZoneNpcArray	m_RegionNpcArray;
	uint8	m_byMoving;			// move : 1, not moving : 0
};