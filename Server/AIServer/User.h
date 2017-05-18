#pragma once

#include "Extern.h"

#include "../shared/STLMap.h"
#include "../GameServer/Unit.h"

class MAP;

class CUser : public Unit
{
public:
	INLINE bool isGM() { return m_byIsOP == AUTHORITY_GAME_MASTER; }

	virtual uint16_t GetID() { return m_iUserId; }
	virtual std::string & GetName() { return m_strUserID; }

	virtual int32_t GetHealth() { return m_sHP; }
	virtual int32_t GetMaxHealth() { return m_sMaxHP; }
	virtual int32_t GetMana() { return m_sMP; }
	virtual int32_t GetMaxMana() { return m_sMaxMP; }

	virtual void GetInOut(Packet &, uint8_t) {}
	virtual void AddToRegion(int16_t sRegionX, int16_t sRegionZ) {}

	virtual void HpChange(int amount, Unit *pAttacker = nullptr, bool bSendToAI = true) {}
	virtual void MSpChange(int amount) {}

	virtual bool isDead() { return m_bLive == AI_USER_DEAD || GetHealth() <= 0; }

	INLINE bool isInParty() { return m_byNowParty != 0; }
	INLINE uint16_t GetPartyID() { return m_sPartyNumber; }

	std::string m_strUserID;
	int16_t	m_iUserId;					// User의 번호
	uint8_t	m_bLive;					// 죽었니? 살았니?

	float			m_fWill_x;			// 다음 X 좌표
	float			m_fWill_y;			// 다음 Y 좌표
	float			m_fWill_z;			// 다음 Z 좌표
	int16_t			m_sSpeed;			// 유저의 스피드	

	int16_t	m_sHP;							// HP
	int16_t	m_sMP;							// MP
	int16_t	m_sMaxHP;						// HP
	int16_t	m_sMaxMP;						// MP

	uint8_t	m_state;						// User의 상태

	uint8_t    m_byNowParty;
	uint8_t	m_byPartyTotalMan;
	int16_t   m_sPartyTotalLevel;
	int16_t	m_sPartyNumber;

	uint16_t	m_sItemAc;

	int16_t  m_sSurroundNpcNumber[8];		// Npc 다굴~

	uint8_t   m_byIsOP;
	uint8_t	m_bInvisibilityType;

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

	int16_t GetDamage(Unit *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false);

	CUser();
	virtual ~CUser();

	// Placeholders, for the magic system.
	// These should really be using the same base class.
	INLINE bool isInClan() { return false; }
	INLINE uint16_t GetClanID() { return 0; }
	INLINE uint8_t GetStat(StatType type) { return 0; }
	INLINE void SetStatBuff(StatType type, uint8_t val) {}

	void RemoveSavedMagic(uint32_t nSkillID) {}
	void SendUserStatusUpdate(UserStatus type, UserStatusBehaviour status) {}
	void SetUserAbility(bool bSendPacket = true) {}
	void Send(Packet * pkt) {}

	time_t	m_tLastRegeneTime;
	uint32_t	m_nOldAbnormalType;
	uint16_t	m_sExpGainAmount;
	uint8_t	m_bMaxWeightAmount, m_bNPGainAmount, m_bNoahGainAmount, 
		m_bPlayerAttackAmount, m_bSkillNPBonus,
		m_bAddWeaponDamage;
	uint16_t	m_sAddArmourAc;
	uint8_t	m_bPctArmourAc;
	bool	m_bPremiumMerchant;
};
