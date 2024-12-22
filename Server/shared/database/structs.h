#pragma once

struct _MAGIC_TABLE
{
	uint32_t	iNum;
	uint32_t	nBeforeAction;
	uint8_t	bTargetAction;
	uint8_t	bSelfEffect;
	uint16_t	bFlyingEffect;
	uint16_t	iTargetEffect;
	uint8_t	bMoral;
	uint16_t	sSkillLevel;	
	uint16_t	sSkill;
	uint16_t	sMsp;
	uint16_t	sHP;
	uint8_t	bItemGroup;
	uint32_t	iUseItem;
	uint8_t	bCastTime;
	uint16_t	sReCastTime;
	uint8_t	bSuccessRate;
	uint8_t	bType[2];
	uint16_t	sRange;
	uint8_t	sUseStanding;
	uint16_t	sEtc;
};

struct _MAGIC_TYPE1
{
	uint32_t	iNum;
	uint8_t	bHitType;
	uint16_t	sHitRate;
	uint16_t	sHit;
	uint16_t	sAddDamage;
	uint8_t	bDelay;
	uint8_t	bComboType;
	uint8_t	bComboCount;
	uint16_t	sComboDamage;
	uint16_t	sRange;
};

struct _MAGIC_TYPE2
{
	uint32_t	iNum;
	uint8_t	bHitType;
	uint16_t	sHitRate;
	uint16_t	sAddDamage;
	uint16_t	sAddRange;
	uint8_t	bNeedArrow;
};

struct _MAGIC_TYPE3
{
	uint32_t	iNum;
	uint8_t	bDirectType;
	uint16_t	sAngle;
	int16_t	sFirstDamage;
	int16_t	sEndDamage;
	int16_t	sTimeDamage;
	uint8_t	bRadius;
	uint8_t	bDuration;  // duration, in seconds
	uint8_t	bAttribute;
};

struct _MAGIC_TYPE4
{
	uint32_t	iNum;
	uint8_t	bBuffType;
	uint8_t	bRadius;
	uint16_t	sDuration;  // duration, in seconds
	uint8_t	bAttackSpeed;
	uint8_t	bSpeed;
	int16_t	sAC;
	uint16_t	sACPct;
	uint8_t	bAttack;
	uint8_t	bMagicAttack;
	uint16_t	sMaxHP;
	uint16_t	sMaxHPPct;
	uint16_t	sMaxMP;
	uint16_t	sMaxMPPct;
	uint8_t	bHitRate;
	uint16_t	sAvoidRate;
	int8_t	bStr;
	int8_t	bSta;
	int8_t	bDex;
	int8_t	bIntel;
	int8_t	bCha;
	uint8_t	bFireR;
	uint8_t	bColdR;
	uint8_t	bLightningR;
	uint8_t	bMagicR;
	uint8_t	bDiseaseR;
	uint8_t	bPoisonR;
	uint16_t	sExpPct;
	uint16_t	sSpecialAmount;

	bool	bIsBuff; // true if buff, false if debuff

	INLINE bool isBuff() { return bIsBuff; }
	INLINE bool isDebuff() { return !bIsBuff; }
};

struct _MAGIC_TYPE5
{
	uint32_t	iNum;
	uint8_t	bType;
	uint8_t	bExpRecover;
	uint16_t	sNeedStone;
};

struct _MAGIC_TYPE6
{
	uint32_t	iNum;
	uint16_t	sSize;
	uint16_t	sTransformID;
	uint16_t	sDuration; // duration, in seconds
	uint16_t	sMaxHp;
	uint16_t	sMaxMp;
	uint8_t	bSpeed;
	uint16_t	sAttackSpeed;
	uint16_t	sTotalHit;
	uint16_t	sTotalAc;
	uint16_t	sTotalHitRate;
	uint16_t	sTotalEvasionRate;
	uint16_t	sTotalFireR;
	uint16_t	sTotalColdR;
	uint16_t	sTotalLightningR;
	uint16_t	sTotalMagicR;
	uint16_t	sTotalDiseaseR;
	uint16_t	sTotalPoisonR;
	uint16_t	sClass;
	uint8_t	bUserSkillUse;
	uint8_t	bNeedItem;
	uint8_t	bSkillSuccessRate;
	uint8_t	bMonsterFriendly;
	uint8_t	bNation;
};


enum TransformationSkillUse
{
	TransformationSkillUseSiege		= 0,
	TransformationSkillUseMonster	= 1,
	TransformationSkillUseNPC		= 3,
	TransformationSkillUseSpecial	= 4, // e.g. Snowman transformations
	TransformationSkillUseSiege2	= 7  // e.g. Moving Towers
};

