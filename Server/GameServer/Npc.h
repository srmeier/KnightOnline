#pragma once

#include "LuaEngine.h"
#include "Unit.h"

class CNpc : public Unit
{
public:
	virtual uint16 GetID() { return m_sNid; }
	virtual std::string & GetName() { return m_strName; }

	uint16	m_sNid; // NPC ID
	uint16	m_sSid; // prototype ID

	short	m_sPid;				// MONSTER(NPC) Picture ID
	short	m_sSize;			// MONSTER(NPC) Size
	int		m_iWeapon_1;
	int		m_iWeapon_2;
	std::string m_strName;		// MONSTER(NPC) Name
	int		m_iMaxHP;			// �ִ� HP
	int		m_iHP;				// ���� HP
	uint8	m_byState;			// ������ (NPC) ����
	uint8	m_tNpcType;			// NPC Type
	// 0 : Normal Monster
	// 1 : NPC
	// 2 : �� �Ա�,�ⱸ NPC
	// 3 : ������
	int   m_iSellingGroup;		// ItemGroup

	uint8	m_NpcState;			// NPC�� ���� - ���Ҵ�, �׾���, ���ִ� ����...
	bool	m_byGateOpen;		// Gate status: true is open, false is closed.

	uint8    m_byObjectType;     // ������ 0, objectŸ��(����, ����)�� 1
	int16	m_byDirection;

	uint8   m_byTrapNumber;

	bool	m_bMonster; // are we a monster or an NPC?

	int16	m_oSocketID; // owner user
	uint16	m_bEventRoom;

public:
	CNpc();

	void Initialize();
	void AddToRegion(int16 new_region_x, int16 new_region_z);

	void MoveResult(float xpos, float ypos, float zpos, float speed);
	void GetInOut(Packet & result, uint8 bType);
	void SendInOut(uint8 bType, float fx, float fz, float fy);
	void GetNpcInfo(Packet & pkt);

	void SendGateFlag(uint8 bFlag = -1, bool bSendAI = true);

	void HpChange(int amount, Unit *pAttacker = nullptr, bool bSendToAI = true); 
	void HpChangeMagic(int amount, Unit *pAttacker = nullptr, AttributeType attributeType = AttributeNone);
	void SendHpChangeToAI(uint16 sTargetID, int amount, AttributeType attributeType = AttributeNone);
	void MSpChange(int amount);

	bool CastSkill(Unit * pTarget, uint32 nSkillID);

	void OnDeath(Unit *pKiller);
	void OnDeathProcess(Unit *pKiller);
	void ChaosStoneProcess(CUser *pUser, uint16 MonsterCount);
	void PVPMonumentProcess(CUser *pUser);
	void BattleMonumentProcess(CUser *pUser);
	void NationMonumentProcess(CUser *pUser);
	void OnRespawn();

	bool isDead() { return m_NpcState == NPC_DEAD || m_iHP <= 0; };

	INLINE bool isMonster() { return m_bMonster; }
	INLINE bool isGate() { return GetType() == NPC_GATE || GetType() == NPC_PHOENIX_GATE || GetType() == NPC_SPECIAL_GATE || GetType() == NPC_VICTORY_GATE || GetType() == NPC_GATE2;};
	INLINE bool isGateOpen() { return m_byGateOpen; };
	INLINE bool isGateClosed() { return !isGateOpen(); };

	INLINE short GetProtoID() { return m_sSid; };
	INLINE uint8 GetType() { return m_tNpcType; };
	INLINE uint8 GetState() { return m_byState; };

	virtual int32 GetHealth() { return m_iHP; }
	virtual int32 GetMaxHealth() { return m_iMaxHP; }
	virtual int32 GetMana() { return 0; }
	virtual int32 GetMaxMana() { return 0; }

	bool isHostileTo(Unit * pTarget);

	short GetDamage(Unit *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false);
	short GetDamage(CUser *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false);
	short GetDamage(CNpc *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false);

	float GetRewardModifier(uint8 byLevel);
	float GetPartyRewardModifier(uint32 nPartyLevel, uint32 nPartyMembers);

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
			LUA_ARG(uint32, 3)
			));
	}
};
