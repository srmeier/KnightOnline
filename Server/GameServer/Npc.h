#pragma once

#include "LuaEngine.h"
#include "Unit.h"

class CNpc : public Unit
{
public:
	virtual uint16_t GetID() { return m_sNid; }
	virtual std::string & GetName() { return m_strName; }

	uint16_t	m_sNid; // NPC ID
	uint16_t	m_sSid; // prototype ID

	int16_t	m_sPid;				// MONSTER(NPC) Picture ID
	int16_t	m_sSize;			// MONSTER(NPC) Size
	int		m_iWeapon_1;
	int		m_iWeapon_2;
	std::string m_strName;		// MONSTER(NPC) Name
	int		m_iMaxHP;			// �ִ� HP
	int		m_iHP;				// ���� HP
	uint8_t	m_byState;			// ������ (NPC) ����
	uint8_t	m_tNpcType;			// NPC Type
	// 0 : Normal Monster
	// 1 : NPC
	// 2 : �� �Ա�,�ⱸ NPC
	// 3 : ������
	int   m_iSellingGroup;		// ItemGroup

	uint8_t	m_NpcState;			// NPC�� ���� - ���Ҵ�, �׾���, ���ִ� ����...
	bool	m_byGateOpen;		// Gate status: true is open, false is closed.

	uint8_t    m_byObjectType;     // ������ 0, objectŸ��(����, ����)�� 1
	int16_t	m_byDirection;

	uint8_t   m_byTrapNumber;

	bool	m_bMonster; // are we a monster or an NPC?

	int16_t	m_oSocketID; // owner user
	uint16_t	m_bEventRoom;

public:
	CNpc();

	void Initialize();
	void AddToRegion(int16_t new_region_x, int16_t new_region_z);

	void MoveResult(float xpos, float ypos, float zpos, float speed);
	void GetInOut(Packet & result, uint8_t bType);
	void SendInOut(uint8_t bType, float fx, float fz, float fy);
	void GetNpcInfo(Packet & pkt);

	void SendGateFlag(uint8_t bFlag = -1, bool bSendAI = true);

	void HpChange(int amount, Unit *pAttacker = nullptr, bool bSendToAI = true); 
	void HpChangeMagic(int amount, Unit *pAttacker = nullptr, AttributeType attributeType = AttributeNone);
	void SendHpChangeToAI(uint16_t sTargetID, int amount, AttributeType attributeType = AttributeNone);
	void MSpChange(int amount);

	bool CastSkill(Unit * pTarget, uint32_t nSkillID);

	void OnDeath(Unit *pKiller);
	void OnDeathProcess(Unit *pKiller);
	void PVPMonumentProcess(CUser *pUser);
	void BattleMonumentProcess(CUser *pUser);
	void NationMonumentProcess(CUser *pUser);
	void OnRespawn();

	bool isDead() { return m_NpcState == NPC_DEAD || m_iHP <= 0; };

	INLINE bool isMonster() { return m_bMonster; }
	INLINE bool isGate() { return GetType() == NPC_GATE || GetType() == NPC_PHOENIX_GATE || GetType() == NPC_SPECIAL_GATE || GetType() == NPC_VICTORY_GATE || GetType() == NPC_GATE2;};
	INLINE bool isGateOpen() { return m_byGateOpen; };
	INLINE bool isGateClosed() { return !isGateOpen(); };

	INLINE int16_t GetProtoID() { return m_sSid; };
	INLINE uint8_t GetType() { return m_tNpcType; };
	INLINE uint8_t GetState() { return m_byState; };

	virtual int32_t GetHealth() { return m_iHP; }
	virtual int32_t GetMaxHealth() { return m_iMaxHP; }
	virtual int32_t GetMana() { return 0; }
	virtual int32_t GetMaxMana() { return 0; }

	bool isHostileTo(Unit * pTarget);

	int16_t GetDamage(Unit *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false);
	int16_t GetDamage(CUser *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false);
	int16_t GetDamage(CNpc *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false);

	float GetRewardModifier(uint8_t byLevel);
	float GetPartyRewardModifier(uint32_t nPartyLevel, uint32_t nPartyMembers);

	virtual ~CNpc();

	DECLARE_LUA_CLASS(CNpc);

	DECLARE_LUA_GETTER(GetID)
	DECLARE_LUA_GETTER(GetProtoID)
	DECLARE_LUA_GETTER(GetName)
	DECLARE_LUA_GETTER(GetNation)
	DECLARE_LUA_GETTER(GetType)
	DECLARE_LUA_GETTER(GetZoneID)
	DECLARE_LUA_GETTER(GetX)
	DECLARE_LUA_GETTER(GetY)
	DECLARE_LUA_GETTER(GetZ)

	DECLARE_LUA_FUNCTION(CastSkill) {
		LUA_RETURN(LUA_GET_INSTANCE()->CastSkill(
			reinterpret_cast<Unit *>(LUA_ARG(CUser *, 2)),
			LUA_ARG(uint32_t, 3)
			));
	}
};
