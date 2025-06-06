﻿#include "stdafx.h"
#include "NpcMagicProcess.h"
#include "ServerDlg.h"
#include "Npc.h"

time_t CNpcMagicProcess::MagicPacket(uint8_t opcode, uint32_t nSkillID, int16_t sCasterID, int16_t sTargetID, int16_t sData1, int16_t sData2, int16_t sData3)
{
	_MAGIC_TABLE * pSkill = g_pMain->m_MagictableArray.GetData(nSkillID);
	if (pSkill == nullptr)
		return -1;

	Packet result(AG_MAGIC_ATTACK_REQ);
	result << opcode << nSkillID << sCasterID << sTargetID << sData1 << sData2 << sData3;
	g_pMain->Send(&result);

	// NOTE: Client controls skill hits.
	// Since we block these, we need to handle this ourselves.
	// This WILL need to be rewritten once spawns are though, as it is HORRIBLE.
	if (opcode != MAGIC_EFFECTING)
	{
		CNpc * pNpc = g_pMain->GetNpcPtr(sCasterID);
		if (pNpc == nullptr)
			return -1;

		pNpc->m_OldNpcState = pNpc->m_NpcState;
		pNpc->m_NpcState = NPC_CASTING;
		pNpc->m_nActiveSkillID = nSkillID;
		pNpc->m_sActiveTargetID = sTargetID;
		pNpc->m_sActiveCastTime = pSkill->bCastTime * MINUTE;

		return pNpc->m_sActiveCastTime;
	}

	return -1;
}
