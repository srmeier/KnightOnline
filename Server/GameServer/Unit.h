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
#include "../shared/types.h"

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

typedef std::map<uint8_t, _BUFF_TYPE4_INFO> Type4BuffMap;
typedef std::map<uint8_t, _BUFF_TYPE9_INFO> Type9BuffMap;

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

	virtual uint16_t GetID() = 0;
	INLINE uint8_t GetZoneID() { return m_bZone; }
	INLINE uint16_t GetEventRoom() { return m_bEventRoom > (uint16_t)MAX_TEMPLE_EVENT_ROOM ? 0 : m_bEventRoom; }

	INLINE bool isInTempleEventZone(uint8_t nZoneID = 0) 
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

	INLINE uint16_t GetSPosX() { return uint16_t(GetX() * 10); };
	INLINE uint16_t GetSPosY() { return uint16_t(GetY() * 10); };
	INLINE uint16_t GetSPosZ() { return uint16_t(GetZ() * 10); };

	INLINE uint16_t GetRegionX() { return m_sRegionX; }
	INLINE uint16_t GetRegionZ() { return m_sRegionZ; }

	INLINE uint16_t GetNewRegionX() { return (uint16_t)(GetX()) / VIEW_DISTANCE; }
	INLINE uint16_t GetNewRegionZ() { return (uint16_t)(GetZ()) / VIEW_DISTANCE; }

	INLINE CRegion * GetRegion() { return m_pRegion; }
	void SetRegion(uint16_t x = -1, uint16_t z = -1);

	virtual std::string & GetName() = 0; // this is especially fun...

	INLINE uint8_t GetNation() { return m_bNation; }
	INLINE uint8_t GetLevel() { return m_bLevel; }

	virtual int32_t GetHealth() = 0;
	virtual int32_t GetMaxHealth() = 0;
	virtual int32_t GetMana() = 0;
	virtual int32_t GetMaxMana() = 0;

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
		return (uint8_t) m_buffMap.size() != m_buffCount; 
	}

	INLINE bool hasBuff(uint8_t buff, bool isOnlyBuff = false)
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

	INLINE bool hasDebuff(uint8_t buff)
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

	virtual void GetInOut(Packet & result, uint8_t bType) = 0;

	bool RegisterRegion();
	virtual void AddToRegion(int16_t new_region_x, int16_t new_region_z) = 0;
	void RemoveRegion(int16_t del_x, int16_t del_z);
	void InsertRegion(int16_t insert_x, int16_t insert_z);

	bool isInAttackRange(Unit * pTarget, _MAGIC_TABLE * pSkill = nullptr);
	virtual int16_t GetDamage(Unit *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false) = 0;
	virtual void OnAttack(Unit * pTarget, AttackType attackType) {}
	virtual void OnDefend(Unit * pAttacker, AttackType attackType) {}

	int16_t GetMagicDamage(int damage, Unit *pTarget, bool bPreviewOnly = false);
	int16_t GetACDamage(int damage, Unit *pTarget);
	uint8_t GetHitRate(float rate);

	virtual void InsertSavedMagic(uint32_t nSkillID, uint16_t sDuration) {}
	virtual bool HasSavedMagic(uint32_t nSkillID) { return false; }
	virtual int16_t GetSavedMagicDuration(uint32_t nSkillID) { return -1; }

	virtual void HpChange(int amount, Unit *pAttacker = nullptr, bool bSendToAI = true) = 0;
	virtual void HpChangeMagic(int amount, Unit *pAttacker = nullptr, AttributeType attributeType = AttributeNone) { HpChange(amount, pAttacker); }
	virtual void MSpChange(int amount) = 0;

	void SendToRegion(Packet *result);
	void Send_AIServer(Packet *result);

	void InitType3();
	void InitType4(bool bRemoveSavedMagic = false, uint8_t buffType = 0);
	void AddType4Buff(uint8_t bBuffType, _BUFF_TYPE4_INFO & pBuffInfo);

	virtual void StateChangeServerDirect(uint8_t bType, uint32_t nBuff) {}
	virtual bool isHostileTo(Unit * pTarget) = 0;
	virtual bool CanAttack(Unit * pTarget);
	virtual bool isAttackable(Unit * pTarget = nullptr);
	virtual bool CanCastRHit(uint16_t m_SocketID);
	virtual bool isSameEventRoom(Unit * pTarget);

	void OnDeath(Unit *pKiller);
	void SendDeathAnimation(Unit *pKiller = nullptr);

	// public for the moment
	// protected:
	KOMap  * m_pMap;
	CRegion * m_pRegion;

	uint8_t	m_bZone;
	float	m_curx, m_curz, m_cury;

	uint16_t	m_sRegionX, m_sRegionZ; // this is probably redundant

	UnitType m_unitType;

	uint8_t	m_bLevel;
	uint8_t	m_bNation;

	uint16_t	m_sTotalHit;
	uint16_t	m_sTotalAc;
	float	m_fTotalHitrate;
	float	m_fTotalEvasionrate;

	int16_t   m_sACAmount;	// additional absolute AC
	int16_t	m_sACPercent;	// percentage of total AC to modify by
	uint8_t   m_bAttackAmount;
	int16_t	m_sMagicAttackAmount;
	int16_t	m_sMaxHPAmount, m_sMaxMPAmount;
	uint8_t	m_bHitRateAmount;
	int16_t	m_sAvoidRateAmount;

	int16_t	m_sAttackSpeedAmount;
	uint8_t   m_bSpeedAmount;

	// Item calculated elemental resistances.
	uint16_t	m_sFireR, m_sColdR, m_sLightningR, 
		m_sMagicR, m_sDiseaseR, m_sPoisonR;

	// Additional elemental resistance amounts from skills (note: NOT percentages)
	uint8_t	m_bAddFireR, m_bAddColdR, m_bAddLightningR,
		m_bAddMagicR, m_bAddDiseaseR, m_bAddPoisonR;

	// Elemental resistance percentages (adjusted by debuffs)
	uint8_t	m_bPctFireR, m_bPctColdR, m_bPctLightningR,
		m_bPctMagicR, m_bPctDiseaseR, m_bPctPoisonR;

	uint8_t	m_bMagicDamageReduction;
	uint8_t	m_bManaAbsorb;
	uint8_t	m_bRadiusAmount;

	uint8_t	m_bResistanceBonus;

	uint8_t    m_bMagicTypeLeftHand;			// The type of magic item in user's left hand  
	uint8_t    m_bMagicTypeRightHand;			// The type of magic item in user's right hand
	int16_t   m_sMagicAmountLeftHand;         // The amount of magic item in user's left hand
	int16_t	m_sMagicAmountRightHand;        // The amount of magic item in user's left hand

	// bonus type -> amount
	typedef std::map<uint8_t, int16_t> ItemBonusMap;

	// slot id -> bonus map
	typedef std::map<uint8_t, ItemBonusMap> EquippedItemBonuses;

	// This map is for applying item bonuses from equipped skills, i.e. resistances, drains, damage reflection, etc.
	// It is indexed by slot ID (this should really work with the item container), and contains a map of each bonus (indexed by type)
	// supported by this item (we support multiple bonuses, official most likely still overrides them).
	EquippedItemBonuses m_equippedItemBonuses;
	std::recursive_mutex m_equippedItemBonusLock;

	// Weapon resistances
	int16_t m_sDaggerR; 
	uint8_t m_byDaggerRAmount;
	int16_t m_sSwordR;
	int16_t m_sAxeR;
	int16_t m_sMaceR;
	int16_t m_sSpearR;
	int16_t m_sBowR; 
	uint8_t m_byBowRAmount;

	struct MagicType3
	{
		bool	m_byUsed;		// indicates whether this element is used
		time_t	m_tHPLastTime;	// time when the durational skill last affected the unit
		int16_t	m_sHPAmount;	// HP amount to affet the unit by (negative for damage, positive for HP recovery)
		uint8_t	m_bHPInterval;	// interval (in seconds) between each durational skill effect
		uint8_t	m_bTickCount;	// 
		uint8_t	m_bTickLimit;	// number of ticks required before the skill expires
		uint16_t	m_sSourceID;	// ID of the unit that used this skill on the unit
		uint8_t	m_byAttribute;	// skill attribute

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
	uint8_t		m_buffCount; // counter for buffs (not debuffs). Used for identifying when the user is buffed.

	bool	m_bIsBlinded;
	bool	m_bCanUseSkills; // blinding prevents you from using skills or attacks, skills like "Full Skill Gear" prevent use of skills only.
	bool	m_bCanUsePotions;
	bool	m_bCanTeleport;
	bool	m_bCanStealth;
	bool	m_bInstantCast;
	bool    m_bBlockCurses, m_bReflectCurses;
	bool	m_bMirrorDamage; 
	uint8_t	m_byMirrorAmount;
	uint8_t	m_bReflectArmorType;
	bool	m_bIsUndead, m_bIsKaul;

	bool m_bBlockPhysical;
	bool m_bBlockMagic;

	int16_t	m_oSocketID; // owner user
	uint16_t	m_bEventRoom;
};