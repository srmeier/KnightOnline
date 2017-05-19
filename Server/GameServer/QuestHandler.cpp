#include "stdafx.h"
#include "KnightsManager.h"

void CUser::QuestDataRequest()
{
	/*
	// Sending this now is probably wrong, but it's cleaner than it was before.
	Packet result(WIZ_QUEST);
	result << uint8_t(1) << uint16_t(m_questMap.size());
	foreach (itr, m_questMap)
		result	<< itr->first << itr->second;
	Send(&result);
	*/

	// TODO: come back and fix this!!
	Packet result(WIZ_QUEST);
	result << 0x02 << 0x00 << 0x00 << 0x00;
	//Send(&result);
}

void CUser::QuestV2PacketProcess(Packet & pkt)
{
	//if (m_sEventNid < 0)
	//	return;

	uint8_t opcode = pkt.read<uint8_t>();
	uint32_t nQuestID = pkt.read<uint32_t>();

	CNpc *pNpc = g_pMain->GetNpcPtr(m_sEventNid);
	_QUEST_HELPER * pQuestHelper = g_pMain->m_QuestHelperArray.GetData(nQuestID);
	// Does this quest helper exist?
	if (pQuestHelper == nullptr)
		return;
	//	// Does the quest NPC exist, and is it alive? 
	//		|| pNpc == nullptr || pNpc->isDead()
	//		// Are we even talking to this NPC?
	//		|| pQuestHelper->sNpcId != pNpc->GetProtoID()
	//		// Are we in range of this NPC?
	//		|| !isInRange(pNpc, MAX_NPC_RANGE)
	//		// Is this quest for this player's nation? NOTE: 3 indicates both (why not 0, I don't know)
	//		|| (pQuestHelper->bNation != 3 && pQuestHelper->bNation != GetNation())
	//		// Is the player's level high enough to do this quest?
	//		|| (pQuestHelper->bLevel > GetLevel())
	//		// Are we the correct class? NOTE: 5 indicates any class.
	//		|| (pQuestHelper->bClass != 5 && !JobGroupCheck(pQuestHelper->bClass))
	//		// Are we in the correct zone? NOTE: This isn't checked officially, may be for good reason.
	//		|| GetZoneID() != pQuestHelper->bZone)
	//		return;

	// If we're the same min. level as the quest requires, 
	// do we have the min. required XP? Seems kind of silly, but OK..
	if (pQuestHelper->bLevel == GetLevel()
		&& pQuestHelper->nExp > m_iExp)
		return;

	switch (opcode)
	{
	case 3:
	case 7:
		QuestV2ExecuteHelper(pQuestHelper);
		QuestV2MonsterDataRequest();
		break;

	case 4:
		QuestV2CheckFulfill(pQuestHelper);
		break;

	case 5:
		if (!CheckExistEvent(pQuestHelper->sEventDataIndex, 2))
		SaveEvent(pQuestHelper->sEventDataIndex, 4);

		if (m_sEventDataIndex > 0 
			&& m_sEventDataIndex == pQuestHelper->sEventDataIndex)
		{
			QuestV2MonsterDataDeleteAll();
			QuestV2MonsterDataRequest();
		}

		// Kick the user out of the quest zone.
		// Monster suppression squad is the only zone I'm aware of that this should apply to.
		if (GetZoneID() >= 81 && GetZoneID() <= 83)
			KickOutZoneUser(true);
		break;

	case 6:
		if (!CheckExistEvent(pQuestHelper->sEventDataIndex, 2))
			SaveEvent(pQuestHelper->sEventDataIndex, 1);
		break;

	case 12:
		if (!CheckExistEvent(pQuestHelper->sEventDataIndex, 3))
			SaveEvent(pQuestHelper->sEventDataIndex, 1);
		break;
	}
}

void CUser::SaveEvent(uint16_t sQuestID, uint8_t bQuestState)
{
	_QUEST_MONSTER * pQuestMonster = g_pMain->m_QuestMonsterArray.GetData(sQuestID);

	if (pQuestMonster != nullptr
		&& bQuestState == 1
		&& m_sEventDataIndex > 0)
		return;

	m_questMap[sQuestID] = bQuestState;

	// Don't need to handle special/kill quests any further
	if (sQuestID >= QUEST_KILL_GROUP1)
		return;

	Packet result(WIZ_QUEST);
	result << uint8_t(2) << sQuestID << bQuestState;
	Send(&result);

	if (m_sEventDataIndex == sQuestID 
		&& bQuestState == 2)
	{
		QuestV2MonsterDataDeleteAll();
		QuestV2MonsterDataRequest();
	}

	if (bQuestState == 1
		&& pQuestMonster != nullptr)
	{
		// TODO: Decipher this into more meaningful code. :p
		int16_t v11 = ((int16_t)((uint32_t)(6711 * sQuestID) >> 16) >> 10) - (sQuestID >> 15);
		int16_t v12 = ((int16_t)((uint32_t)(5243 * (int16_t)(sQuestID - 10000 * v11)) >> 16) >> 3) - ((int16_t)(sQuestID - 10000 * v11) >> 15);

		SaveEvent(32005, (uint8_t)v11);
		SaveEvent(32006, (uint8_t)v12);
		SaveEvent(32007, sQuestID - 100 * v12);
		m_sEventDataIndex = sQuestID;
		QuestV2MonsterDataRequest();
	}

}

