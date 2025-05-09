// CUIStatSkillReset.cpp: implementation of the CUIStatSkillReset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "UIStatSkillReset.h"

#include "GameProcedure.h"
#include "GameProcMain.h"

#include "PlayerMySelf.h"
#include "PlayerOtherMgr.h"

#include "UIManager.h"

#include "APISocket.h"
#include "GameDef.h"
#include "PacketDef.h"

#include <cmath>

#include <afx.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUIStatSkillReset::CUIStatSkillReset()
{
	//get player info
	m_pInfo = &(CGameProcedure::s_pPlayer->m_InfoBase);
	m_pInfoExt = &(CGameProcedure::s_pPlayer->m_InfoExt);
}

CUIStatSkillReset::~CUIStatSkillReset()
{
	
}

void CUIStatSkillReset::Release()
{
	CN3UIBase::Release();
}

bool CUIStatSkillReset::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false)
		return false;
	
	m_pBtn_WalkAway = (CN3UIButton*) GetChildByID("btn_close");		
	__ASSERT(m_pBtn_WalkAway, "NULL UI Component!!");
	m_pBtn_ResetStats = (CN3UIButton*) GetChildByID("Btn_repoint0");
	__ASSERT(m_pBtn_ResetStats, "NULL UI Component!!");
	m_pBtn_ResetSkills = (CN3UIButton*) GetChildByID("Btn_repoint1");
	__ASSERT(m_pBtn_ResetSkills, "NULL UI Component!!");

	return true;
}

void CUIStatSkillReset::UpdateRequiredCoinValues()
{
	//stats
	//Required coins for stat reset
	int RequiredCoinsStatReset = (int) pow((m_pInfo->iLevel * 2.0f), 3.4f);
	if (m_pInfo->iLevel < 30)
		RequiredCoinsStatReset = (int) (RequiredCoinsStatReset * 0.4f);
	else if (m_pInfo->iLevel >= 60)
		RequiredCoinsStatReset = (int) (RequiredCoinsStatReset * 1.5f);

	// discount need to be implemented for client
	/*if ((CGameBase::g_pMain->m_sDiscount == 1 && g_pMain->m_byOldVictory == GetNation())
		|| g_pMain->m_sDiscount == 2)
		RequiredCoinsStatReset /= 2;
	*/
	m_iRequiredCoinsStatReset = RequiredCoinsStatReset;

	//skills

	//Required coins for skill reset
	int RequiredCoinsSkillReset = (int) pow((m_pInfo->iLevel * 2.0f), 3.4f);
	if (m_pInfo->iLevel < 30)
		RequiredCoinsSkillReset = (int) (RequiredCoinsSkillReset * 0.4f);
	else if (m_pInfo->iLevel >= 60)
		RequiredCoinsSkillReset = (int) (RequiredCoinsSkillReset * 1.5f);

	RequiredCoinsSkillReset = (int) (RequiredCoinsSkillReset * 1.5f);

	// need to be implemented for client
	/*
	if (g_pMain->m_sDiscount == 2 // or war discounts are enabled
		|| (g_pMain->m_sDiscount == 1 && g_pMain->m_byOldVictory == m_bNation))
		RequiredCoinsSkillReset /= 2;
	*/
	m_iRequiredCoinsSkillReset = RequiredCoinsSkillReset;
}


bool CUIStatSkillReset::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{

	
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == m_pBtn_WalkAway)
		{
			this->SetVisible(false);
		}
		else if (pSender == m_pBtn_ResetStats)
		{
			//close UI first
			SetVisible(false);


			//Send packet
			std::string szTmp = "Resetting stats will cost %d, Do you want to proceed ?";
			//::_LoadStringFromResource(IDS_CLAN_WARNING_COST, szTmp);
			
			char szMsg[80];
			sprintf(szMsg, szTmp.c_str(), m_iRequiredCoinsStatReset);
			CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_STAT_RESET);

		}
		else if (pSender == m_pBtn_ResetSkills)
		{

			//close UI first
			SetVisible(false);

		

			//Send packet
			std::string szTmp = "Resetting skills will cost %d, Do you want to proceed ?";
			//::_LoadStringFromResource(IDS_CLAN_WARNING_COST, szTmp);
			char szMsg[80];
			sprintf(szMsg, szTmp.c_str(), m_iRequiredCoinsSkillReset);
			CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_SKILL_RESET);

		}


		
	}

	return true;
}



