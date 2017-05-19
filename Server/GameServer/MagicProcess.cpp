#include "stdafx.h"
#include "MagicProcess.h"
#include "MagicInstance.h"
#include "Map.h"


#if defined(GAMESERVER)
#	include "GameServerDlg.h"
#	include "../shared/DateTime.h"
#else
#	include "../AIServer/ServerDlg.h"
#	include "../AIServer/User.h"
#endif

#if defined(GAMESERVER)
void CMagicProcess::MagicPacket(Packet & pkt, Unit * pCaster /*= nullptr*/)
{
	if (g_pMain->m_IsMagicTableInUpdateProcess)
		return;

	MagicInstance instance;
	pkt >> instance.bOpcode >> instance.nSkillID;

	instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);
	if (instance.pSkill == nullptr)
	{
		if (pCaster != nullptr)
			TRACE("[%s] Used skill %d but it does not exist.\n", pCaster->GetName().c_str(), instance.nSkillID);

		if (pCaster->isPlayer() && instance.nSkillID < 0)
		{
			DateTime time;
			g_pMain->SendFormattedNotice("%s is currently disconnect for skill hack.",Nation::ALL, pCaster->GetName().c_str());
			g_pMain->WriteCheatLogFile(string_format("[ SkillHack - %d:%d:%d ] %s Disconnected for SkillHack.\n", time.GetHour(),time.GetMinute(),time.GetSecond(), pCaster->GetName().c_str()));
			TO_USER(pCaster)->Disconnect();
		}

		return;
	}

	pkt >> instance.sCasterID >> instance.sTargetID
		>> instance.sData[0] >> instance.sData[1] >> instance.sData[2] >> instance.sData[3]
	>> instance.sData[4] >> instance.sData[5] >> instance.sData[6];

	// Prevent users from faking other players or NPCs.
	if (pCaster != nullptr // if it's nullptr, it's from AI.
		&& (instance.sCasterID >= NPC_BAND 
		|| instance.sCasterID != pCaster->GetID()))
		return;

	instance.bIsRecastingSavedMagic = false;
	instance.Run();
}

void CMagicProcess::UpdateAIServer(uint32_t nSkillID, AISkillOpcode opcode, 
								   Unit * pTarget, Unit * pCaster /*= nullptr*/, 
								   bool bIsRecastingSavedMagic /*= false*/)
{
	Packet result(AG_MAGIC_ATTACK_REQ);
	int16_t	sCasterID = (pCaster == nullptr ? -1 : pCaster->GetID()),
		sTargetID = (pTarget == nullptr ? -1 : pTarget->GetID());
	result << uint8_t(opcode) << nSkillID << sCasterID << sTargetID << bIsRecastingSavedMagic;
	g_pMain->Send_AIServer(&result);
}
#else
void CMagicProcess::MagicPacket(Packet & pkt, Unit * pCaster /*= nullptr*/)
{
	_MAGIC_TABLE * pSkill;
	_MAGIC_TYPE4 * pType4;
	Unit * pSkillCaster, * pSkillTarget;
	uint32_t nSkillID;
	uint16_t sCasterID, sTargetID;
	uint8_t bOpcode;
	bool bIsRecastingSavedMagic;

	pkt >> bOpcode >> nSkillID >> sCasterID >> sTargetID >> bIsRecastingSavedMagic;

	pSkill = g_pMain->m_MagictableArray.GetData(nSkillID);
	if (pSkill == nullptr)
		return;

	pSkillCaster = g_pMain->GetUnitPtr(sCasterID);
	pSkillTarget = g_pMain->GetUnitPtr(sTargetID);

	if (bOpcode == AISkillOpcodeBuff || bOpcode == AISkillOpcodeRemoveBuff)
	{
		pType4 = g_pMain->m_Magictype4Array.GetData(nSkillID);
		if (pType4 == nullptr)
			return;
	}

	switch (bOpcode)
	{
	case AISkillOpcodeBuff:
		if (pSkillCaster == nullptr || pSkillTarget == nullptr)
			return;

		GrantType4Buff(pSkill, pType4, pSkillCaster, pSkillTarget, bIsRecastingSavedMagic);
		break;

	case AISkillOpcodeRemoveBuff:
		if (pSkillTarget == nullptr)
			return;

		RemoveType4Buff(pType4->bBuffType, pSkillTarget);
		break;
	}

}
#endif

