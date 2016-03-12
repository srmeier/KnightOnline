#include "stdafx.h"
#include "Map.h"
#include <set>
#include "../shared/SMDFile.h"

/* passthru methods */
int C3DMap::GetXRegionMax() { return m_smdFile->GetXRegionMax(); }
int C3DMap::GetZRegionMax() { return m_smdFile->GetZRegionMax(); }
bool C3DMap::IsValidPosition(float x, float z, float y) { return m_smdFile->IsValidPosition(x, z, y); }
_REGENE_EVENT * C3DMap::GetRegeneEvent(int objectindex) { return m_smdFile->GetRegeneEvent(objectindex); }
_WARP_INFO * C3DMap::GetWarp(int warpID) { return m_smdFile->GetWarp(warpID); }
void C3DMap::GetWarpList(int warpGroup, std::set<_WARP_INFO *> & warpEntries) { m_smdFile->GetWarpList(warpGroup, warpEntries); }

C3DMap::C3DMap() : m_smdFile(nullptr), m_ppRegion(nullptr),
	m_nZoneNumber(0), m_sMaxUser(150), m_wBundle(1)
{
}

bool C3DMap::Initialize(_ZONE_INFO *pZone)
{
	m_nServerNo = pZone->m_nServerNo;
	m_nZoneNumber = pZone->m_nZoneNumber;
	m_fInitX = pZone->m_fInitX;
	m_fInitY = pZone->m_fInitY;
	m_fInitZ = pZone->m_fInitZ;

	m_smdFile = SMDFile::Load(pZone->m_MapName, true /* load warps & regene events */);

	if (m_smdFile != nullptr)
	{
		SetZoneAttributes(m_nZoneNumber);

		// NOTE: implementing this for game server
		// - was already on AIServer but for the game server it was replaced by K_OBJECTPOS table
		foreach_stlmap(itr, m_smdFile->m_ObjectEventArray) {
			_OBJECT_EVENT * pEvent = itr->second;
			_OBJECT_EVENT * pEventMap = new _OBJECT_EVENT;

			pEvent->nIndex  = 0; // NOTE: index into DB table?
			pEvent->sZoneID = m_nZoneNumber;
			pEvent->byLife  = 0; // NOTE: I guess if the "NPC" can take damage?

			*pEventMap = *pEvent;

			// NOTE: not sure how these should be actually indexed
			int iIndex = g_pMain->m_ObjectEventArray.GetSize();
			if (!g_pMain->m_ObjectEventArray.PutData(iIndex, pEventMap)) {
				TRACE("Object Event PutData Fail - %d\n", pEvent->sIndex);
			}
		}

		m_ppRegion = new CRegion*[m_smdFile->m_nXRegion];
		for (int i = 0; i < m_smdFile->m_nXRegion; i++)
			m_ppRegion[i] = new CRegion[m_smdFile->m_nZRegion];
	}

	return (m_smdFile != nullptr);
}

CRegion * C3DMap::GetRegion(uint16 regionX, uint16 regionZ)
{
	if (regionX > GetXRegionMax()
		|| regionZ > GetZRegionMax())
		return nullptr;

	Guard lock(m_lock);
	return &m_ppRegion[regionX][regionZ];
}

bool C3DMap::RegionItemAdd(uint16 rx, uint16 rz, _LOOT_BUNDLE * pBundle)
{
	if (pBundle == nullptr)
		return false;

	Guard lock(m_lock);
	CRegion * pRegion = GetRegion(rx, rz);

	if (pRegion == nullptr)
		return false;

	pBundle->nBundleID = m_wBundle++;
	pRegion->m_RegionItemArray.PutData(pBundle->nBundleID, pBundle);

	if (m_wBundle > ZONEITEM_MAX)
		m_wBundle = 1;

	return true;
}

/**
* @brief	Removes an item from a region's bundle.
* 			If the bundle's empty, the bundle is then 
* 			removed from the region.
*
* @param	pRegion	The region.
* @param	pBundle	The bundle.
* @param	pItem  	The item being removed from the bundle.
*/
void C3DMap::RegionItemRemove(CRegion * pRegion, _LOOT_BUNDLE * pBundle, _LOOT_ITEM * pItem)
{
	if (pRegion == nullptr
		|| pBundle == nullptr)
		return;

	Guard lock(pRegion->m_RegionItemArray.m_lock);

	// If the bundle exists, and the item matches what the user's removing
	// we can remove this item from the bundle.
	foreach (itr, pBundle->Items)
	{
		if (&(*itr) == pItem)
		{
			pBundle->Items.erase(itr);

			// If this was the last item in the bundle, remove the bundle from the region.
			if (!pBundle->Items.empty())
				return;

			pRegion->m_RegionItemArray.DeleteData(pBundle->nBundleID);
			return;
		}
	}
}

