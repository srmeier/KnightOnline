// UICmdList.cpp: implementation of the CUICmdList class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "resource.h"
#include "GameDef.h"
#include "UICmdList.h"
#include "GameProcedure.h"
#include "LocalInput.h"

#include "N3UIProgress.h"
#include "N3UIString.h"
#include "N3UIImage.h"
#include "GameProcMain.h"
#include "APISocket.h"
#include "PacketDef.h"
#include "PlayerMySelf.h"
#include "UIManager.h"

#include "N3Texture.h"
#include "N3UIDBCLButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUICmdList::CUICmdList()
{
	m_bOpenningNow = false; // 열리고 있다..
	m_bClosingNow = false;	// 닫히고 있다..
	m_fMoveDelta = 0.0f; // 부드럽게 열리고 닫히게 만들기 위해서 현재위치 계산에 부동소수점을 쓴다..

	m_pBtn_cancel = NULL;
	m_pList_CmdCat = NULL;
	m_pList_Cmds = NULL;
}

CUICmdList::~CUICmdList()
{
		delete m_pBtn_cancel;
		delete m_pList_CmdCat;
		delete m_pList_Cmds;
		//SavvyNik - need to make sure to delete any pointers
}

bool CUICmdList::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;

	m_pBtn_cancel = (CN3UIButton*)this->GetChildByID("btn_cancel");		__ASSERT(m_pBtn_cancel, "NULL UI Component!!!");
	m_pList_CmdCat = (CN3UIList*)this->GetChildByID("list_curtailment");  __ASSERT(m_pList_CmdCat, "NULL UI Component!!!");
	m_pList_Cmds = (CN3UIList*)this->GetChildByID("list_content");		__ASSERT(m_pList_Cmds, "NULL UI Component!!!");

	CreateCategoryList();
	return true;
}

void CUICmdList::Release()
{
	if (m_bOpenningNow) // 오른쪽에서 왼쪽으로 스르륵...열려야 한다면..
	{
		POINT ptCur = this->GetPos();
		RECT rc = this->GetRegion();
		float fWidth = (float)(rc.right - rc.left);

		float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
		fDelta *= (fWidth - m_fMoveDelta) / fWidth;
		if (fDelta < 2.0f) fDelta = 2.0f;
		m_fMoveDelta += fDelta;

		int iXLimit = CN3Base::s_CameraData.vp.Width - (int)fWidth;
		ptCur.x = CN3Base::s_CameraData.vp.Width - (int)m_fMoveDelta;
		if (ptCur.x <= iXLimit) // 다열렸다!!
		{
			ptCur.x = iXLimit;
			m_bOpenningNow = false;
		}

		this->SetPos(ptCur.x, ptCur.y);
	}
	else if (m_bClosingNow) // 오른쪽에서 왼쪽으로 스르륵...열려야 한다면..
	{
		POINT ptCur = this->GetPos();
		RECT rc = this->GetRegion();
		float fWidth = (float)(rc.right - rc.left);

		float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
		fDelta *= (fWidth - m_fMoveDelta) / fWidth;
		if (fDelta < 2.0f) fDelta = 2.0f;
		m_fMoveDelta += fDelta;

		int iXLimit = CN3Base::s_CameraData.vp.Width;
		ptCur.x = CN3Base::s_CameraData.vp.Width - (int)(fWidth - m_fMoveDelta);
		if (ptCur.x >= iXLimit) // 다 닫혔다..!!
		{
			ptCur.x = iXLimit;
			m_bClosingNow = false;

			this->SetVisibleWithNoSound(false, false, true); // 다 닫혔으니 눈에서 안보이게 한다.
		}

		this->SetPos(ptCur.x, ptCur.y);
	}

	CN3UIBase::Tick();
	CN3UIBase::Release();

}

void CUICmdList::Render()
{
	if (false == m_bVisible) return;

	CN3UIBase::Render();
}