void CUser::DeleteEvent(uint16_t sQuestID)
{
	m_questMap.erase(sQuestID);
}

bool CUser::CheckExistEvent(uint16_t sQuestID, uint8_t bQuestState)
{
	// Attempt to find a quest with that ID in the map
	QuestMap::iterator itr = m_questMap.find(sQuestID);

	// If it doesn't exist, it doesn't exist. 
	// Unless of course, we wanted it to not exist, in which case we're right!
	// (this does seem silly, but this behaviour is STILL expected, so do not remove it.)
	if (itr == m_questMap.end())
		return bQuestState == 0;

	return itr->second == bQuestState;
}

void CUser::QuestV2MonsterCountAdd(uint16_t sNpcID)
{
	if (m_sEventDataIndex == 0)
		return;

	// it looks like they use an active quest ID which is kind of dumb
	// we'd rather search through the player's active quests for applicable mob counts to increment
	// but then, this system can't really handle that (static counts). More research is necessary.
	uint16_t sQuestNum = m_sEventDataIndex; // placeholder so that we can implement logic mockup
	_QUEST_MONSTER *pQuestMonster = g_pMain->m_QuestMonsterArray.GetData(sQuestNum);
	if (pQuestMonster == nullptr)
		return;

	// TODO: Implement obscure zone ID logic
	for (int group = 0; group < QUEST_MOB_GROUPS; group++)
	{
		for (int i = 0; i < QUEST_MOBS_PER_GROUP; i++)
		{
			if (pQuestMonster->sNum[group][i] != sNpcID)
				continue;

			if (m_bKillCounts[group] + 1 > pQuestMonster->sCount[group])
				return;

			m_bKillCounts[group]++;
			SaveEvent(QUEST_KILL_GROUP1 + group, m_bKillCounts[group]);
			//1453
			Packet result(WIZ_QUEST);
			result << uint8_t(9) << uint8_t(2) << uint8_t(group + 1) << m_bKillCounts[group];
			//18xx
			//Packet result(WIZ_QUEST);
			//result << uint8_t(9) << uint8_t(2) << uint16_t(sQuestNum) << uint8_t(1) << uint16_t(m_bKillCounts[group]);
			Send(&result);
			return;
		}
	}
}

uint8_t CUser::QuestV2CheckMonsterCount(uint16_t sQuestID)
{
	// Attempt to find a quest with that ID in the map
	QuestMap::iterator itr = m_questMap.find(sQuestID);

	// If it doesn't exist, it doesn't exist. 
	if (itr == m_questMap.end())
		return 0;

	return itr->second;
}

void CUser::QuestV2MonsterDataDeleteAll()
{
	memset(&m_bKillCounts, 0, sizeof(m_bKillCounts));
	m_sEventDataIndex = 0;

	for (int i = QUEST_KILL_GROUP1; i <= 32007; i++)
		DeleteEvent(i);
}

void CUser::QuestV2MonsterDataRequest()
{
	// Still not sure, but it's generating an ID.
	m_sEventDataIndex = 
		10000	*	QuestV2CheckMonsterCount(32005) +
		100		*	QuestV2CheckMonsterCount(32006) +
		QuestV2CheckMonsterCount(32007);

	// Lookup the current kill counts for each mob group in the active quest
	m_bKillCounts[0] = QuestV2CheckMonsterCount(QUEST_KILL_GROUP1);
	m_bKillCounts[1] = QuestV2CheckMonsterCount(QUEST_KILL_GROUP2);
	m_bKillCounts[2] = QuestV2CheckMonsterCount(QUEST_KILL_GROUP3);
	m_bKillCounts[3] = QuestV2CheckMonsterCount(QUEST_KILL_GROUP4);

	Packet result(WIZ_QUEST);
	result << uint8_t(9)
		<< uint8_t(1)
		<< m_sEventDataIndex
		<< m_bKillCounts[0] << m_bKillCounts[1]
	<< m_bKillCounts[2] << m_bKillCounts[3];

	Send(&result);
}

void CUser::QuestV2ExecuteHelper(_QUEST_HELPER * pQuestHelper)
{
	if (pQuestHelper == nullptr
		/*|| !CheckExistEvent(pQuestHelper->sEventDataIndex, 2)*/ && pQuestHelper->bQuestType != 3)//görev verme sorunu
		return;

	QuestV2RunEvent(pQuestHelper, pQuestHelper->nEventTriggerIndex); // NOTE: Fulfill will use nEventCompleteIndex
}

