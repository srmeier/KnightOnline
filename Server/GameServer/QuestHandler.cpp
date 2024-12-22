#include "stdafx.h"
#include "KnightsManager.h"

void CUser::QuestDataRequest(
	Packet& pkt)
{
	/*
	// Sending this now is probably wrong, but it's cleaner than it was before.
	Packet result(WIZ_QUEST);
	result << uint8_t(1) << uint16_t(m_questMap.size());
	foreach (itr, m_questMap)
		result	<< itr->first << itr->second;
	Send(&result);
	*/
}

void CUser::SendQuestStateUpdate(
	uint16_t sQuestID,
	uint8_t byQuestState,
	bool bIsUpdate /*= true*/)
{
	Packet result(WIZ_QUEST);
	result
		<< uint8_t(2) // update
		<< uint16_t(sQuestID)
		<< uint8_t(byQuestState)
		<< bool(bIsUpdate);
	Send(&result);	
}

void CUser::SaveEvent(
	uint16_t sQuestID,
	uint8_t byQuestState)
{
	m_questMap[sQuestID] = byQuestState;

	// Officially we only send quest updates for class mastery quests.
	// This is because it has to update class requirements appropriately.
	switch (sQuestID)
	{
		case QUEST_ID_MASTERY_WARRIOR:
		case QUEST_ID_MASTERY_ROGUE:
		case QUEST_ID_MASTERY_MAGE:
		case QUEST_ID_MASTERY_PRIEST:
			SendQuestStateUpdate(sQuestID, byQuestState);
			break;
	}
}

void CUser::DeleteEvent(uint16_t sQuestID)
{
	m_questMap.erase(sQuestID);
}

bool CUser::CheckExistEvent(uint16_t sQuestID, uint8_t byQuestState)
{
	// Attempt to find a quest with that ID in the map
	QuestMap::iterator itr = m_questMap.find(sQuestID);

	// If it doesn't exist, it doesn't exist. 
	// Unless of course, we wanted it to not exist, in which case we're right!
	// (this does seem silly, but this behaviour is STILL expected, so do not remove it.)
	if (itr == m_questMap.end())
		return byQuestState == 0;

	return itr->second == byQuestState;
}

// First job change; you're a [novice], Harry!
bool CUser::PromoteUserNovice()
{
	uint8_t bNewClasses[] = { ClassWarriorNovice, ClassRogueNovice, ClassMageNovice, ClassPriestNovice };
	uint8_t bOldClass = GetClassType() - 1; // convert base class 1,2,3,4 to 0,1,2,3 to align with bNewClasses

	// Make sure it's a beginner class.
	if (!isBeginner())
		return false;

	// Build the new class.
	uint16_t sNewClass = (GetNation() * 100) + bNewClasses[bOldClass];

	Packet result(WIZ_CLASS_CHANGE);
	result << uint8_t(6) << sNewClass << GetID();
	SendToRegion(&result);

	// Change the class & update party.
	result.Initialize(WIZ_CLASS_CHANGE);
	result << uint8_t(2) << sNewClass;
	result.rpos(1);
	ClassChange(result, false); // TODO: Clean this up. Shouldn't need to build a packet for this.

	// Update the clan.
	result.clear();
	result << uint16_t(0);
	CKnightsManager::CurrentKnightsMember(this, result); // TODO: Clean this up too.
	return true;
}

// From novice to master.
bool CUser::PromoteUser()
{
	/* unlike the official, the checks & item removal should be handled in the script, not here */
	uint8_t bOldClass = GetClassType();

	// We must be a novice before we can be promoted to master.
	if (!isNovice()) 
		return false;

	// Build the new class.
	uint16_t sNewClass = (GetNation() * 100) + bOldClass + 1;

	Packet result(WIZ_CLASS_CHANGE);
	result << uint8_t(6) << sNewClass << GetID();
	SendToRegion(&result);

	// Change the class & update party.
	result.Initialize(WIZ_CLASS_CHANGE);
	result << uint8_t(2) << sNewClass;
	result.rpos(1);
	ClassChange(result, false); // TODO: Clean this up. Shouldn't need to build a packet for this.

	// use integer division to get from 5/7/9/11 (novice classes) to 1/2/3/4 (base classes)
	uint8_t bBaseClass = (bOldClass / 2) - 1; 

	// this should probably be moved to the script
	SaveEvent(bBaseClass, 2); 

	// Update the clan.
	result.clear();
	result << uint16_t(0);
	CKnightsManager::CurrentKnightsMember(this, result); // TODO: Clean this up too.
	return true;
}

void CUser::PromoteClan(ClanTypeFlag byFlag)
{
	if (!isInClan())
		return;

	CKnightsManager::UpdateKnightsGrade(GetClanID(), byFlag);
}

void CUser::SendClanPointChange(int32_t nChangeAmount)
{
	if (!isInClan())
		return;

	CKnightsManager::UpdateClanPoint(GetClanID(), nChangeAmount);
}

uint8_t CUser::GetClanGrade()
{
	if (!isInClan())
		return 0;

	CKnights * pClan = g_pMain->GetClanPtr(GetClanID());
	if (pClan == nullptr)
		return 0;

	return pClan->m_byGrade;
}

uint32_t CUser::GetClanPoint()
{
	if (!isInClan())
		return 0;

	CKnights * pClan = g_pMain->GetClanPtr(GetClanID());
	if (pClan == nullptr)
		return 0;

	return pClan->m_nClanPointFund;
}

uint8_t CUser::GetClanRank()
{
	if (!isInClan())
		return ClanTypeNone;

	CKnights * pClan = g_pMain->GetClanPtr(GetClanID());
	if (pClan == nullptr)
		return ClanTypeNone;

	return pClan->m_byFlag;
}

uint8_t CUser::GetBeefRoastVictory() 
{
	if( g_pMain->m_sBifrostTime <= 90 * MINUTE && g_pMain->m_BifrostVictory != ALL )
		return g_pMain->m_sBifrostVictoryAll; 
		else
			return g_pMain->m_BifrostVictory; 
}

uint8_t CUser::GetWarVictory() { return g_pMain->m_bVictory; }

uint8_t CUser::CheckMiddleStatueCapture() { return g_pMain->m_bMiddleStatueNation == GetNation() ? 1 : 0; }

void CUser::MoveMiddleStatue() { Warp((GetNation() == KARUS ? DODO_CAMP_WARP_X : LAON_CAMP_WARP_X) + myrand(0, DODO_LAON_WARP_RADIUS),(GetNation() == KARUS ? DODO_CAMP_WARP_Z : LAON_CAMP_WARP_Z) + myrand(0, DODO_LAON_WARP_RADIUS)); }

uint8_t CUser::GetPVPMonumentNation() { return g_pMain->m_nPVPMonumentNation[GetZoneID()]; }
