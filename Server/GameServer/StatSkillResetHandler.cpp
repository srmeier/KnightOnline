#include "stdafx.h"
#include "GameDefine.h"
#include <afx.h>

void CUser::OpenStatSkillReset()
{
	Packet result(WIZ_STAT_SKILL_RESET);

	result
		<< uint8_t(STAT_SKILL_RESET_REQ);

	Send(&result);
}

void CUser::HandleStatSkillReset(Packet& pkt)
{
	uint8_t opcode = pkt.read<uint8_t>();
	uint16_t iPlayerTargetID = pkt.read<uint16_t>();

	CNpc* pNpc = g_pMain->GetNpcPtr(iPlayerTargetID);
	
	if (pNpc == nullptr)
	{
		return;
	}

	if (!isInRange(pNpc, MAX_NPC_RANGE))
	{
		return;
	}
		
	switch (opcode)
	{
		case STAT_RESET_REQ:
			StatReset();
			break;
		case SKILL_RESET_REQ:
			SkillReset();
			break;
	}

}

void CUser::SendStatSkillResetFailed(e_StatSkillResetOpcode opcode)
{
	Packet result(WIZ_STAT_SKILL_RESET);
	result << uint8_t(opcode);
	Send(&result);
}

void CUser::SkillReset()
{
	if (isDead() || isTrading() || isStoreOpen() || isMerchanting() || isSellingMerchant())
		return;

	//unused skill points
	if (m_bstrSkill[0] > 0)
	{
		SendStatSkillResetFailed(SKILL_RESET_UNUSED_POINTS);
		return;
	}

	//Required coins for skill reset
	uint32_t iRequiredCoinsSkillReset = (int) pow((GetLevel() * 2.0f), 3.4f);
	if (GetLevel() < 30)
		iRequiredCoinsSkillReset = (int) (iRequiredCoinsSkillReset * 0.4f);
	else if (GetLevel() >= 60)
		iRequiredCoinsSkillReset = (int) (iRequiredCoinsSkillReset * 1.5f);

	iRequiredCoinsSkillReset = (int) (iRequiredCoinsSkillReset * 1.5f);

	// If global discounts are enabled 
	if (g_pMain->m_sDiscount == 2 // or war discounts are enabled
		|| (g_pMain->m_sDiscount == 1 && g_pMain->m_byOldVictory == m_bNation))
		iRequiredCoinsSkillReset /= 2;

	//calculate currentSkillPoints
	uint16_t iCurrentSkillPoints;

	if (GetLevel() >= 10)
		iCurrentSkillPoints = (GetLevel() - 9) * 2;
	else
		iCurrentSkillPoints = 0;

	if (GetLevel() < 10)
	{
		SendStatSkillResetFailed(SKILL_RESET_LEVEL_TOO_LOW);
		return;
	}

	if (!hasCoins(iRequiredCoinsSkillReset))
	{
		SendStatSkillResetFailed(SKILL_RESET_NO_GOLD);
		return;
	}

	// Reset skill points.
	
	m_bstrSkill[0] = (GetLevel() - 9) * 2;
	
	for (int i = 1; i < 9; i++)
		m_bstrSkill[i] = 0;

	m_iGold -= iRequiredCoinsSkillReset;
	
	SetUserAbility();

	Packet result(WIZ_STAT_SKILL_RESET);
	result << uint8_t(SKILL_RESET_SUCCESS)
		   << uint32_t(iRequiredCoinsSkillReset)
		   << uint32_t(m_iGold)
		   << uint8_t(m_bstrSkill[0]);

	Send(&result);
}

