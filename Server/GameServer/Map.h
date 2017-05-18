#pragma once

#include "Region.h"
#include "GameEvent.h"
#include "../shared/STLMap.h"

class CGameEvent;
typedef CSTLMap <CGameEvent>		EventArray;

class CUser;
class CGameServerDlg;
class SMDFile;
class C3DMap
{
public:
	// Passthru methods
	int GetXRegionMax();
	int GetZRegionMax(); 
	bool IsValidPosition(float x, float z, float y);
	_OBJECT_EVENT * GetObjectEvent(int objectindex);
	_REGENE_EVENT * GetRegeneEvent(int objectindex);
	_WARP_INFO * GetWarp(int warpID);
	void GetWarpList(int warpGroup, std::set<_WARP_INFO *> & warpEntries);

	C3DMap();
	bool Initialize(_ZONE_INFO *pZone);
	CRegion * GetRegion(uint16_t regionX, uint16_t regionZ);
	bool CheckEvent( float x, float z, CUser* pUser = nullptr );
	void RegionItemRemove(CRegion * pRegion, _LOOT_BUNDLE * pBundle, _LOOT_ITEM * pItem);
	bool RegionItemAdd(uint16_t rx, uint16_t rz, _LOOT_BUNDLE * pBundle);
	virtual ~C3DMap();

	EventArray	m_EventArray;

	int	m_nServerNo, m_nZoneNumber;
	float m_fInitX, m_fInitZ, m_fInitY;
	int16_t	m_sMaxUser;

	CRegion**	m_ppRegion;

	uint32_t	m_wBundle;	// Zone Item Max Count

	SMDFile *m_smdFile;

	std::recursive_mutex m_lock;

	INLINE uint16_t GetID() { return m_nZoneNumber; }

	/* the following should all be duplicated to AI server's map class for now */

	INLINE bool canTradeWithOtherNation() { return (m_zoneFlags & ZF_TRADE_OTHER_NATION) != 0; }
	INLINE bool canTalkToOtherNation() { return (m_zoneFlags & ZF_TALK_OTHER_NATION) != 0; }
	INLINE bool canAttackOtherNation() { return (m_zoneFlags & ZF_ATTACK_OTHER_NATION) != 0; }
	INLINE bool canAttackSameNation() { return (m_zoneFlags & ZF_ATTACK_SAME_NATION) != 0; } 
	INLINE bool isWarZone() { return (m_zoneFlags & ZF_WAR_ZONE) != 0; }
	INLINE bool isNationPVPZone() { return canAttackOtherNation() && !canAttackSameNation(); }
	INLINE bool areNPCsFriendly() { return (m_zoneFlags & ZF_FRIENDLY_NPCS) != 0; }
	INLINE bool canUpdateClan() { return (m_zoneFlags & ZF_CLAN_UPDATE) != 0; }

	INLINE uint16_t GetZoneFlags(void) { return m_zoneFlags; }
	INLINE uint8_t GetZoneType() { return m_zoneType; }
	INLINE uint8_t GetTariff() { return m_byTariff; }
	INLINE void SetTariff(uint8_t tariff) { m_byTariff = tariff; }

	INLINE uint8_t GetMinLevelReq() { return m_byMinLevel; }
	INLINE uint8_t GetMaxLevelReq() { return m_byMaxLevel; }

protected:
	void SetZoneAttributes(int zoneNumber);

	ZoneAbilityType m_zoneType;
	uint16_t m_zoneFlags;
	uint8_t m_byTariff;
	uint8_t m_byMinLevel, m_byMaxLevel;
};