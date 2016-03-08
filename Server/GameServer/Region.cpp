#include "stdafx.h"
#include "Region.h"
#include "User.h"
#include "Npc.h"

/**
* @brief	Adds user instance to the region.
*
* @param	pUser	The user to add.
*/
void CRegion::Add(CUser * pUser)
{
	Guard lock(m_lock);
	m_RegionUserArray.insert(pUser->GetID());
}

/**
* @brief	Removes the given user instance from the region.
*
* @param	pUser	The user to remove.
*/
void CRegion::Remove(CUser * pUser)
{
	Guard lock(m_lock);
	m_RegionUserArray.erase(pUser->GetID());
}

/**
* @brief	Adds the given NPC to the region.
*
* @param	pNpc	The NPC to add.
*/
void CRegion::Add(CNpc * pNpc)
{
	Guard lock(m_lock);
	m_RegionNpcArray.insert(pNpc->GetID());
}

/**
* @brief	Removes the given NPC from the region.
*
* @param	pNpc	The NPC to remove.
*/
void CRegion::Remove(CNpc * pNpc)
{
	Guard lock(m_lock);
	m_RegionNpcArray.erase(pNpc->GetID());
}