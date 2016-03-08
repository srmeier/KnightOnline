#pragma once

#if defined(AI_SERVER)
#	define KOMap MAP
#else
#	define KOMap C3DMap
#endif

#include "Define.h"
#include "GameDefine.h"
#include "../shared/ReferenceObject.h"
#include <map>

// Maximum range allowed between a player and an NPC.
#define MAX_NPC_RANGE		(121.0f) // pow(11.0f, 2.0f), to save having to calculate it in the code.

// Maximum range allowed between a unit and an object
#define MAX_OBJECT_RANGE	(100.0f) // pow(10.0f, 20.0f)

// Maximum range allowed between a player & their loot.
#define MAX_LOOT_RANGE		(121.0f) // pow(11.0f, 2.0f)

struct _MAGIC_TABLE;
struct _BUFF_TYPE4_INFO;
struct _BUFF_TYPE9_INFO;
class CRegion;
class KOMap;
class Packet;

typedef std::map<uint8, _BUFF_TYPE4_INFO> Type4BuffMap;
typedef std::map<uint8, _BUFF_TYPE9_INFO> Type9BuffMap;

enum AttackType { AttackTypeNone, AttackTypePhysical, AttackTypeMagic };
enum UnitType { UnitPlayer, UnitNPC, UnitPet };

/**
* This class is a bridge between the CNpc & CUser classes
**/
class Unit : public ReferenceObject
{
public:
	Unit(UnitType unitType);

	virtual void Initialize();

	INLINE UnitType GetUnitType() { return m_unitType; }
	INLINE void SetUnitType(UnitType unitType) { m_unitType = unitType; }

	INLINE bool isPlayer() { return GetUnitType() == UnitPlayer; }
	INLINE bool isNPC() { return GetUnitType() == UnitNPC; }

	INLINE KOMap * GetMap() { return m_pMap; }

	virtual uint16 GetID() = 0;
	INLINE uint8 GetZoneID() { return m_bZone; }
	INLINE uint16 GetEventRoom() { return m_bEventRoom > (uint16)MAX_TEMPLE_EVENT_ROOM ? 0 : m_bEventRoom; }

	INLINE bool isInTempleEventZone(uint8 nZoneID = 0) 
	{
		if (nZoneID == 0)
			nZoneID = GetZoneID();

		return nZoneID == ZONE_BORDER_DEFENSE_WAR || nZoneID == ZONE_CHAOS_DUNGEON || nZoneID == ZONE_JURAD_MOUNTAIN; 
	}

	INLINE float GetX() { return m_curx; }
	INLINE float GetY() { return m_cury; }
	INLINE float GetZ() { return m_curz; }

	INLINE void SetPosition(float fx, float fy, float fz)
	{
		m_curx = fx;
		m_curz = fz;
		m_cury = fy;
	}

	INLINE uint16 GetSPosX() { return uint16(GetX() * 10); };
	INLINE uint16 GetSPosY() { return uint16(GetY() * 10); };
	INLINE uint16 GetSPosZ() { return uint16(GetZ() * 10); };

	INLINE uint16 GetRegionX() { return m_sRegionX; }
	INLINE uint16 GetRegionZ() { return m_sRegionZ; }

	INLINE uint16 GetNewRegionX() { return (uint16)(GetX()) / VIEW_DISTANCE; }
	INLINE uint16 GetNewRegionZ() { return (uint16)(GetZ()) / VIEW_DISTANCE; }

	INLINE CRegion * GetRegion() { return m_pRegion; }
	void SetRegion(uint16 x = -1, uint16 z = -1);

	virtual std::string & GetName() = 0; // this is especially fun...

	INLINE uint8 GetNation() { return m_bNation; }
	INLINE uint8 GetLevel() { return m_bLevel; }

	virtual int32 GetHealth() = 0;
	virtual int32 GetMaxHealth() = 0;
	virtual int32 GetMana() = 0;
	virtual int32 GetMaxMana() = 0;

	INLINE bool isIncapacitated() { return isDead() || isBlinded() || isBlinking() || isKaul(); }
	INLINE bool isBlinded() { return m_bIsBlinded; }
	INLINE bool canUseSkills() { return !(!isBlinded() && hasBuff(BUFF_TYPE_BLIND)) && m_bCanUseSkills && !isKaul(); } 
	INLINE bool canUsePotions() { return m_bCanUsePotions; }
	INLINE bool canTeleport() { return m_bCanTeleport; }
	INLINE bool isKaul() { return m_bIsKaul; }

	INLINE bool isBuffed(bool bIsOnlyScroll = false)
	{
		Guard lock(m_buffLock);

		// Check the buff counter.
		// We cannot check the map itself, as the map contains both buffs and debuffs.
		if (bIsOnlyScroll)
		{
			foreach (itr, m_buffMap)
				if (itr->second.m_nSkillID > 500000)
					return true;
		}
		return false;
	}