void CUser::QuestV2CheckFulfill(_QUEST_HELPER * pQuestHelper)
{
	if (pQuestHelper == nullptr
		|| !CheckExistEvent(pQuestHelper->sEventDataIndex, 1))
		return;

	QuestV2RunEvent(pQuestHelper, pQuestHelper->nEventCompleteIndex);
}

bool CUser::QuestV2RunEvent(_QUEST_HELPER * pQuestHelper, uint32_t nEventID, int8_t bSelectedReward /*= -1*/)
{
	// Lookup the corresponding NPC.
	if (pQuestHelper->strLuaFilename == "01_main.lua")
	{
		m_sEventNid = 10000;
	}

	CNpc * pNpc = g_pMain->GetNpcPtr(m_sEventNid);
	bool result = false;

	// Make sure the NPC exists and is not dead (we should also check if it's in range)
	if (pNpc == nullptr
		|| pNpc->isDead())
		return result;

	// Increase the NPC's reference count to ensure it doesn't get freed while executing a script
	pNpc->IncRef();

	m_nQuestHelperID = pQuestHelper->nIndex;
	result = g_pMain->GetLuaEngine()->ExecuteScript(this, pNpc, nEventID, bSelectedReward, 
		pQuestHelper->strLuaFilename.c_str());

	// Decrease it now that we've finished with it + free if necessary
	pNpc->DecRef();

	return result;
}

/* 
These are called by quest scripts. 
*/

void CUser::QuestV2SaveEvent(uint16_t sEventDataIndex, int8_t bEventStatus)//uint16_t sQuestID)
{
	/*
	_QUEST_HELPER * pQuestHelper = g_pMain->m_QuestHelperArray.GetData(sQuestID);
	if (pQuestHelper == nullptr)
		return;

	SaveEvent(pQuestHelper->sEventDataIndex, pQuestHelper->bEventStatus);
	*/

	// NOTE(srmeier): this allows for easy Evt convertion to new system
	SaveEvent(sEventDataIndex, bEventStatus);
}

void CUser::QuestV2SendNpcMsg(uint32_t nQuestID, uint16_t sNpcID)
{
	Packet result(WIZ_QUEST);
	result << uint8_t(7) << nQuestID << sNpcID;
	Send(&result);
}

void CUser::QuestV2ShowGiveItem(uint32_t nUnk1, uint32_t sUnk1, 
								uint32_t nUnk2, uint32_t sUnk2,
								uint32_t nUnk3, uint32_t sUnk3,
								uint32_t nUnk4, uint32_t sUnk4)
{
	Packet result(WIZ_QUEST);
	result << uint8_t(10)
		<< nUnk1 << sUnk1
		<< nUnk2 << sUnk2
		<< nUnk3 << sUnk3
		<< nUnk4 << sUnk4;
	Send(&result);
}

uint16_t CUser::QuestV2SearchEligibleQuest(uint16_t sEventDataIndex)//, uint16_t sNpcID)
{	//The quest must be searched by just its sEventDataIndex
	Guard lock(g_pMain->m_questNpcLock);
	//QuestNpcList::iterator itr = g_pMain->m_QuestNpcList.find(sNpcID);
	QuestEventIndex::iterator itr = g_pMain->m_QuestEventIndex.find(sEventDataIndex);
	if (itr == g_pMain->m_QuestEventIndex.end()
		|| itr->second.empty())
		return 0;

	// Loop through all the QuestHelper instances attached to that NPC.
	_QUEST_HELPER * pHelper = NULL;
	foreach (itr2, itr->second)
	{
		pHelper = (*itr2);

		if (pHelper->sEventDataIndex != sEventDataIndex) continue;

		if (/*pHelper->bLevel > GetLevel()
			|| (pHelper->bLevel == GetLevel() && pHelper->nExp > m_iExp)
			||*/ (pHelper->bClass != 5 && !JobGroupCheck(pHelper->bClass))
			|| (pHelper->bNation != 3 && pHelper->bNation != GetNation())
			|| (pHelper->sEventDataIndex == 0)
			//|| (pHelper->bEventStatus < 0 || CheckExistEvent(pHelper->sEventDataIndex, 2))
			|| !CheckExistEvent(pHelper->sEventDataIndex, pHelper->bEventStatus))
			continue;

		// NOTE: testing the option to just return the status (makes EVT conversion easier)
		return pHelper->bEventStatus;//pHelper->nEventTriggerIndex;
	}

	return 0;
}

void CUser::QuestV2ShowMap(uint32_t nQuestHelperID)
{
	Packet result(WIZ_QUEST);
	result << uint8_t(11) << nQuestHelperID;
	Send(&result);
}

uint8_t CUser::CheckMonsterCount(uint8_t bGroup)
{
	_QUEST_MONSTER * pQuestMonster = g_pMain->m_QuestMonsterArray.GetData(m_sEventDataIndex);
	if (pQuestMonster == nullptr
		|| bGroup == 0
		|| bGroup >= QUEST_MOB_GROUPS)
		return 0;

	return m_bKillCounts[bGroup];
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