// TODO: Clean this up (even using unit code...)
bool CMagicProcess::UserRegionCheck(Unit * pSkillCaster, Unit * pSkillTarget, _MAGIC_TABLE * pSkill, int radius, int16_t mousex /*= 0*/, int16_t mousez /*= 0*/)
{
	if (pSkillCaster->isDead()
		|| pSkillTarget == nullptr)
		return false;

	switch (pSkill->bMoral)
	{
	case MORAL_PARTY_ALL:		// Check that it's your party.
		// NPCs cannot be in parties.
		if (pSkillCaster->isNPC()
			|| pSkillTarget->isNPC())
			return false;

		if (!TO_USER(pSkillTarget)->isInParty())
			return (pSkillTarget == pSkillCaster);

		if (TO_USER(pSkillTarget)->GetPartyID() == TO_USER(pSkillCaster)->GetPartyID()
			&& pSkill->bType[0] != 8)
			goto final_test;
		else if (TO_USER(pSkillTarget)->GetPartyID() == TO_USER(pSkillCaster)->GetPartyID() 
			&& pSkill->bType[0] == 8)
		{
			if (pSkillTarget->GetMap()->isWarZone() && (UNIXTIME - TO_USER(pSkillTarget)->m_tLastRegeneTime < CLAN_SUMMON_TIME))
				return false;

			goto final_test;	
		}

		break;

		// Nation alone cannot dictate whether a unit can attack another.
		// As such, we must check behaviour specific to these entities.
		// For example: same nation players attacking each other in an arena.
	case MORAL_SELF_AREA:
	case MORAL_AREA_ENEMY:
		if (pSkillCaster->isHostileTo(pSkillTarget))
			goto final_test;
		break;
	
	case MORAL_AREA_ALL:
		if (pSkillCaster->isNPC()
			|| pSkillTarget->isNPC())
			return false;

		if ((TO_USER(pSkillCaster)->isInArena() 
			&& TO_USER(pSkillTarget)->isInArena())
			|| (TO_USER(pSkillCaster)->isInPVPZone() 
			&& TO_USER(pSkillTarget)->isInPVPZone())
			|| (TO_USER(pSkillCaster)->isInTempleEventZone()
			&& TO_USER(pSkillTarget)->isInTempleEventZone()))
		goto final_test;
		
		// Players cant attack other players in the safety area.
		if (TO_USER(pSkillCaster)->isInSafetyArea() 
			&& TO_USER(pSkillTarget)->isInSafetyArea())
			return false;
		break;

	case MORAL_AREA_FRIEND:
		if (!pSkillCaster->isHostileTo(pSkillTarget))
			goto final_test;
		break;

	case MORAL_CLAN_ALL:
		// NPCs cannot be in clans.
		if (pSkillCaster->isNPC()
			|| pSkillTarget->isNPC())
			return false;

		if (!TO_USER(pSkillTarget)->isInClan())
			return (pSkillTarget == pSkillCaster);

		if (TO_USER(pSkillTarget)->GetClanID() == TO_USER(pSkillCaster)->GetClanID() 
			&& pSkill->bType[0] != 8)
			goto final_test;
		else if (TO_USER(pSkillTarget)->GetClanID() == TO_USER(pSkillCaster)->GetClanID() 
			&& pSkill->bType[0] == 8)
		{
			if (pSkillTarget->GetMap()->isWarZone() && (UNIXTIME - TO_USER(pSkillTarget)->m_tLastRegeneTime < CLAN_SUMMON_TIME))
				return false;
			goto final_test;	
		}
		break;
	}
	return false;	

final_test:
	return (radius == 0 || pSkillTarget->isInRangeSlow(mousex, mousez, (float) radius));
}

#if defined(GAMESERVER)
void CMagicProcess::CheckExpiredType6Skills(Unit * pTarget)
{
	if (!pTarget->isPlayer()
		|| !TO_USER(pTarget)->isTransformed()
		|| (UNIXTIME - TO_USER(pTarget)->m_tTransformationStartTime) < TO_USER(pTarget)->m_sTransformationDuration)
		return;

	MagicInstance instance;
	instance.pSkillCaster = pTarget;
	instance.Type6Cancel();
}

void CMagicProcess::CheckExpiredType9Skills(Unit * pTarget, bool bForceExpiration /*= false*/)
{
	if (!pTarget->isPlayer())
		return;

	Guard lock(pTarget->m_buffLock);
	Type9BuffMap & buffMap = pTarget->m_type9BuffMap;

	MagicInstance instance;
	instance.pSkillCaster = pTarget;

	for (auto itr = buffMap.begin(); itr != buffMap.end();)
	{
		if (bForceExpiration || UNIXTIME >= itr->second.tEndTime) 
		{
			// Cancel the skill, but don't remove it from the map. We'll do that.
			instance.nSkillID = itr->second.nSkillID;
			instance.Type9Cancel(false); 
			itr = buffMap.erase(itr);
		}
		else 
		{
			++itr;
		}
	}
}

void CMagicProcess::RemoveStealth(Unit * pTarget, InvisibilityType bInvisibilityType)
{
	if (bInvisibilityType != INVIS_DISPEL_ON_MOVE
		&& bInvisibilityType != INVIS_DISPEL_ON_ATTACK)
		return;

	Guard lock(pTarget->m_buffLock);
	Type9BuffMap & buffMap = pTarget->m_type9BuffMap;
	MagicInstance instance;

	// Buff type is 1 for 'dispel on move' skills and 2 for 'dispel on attack' skills.
	uint8_t bType = (bInvisibilityType == INVIS_DISPEL_ON_MOVE ? 1 : 2);

	// If there's no such skill active on the user, no reason to remove it.
	auto itr = buffMap.find(bType);
	if (itr == buffMap.end())
		return;

	instance.pSkillCaster = pTarget;
	instance.nSkillID = itr->second.nSkillID;
	instance.Type9Cancel();
}
#endif

