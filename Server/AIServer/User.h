#pragma once

#include "Extern.h"

#include "../shared/STLMap.h"
#include "../GameServer/Unit.h"

class MAP;

class CUser : public Unit
{
public:
	INLINE bool isGM() { return m_byIsOP == AUTHORITY_GAME_MASTER; }

	virtual uint16 GetID() { return m_iUserId; }
	virtual std::string & GetName() { return m_strUserID; }

	virtual int32 GetHealth() { return m_sHP; }
	virtual int32 GetMaxHealth() { return m_sMaxHP; }
	virtual int32 GetMana() { return m_sMP; }
	virtual int32 GetMaxMana() { return m_sMaxMP; }

	virtual void GetInOut(Packet &, uint8) {}
	virtual void AddToRegion(int16 sRegionX, int16 sRegionZ) {}

	virtual void HpChange(int amount, Unit *pAttacker = nullptr, bool bSendToAI = true) {}
	virtual void MSpChange(int amount) {}

	virtual bool isDead() { return m_bLive == AI_USER_DEAD || GetHealth() <= 0; }

	INLINE bool isInParty() { return m_byNowParty != 0; }
	INLINE uint16 GetPartyID() { return m_sPartyNumber; }

	std::string m_strUserID;
	short	m_iUserId;					// User의 번호
	uint8	m_bLive;					// 죽었니? 살았니?

	float			m_fWill_x;			// 다음 X 좌표
	float			m_fWill_y;			// 다음 Y 좌표
	float			m_fWill_z;			// 다음 Z 좌표
	short			m_sSpeed;			// 유저의 스피드	

	short	m_sHP;							// HP
	short	m_sMP;							// MP
	short	m_sMaxHP;						// HP
	short	m_sMaxMP;						// MP

	uint8	m_state;						// User의 상태

	uint8    m_byNowParty;
	uint8	m_byPartyTotalMan;
	short   m_sPartyTotalLevel;
	short	m_sPartyNumber;

	uint16	m_sItemAc;

	short  m_sSurroundNpcNumber[8];		// Npc 다굴~

	uint8   m_byIsOP;
	uint8	m_bInvisibilityType;

public:
	void Initialize();
	void InitNpcAttack();
	void OnDeath(Unit * pKiller);
	int IsSurroundCheck(float fX, float fY, float fZ, int NpcID);
	void HealMagic();
	void HealAreaCheck(int rx, int rz);

	bool isHostileTo(Unit * pTarget);
	bool isInArena();
	bool isInPVPZone();
	bool isInSafetyArea();

	short GetDamage(Unit *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false);

	CUser();
	virtual ~CUser();

	// Placeholders, for the magic system.
	// These should really be using the same base class.
	INLINE bool isInClan() { return false; }
	INLINE uint16 GetClanID() { return 0; }
	INLINE uint8 GetStat(StatType type) { return 0; }
	INLINE void SetStatBuff(StatType type, uint8 val) {}

	void RemoveSavedMagic(uint32 nSkillID) {}
	void SendUserStatusUpdate(UserStatus type, UserStatusBehaviour status) {}
	void SetUserAbility(bool bSendPacket = true) {}
	void Send(Packet * pkt) {}

	time_t	m_tLastRegeneTime;
	uint32	m_nOldAbnormalType;
	uint16	m_sExpGainAmount;
	uint8	m_bMaxWeightAmount, m_bNPGainAmount, m_bNoahGainAmount, 
		m_bPlayerAttackAmount, m_bSkillNPBonus,
		m_bAddWeaponDamage;
	uint16	m_sAddArmourAc;
	uint8	m_bPctArmourAc;
	bool	m_bPremiumMerchant;
};