bool C3DMap::CheckEvent(float x, float z, CUser* pUser)
{
	CGameEvent *pEvent;

	int event_index = m_smdFile->GetEventID((int)(x / m_smdFile->GetUnitDistance()), (int)(z / m_smdFile->GetUnitDistance()));
	if (event_index < 2)
	{
		if (g_pMain->m_byBattleOpen == NATION_BATTLE && pUser->GetMap()->isWarZone() && g_pMain->m_byBattleZoneType == 0)
		{
			pEvent = m_EventArray.GetData(1010 + (pUser->GetNation() == ELMORAD ? 1 : 2));

			if (pEvent != nullptr)
			{
				if ((x > pEvent->m_iCond[0] && x < pEvent->m_iCond[1]) && (z > pEvent->m_iCond[2] && z < pEvent->m_iCond[3]))
					pUser->ZoneChange(pEvent->m_iExec[0],(float)pEvent->m_iExec[1],(float)pEvent->m_iExec[2]);
				else
				{
					pEvent = m_EventArray.GetData(1010 + pUser->GetNation());

					if (pEvent != nullptr)
					{
						if ((x > pEvent->m_iCond[0] && x < pEvent->m_iCond[1]) && (z > pEvent->m_iCond[2] && z < pEvent->m_iCond[3]))
							if (g_pMain->m_bVictory == pUser->GetNation())
								pUser->ZoneChange(pEvent->m_iExec[0],(float)pEvent->m_iExec[1],(float)pEvent->m_iExec[2]);
					}
				}
			}

			return false;
		}
		else
			return false;
	}

	if ( g_pMain->m_byBattleOpen == NATION_BATTLE && 
		pUser->GetZoneID() != ZONE_KARUS_ESLANT && 
		pUser->GetZoneID() != ZONE_ELMORAD_ESLANT )
		event_index += g_pMain->m_byBattleZone -1;

	if ( g_pMain->m_byBattleOpen == SNOW_BATTLE && 
		pUser->GetZoneID() != ZONE_KARUS_ESLANT && 
		pUser->GetZoneID() != ZONE_ELMORAD_ESLANT )
		event_index += g_pMain->m_byBattleZone +10;

	pEvent = m_EventArray.GetData( event_index );
	if (pEvent == nullptr)
		return false;

	if (pEvent->m_bType == 1 && (pEvent->m_iExec[0] > ZONE_BATTLE_BASE && pEvent->m_iExec[0] <= ZONE_BATTLE6) && g_pMain->m_byBattleOpen != NATION_BATTLE ) 
		return false;
	else if (pEvent->m_bType == 1 && pEvent->m_iExec[0] == ZONE_SNOW_BATTLE && g_pMain->m_byBattleOpen != SNOW_BATTLE )
		return false;
	else if (pEvent->m_iExec[0] > ZONE_BATTLE_BASE && pEvent->m_iExec[0] <= ZONE_BATTLE6)
	{
		if ((pUser->GetNation() == KARUS && g_pMain->m_sKarusCount > MAX_BATTLE_ZONE_USERS
			|| pUser->GetNation() == ELMORAD && g_pMain->m_sElmoradCount > MAX_BATTLE_ZONE_USERS))
		{
			TRACE("%s cannot enter war zone %d, too many users.\n", pUser->GetName().c_str(), pEvent->m_iExec[0]);
			return false;
		}

		if (g_pMain->m_byBattleZoneType == ZONE_ARDREAM && (pUser->GetLevel() < MIN_LEVEL_NIEDS_TRIANGLE || pUser->GetLevel() > MAX_LEVEL_NIEDS_TRIANGLE || !pUser->CanLevelQualify(MAX_LEVEL_NIEDS_TRIANGLE)))
			return false;
	}

	pEvent->RunEvent(pUser);
	return true;
}

_OBJECT_EVENT * C3DMap::GetObjectEvent(int objectindex) 
{ 
	foreach_stlmap(itr, g_pMain->m_ObjectEventArray)
	{
		if (itr->second->sZoneID == m_nZoneNumber
			&& itr->second->sIndex == objectindex)
			return itr->second;
	}

	return nullptr;
}

C3DMap::~C3DMap()
{
	m_EventArray.DeleteAllData();

	if (m_ppRegion != nullptr)
	{
		for (int i = 0; i <= GetXRegionMax(); i++)
			delete [] m_ppRegion[i];

		delete [] m_ppRegion;
		m_ppRegion = nullptr;
	}

	if (m_smdFile != nullptr)
		m_smdFile->DecRef();
}