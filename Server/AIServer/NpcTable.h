#pragma once

#define MONSTER_SPEED	1500

class CNpcTable  
{
public:
	uint16_t	m_sSid;				// MONSTER(NPC) Serial ID
	std::string	m_strName;		// MONSTER(NPC) Name
	uint16_t	m_sPid;				// MONSTER(NPC) Picture ID
	uint16_t	m_sSize;			// 캐릭터의 비율(100 퍼센트 기준)
	uint32_t	m_iWeapon_1;		// 무기계열(오른손)
	uint32_t	m_iWeapon_2;		// 무기계열(왼손)
	uint8_t	m_byGroup;			// 소속집단
	uint8_t	m_byActType;		// 행동패턴
	uint8_t	m_tNpcType;			// NPC Type
	// 0 : Monster
	// 1 : Normal NPC
	uint8_t	m_byFamilyType;		// 몹들사이에서 가족관계를 결정한다.
	uint8_t	m_byRank;			// 작위
	uint8_t	m_byTitle;			// 지위
	uint32_t	m_iSellingGroup;	// 아이템 그룹(물건매매 담당 NPC의 경우만)
	uint16_t	m_sLevel;			// level
	uint32_t	m_iExp;				// 경험치
	uint32_t	m_iLoyalty;			// loyalty

	uint32_t	m_iMaxHP;			// 최대 HP
	uint16_t	m_sMaxMP;			// 최대 MP

	uint16_t	m_sAttack;			// 공격값
	uint16_t	m_sDefense;			// 방어값
	uint16_t	m_sHitRate;			// 타격성공률
	uint16_t	m_sEvadeRate;		// 회피성공률
	uint16_t	m_sDamage;			// 기본 데미지
	uint16_t	m_sAttackDelay;		// 공격딜레이
	uint16_t	m_sSpeed;			// 이동속도	
	uint8_t	m_bySpeed_1;		// 기본 이동 타입
	uint8_t	m_bySpeed_2;		// 뛰는 이동 타입..
	uint16_t	m_sStandTime;		// 서있는 시간
	uint32_t	m_iMagic1;			// 사용마법 1
	uint32_t	m_iMagic2;			// 사용마법 2
	uint32_t	m_iMagic3;			// 사용마법 3	

	uint16_t	m_byFireR;			// 화염 저항력
	uint16_t	m_byColdR;			// 냉기 저항력
	uint16_t	m_byLightningR;		// 전기 저항력
	uint16_t	m_byMagicR;			// 마법 저항력
	uint16_t	m_byDiseaseR;		// 저주 저항력
	uint16_t	m_byPoisonR;		// 독 저항력

	float	m_fBulk;

	uint8_t	m_bySearchRange;	// 적 탐지 범위
	uint8_t	m_byAttackRange;	// 사정거리
	uint8_t	m_byTracingRange;	// 추격거리

	uint32_t	m_iMoney;			// 떨어지는 돈
	uint16_t	m_iItem;			// 떨어지는 아이템
	uint8_t	m_byDirectAttack;	// 공격방법( 0 : 직접, 1:롱공격(간접공격), 2:직,간접공격 )
	uint8_t	m_byMagicAttack;	// 마법공격( 0:마법공격 없음, 1:마법공격, 2:독공격, 3:힐링)
	uint8_t	m_byGroupSpecial;

	CNpcTable() : m_sSpeed(MONSTER_SPEED)
	{
	}
};