struct _MAGIC_TYPE7
{
	uint32_t	iNum;
	uint8_t	bValidGroup;
	uint8_t	bNationChange;
	uint16_t	sMonsterNum;
	uint8_t	bTargetChange;
	uint8_t	bStateChange;
	uint8_t	bRadius;
	uint16_t	sHitRate;
	uint16_t	sDuration;
	uint16_t	sDamage;
	uint8_t	bVision;
	uint32_t	nNeedItem;
};

struct _MAGIC_TYPE8
{
	uint32_t	iNum;
	uint8_t	bTarget;
	uint16_t	sRadius;
	uint8_t	bWarpType;
	uint16_t	sExpRecover;
	uint16_t	sKickDistance; // used exclusively by soccer ball-control skills.
};

struct _MAGIC_TYPE9
{
	uint32_t	iNum;
	uint8_t	bValidGroup;
	uint8_t	bNationChange;
	uint16_t	sMonsterNum;
	uint8_t	bTargetChange;
	uint8_t	bStateChange;
	uint16_t	sRadius;
	uint16_t	sHitRate;
	uint16_t	sDuration;
	uint16_t	sDamage;
	uint16_t	sVision;
	uint32_t	nNeedItem;
};

struct _SERVER_RESOURCE
{
	uint32_t nResourceID;
	std::string strResource;
};

struct _OBJECT_EVENT
{
	int nIndex;
	uint16_t sZoneID;
	int sBelong;
	int16_t sIndex;
	int16_t sType;
	int16_t sControlNpcID;
	int16_t sStatus;
	float fPosX;
	float fPosY;
	float fPosZ;
	uint8_t byLife;
};

#pragma pack(push, 1)
struct _REGENE_EVENT
{
	float fRegenePosX;
	float fRegenePosY;
	float fRegenePosZ;
	float fRegeneAreaZ;
	float fRegeneAreaX;
	int	  sRegenePoint;
};

struct _WARP_INFO
{
	int16_t	sWarpID;
	char	strWarpName[32];
	char	strAnnounce[256];
	uint16_t	sUnk0; // padding?
	uint32_t	dwPay;
	int16_t	sZone;
	uint16_t	sUnk1; // padding?
	float	fX;
	float	fY;
	float	fZ;
	float	fR;
	int16_t	sNation;
	uint16_t	sUnk2; // padding?

	_WARP_INFO() { memset(this, 0, sizeof(_WARP_INFO)); };
};
#pragma pack(pop)

struct _ZONE_INFO
{
	uint16_t	m_nServerNo;
	uint16_t	m_nZoneNumber;
	std::string	m_MapName;

#if defined(AI_SERVER)
	uint8_t	m_byRoomEvent;
#else
	float m_fInitX, m_fInitY, m_fInitZ;
	uint8_t m_bType, isAttackZone;
#endif
};

struct _MONSTER_SUMMON_LIST
{
	uint16_t	sSid;
	uint16_t	sLevel;
	uint16_t	sProbability;
	uint8_t	bType;
};

#define MAX_PARTY_USERS		8
struct	_PARTY_GROUP
{
	uint16_t	wIndex;
	int16_t	uid		[MAX_PARTY_USERS];

#ifdef GAMESERVER
	uint8_t	bItemRouting;
	std::string	WantedMessage;
	uint16_t	sWantedClass;
#endif

	_PARTY_GROUP()
	{
		for (int i = 0; i < MAX_PARTY_USERS; i++)
			uid[i] = -1; 

#ifdef GAMESERVER
		bItemRouting = 0;
#endif
	}
};

struct _BUFF_TYPE4_INFO
{
	uint32_t	m_nSkillID;
	uint8_t	m_bBuffType;
	bool	m_bIsBuff; // Is it a buff or a debuff?
	bool	m_bDurationExtended;
	time_t	m_tEndTime;

	INLINE bool isBuff() { return m_bIsBuff; }
	INLINE bool isDebuff() { return !m_bIsBuff; }

	_BUFF_TYPE4_INFO() 
		: m_nSkillID(0), m_bBuffType(0), m_bIsBuff(false), m_bDurationExtended(false), m_tEndTime(0)
	{
	}
};

struct _BUFF_TYPE9_INFO
{
	uint32_t	nSkillID;
	time_t	tEndTime;

	_BUFF_TYPE9_INFO(uint32_t nSkillID, time_t tEndTime)
	{
		this->nSkillID = nSkillID;
		this->tEndTime = tEndTime;
	}
};