bool CMagicProcess::GrantType4Buff(_MAGIC_TABLE * pSkill, _MAGIC_TYPE4 *pType, Unit * pCaster, Unit *pTarget, bool bIsRecastingSavedMagic /*= false*/)
{
	// Buff mustn't already be added at this point.
	Guard lock(pTarget->m_buffLock);
	if (!bIsRecastingSavedMagic
		&& pTarget->m_buffMap.find(pType->bBuffType) != pTarget->m_buffMap.end())
		return false;

	switch (pType->bBuffType)
	{
	case BUFF_TYPE_HP_MP:
		if (pType->sMaxHP == 0 && pType->sMaxHPPct > 0)
			pTarget->m_sMaxHPAmount = (pType->sMaxHPPct - 100) * (pTarget->GetMaxHealth() - pTarget->m_sMaxHPAmount) / 100;
		else
			pTarget->m_sMaxHPAmount = pType->sMaxHP;

		if (pType->sMaxMP == 0 && pType->sMaxMPPct > 0)
			pTarget->m_sMaxMPAmount = (pType->sMaxMPPct - 100) * (pTarget->GetMaxMana() - pTarget->m_sMaxMPAmount) / 100;
		else
			pTarget->m_sMaxMPAmount = pType->sMaxMP;
		break;

	case BUFF_TYPE_AC:
		if (pType->sAC == 0 && pType->sACPct > 0)
			pTarget->m_sACPercent += (pType->sACPct - 100);
		else
			pTarget->m_sACAmount += pType->sAC;
		break;

	case BUFF_TYPE_SIZE:
		if (pCaster->isPlayer())
		{
			// Unfortunately there's no way to differentiate which does what.
			// Officially it also resorts to checking the skill ID.
			uint8_t bEffect = ABNORMAL_NORMAL;
			switch (pSkill->iNum)
			{
			case 490034: // Bezoar
				bEffect = ABNORMAL_GIANT;
				break;

			case 490401: // Maximize Scroll
				bEffect = ABNORMAL_GIANT_TARGET; // NOTE: not sure why, but this is what it uses officially.
				break;

			case 490035: // Rice cake
			case 490100: // unknown, possibly intended to be "Minimize Scroll"
				bEffect = ABNORMAL_DWARF;
				break;
			}

			if (bEffect != ABNORMAL_NORMAL)
				pTarget->StateChangeServerDirect(3, bEffect);
		}
		break;

	case BUFF_TYPE_DAMAGE:
		pTarget->m_bAttackAmount = pType->bAttack;
		break;

	case BUFF_TYPE_ATTACK_SPEED:
		pTarget->m_sAttackSpeedAmount += (pType->bAttackSpeed - 100);
		break;

	case BUFF_TYPE_SPEED:
		pTarget->m_bSpeedAmount = pType->bSpeed;
		break;

	case BUFF_TYPE_STATS:
		if (pTarget->isPlayer())
		{
			TO_USER(pTarget)->SetStatBuff(STAT_STR, pType->bStr);
			TO_USER(pTarget)->SetStatBuff(STAT_STA, pType->bSta);
			TO_USER(pTarget)->SetStatBuff(STAT_DEX, pType->bDex);
			TO_USER(pTarget)->SetStatBuff(STAT_INT, pType->bIntel);
			TO_USER(pTarget)->SetStatBuff(STAT_CHA, pType->bCha);	
		}
		break;

	case BUFF_TYPE_RESISTANCES:
		pTarget->m_bAddFireR = pType->bFireR;
		pTarget->m_bAddColdR = pType->bColdR;
		pTarget->m_bAddLightningR = pType->bLightningR;
		pTarget->m_bAddMagicR = pType->bMagicR;
		pTarget->m_bAddDiseaseR = pType->bDiseaseR;
		pTarget->m_bAddPoisonR = pType->bPoisonR;
		break;

	case BUFF_TYPE_ACCURACY:
		pTarget->m_bHitRateAmount = pType->bHitRate;
		pTarget->m_sAvoidRateAmount = pType->sAvoidRate;
		break;	

	case BUFF_TYPE_MAGIC_POWER:
		if (pTarget->isPlayer())
			pTarget->m_sMagicAttackAmount = (pType->bMagicAttack - 100) * TO_USER(pTarget)->GetStat(STAT_CHA) / 100;
		break;

	case BUFF_TYPE_EXPERIENCE:
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_sExpGainAmount = (uint8_t) pType->sExpPct;
		break;

	case BUFF_TYPE_WEIGHT:
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_bMaxWeightAmount = (uint8_t) pType->sExpPct;
		break;

	case BUFF_TYPE_WEAPON_DAMAGE:
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_bAddWeaponDamage = pType->bAttack;
		break;

	case BUFF_TYPE_WEAPON_AC:
		if (pTarget->isPlayer())
		{
			if (pType->sAC == 0 && pType->sACPct > 0)
				TO_USER(pTarget)->m_bPctArmourAc += (pType->sACPct - 100);
			else
				TO_USER(pTarget)->m_sAddArmourAc += pType->sAC;
		}
		break;

	case BUFF_TYPE_LOYALTY:
		if(pTarget->isPlayer())
			TO_USER(pTarget)->m_bNPGainAmount = (uint8_t) pType->sExpPct;
		break;

	case BUFF_TYPE_NOAH_BONUS:
		if(pTarget->isPlayer())
			TO_USER(pTarget)->m_bNoahGainAmount = (uint8_t) pType->sExpPct;
		break;

	case BUFF_TYPE_PREMIUM_MERCHANT:
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_bPremiumMerchant = true;
		break;

	case BUFF_TYPE_ATTACK_SPEED_ARMOR:
		// NOTE: This officially uses the Attack field (usually used for AP), but the skill is designed to adjust attack speed.
		pTarget->m_sACAmount += pType->sAC;
		pTarget->m_sAttackSpeedAmount += (pType->bAttack - 100); 
		break;

	case BUFF_TYPE_DAMAGE_DOUBLE:
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_bPlayerAttackAmount = pType->bAttack;
		break;

	case BUFF_TYPE_DISABLE_TARGETING:
		pTarget->m_bIsBlinded = true;
		break;

	case BUFF_TYPE_BLIND:
		// The only skill that uses this buff type is "Blinding Strafe", which states:
		// Description: Shoots an arrow that inflicts 400% damage and blinds the enemy. Does not apply to monsters.
		// As such, we should not blind monsters.
		if (pTarget->isPlayer())
			pTarget->m_bIsBlinded = true;
		break;

	case BUFF_TYPE_FREEZE:
		// Proportional to the target user's current HP.
		pTarget->m_bSpeedAmount = pType->bSpeed;
		break;

	case BUFF_TYPE_INSTANT_MAGIC:
		pTarget->m_bInstantCast = true;
		break;

	case BUFF_TYPE_DECREASE_RESIST:
		pTarget->m_bPctFireR		= (100 - pType->bFireR);
		pTarget->m_bPctColdR		= (100 - pType->bColdR);
		pTarget->m_bPctLightningR	= (100 - pType->bLightningR);
		pTarget->m_bPctMagicR		= (100 - pType->bMagicR);
		pTarget->m_bPctDiseaseR		= (100 - pType->bDiseaseR);
		pTarget->m_bPctPoisonR		= (100 - pType->bPoisonR);
		break;

	case BUFF_TYPE_MAGE_ARMOR:
		pTarget->m_bReflectArmorType = (pSkill->sSkill % 100);
		break;

	case BUFF_TYPE_PROHIBIT_INVIS:
		pTarget->m_bCanStealth = true;
		break;

	case BUFF_TYPE_RESIS_AND_MAGIC_DMG: // Elysian Web
		pTarget->m_bMagicDamageReduction = (uint8_t) pType->sExpPct;
		break;

	case BUFF_TYPE_TRIPLEAC_HALFSPEED:	// Wall of Iron
		pTarget->m_sACPercent += 300; // 300%, or 3x
		pTarget->m_bSpeedAmount = pTarget->m_bSpeedAmount / 2;
		if (pTarget->m_bSpeedAmount == 0)
			pTarget->m_bSpeedAmount = 1;
		break;

	case BUFF_TYPE_BLOCK_CURSE:			// Counter Curse
		pTarget->m_bBlockCurses = true;
		break;

	case BUFF_TYPE_BLOCK_CURSE_REFLECT:	// Curse Refraction
		pTarget->m_bReflectCurses = true;
		break;

	case BUFF_TYPE_MANA_ABSORB:		// Outrage / Frenzy / Mana Shield
		pTarget->m_bManaAbsorb = (uint8_t) pType->sExpPct;
		break;

	case BUFF_TYPE_VARIOUS_EFFECTS: //... whatever the event item grants.
		// what is tweaked in the database: AC, Attack, MaxHP, resistances
		// AC
		if (pType->sAC == 0 && pType->sACPct > 100)
			pTarget->m_sACPercent += (pType->sACPct - 100);
		else if (pType->sAC > 0 && pType->sACPct == 100)
			pTarget->m_sACAmount += pType->sAC;

		// Attack
		if (pType->bAttack > 100)
			pTarget->m_bAttackAmount += (pType->bAttack - 100);
		/*
		// NP Bonus
		if (pTarget->isPlayer() && pType->sSpecialAmount > 0)
			TO_USER(pTarget)->m_bSkillNPBonus += pType->sSpecialAmount;
		*/
		break;

	case BUFF_TYPE_IGNORE_WEAPON:		// Weapon cancellation
		// Disarms the opponent. (rendering them unable to attack)
#if defined(GAMESERVER)
		if (pTarget->isPlayer())
		{
			CUser * pTUser = TO_USER(pTarget);

			pTUser->m_bWeaponsDisabled = true;
			pTUser->UserLookChange(RIGHTHAND, 0, 0);

			_ITEM_TABLE * pLeftHand = pTUser->GetItemPrototype(LEFTHAND);
			if (pLeftHand != nullptr && !pLeftHand->isShield())
				pTUser->UserLookChange(LEFTHAND, 0, 0);
		}
#endif
		break;

	case BUFF_TYPE_PASSION_OF_SOUL:		// Passion of the Soul
		// Increase pet's HP by 120
		break;

	case BUFF_TYPE_FIRM_DETERMINATION:	// Firm Determination
		// Increase pet's AC by 20
		break;

	case BUFF_TYPE_SPEED2:				// Cold Wave
		pTarget->m_bSpeedAmount = (pTarget->m_bSpeedAmount / 100 * pType->bSpeed);
		break;

	case BUFF_TYPE_UNK_EXPERIENCE:		// unknown buff type, used for something relating to XP.
		break;

	case BUFF_TYPE_ATTACK_RANGE_ARMOR:	// Inevitable Murderous
		pTarget->m_sACAmount += 100;
		pTarget->m_bRadiusAmount = pType->bRadius;
		break;

	case BUFF_TYPE_MIRROR_DAMAGE_PARTY: // Minak's Thorn
		/*
		pTarget->m_bMirrorDamage = true;
		pTarget->m_byMirrorAmount = (uint8_t) pType->sSpecialAmount;
		*/
		break;

	case BUFF_TYPE_DAGGER_BOW_DEFENSE: // Eskrima
		// Inflicts attacks as well as a bleeding curse on the enemy. Decreases 10% Dagger and Bow Defense of the enemy under the bleeding curse buff.
		// NOTE: overwrite the percentage for now (nothing else uses it)
		// Also: the amount is 20 in the database. Could be that it's divided by 2 (i.e. splitting it between dagger/bow), the skill description's inaccurate
		// or the description roughly reflects the final damage after player damage reduction. For now, we'll just assume it's the latter.
		/*
		pTarget->m_byDaggerRAmount = pTarget->m_byBowRAmount = 100 - (uint8_t) pType->sSpecialAmount;
		*/
		break;

	case BUFF_TYPE_STUN:
		pTarget->m_bSpeedAmount = pType->bSpeed; 
		break;

	case BUFF_TYPE_LOYALTY_AMOUNT:		// Santa's Present (gives an extra +2NP per kill, unlike BUFF_TYPE_LOYALTY which uses an percent).
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_bSkillNPBonus += 2;
		break;

	case BUFF_TYPE_NO_RECALL:			// prevents teleportation.
		pTarget->m_bCanTeleport = false;
		break;

	case BUFF_TYPE_REDUCE_TARGET:		// "Reduction" (reduces target's stats, but enlarges their character to make them easier to attack)
		// NOTE: Skill description says "Enlarge enemy, but decrease attack and defense rate by 5%"
		// There's nothing else set in the client to give those stats, and AC's reduced by 15% according to the data...
		// Just working with the TBL data for now (i.e. just the 15% AC reduction).
		if (pTarget->isPlayer())
		{
			pTarget->StateChangeServerDirect(3, ABNORMAL_GIANT_TARGET);
			pTarget->m_sACPercent += (pType->sACPct - 100);
		}
		break;

	case BUFF_TYPE_SILENCE_TARGET:		// Silences the target to prevent them from using any skills (or potions)
		pTarget->m_bCanUseSkills = false;
		break;

	case BUFF_TYPE_NO_POTIONS:			// "No Potion" prevents target from using potions.
		pTarget->m_bCanUsePotions = false;
		break;

	case BUFF_TYPE_KAUL_TRANSFORMATION:	// Transforms the target into a Kaul (a pig thing), preventing you from /town'ing or attacking, but increases defense.
		if (pTarget->isPlayer())
		{
			pTarget->m_bIsKaul = true;
			pTarget->m_sACAmount += 500;
			pTarget->StateChangeServerDirect(3, pType->iNum);
		}
		break;

	case BUFF_TYPE_UNDEAD:				// User becomes undead, increasing defense but preventing the use of potions and converting all health received into damage.
		pTarget->m_bIsUndead = true;
		pTarget->m_sACPercent += (pType->sACPct - 100);
		break;

	case BUFF_TYPE_UNSIGHT:				// Blocks the caster's sight (not the target's).
		pTarget->m_bIsBlinded = true;
		break;

	case BUFF_TYPE_BLOCK_PHYSICAL_DAMAGE: // Blocks all physical damage.
		pTarget->m_bBlockPhysical = true;
		break;

	case BUFF_TYPE_BLOCK_MAGICAL_DAMAGE: // Blocks all magical/skill damage.
		pTarget->m_bBlockMagic = true;
		break;

	case BUFF_TYPE_UNK_POTION:			// unknown potion, "Return of the Warrior", "Comeback potion", perhaps some sort of revive?
		break;

	case BUFF_TYPE_SLEEP:				// Zantman(Sandman), puts enemies to sleep.
		break;

	case BUFF_TYPE_INVISIBILITY_POTION:	// "Unidentified potion"
		break;

	case BUFF_TYPE_GODS_BLESSING:		// Increases your defense/max HP 
		break;

	case BUFF_TYPE_HELP_COMPENSATION:	// Compensation for using the help system (to help, ask for help, both?)
		break;

	default:
		return false;
	}

#if defined(GAMESERVER)		// update the target data in the AI server.
	UpdateAIServer(pSkill->iNum, AISkillOpcodeBuff, pCaster, pTarget, bIsRecastingSavedMagic);
#elif defined(AI_SERVER)	// on the AI server's side, add the buff to the target's buff map.
	_BUFF_TYPE4_INFO buffInfo;
	buffInfo.m_nSkillID = pSkill->iNum;
	pTarget->AddType4Buff(pType->bBuffType, buffInfo);
#endif

	return true;
}