void CUICmdList::Tick()
{
	if (m_bOpenningNow) // 오른쪽에서 왼쪽으로 스르륵...열려야 한다면..
	{
		POINT ptCur = this->GetPos();
		RECT rc = this->GetRegion();
		float fWidth = (float)(rc.right - rc.left);

		float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
		fDelta *= (fWidth - m_fMoveDelta) / fWidth;
		if (fDelta < 2.0f) fDelta = 2.0f;
		m_fMoveDelta += fDelta;

		int iXLimit = CN3Base::s_CameraData.vp.Width - (int)fWidth;
		ptCur.x = CN3Base::s_CameraData.vp.Width - (int)m_fMoveDelta;
		if (ptCur.x <= iXLimit) // 다열렸다!!
		{
			ptCur.x = iXLimit;
			m_bOpenningNow = false;
		}

		this->SetPos(ptCur.x, ptCur.y);
	}
	else if (m_bClosingNow) // 오른쪽에서 왼쪽으로 스르륵...열려야 한다면..
	{
		POINT ptCur = this->GetPos();
		RECT rc = this->GetRegion();
		float fWidth = (float)(rc.right - rc.left);

		float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
		fDelta *= (fWidth - m_fMoveDelta) / fWidth;
		if (fDelta < 2.0f) fDelta = 2.0f;
		m_fMoveDelta += fDelta;

		int iXLimit = CN3Base::s_CameraData.vp.Width;
		ptCur.x = CN3Base::s_CameraData.vp.Width - (int)(fWidth - m_fMoveDelta);
		if (ptCur.x >= iXLimit) // 다 닫혔다..!!
		{
			ptCur.x = iXLimit;
			m_bClosingNow = false;

			this->SetVisibleWithNoSound(false, false, true); // 다 닫혔으니 눈에서 안보이게 한다.
		}

		this->SetPos(ptCur.x, ptCur.y);
	}

	CN3UIBase::Tick();
}

bool CUICmdList::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (NULL == pSender) return false;

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender->m_szID == "btn_cancel")
			SetVisible(false);
	}
	else if (pSender == m_pList_CmdCat) {
		uint8_t iSel = m_pList_CmdCat->GetCurSel();
		UpdateCommandList(iSel);
	}
	else if (pSender == m_pList_Cmds) {
		uint8_t iSel = m_pList_Cmds->GetCurSel();
		ExecuteCommand(iSel);
	}
	
	return false;
}

