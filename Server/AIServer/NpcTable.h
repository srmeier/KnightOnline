#pragma once

#define MONSTER_SPEED	1500

class CNpcTable  
{
public:
	uint16	m_sSid;				// MONSTER(NPC) Serial ID
	std::string	m_strName;		// MONSTER(NPC) Name
	uint16	m_sPid;				// MONSTER(NPC) Picture ID
	uint16	m_sSize;			// 캐릭터의 비율(100 퍼센트 기준)
	uint32	m_iWeapon_1;		// 무기계열(오른손)
	uint32	m_iWeapon_2;		// 무기계열(왼손)
	uint8	m_byGroup;			// 소속집단
	uint8	m_byActType;		// 행동패턴
	uint8	m_tNpcType;			// NPC Type
	// 0 : Monster
	// 1 : Normal NPC
	uint8	m_byFamilyType;		// 몹들사이에서 가족관계를 결정한다.
	uint8	m_byRank;			// 작위
	uint8	m_byTitle;			// 지위
	uint32	m_iSellingGroup;	// 아이템 그룹(물건매매 담당 NPC의 경우만)
	uint16	m_sLevel;			// level
	uint32	m_iExp;				// 경험치
	uint32	m_iLoyalty;			// loyalty

	uint32	m_iMaxHP;			// 최대 HP
	uint16	m_sMaxMP;			// 최대 MP

	uint16	m_sAttack;			// 공격값
	uint16	m_sDefense;			// 방어값
	uint16	m_sHitRate;			// 타격성공률
	uint16	m_sEvadeRate;		// 회피성공률
	uint16	m_sDamage;			// 기본 데미지
	uint16	m_sAttackDelay;		// 공격딜레이
	uint16	m_sSpeed;			// 이동속도	
	uint8	m_bySpeed_1;		// 기본 이동 타입
	uint8	m_bySpeed_2;		// 뛰는 이동 타입..
	uint16	m_sStandTime;		// 서있는 시간
	uint32	m_iMagic1;			// 사용마법 1
	uint32	m_iMagic2;			// 사용마법 2
	uint32	m_iMagic3;			// 사용마법 3	

	uint16	m_byFireR;			// 화염 저항력
	uint16	m_byColdR;			// 냉기 저항력
	uint16	m_byLightningR;		// 전기 저항력
	uint16	m_byMagicR;			// 마법 저항력
	uint16	m_byDiseaseR;		// 저주 저항력
	uint16	m_byPoisonR;		// 독 저항력

	float	m_fBulk;

	uint8	m_bySearchRange;	// 적 탐지 범위
	uint8	m_byAttackRange;	// 사정거리
	uint8	m_byTracingRange;	// 추격거리

	uint32	m_iMoney;			// 떨어지는 돈
	uint16	m_iItem;			// 떨어지는 아이템
	uint8	m_byDirectAttack;	// 공격방법( 0 : 직접, 1:롱공격(간접공격), 2:직,간접공격 )
	uint8	m_byMagicAttack;	// 마법공격( 0:마법공격 없음, 1:마법공격, 2:독공격, 3:힐링)
	uint8	m_byGroupSpecial;

	CNpcTable() : m_sSpeed(MONSTER_SPEED)
	{
	}
};