bool CMagicProcess::RemoveType4Buff(uint8_t byBuffType, Unit *pTarget, bool bRemoveSavedMagic /*= true*/, bool bRecastSavedMagic /*= false*/)
{
	// Buff must be added at this point. If it doesn't exist, we can't remove it twice.
	Guard lock(pTarget->m_buffLock);
	auto itr = pTarget->m_buffMap.find(byBuffType);
	if (itr == pTarget->m_buffMap.end())
		return false;

	_MAGIC_TABLE * pSkill = g_pMain->m_MagictableArray.GetData(itr->second.m_nSkillID);
	if (pSkill == nullptr)
		return false;

	_MAGIC_TYPE4 * pType = g_pMain->m_Magictype4Array.GetData(pSkill->iNum);
	if (pType == nullptr)
		return false;

	// If this buff persists across logout, it should be removed here too.
	if (bRemoveSavedMagic
		&& pTarget->isPlayer()
		&& pTarget->HasSavedMagic(pSkill->iNum))
		TO_USER(pTarget)->RemoveSavedMagic(pSkill->iNum);

	if (itr->second.isBuff())
		pTarget->m_buffCount--;

	pTarget->m_buffMap.erase(itr);

	switch (byBuffType)
	{
	case BUFF_TYPE_HP_MP:
		pTarget->m_sMaxHPAmount = 0;
		pTarget->m_sMaxMPAmount = 0;
		break;

	case BUFF_TYPE_AC:
		if (pType->sAC == 0 && pType->sACPct > 0)
			pTarget->m_sACPercent -= (pType->sACPct - 100);
		else
			pTarget->m_sACAmount -= pType->sAC;
		break;

	case BUFF_TYPE_SIZE:
		pTarget->StateChangeServerDirect(3, ABNORMAL_NORMAL);
		break;

	case BUFF_TYPE_DAMAGE:
		if (pType->bAttack > 100)
			pTarget->m_bAttackAmount -= (pType->bAttack - 100);
		else
			pTarget->m_bAttackAmount -= pType->bAttack;
		break;

	case BUFF_TYPE_ATTACK_SPEED:
		pTarget->m_sAttackSpeedAmount -= (pType->bAttackSpeed - 100);
		break;

	case BUFF_TYPE_SPEED:
		pTarget->m_bSpeedAmount = 100;
		break;

	case BUFF_TYPE_STATS:
		if (pTarget->isPlayer())
		{
			TO_USER(pTarget)->SetStatBuff(STAT_STR, 0);
			TO_USER(pTarget)->SetStatBuff(STAT_STA, 0);
			TO_USER(pTarget)->SetStatBuff(STAT_DEX, 0);
			TO_USER(pTarget)->SetStatBuff(STAT_INT, 0);
			TO_USER(pTarget)->SetStatBuff(STAT_CHA, 0);	
		}
		break;

	case BUFF_TYPE_RESISTANCES:
		pTarget->m_bAddFireR		= 0;
		pTarget->m_bAddColdR		= 0;
		pTarget->m_bAddLightningR	= 0;
		pTarget->m_bAddMagicR		= 0;
		pTarget->m_bAddDiseaseR		= 0;
		pTarget->m_bAddPoisonR		= 0;
		break;

	case BUFF_TYPE_ACCURACY:
		pTarget->m_bHitRateAmount = 100;
		pTarget->m_sAvoidRateAmount = 100;
		break;	

	case BUFF_TYPE_MAGIC_POWER:
		pTarget->m_sMagicAttackAmount = 0;
		break;

	case BUFF_TYPE_EXPERIENCE:
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_sExpGainAmount = 100;
		break;

	case BUFF_TYPE_WEIGHT:
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_bMaxWeightAmount = 100;
		break;

	case BUFF_TYPE_WEAPON_DAMAGE:
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_bAddWeaponDamage = 0;
		break;

	case BUFF_TYPE_WEAPON_AC:
		if (pType->sAC == 0 && pType->sACPct > 0)
			TO_USER(pTarget)->m_bPctArmourAc -= (pType->sACPct - 100);
		else
			TO_USER(pTarget)->m_sAddArmourAc -= pType->sAC;
		break;

	case BUFF_TYPE_LOYALTY:
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_bNPGainAmount = 100;
		break;

	case BUFF_TYPE_NOAH_BONUS:
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_bNoahGainAmount = 100;
		break;

	case BUFF_TYPE_PREMIUM_MERCHANT:
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_bPremiumMerchant = false;
		break;

	case BUFF_TYPE_ATTACK_SPEED_ARMOR:
		// NOTE: This officially uses the Attack field (usually used for AP), but the skill is designed to adjust attack speed.
		pTarget->m_sACAmount -= pType->sAC;
		pTarget->m_sAttackSpeedAmount -= (pType->bAttack - 100);
		break;

	case BUFF_TYPE_DAMAGE_DOUBLE:
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_bPlayerAttackAmount = 100;
		break;

	case BUFF_TYPE_DISABLE_TARGETING:
		pTarget->m_bIsBlinded = false;
		break;

	case BUFF_TYPE_BLIND:
		// Only players can be blinded (at least by the only skill - "Blinding Strafe" - that uses this type).
		if (pTarget->isPlayer())
		{
			pTarget->m_bIsBlinded = false;
			TO_USER(pTarget)->SendUserStatusUpdate(USER_STATUS_POISON, USER_STATUS_CURE);
		}
		break;

	case BUFF_TYPE_FREEZE:
		// Proportional to the target user's current HP.
		pTarget->m_bSpeedAmount = 100;
		break;

	case BUFF_TYPE_INSTANT_MAGIC:
		pTarget->m_bInstantCast = false;
		break;

	case BUFF_TYPE_DECREASE_RESIST:
		pTarget->m_bPctFireR		= 100;
		pTarget->m_bPctColdR		= 100;
		pTarget->m_bPctLightningR	= 100;
		pTarget->m_bPctMagicR		= 100;
		pTarget->m_bPctDiseaseR		= 100;
		pTarget->m_bPctPoisonR		= 100;
		break;

	case BUFF_TYPE_MAGE_ARMOR:
		pTarget->m_bReflectArmorType = 0;
		break;

	case BUFF_TYPE_PROHIBIT_INVIS:
		pTarget->m_bCanStealth = false;
		break;

	case BUFF_TYPE_RESIS_AND_MAGIC_DMG: // Elysian Web
		pTarget->m_bMagicDamageReduction = 100;
		if(pTarget->isPlayer())
			TO_USER(pTarget)->SendUserStatusUpdate(USER_STATUS_POISON,USER_STATUS_CURE);
		break;

	case BUFF_TYPE_TRIPLEAC_HALFSPEED:	// Wall of Iron
		pTarget->m_sACPercent -= 300; // 300%, or 3x
		pTarget->m_bSpeedAmount = 100;
		break;

	case BUFF_TYPE_BLOCK_CURSE:			// Counter Curse
		pTarget->m_bBlockCurses = false;
		break;

	case BUFF_TYPE_BLOCK_CURSE_REFLECT:	// Curse Refraction
		pTarget->m_bReflectCurses = false;
		break;

	case BUFF_TYPE_MANA_ABSORB:		// Outrage / Frenzy / Mana Shield
		pTarget->m_bManaAbsorb = 0;
		break;

	case BUFF_TYPE_IGNORE_WEAPON:		// Weapon cancellation
#if defined(GAMESERVER)
		if (pTarget->isPlayer())
		{
			CUser * pTUser = TO_USER(pTarget);
			_ITEM_DATA * pLeftItem, * pRightItem;

			_ITEM_TABLE * pLeftHand  = pTUser->GetItemPrototype(LEFTHAND, pLeftItem),
				* pRightHand = pTUser->GetItemPrototype(RIGHTHAND, pRightItem);

			pTUser->m_bWeaponsDisabled = false;

			if (pLeftHand != nullptr)
				pTUser->UserLookChange(LEFTHAND, pLeftItem->nNum, pLeftItem->sDuration);

			if (pRightHand != nullptr)
				pTUser->UserLookChange(RIGHTHAND, pRightItem->nNum, pRightItem->sDuration);
		}
#endif
		break;

	case BUFF_TYPE_VARIOUS_EFFECTS: //... whatever the event item grants.
		// what is tweaked in the database: AC, Attack, MaxHP, resistances
		// AC
		if (pType->sAC == 0 && pType->sACPct > 100)
			pTarget->m_sACPercent -= (pType->sACPct - 100);
		else if (pType->sAC > 0 && pType->sACPct == 100)
			pTarget->m_sACAmount -= pType->sAC;

		// Attack
		if (pType->bAttack > 100)
			pTarget->m_bAttackAmount -= (pType->bAttack - 100);
		/*
		// NP Bonus
		if (pTarget->isPlayer() && pType->sSpecialAmount > 0)
			TO_USER(pTarget)->m_bSkillNPBonus -= pType->sSpecialAmount;
		*/
		break;

	case BUFF_TYPE_PASSION_OF_SOUL:		// Passion of the Soul
		// Increase pet's HP by 120
		break;

	case BUFF_TYPE_FIRM_DETERMINATION:	// Firm Determination
		// Increase pet's AC by 20
		break;

	case BUFF_TYPE_SPEED2:				// Cold Wave
		pTarget->m_bSpeedAmount = 100;
		break;

	case BUFF_TYPE_UNK_EXPERIENCE:		// unknown buff type, used for something relating to XP.
		break;

	case BUFF_TYPE_ATTACK_RANGE_ARMOR:	// Inevitable Murderous
		pTarget->m_sACAmount -= 100;
		pTarget->m_bRadiusAmount = 0;
		break;

	case BUFF_TYPE_MIRROR_DAMAGE_PARTY: // Minak's Thorn
		pTarget->m_bMirrorDamage = false;
		pTarget->m_byMirrorAmount = 0;
		break;

	case BUFF_TYPE_DAGGER_BOW_DEFENSE: // Eskrima
		// Inflicts attacks as well as a bleeding curse on the enemy. Decreases 10% Dagger and Bow Defense of the enemy under the bleeding curse buff.
		pTarget->m_byDaggerRAmount = pTarget->m_byBowRAmount = 100; // note: overwrite the percentage for now (nothing else uses it)
		break;

	case BUFF_TYPE_STUN : // Lighting Skill
		pTarget->m_bSpeedAmount = 100;
		break; 

	case BUFF_TYPE_LOYALTY_AMOUNT:		// Santa's Present (gives an extra +2NP per kill, unlike BUFF_TYPE_LOYALTY which uses an percent).
		if (pTarget->isPlayer())
			TO_USER(pTarget)->m_bSkillNPBonus -= 2;
		break;

	case BUFF_TYPE_NO_RECALL:			// prevents teleportation.
		pTarget->m_bCanTeleport = true;
		break;

	case BUFF_TYPE_REDUCE_TARGET:		// "Reduction" (reduces target's stats, but enlarges their character to make them easier to attack)
		// NOTE: Skill description says "Enlarge enemy, but decrease attack and defense rate by 5%"
		// There's nothing else set in the client to give those stats, and AC's reduced by 15% according to the data...
		// Just working with the TBL data for now (i.e. just the 15% AC reduction).
		if (pTarget->isPlayer())
		{
			pTarget->StateChangeServerDirect(3, ABNORMAL_NORMAL);
			pTarget->m_sACPercent -= (pType->sACPct - 100);
		}
		break;

	case BUFF_TYPE_SILENCE_TARGET:		// Silences the target to prevent them from using any skills (or potions)
		pTarget->m_bCanUseSkills = true;
		break;

	case BUFF_TYPE_NO_POTIONS:			// "No Potion" prevents target from using potions.
		pTarget->m_bCanUsePotions = true;
		break;

	case BUFF_TYPE_KAUL_TRANSFORMATION:	// Transforms the target into a Kaul (a pig thing), preventing you from /town'ing or attacking, but increases defense.
		if (pTarget->isPlayer())
		{
			pTarget->m_bIsKaul = false;
			pTarget->m_sACAmount -= 500;
			pTarget->StateChangeServerDirect(3, TO_USER(pTarget)->m_nOldAbnormalType);
		}
		break;

	case BUFF_TYPE_UNDEAD:				// User becomes undead, increasing defense but preventing the use of potions and converting all health received into damage.
		pTarget->m_bIsUndead = false;
		pTarget->m_sACPercent -= (pType->sACPct - 100);
		break;

	case BUFF_TYPE_UNSIGHT:				// Blocks the caster's sight (not the target's).
		pTarget->m_bIsBlinded = false;
		break;

	case BUFF_TYPE_BLOCK_PHYSICAL_DAMAGE: // Blocks all physical damage.
		pTarget->m_bBlockPhysical = false;
		break;

	case BUFF_TYPE_BLOCK_MAGICAL_DAMAGE: // Blocks all magical/skill damage.
		pTarget->m_bBlockMagic = false;
		break;

	case BUFF_TYPE_UNK_POTION:			// unknown potion, "Return of the Warrior", "Comeback potion", perhaps some sort of revive?
		break;

	case BUFF_TYPE_SLEEP:				// Zantman(Sandman), puts enemies to sleep.
		break;

	case BUFF_TYPE_INVISIBILITY_POTION:	// "Unidentified potion"
		break;

	case BUFF_TYPE_GODS_BLESSING:		// Increases your defense/max HP 
		break;

	case BUFF_TYPE_HELP_COMPENSATION:	// Compensation for using the help system (to help, ask for help, both?)
		break;

	default:
		return false;
	}

	if (pTarget->isPlayer())
	{
		if (pSkill->bMoral >= MORAL_ENEMY)
		{
			if (byBuffType == BUFF_TYPE_SPEED || byBuffType == BUFF_TYPE_SPEED2)
				TO_USER(pTarget)->SendUserStatusUpdate(USER_STATUS_SPEED, USER_STATUS_CURE);
		}

		TO_USER(pTarget)->SetUserAbility();

		if (bRemoveSavedMagic && pType->bBuffType != BUFF_TYPE_MAGE_ARMOR)
		{
			Packet result(WIZ_MAGIC_PROCESS);
			result << uint8_t(MAGIC_DURATION_EXPIRED) << byBuffType;
			TO_USER(pTarget)->Send(&result);
		}
	}

#if defined(GAMESERVER) // update the target data in the AI server.

	if (bRecastSavedMagic && TO_USER(pTarget)->isLockableScroll(pType->bBuffType))
	{
		TO_USER(pTarget)->SendUserStatusUpdate(USER_STATUS_POISON, USER_STATUS_CURE);
		TO_USER(pTarget)->RecastLockableScrolls(pType->bBuffType);
	}

	UpdateAIServer(pType->iNum, AISkillOpcodeRemoveBuff, pTarget);
#endif

	return true;
}

