// CUIStatSkillReset.cpp: implementation of the CUIStatSkillReset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "resource.h"

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
	//Required coins for stat reset
	int iCoinsStatReset = (int) pow((m_pInfo->iLevel * 2.0f), 3.4f);
	if (m_pInfo->iLevel < 30)
		iCoinsStatReset = (int) (iCoinsStatReset * 0.4f);
	else if (m_pInfo->iLevel >= 60)
		iCoinsStatReset = (int) (iCoinsStatReset * 1.5f);

	// discount need to be implemented for client
	/*if ((g_pMain->m_sDiscount == 1 && g_pMain->m_byOldVictory == GetNation())
		|| g_pMain->m_sDiscount == 2)
		iCoinsStatReset /= 2;
	*/

	m_iRequiredCoinsStatReset = iCoinsStatReset;
	m_sRequiredCoinsStatReset = FormatCoin(iCoinsStatReset);

	//Required coins for skill reset
	int iCoinsSkillReset = (int) pow((m_pInfo->iLevel * 2.0f), 3.4f);
	if (m_pInfo->iLevel < 30)
		iCoinsSkillReset = (int) (iCoinsSkillReset * 0.4f);
	else if (m_pInfo->iLevel >= 60)
		iCoinsSkillReset = (int) (iCoinsSkillReset * 1.5f);

	iCoinsSkillReset = (int) (iCoinsSkillReset * 1.5f);
	// discount need to be implemented for client
	/*
	if (g_pMain->m_sDiscount == 2 // or war discounts are enabled
		|| (g_pMain->m_sDiscount == 1 && g_pMain->m_byOldVictory == m_bNation))
		iCoinsSkillReset /= 2;
	*/

	m_iRequiredCoinsSkillReset = iCoinsSkillReset;
	m_sRequiredCoinsSkillReset = FormatCoin(iCoinsSkillReset);
}

std::string CUIStatSkillReset::FormatCoin(int iCoin)
{
	//same as in UIInventory
	char szBuff[32] = "";
	sprintf(szBuff, "%d", iCoin);
	int buffLength = strlen(szBuff);
	int goldLength = buffLength + (buffLength / 3) - (buffLength % 3 == 0 ? 1 : 0);

	char szGold[42] = "";
	szGold[goldLength] = '\0';

	for (int i = buffLength - 1, k = goldLength - 1; i >= 0; i--, k--)
	{
		if ((buffLength - 1 - i) % 3 == 0 && (buffLength - 1 != i))
			szGold[k--] = ',';

		szGold[k] = szBuff[i];
	}

	return std::string(szGold);
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
			std::string szTmp = "Resetting stats will cost %s. Do you want to proceed ?";
			char szMsg[80];
			sprintf(szMsg, szTmp.c_str(), m_sRequiredCoinsStatReset.c_str());
			CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_STAT_RESET);

		}
		else if (pSender == m_pBtn_ResetSkills)
		{
			//close UI first
			SetVisible(false);

			std::string szTmp = "Resetting skills will cost %s. Do you want to proceed ?";
			char szMsg[80];
			sprintf(szMsg, szTmp.c_str(), m_sRequiredCoinsSkillReset.c_str());
			CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_SKILL_RESET);
		}



	}

	return true;
}



bool CUIStatSkillReset::CanChangeStats()
{

	//TODO: basic controlls should be added to avoid sending wrong packet on click
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

	//avoid sending wrong packet at Client
	//TODO: inventory needs to be checked, no items should be equiped to reset stats




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

//NOTE: error messages can be merged into one function ShowMessage(int iTextID)
//but some of the messages are not in TBL files
void CUIStatSkillReset::ShowStatErrorLevel()
{
	std::string strMsg;
	CGameBase::GetText(6610, &strMsg);
	CGameProcedure::s_pProcMain->MessageBoxPost(strMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowStatErrorCoins()
{
	std::string strMsg;
	CGameBase::GetText(6306, &strMsg);
	CGameProcedure::s_pProcMain->MessageBoxPost(strMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowStatErrorEquipedItem()
{
	std::string strMsg;
	CGameBase::GetText(6112, &strMsg);
	CGameProcedure::s_pProcMain->MessageBoxPost(strMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowSkillErrorLevel()
{
	std::string strMsg;
	CGameBase::GetText(6610, &strMsg);
	CGameProcedure::s_pProcMain->MessageBoxPost(strMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowSkillErrorCoins()
{
	std::string strMsg;
	CGameBase::GetText(6306, &strMsg);
	CGameProcedure::s_pProcMain->MessageBoxPost(strMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowStatErrorUnusedPoints()
{
	std::string strMsg = "You have unused stat points!";
	//CGameBase::GetText(iID, &strMsg);
	CGameProcedure::s_pProcMain->MessageBoxPost(strMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowSkillErrorUnusedPoints()
{
	std::string strMsg = "You have unused skill points!";
	//CGameBase::GetText(iID, &strMsg);
	CGameProcedure::s_pProcMain->MessageBoxPost(strMsg, "", MB_OK);
}

//NOTE: Trigger healing animation instead of showing message on success
//but healing animation crash game FXParticles 
void CUIStatSkillReset::ShowStatResetSuccess()
{
	std::string strMsg = "Stats have been reset successfully.";
	CGameProcedure::s_pProcMain->MessageBoxPost(strMsg, "", MB_OK);
}

void CUIStatSkillReset::ShowSkillResetSuccess()
{
	std::string strMsg = "Skills have been reset successfully.";
	CGameProcedure::s_pProcMain->MessageBoxPost(strMsg, "", MB_OK);
}

//packet senders
void CUIStatSkillReset::MsgSend_ResetStats()
{
	//don't send packet if not met requirements
	if (!CanChangeStats())
	{
		return;
	}
	
	uint16_t iPlayerTargetID = CGameProcMain::s_pPlayer->m_iIDTarget;

	uint8_t byBuff[8];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_STAT_SKILL_RESET);
	CAPISocket::MP_AddByte(byBuff, iOffset, STAT_RESET_REQ);
	CAPISocket::MP_AddWord(byBuff, iOffset, iPlayerTargetID);

	CGameProcedure::s_pSocket->Send(byBuff, iOffset);
}

void CUIStatSkillReset::MsgSend_ResetSkills()
{
	//don't send packet if not met requirements
	if (!CanChangeSkills())
	{
		return;
	}

	uint16_t iPlayerTargetID = CGameProcMain::s_pPlayer->m_iIDTarget;
	
	uint8_t byBuff[8];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_STAT_SKILL_RESET);
	CAPISocket::MP_AddByte(byBuff, iOffset, SKILL_RESET_REQ);
	CAPISocket::MP_AddWord(byBuff, iOffset, iPlayerTargetID);

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