	INLINE bool isDebuffed()
	{
		Guard lock(m_buffLock);

		// As the 'buff' map contains both buffs and debuffs, if the number of buffs/debuffs in the map doesn't 
		// match the number of buffs we have, we can conclude we have some debuffs in there.
		return (uint8) m_buffMap.size() != m_buffCount; 
	}

	INLINE bool hasBuff(uint8 buff, bool isOnlyBuff = false)
	{
		Guard lock(m_buffLock);
		if (isOnlyBuff)
		{
			auto itr = m_buffMap.find(buff);
			if (itr != m_buffMap.end() && itr->second.isBuff())
				return true;
		}
		return m_buffMap.find(buff) != m_buffMap.end();
	}

	INLINE bool hasDebuff(uint8 buff)
	{
		Guard lock(m_buffLock);
		auto itr = m_buffMap.find(buff);
		if (itr != m_buffMap.end() && itr->second.isDebuff())
			return true;

		return false;
	}

	INLINE bool canInstantCast() { return m_bInstantCast; }
	INLINE bool canStealth()	{ return m_bCanStealth; }

	virtual bool isBlinking() { return false; }

	virtual bool isDead() = 0;
	virtual bool isAlive() { return !isDead(); }

	float GetDistance(float fx, float fz);
	float GetDistance(Unit * pTarget);
	float GetDistanceSqrt(Unit * pTarget);

	bool isInRange(Unit * pTarget, float fSquaredRange);
	bool isInRange(float fx, float fz, float fSquaredRange);
	bool isInRangeSlow(Unit * pTarget, float fNonSquaredRange);
	bool isInRangeSlow(float fx, float fz, float fNonSquaredRange);

	static float GetDistance(float fStartX, float fStartZ, float fEndX, float fEndZ);
	static bool isInRange(float fStartX, float fStartZ, float fEndX, float fEndZ, float fSquaredRange);
	static bool isInRangeSlow(float fStartX, float fStartZ, float fEndX, float fEndZ, float fNonSquaredRange);

	virtual void GetInOut(Packet & result, uint8 bType) = 0;

	bool RegisterRegion();
	virtual void AddToRegion(int16 new_region_x, int16 new_region_z) = 0;
	void RemoveRegion(int16 del_x, int16 del_z);
	void InsertRegion(int16 insert_x, int16 insert_z);

	bool isInAttackRange(Unit * pTarget, _MAGIC_TABLE * pSkill = nullptr);
	virtual short GetDamage(Unit *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false) = 0;
	virtual void OnAttack(Unit * pTarget, AttackType attackType) {}
	virtual void OnDefend(Unit * pAttacker, AttackType attackType) {}

	short GetMagicDamage(int damage, Unit *pTarget, bool bPreviewOnly = false);
	short GetACDamage(int damage, Unit *pTarget);
	uint8 GetHitRate(float rate);

	virtual void InsertSavedMagic(uint32 nSkillID, uint16 sDuration) {}
	virtual bool HasSavedMagic(uint32 nSkillID) { return false; }
	virtual int16 GetSavedMagicDuration(uint32 nSkillID) { return -1; }

	virtual void HpChange(int amount, Unit *pAttacker = nullptr, bool bSendToAI = true) = 0;
	virtual void HpChangeMagic(int amount, Unit *pAttacker = nullptr, AttributeType attributeType = AttributeNone) { HpChange(amount, pAttacker); }
	virtual void MSpChange(int amount) = 0;

	void SendToRegion(Packet *result);
	void Send_AIServer(Packet *result);

	void InitType3();
	void InitType4(bool bRemoveSavedMagic = false, uint8 buffType = 0);
	void AddType4Buff(uint8 bBuffType, _BUFF_TYPE4_INFO & pBuffInfo);

	virtual void StateChangeServerDirect(uint8 bType, uint32 nBuff) {}
	virtual bool isHostileTo(Unit * pTarget) = 0;
	virtual bool CanAttack(Unit * pTarget);
	virtual bool isAttackable(Unit * pTarget = nullptr);
	virtual bool CanCastRHit(uint16 m_SocketID);
	virtual bool isSameEventRoom(Unit * pTarget);

	void OnDeath(Unit *pKiller);
	void SendDeathAnimation(Unit *pKiller = nullptr);

	// public for the moment
	// protected:
	KOMap  * m_pMap;
	CRegion * m_pRegion;

	uint8	m_bZone;
	float	m_curx, m_curz, m_cury;

	uint16	m_sRegionX, m_sRegionZ; // this is probably redundant

	UnitType m_unitType;

	uint8	m_bLevel;
	uint8	m_bNation;