/**
* @brief	Test if the specified skill is a buff
* 			or a debuff.
*
* @param	pSkill	The specified skill.
*
* @return	true if skill is a buff, false if debuff.
*/
bool CMagicProcess::IsBuff(_MAGIC_TYPE4 * pType)
{
	switch (pType->bBuffType)
	{
	case BUFF_TYPE_NONE: // used by things like firecrackers.
		return true;

	case BUFF_TYPE_HP_MP:
		if (pType->sMaxHP > 0
			|| pType->sMaxMP > 0)
			return true;

		// If either the max HP percent or max MP percent are less than 100%
		// it is a debuff we are dealing with, not a buff.
		return (pType->sMaxHPPct >= 100 && pType->sMaxMPPct >= 100);

	case BUFF_TYPE_AC:
		if (pType->sAC == 0 && pType->sACPct > 0)
			return pType->sACPct >= 100;
		else
			return pType->sAC >= 0;

		// Size changes (via Bezoars, Rice cakes, etc)
		// are buffs.
	case BUFF_TYPE_SIZE:
		return true;

	case BUFF_TYPE_DAMAGE:
		return pType->bAttack >= 100;

	case BUFF_TYPE_ATTACK_SPEED:
		return pType->bAttackSpeed >= 100;

	case BUFF_TYPE_SPEED:
		return pType->bSpeed >= 100;

		// If any of the stats are below 0, it's a debuff.
	case BUFF_TYPE_STATS:
		return !(pType->bStr < 0 || pType->bSta < 0 || pType->bDex < 0 || pType->bIntel < 0 || pType->bCha < 0); 

		// There are no skills that negatively affect resistances, so it will always be a buff.
	case BUFF_TYPE_RESISTANCES:
		return true;

	case BUFF_TYPE_ACCURACY:
		return (pType->bHitRate >= 100 && pType->sAvoidRate >= 100);

	case BUFF_TYPE_MAGIC_POWER:
		return pType->bMagicAttack >= 100;

	case BUFF_TYPE_EXPERIENCE:
	case BUFF_TYPE_WEIGHT:
		return true;

	case BUFF_TYPE_WEAPON_DAMAGE:
		return pType->bAttack > 0;

	case BUFF_TYPE_WEAPON_AC:
		if (pType->sAC == 0 && pType->sACPct > 0)
			return pType->sACPct >= 100;
		else
			return pType->sAC > 0;

	case BUFF_TYPE_LOYALTY:
	case BUFF_TYPE_NOAH_BONUS:
	case BUFF_TYPE_PREMIUM_MERCHANT:
	case BUFF_TYPE_ATTACK_SPEED_ARMOR: // only used by "Berserk Echo", which is a buff.
	case BUFF_TYPE_DAMAGE_DOUBLE:
		return true;

	case BUFF_TYPE_DISABLE_TARGETING:
	case BUFF_TYPE_BLIND:
	case BUFF_TYPE_FREEZE:
		return false;

	case BUFF_TYPE_INSTANT_MAGIC:
		return true;

	case BUFF_TYPE_DECREASE_RESIST:
		return false;

	case BUFF_TYPE_MAGE_ARMOR:
	case BUFF_TYPE_PROHIBIT_INVIS:
	case BUFF_TYPE_RESIS_AND_MAGIC_DMG: // Elysian Web
	case BUFF_TYPE_TRIPLEAC_HALFSPEED:	// Wall of Iron
	case BUFF_TYPE_BLOCK_CURSE:			// Counter Curse
	case BUFF_TYPE_BLOCK_CURSE_REFLECT:	// Curse Refraction
	case BUFF_TYPE_MANA_ABSORB:		// Outrage / Frenzy / Mana Shield
		return true;

	case BUFF_TYPE_IGNORE_WEAPON:		// Weapon cancellation
		// Disarms the opponent. (rendering them unable to attack)
		return false;

	case BUFF_TYPE_VARIOUS_EFFECTS: //... whatever the event item grants.
		return true; // everything seems positive, however it might be prudent to have this check.

	case BUFF_TYPE_PASSION_OF_SOUL:		// Passion of the Soul
	case BUFF_TYPE_FIRM_DETERMINATION:	// Firm Determination
		return true;

	case BUFF_TYPE_SPEED2:				// Cold Wave
		// skill explicitly slows
		return false;

	case BUFF_TYPE_UNK_EXPERIENCE:		//unknown buff type, used for something relating to XP.
		return true;

	case BUFF_TYPE_ATTACK_RANGE_ARMOR:	// Inevitable Murderous
	case BUFF_TYPE_MIRROR_DAMAGE_PARTY:	// Minak's Thorn
		return true;

	case BUFF_TYPE_DAGGER_BOW_DEFENSE:	// Eskrima
		return false;

	case BUFF_TYPE_STUN:	// Stun
		return false;

	case BUFF_TYPE_LOYALTY_AMOUNT:		// Santa's Present (gives an extra +2NP per kill).
		return true;

	case BUFF_TYPE_NO_RECALL:			// prevents teleportation.
	case BUFF_TYPE_REDUCE_TARGET:		// "Reduction" (reduces target's stats, but enlarges their character to make them easier to attack)
	case BUFF_TYPE_SILENCE_TARGET:		// Silences the target to prevent them from using any skills (or potions)
	case BUFF_TYPE_NO_POTIONS:			// "No Potion" prevents target from using potions.
	case BUFF_TYPE_KAUL_TRANSFORMATION:	// Transforms the target into a Kaul (a pig thing), preventing you from /town'ing or attacking, but increases defense.
		return false;

	case BUFF_TYPE_UNDEAD:				// User becomes undead, increasing defense but preventing the use of potions and converting all health received into damage.
	case BUFF_TYPE_UNSIGHT:				// Blocks the caster's sight (not the target's).
		return false;

	case BUFF_TYPE_BLOCK_PHYSICAL_DAMAGE: // Blocks all physical damage.
	case BUFF_TYPE_BLOCK_MAGICAL_DAMAGE:  // Blocks all magical/skill damage.
	case BUFF_TYPE_UNK_POTION:			  // unknown potion, "Return of the Warrior", "Comeback potion", perhaps some sort of revive?
		return true;

	case BUFF_TYPE_SLEEP:				// Zantman(Sandman), puts enemies to sleep.
		return false;

	case BUFF_TYPE_INVISIBILITY_POTION:	// "Unidentified potion", it debuffs, but hides in the interest of the user. Needs to be a buff.
	case BUFF_TYPE_GODS_BLESSING:		// Increases your defense/max HP 
	case BUFF_TYPE_HELP_COMPENSATION:	// Compensation for using the help system (to help, ask for help, both?)
		return true;

		// TODO: Identify and name these.
	case 48: // DC/War/Exp Flash - grants additional NP/XP
		return true;

	case 166: // unknown, assume debuff for now.
		return false;
	}

	printf("WARNING: Unhandled buff type (%d) for skill %d, assuming it's a debuff.\n", pType->bBuffType, pType->iNum);
	return false;
}