void CUser::StatReset()
{
	if (isDead() || isTrading() || isStoreOpen() || isMerchanting() || isSellingMerchant())
		return;

	//check level
	if (GetLevel() < 10)
	{
		SendStatSkillResetFailed(STAT_RESET_LEVEL_TOO_LOW);
		return;
	}

	//unused stat points
	if (m_sPoints > 0)
	{
		SendStatSkillResetFailed(STAT_RESET_UNUSED_POINTS);
		return;
	}

	//Calculate Required coins for stat reset
	uint32_t iCoinsStatReset = (int) pow((GetLevel() * 2.0f), 3.4f);
	if (GetLevel() < 30)
		iCoinsStatReset = (int) (iCoinsStatReset * 0.4f);
	else if (GetLevel() >= 60)
		iCoinsStatReset = (int) (iCoinsStatReset * 1.5f);

	// discount
	if ((g_pMain->m_sDiscount == 1 && g_pMain->m_byOldVictory == GetNation())
		|| g_pMain->m_sDiscount == 2)
		iCoinsStatReset /= 2;

	//check gold
	if (!hasCoins(iCoinsStatReset))
	{
		SendStatSkillResetFailed(STAT_RESET_NO_GOLD);
		return;
	}

	//check if user has equiped item, accessories not important.
	//check for 1,4,6,8,10,12,13,

	int aiEquipControlSlots[7] = { 1,4,6,8,10,12,13 };

	for (int slot : aiEquipControlSlots)
	{
		if (m_sItemArray[slot].nSerialNum != 0)
		{
			SendStatSkillResetFailed(STAT_RESET_ITEMS_EQUIPED);
			return;
		}
	}
	
	//calculate stat points user have at the moment.
	uint16_t iCurrentStatPoints;
	uint8_t iStartingStatPoints = 10;
	
	if (GetLevel() < 60)
	{
		iCurrentStatPoints = (GetLevel() - 1) * 3;
		iCurrentStatPoints += iStartingStatPoints;
	}
	else
	{
		iCurrentStatPoints = 59 * 3;
		iCurrentStatPoints += (GetLevel() - 60) * 5;
		iCurrentStatPoints += iStartingStatPoints;
	}

	// TODO: Pull this from the database.
	switch (m_bRace)
	{
		case KARUS_BIG:
			if (isWarrior())
			{
				SetStat(STAT_STR, 65);
				SetStat(STAT_STA, 65);
				SetStat(STAT_DEX, 60);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
			}
			break;
		case KARUS_MIDDLE:
			if (isWarrior())
			{
				SetStat(STAT_STR, 65);
				SetStat(STAT_STA, 65);
				SetStat(STAT_DEX, 60);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
			}
			else
			{
				SetStat(STAT_STR, 60);
				SetStat(STAT_STA, 60);
				SetStat(STAT_DEX, 70);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
			}
			break;
		case KARUS_SMALL:
			if (isWarrior())
			{
				SetStat(STAT_STR, 65);
				SetStat(STAT_STA, 65);
				SetStat(STAT_DEX, 60);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
			}
			else if (isRogue())
			{
				SetStat(STAT_STR, 60);
				SetStat(STAT_STA, 60);
				SetStat(STAT_DEX, 70);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
			}
			else if (isPriest())
			{
				SetStat(STAT_STR, 50);
				SetStat(STAT_STA, 50);
				SetStat(STAT_DEX, 70);
				SetStat(STAT_INT, 70);
				SetStat(STAT_CHA, 50);
			}
			else  if (isMage())
			{
				SetStat(STAT_STR, 50);
				SetStat(STAT_STA, 60);
				SetStat(STAT_DEX, 60);
				SetStat(STAT_INT, 70);
				SetStat(STAT_CHA, 50);
			}
			break;
		case KARUS_WOMAN:
			if (isWarrior())
			{
				SetStat(STAT_STR, 65);
				SetStat(STAT_STA, 65);
				SetStat(STAT_DEX, 60);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
			}
			else if (isRogue())
			{
				SetStat(STAT_STR, 60);
				SetStat(STAT_STA, 60);
				SetStat(STAT_DEX, 70);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
			}
			else if (isPriest())
			{
				SetStat(STAT_STR, 50);
				SetStat(STAT_STA, 50);
				SetStat(STAT_DEX, 70);
				SetStat(STAT_INT, 70);
				SetStat(STAT_CHA, 50);
			}
			else  if (isMage())
			{
				SetStat(STAT_STR, 50);
				SetStat(STAT_STA, 60);
				SetStat(STAT_DEX, 60);
				SetStat(STAT_INT, 70);
				SetStat(STAT_CHA, 50);
			}
			break;
		case BABARIAN:
			if (isWarrior())
			{
				SetStat(STAT_STR, 65);
				SetStat(STAT_STA, 65);
				SetStat(STAT_DEX, 60);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
			}
			break;
		case ELMORAD_MAN:
			if (isWarrior())
			{
				SetStat(STAT_STR, 65);
				SetStat(STAT_STA, 65);
				SetStat(STAT_DEX, 60);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
			}
			else if (isRogue())
			{
				SetStat(STAT_STR, 60);
				SetStat(STAT_STA, 60);
				SetStat(STAT_DEX, 70);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
			}
			else if (isPriest())
			{
				SetStat(STAT_STR, 50);
				SetStat(STAT_STA, 50);
				SetStat(STAT_DEX, 70);
				SetStat(STAT_INT, 70);
				SetStat(STAT_CHA, 50);
			}
			else  if (isMage())
			{
				SetStat(STAT_STR, 50);
				SetStat(STAT_STA, 60);
				SetStat(STAT_DEX, 60);
				SetStat(STAT_INT, 70);
				SetStat(STAT_CHA, 50);
			}
			break;
		case ELMORAD_WOMAN:
			if (isWarrior())
			{
				SetStat(STAT_STR, 65);
				SetStat(STAT_STA, 65);
				SetStat(STAT_DEX, 60);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
			}
			else if (isRogue())
			{
				SetStat(STAT_STR, 60);
				SetStat(STAT_STA, 60);
				SetStat(STAT_DEX, 70);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
			}
			else if (isPriest())
			{
				SetStat(STAT_STR, 50);
				SetStat(STAT_STA, 50);
				SetStat(STAT_DEX, 70);
				SetStat(STAT_INT, 70);
				SetStat(STAT_CHA, 50);
			}
			else  if (isMage())
			{
				SetStat(STAT_STR, 50);
				SetStat(STAT_STA, 60);
				SetStat(STAT_DEX, 60);
				SetStat(STAT_INT, 70);
				SetStat(STAT_CHA, 50);
			}
			break;
	}

	//NOTE: As final control weight check can be performed for the items in inventory


	//turn back points to the user

	if (iCurrentStatPoints > 290)
		iCurrentStatPoints = 290;

	m_sPoints = iCurrentStatPoints;
	
	m_iGold -= iCoinsStatReset;

	//recalculate resistances, ac and other things which change with stats
	//this also updates AI server about user
	SetUserAbility();

	Packet result(WIZ_STAT_SKILL_RESET);
	result << uint8_t(STAT_RESET_SUCCESS)
		<< uint32_t(iCoinsStatReset)
		<< uint32_t(m_iGold)
		<< uint16_t(GetStat(STAT_STR))
		<< uint16_t(GetStat(STAT_STA))
		<< uint16_t(GetStat(STAT_DEX))
		<< uint16_t(GetStat(STAT_INT))
		<< uint16_t(GetStat(STAT_CHA))
		<< uint16_t(m_sPoints);

	Send(&result);

	return;

}