	uint16	m_sTotalHit;
	uint16	m_sTotalAc;
	float	m_fTotalHitrate;
	float	m_fTotalEvasionrate;

	short   m_sACAmount;	// additional absolute AC
	int16	m_sACPercent;	// percentage of total AC to modify by
	uint8   m_bAttackAmount;
	short	m_sMagicAttackAmount;
	short	m_sMaxHPAmount, m_sMaxMPAmount;
	uint8	m_bHitRateAmount;
	short	m_sAvoidRateAmount;

	int16	m_sAttackSpeedAmount;
	uint8   m_bSpeedAmount;

	// Item calculated elemental resistances.
	uint16	m_sFireR, m_sColdR, m_sLightningR, 
		m_sMagicR, m_sDiseaseR, m_sPoisonR;

	// Additional elemental resistance amounts from skills (note: NOT percentages)
	uint8	m_bAddFireR, m_bAddColdR, m_bAddLightningR,
		m_bAddMagicR, m_bAddDiseaseR, m_bAddPoisonR;

	// Elemental resistance percentages (adjusted by debuffs)
	uint8	m_bPctFireR, m_bPctColdR, m_bPctLightningR,
		m_bPctMagicR, m_bPctDiseaseR, m_bPctPoisonR;

	uint8	m_bMagicDamageReduction;
	uint8	m_bManaAbsorb;
	uint8	m_bRadiusAmount;

	uint8	m_bResistanceBonus;

	BYTE    m_bMagicTypeLeftHand;			// The type of magic item in user's left hand  
	BYTE    m_bMagicTypeRightHand;			// The type of magic item in user's right hand
	short   m_sMagicAmountLeftHand;         // The amount of magic item in user's left hand
	short	m_sMagicAmountRightHand;        // The amount of magic item in user's left hand

	// bonus type -> amount
	typedef std::map<uint8, int16> ItemBonusMap;

	// slot id -> bonus map
	typedef std::map<uint8, ItemBonusMap> EquippedItemBonuses;

	// This map is for applying item bonuses from equipped skills, i.e. resistances, drains, damage reflection, etc.
	// It is indexed by slot ID (this should really work with the item container), and contains a map of each bonus (indexed by type)
	// supported by this item (we support multiple bonuses, official most likely still overrides them).
	EquippedItemBonuses m_equippedItemBonuses;
	std::recursive_mutex m_equippedItemBonusLock;

	// Weapon resistances
	int16 m_sDaggerR; 
	uint8 m_byDaggerRAmount;
	int16 m_sSwordR;
	int16 m_sAxeR;
	int16 m_sMaceR;
	int16 m_sSpearR;
	int16 m_sBowR; 
	uint8 m_byBowRAmount;

	struct MagicType3
	{
		bool	m_byUsed;		// indicates whether this element is used
		time_t	m_tHPLastTime;	// time when the durational skill last affected the unit
		int16	m_sHPAmount;	// HP amount to affet the unit by (negative for damage, positive for HP recovery)
		uint8	m_bHPInterval;	// interval (in seconds) between each durational skill effect
		uint8	m_bTickCount;	// 
		uint8	m_bTickLimit;	// number of ticks required before the skill expires
		uint16	m_sSourceID;	// ID of the unit that used this skill on the unit
		uint8	m_byAttribute;	// skill attribute

		MagicType3() { Reset(); }

		INLINE void Reset()
		{
			m_byUsed = false;
			m_tHPLastTime = 0;
			m_sHPAmount = 0;
			m_bHPInterval = 0;
			m_bTickCount = 0;
			m_bTickLimit = 0;
			m_sSourceID = -1;
			m_byAttribute = AttributeNone;
		}
	};

	MagicType3 m_durationalSkills[MAX_TYPE3_REPEAT];
	bool	m_bType3Flag;

	Type4BuffMap m_buffMap;
	Type9BuffMap m_type9BuffMap;
	std::recursive_mutex	m_buffLock;
	uint8		m_buffCount; // counter for buffs (not debuffs). Used for identifying when the user is buffed.

	bool	m_bIsBlinded;
	bool	m_bCanUseSkills; // blinding prevents you from using skills or attacks, skills like "Full Skill Gear" prevent use of skills only.
	bool	m_bCanUsePotions;
	bool	m_bCanTeleport;
	bool	m_bCanStealth;
	bool	m_bInstantCast;
	bool    m_bBlockCurses, m_bReflectCurses;
	bool	m_bMirrorDamage; 
	uint8	m_byMirrorAmount;
	uint8	m_bReflectArmorType;
	bool	m_bIsUndead, m_bIsKaul;

	bool m_bBlockPhysical;
	bool m_bBlockMagic;

	int16	m_oSocketID; // owner user
	uint16	m_bEventRoom;
};