bool CUICmdList::OnKeyPress(int iKey)
{
	switch (iKey)
	{
	case SDL_SCANCODE_ESCAPE://DIK_ESCAPE:
	{	//hotkey가 포커스 잡혀있을때는 다른 ui를 닫을수 없으므로 DIK_ESCAPE가 들어오면 포커스를 다시잡고
		//열려있는 다른 유아이를 닫아준다.
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
		CN3UIBase* pFocus = CGameProcedure::s_pUIMgr->GetFocusedUI();
		if (pFocus && pFocus != this) pFocus->OnKeyPress(iKey);
	}
	return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}

void CUICmdList::Open()
{
	// 스르륵 열린다!!
	SetVisible(true);
	this->SetPos(CN3Base::s_CameraData.vp.Width, 10);
	m_fMoveDelta = 0;
	m_bOpenningNow = true;
	m_bClosingNow = false;

	m_iRBtnDownOffs = -1;
}


void CUICmdList::Close()
{
	//SetVisible(false); 
	RECT rc = this->GetRegion();
	this->SetPos(CN3Base::s_CameraData.vp.Width - (rc.right - rc.left), 10);
	m_fMoveDelta = 0;
	m_bOpenningNow = false;
	m_bClosingNow = true;

	m_iRBtnDownOffs = -1;
}

void CUICmdList::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if (bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}

bool CUICmdList::CreateCategoryList() {

	if (m_pList_CmdCat == NULL) return false;

	std::string szCategory;
	::_LoadStringFromResource(IDS_PRIVATE_CMD_CAT, szCategory);
	m_pList_CmdCat->AddString(szCategory);
	::_LoadStringFromResource(IDS_TRADE_CMD_CAT, szCategory);
	m_pList_CmdCat->AddString(szCategory);
	::_LoadStringFromResource(IDS_PARTY_CMD_CAT, szCategory);
	m_pList_CmdCat->AddString(szCategory);
	::_LoadStringFromResource(IDS_CLAN_CMD_CAT, szCategory);
	m_pList_CmdCat->AddString(szCategory);
	::_LoadStringFromResource(IDS_KNIGHTS_CMD_CAT, szCategory);
	m_pList_CmdCat->AddString(szCategory);
	::_LoadStringFromResource(IDS_GUARDIAN_MON_CMD_CAT, szCategory);
	m_pList_CmdCat->AddString(szCategory);
	::_LoadStringFromResource(IDS_KING_CMD_CAT, szCategory);
	m_pList_CmdCat->AddString(szCategory);
	::_LoadStringFromResource(IDS_GM_CMD_CAT, szCategory);
	m_pList_CmdCat->AddString(szCategory);

	m_pList_CmdCat->SetFontColor(0xffffff00); //green

	UpdateCommandList(PRIVATE_CMD_LIST); //Initialize a cmd list for viewing 
	return true;
}

bool CUICmdList::UpdateCommandList(uint8_t cmdCat ) {

	if (m_pList_Cmds == NULL) return false;

	std::string szCommand;

	switch ( cmdCat ) {
	
	case PRIVATE_CMD_LIST:
		::_LoadStringFromResource(IDS_PRIVATE_PM_CMD, szCommand);
		m_pList_Cmds->AddString(szCommand);
		::_LoadStringFromResource(IDS_PRIVATE_TWN_CMD, szCommand);
		m_pList_Cmds->AddString(szCommand);
		::_LoadStringFromResource(IDS_PRIVATE_EXIT_CMD, szCommand);
		m_pList_Cmds->AddString(szCommand);
		::_LoadStringFromResource(IDS_PRIVATE_GREET_CMD, szCommand);
		m_pList_Cmds->AddString(szCommand);
		::_LoadStringFromResource(IDS_PRIVATE_GREET2_CMD, szCommand);
		m_pList_Cmds->AddString(szCommand);
		::_LoadStringFromResource(IDS_PRIVATE_GREET3_CMD, szCommand);
		m_pList_Cmds->AddString(szCommand);
		::_LoadStringFromResource(IDS_PRIVATE_PROVOKE_CMD, szCommand);
		m_pList_Cmds->AddString(szCommand);
		::_LoadStringFromResource(IDS_PRIVATE_PROVOKE2_CMD, szCommand);
		m_pList_Cmds->AddString(szCommand);
		::_LoadStringFromResource(IDS_PRIVATE_PROVOKE3_CMD, szCommand);
		m_pList_Cmds->AddString(szCommand);
		::_LoadStringFromResource(IDS_PRIVATE_SAVE_CMD, szCommand);
		m_pList_Cmds->AddString(szCommand);
		::_LoadStringFromResource(IDS_PRIVATE_BATTLE_CMD, szCommand);
		m_pList_Cmds->AddString(szCommand);
		break;

	case TRADE_CMD_LIST:
		break;

	case PARTY_CMD_LIST:
		break;

	case CLAN_CMD_LIST:
		break;

	case KNIGHTS_CMD_LIST:
		break;

	case GUARDIAN_CMD_LIST:
		break;

	case KING_CMD_LIST:
		break;

	case GM_CMD_LIST:
		break;

	default:
		m_pList_Cmds = NULL;
		break;
	}

	return true;
}

bool CUICmdList::ExecuteCommand(uint8_t cmdSel) {

}