bool CUIStatSkillReset::CanChangeStats()
{
	
	//basic controlls should be added to avoid sending wrong packet on click
	//isDead() ,isTrading(), isStoreOpen() ,isMerchanting(), isSellingMerchant()
	
	//level check, level must be higher than 10
	if (m_pInfo->iLevel < m_iStatResetLevelMin)
	{
		ShowStatErrorLevel();
		return false;
	}

	//Coins check
	if (m_iRequiredCoinsStatReset > m_pInfoExt->iGold)
	{
		ShowStatErrorCoins();
		return false;
	}

	//avoid sending wrong packet at UI
	//inventory needs to be checked, no items should be equiped to reset stats



	
	return true;
}

bool CUIStatSkillReset::CanChangeSkills()
{

	//level check, level must be higher than 10


	if (m_pInfo->iLevel < m_iSkillResetLevelMin)
	{
		ShowSkillErrorLevel();
		return false;
	}

	//Coins check

	if (m_iRequiredCoinsSkillReset > m_pInfoExt->iGold)
	{
		ShowSkillErrorCoins();
		return false;
	}

	return true;
}

//show error messages to user
//NOTE: this needs better organization, done to keep clean other parts
//6116 = szText, npc distance need to checked
void CUIStatSkillReset::ShowStatErrorLevel()
{
	std::string szTmp;
	::_LoadStringFromResource(6610, szTmp);
	char szMsg[80];
	sprintf(szMsg, szTmp.c_str(), m_iSkillResetLevelMin);
	CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowStatErrorCoins()
{
	//std::string szTmp = "You need %d Coins, to reset your stats !";
	std::string szMsg;
	::_LoadStringFromResource(6306, szMsg);
	CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowStatErrorEquipedItem()
{
	std::string szMsg;
	::_LoadStringFromResource(6112, szMsg);
	CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowStatErrorAlreadyReset()
{
	std::string szMsg = "Error : Stats have already been reset !.";
		//::_LoadStringFromResource(IDS_CLAN_WARNING_COST, szTmp);

	CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowSkillErrorAlreadyReset()
{
	std::string szMsg = "Error : Skills have already been reset !.";
		//::_LoadStringFromResource(IDS_CLAN_WARNING_COST, szTmp);

	CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowSkillErrorLevel()
{
	std::string szTmp;
	::_LoadStringFromResource(6610, szTmp);
	char szMsg[80];
	sprintf(szMsg, szTmp.c_str(), m_iSkillResetLevelMin);
	CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowSkillErrorCoins()
{
	std::string szMsg;
	::_LoadStringFromResource(6306, szMsg);
	CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowStatResetSuccess()
{
	std::string szMsg = "Stats have been reset successfully.";
		//::_LoadStringFromResource(IDS_CLAN_WARNING_COST, szTmp);
	
	CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowSkillResetSuccess()
{
	std::string szMsg = "Skills have been reset successfully.";
		//::_LoadStringFromResource(IDS_CLAN_WARNING_COST, szTmp);

	CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_OK);
}


//end of error messages

void CUIStatSkillReset::MsgSend_ResetStats()
{

	
	//don't send packet if not met requirements
	if (!CanChangeStats()) 
	{
		return;
	}

	uint8_t byBuff[2];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_STAT_SKILL_RESET);
	CAPISocket::MP_AddByte(byBuff, iOffset, STAT_RESET_REQ);
	CGameProcedure::s_pSocket->Send(byBuff, iOffset);
}

void CUIStatSkillReset::MsgSend_ResetSkills()
{
	
	//don't send packet if not met requirements
	if (!CanChangeSkills())
	{
		return;
	}

	uint8_t byBuff[2];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_STAT_SKILL_RESET);
	CAPISocket::MP_AddByte(byBuff, iOffset, SKILL_RESET_REQ);
	CGameProcedure::s_pSocket->Send(byBuff, iOffset);

}

void CUIStatSkillReset::Open()
{
	SetVisible(true);
	if (m_pBtn_WalkAway)
	{
		if (!m_pBtn_WalkAway->IsVisible())
		{
			m_pBtn_WalkAway->SetVisible(true);
		}
	}

	if (m_pBtn_ResetStats)
	{
		if (!m_pBtn_ResetStats->IsVisible())
		{
			m_pBtn_ResetStats->SetVisible(true);
		}
	}

	if (m_pBtn_ResetSkills)
	{
		if (!m_pBtn_ResetSkills->IsVisible())
		{
			m_pBtn_ResetSkills->SetVisible(true);
		}
	}

	UpdateRequiredCoinValues();
}



bool CUIStatSkillReset::OnKeyPress(int iKey)
{
	if (DIK_ESCAPE == iKey)
	{
		SetVisible(false);
		return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}

void CUIStatSkillReset::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if (bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();
}

