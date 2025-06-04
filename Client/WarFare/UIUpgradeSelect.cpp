#include "StdAfx.h"
#include "UIUpgradeSelect.h"
#include "GameProcMain.h"
#include "UIItemUpgrade.h"
#include "UIRingUpgrade.h"
#include "UIManager.h"

#include <N3BASE/N3UIButton.h>

CUIUpgradeSelect::CUIUpgradeSelect()
{
	m_pBtn_Upgrade_1	= nullptr;
	m_pBtn_Upgrade_2	= nullptr;
	m_pBtn_Close		= nullptr;
}

CUIUpgradeSelect::~CUIUpgradeSelect()
{
}

bool CUIUpgradeSelect::Load(
	HANDLE hFile)
{
	if (!CN3UIBase::Load(hFile))
		return false;

	N3_VERIFY_UI_COMPONENT(m_pBtn_Upgrade_1,	(CN3UIButton*) GetChildByID("upgrade_1"));
	N3_VERIFY_UI_COMPONENT(m_pBtn_Upgrade_2,	(CN3UIButton*) GetChildByID("upgrade_2"));
	N3_VERIFY_UI_COMPONENT(m_pBtn_Close,		(CN3UIButton*) GetChildByID("btn_close"));

	return true;
}

bool CUIUpgradeSelect::ReceiveMessage(
	CN3UIBase* pSender,
	uint32_t dwMsg)
{
	if (pSender == nullptr)
		return false;

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == m_pBtn_Upgrade_1)
		{
			CUIItemUpgrade* pUIItemUpgrade = CGameProcedure::s_pProcMain->m_pUIItemUpgrade;
			if (pUIItemUpgrade != nullptr)
			{
				CGameProcedure::s_pProcMain->UpgradeTransactionState();
				pUIItemUpgrade->Open();
			}


			SetVisible(false);
		}
		else if (pSender == m_pBtn_Upgrade_2)
		{
			CUIRingUpgrade* pUIRingUpgrade = CGameProcedure::s_pProcMain->m_pUIRingUpgrade;
			if (pUIRingUpgrade != nullptr)
			{
				CGameProcedure::s_pProcMain->UpgradeTransactionState();
				pUIRingUpgrade->Open();
			}


			SetVisible(false);
		}
		else if (pSender == m_pBtn_Close)
		{
			SetVisible(false);
		}
	}

	return true;
}

void CUIUpgradeSelect::SetVisible(
	bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);

	if (bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();
}
