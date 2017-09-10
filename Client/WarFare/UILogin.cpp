// UILogIn.cpp: implementation of the CUILogIn class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "resource.h"
#include "UILogIn.h"
#include "GameProcLogIn.h"

#include "N3UIEdit.h"
#include "N3UIButton.h"
#include "N3UIList.h"
#include "UIMessageBoxManager.h"
#include <algorithm>
#include <shellapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUILogIn::CUILogIn()
{
	m_pEdit_id = NULL;
	m_pEdit_pw = NULL;

	m_pBtn_LogIn = NULL;
	m_pBtn_Connect = NULL;
	m_pBtn_Cancel = NULL;
	m_pBtn_Option = NULL;
	m_pBtn_Join = NULL;

	m_pGroup_ServerList = NULL;
	m_pGroup_LogIn = NULL;

	m_pText_Rights = NULL;
	m_pImg_MGameLogo = NULL;
	m_pImg_DaumLogo = NULL;
	m_pImg_GradeLogo = NULL;

	m_pList_Server = NULL;

	m_bOpenningNow = false; // 위에서 아래로 스르륵...열려야 한다면..
	m_fMoveDelta = 0;

	m_bLogIn = false;
}

CUILogIn::~CUILogIn()
{

}

bool CUILogIn::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (NULL == pSender) return false;

	//s_CameraData.vp;  //불러 오는 과정을 살펴본다 
	//uint32_t mm = s_CameraData.vp.Height;
	//uint32_t ss = s_CameraData.vp.Width;	

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == m_pBtn_LogIn && m_pEdit_id && m_pEdit_pw)
		{
			CGameProcedure::s_pProcLogIn->MsgSend_AccountLogIn(LIC_KNIGHTONLINE);
			return true;
		}
		else if (pSender == m_pBtn_Connect)
		{
			CGameProcedure::s_pProcLogIn->ConnectToGameServer(); // 고른 게임 서버에 접속
			return true;
		}
		else if (pSender == m_pBtn_Cancel)
		{
			//PostQuitMessage(0);	// 종료...
			CGameBase::s_bRunning = false;
			return true;
		}
		else if (pSender == m_pBtn_Option) // 옵션..
		{
			std::string szMsg;
			::_LoadStringFromResource(IDS_CONFIRM_EXECUTE_OPTION, szMsg);
			CGameProcedure::MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_EXECUTE_OPTION);
			return true;
		}
		else if (pSender == m_pBtn_Join)
		{
			if (!CGameProcedure::s_pProcLogIn->m_szRegistrationSite.empty())
			{
				ShellExecute(NULL, "open", CGameProcedure::s_pProcLogIn->m_szRegistrationSite.c_str(), NULL, NULL, SW_SHOWNORMAL);
			}

			return true;
		}
	}
	else if (UIMSG_LIST_DBLCLK == dwMsg)
	{
		CGameProcedure::s_pProcLogIn->ConnectToGameServer(); // 고른 게임 서버에 접속
		return true;
	}
	else if (dwMsg == UIMSG_EDIT_RETURN)
	{
		// TEMP(srmeier): there is a weird issue where the key inputs aren't going
		// through CGameProcedure::ProcessUIKeyInput() so CUILogIn::OnKeyPress() isn't
		// being called...
		if (!m_bLogIn && m_pEdit_id && m_pEdit_pw)
		{
			CN3UIBase* pMsgBox = CGameProcedure::s_pMsgBoxMgr->GetFocusMsgBox();
			if (!(pMsgBox && pMsgBox->IsVisible()))
				CGameProcedure::s_pProcLogIn->MsgSend_AccountLogIn(LIC_KNIGHTONLINE);
		}
		else
		{
			return ReceiveMessage(m_pBtn_Connect, UIMSG_BUTTON_CLICK);
		}
	}
	else if (dwMsg == UIMSG_EDIT_TAB)
	{
		// TEMP(srmeier): there is a weird issue where the key inputs aren't going
		// through CGameProcedure::ProcessUIKeyInput() so CUILogIn::OnKeyPress() isn't
		// being called...
		FocusCircular();
		return true;
	}

	return false;
}

bool CUILogIn::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;

	//PrintChildIDs();

	m_pGroup_LogIn = GetChildByID("Group_LogIn");
	__ASSERT(m_pGroup_LogIn, "NULL UI Component!!");

	if (m_pGroup_LogIn)
	{
		m_pBtn_LogIn = (CN3UIButton*)m_pGroup_LogIn->GetChildByID("Btn_Login");		__ASSERT(m_pBtn_LogIn, "NULL UI Component!!");
		m_pBtn_Cancel = (CN3UIButton*)m_pGroup_LogIn->GetChildByID("Btn_Cancel");	__ASSERT(m_pBtn_Cancel, "NULL UI Component!!");
		m_pBtn_Option = (CN3UIButton*)m_pGroup_LogIn->GetChildByID("Btn_Option");	__ASSERT(m_pBtn_Option, "NULL UI Component!!");
		m_pBtn_Join = (CN3UIButton*)m_pGroup_LogIn->GetChildByID("Btn_Join");		__ASSERT(m_pBtn_Join, "NULL UI Component!!");

		m_pEdit_id = (CN3UIEdit*)m_pGroup_LogIn->GetChildByID("Edit_ID");			__ASSERT(m_pEdit_id, "NULL UI Component!!");
		m_pEdit_pw = (CN3UIEdit*)m_pGroup_LogIn->GetChildByID("Edit_PW");			__ASSERT(m_pEdit_pw, "NULL UI Component!!");

		//m_pImg_GradeLogo = m_pGroup_LogIn->GetChildByID("Img_Grade");	__ASSERT(m_pImg_GradeLogo, "NULL UI Component!!");
	}

	m_pText_Rights = GetChildByID("Text_Rights");	__ASSERT(m_pText_Rights, "NULL UI Component!!");
	m_pImg_MGameLogo = GetChildByID("Img_MGame");	__ASSERT(m_pImg_MGameLogo, "NULL UI Component!!");
	m_pImg_DaumLogo = GetChildByID("Img_Daum");		__ASSERT(m_pImg_DaumLogo, "NULL UI Component!!");

	if (m_pText_Rights) m_pText_Rights->SetVisible(false);
	if (m_pImg_MGameLogo) m_pImg_MGameLogo->SetVisible(false);
	if (m_pImg_DaumLogo) m_pImg_DaumLogo->SetVisible(false);

	GetChildByID("Group_Notice_1")->SetVisible(false);
	GetChildByID("Group_Notice_2")->SetVisible(false);
	GetChildByID("Group_Notice_3")->SetVisible(false);
	GetChildByID("premium")->SetVisible(false);

	m_pGroup_ServerList = GetChildByID("Group_ServerList");		__ASSERT(m_pGroup_ServerList, "NULL UI Component!!");
	if (m_pGroup_ServerList)
	{
		m_pList_Server = (CN3UIList*)(m_pGroup_ServerList->GetChildByID("List_Server"));	__ASSERT(m_pList_Server, "NULL UI Component!!");
		m_pBtn_Connect = (CN3UIButton*)m_pGroup_ServerList->GetChildByID("Btn_Connect");	__ASSERT(m_pBtn_Connect, "NULL UI Component!!");

		m_pGroup_ServerList->SetVisible(false);
	}

	return true;
}

void CUILogIn::AccountIDGet(std::string& szID)
{
	if (m_pEdit_id)
		szID = m_pEdit_id->GetString();
	else
		szID = "";
}

void CUILogIn::AccountPWGet(std::string& szPW)
{
	if (m_pEdit_pw)
		szPW = m_pEdit_pw->GetString();
	else
		szPW = "";
}

void CUILogIn::ConnectButtonSetEnable(bool bEnable)
{
	eUI_STATE eState1 = (bEnable ? UI_STATE_BUTTON_NORMAL : UI_STATE_BUTTON_DISABLE);
	eUI_STATE eState2 = (bEnable ? UI_STATE_LIST_ENABLE : UI_STATE_LIST_DISABLE);

	if (m_pBtn_Connect) m_pBtn_Connect->SetState(eState1);
	if (m_pList_Server) m_pList_Server->SetState(eState2);
}

void CUILogIn::FocusToID()
{
	if (m_pEdit_id) m_pEdit_id->SetFocus();
}

void CUILogIn::FocusCircular()
{
	if (NULL == m_pEdit_id || NULL == m_pEdit_pw) return;

	if (m_pEdit_id->HaveFocus())
		m_pEdit_pw->SetFocus();
	else
		m_pEdit_id->SetFocus();
}

void CUILogIn::InitEditControls()
{
	if (m_pEdit_id)
	{
		m_pEdit_id->SetString("");
		m_pEdit_id->SetFocus();
	}

	if (m_pEdit_pw)
	{
		m_pEdit_pw->SetString("");
	}
}

bool CUILogIn::ServerInfoAdd(const __GameServerInfo& GSI)
{
	m_ListServerInfos.push_back(GSI);
	return true;
}

bool CUILogIn::ServerInfoGet(size_t iIndex, __GameServerInfo& GSI)
{
	if (NULL == m_pList_Server
		|| iIndex >= m_ListServerInfos.size())
		return false;

	GSI = m_ListServerInfos[iIndex];
	return true;
}

bool CUILogIn::ServerInfoGetCur(__GameServerInfo& GSI)
{
	GSI.Init();
	if (NULL == m_pList_Server) return false;

	int iIndex = m_pList_Server->GetCurSel();
	return this->ServerInfoGet(iIndex, GSI);
}

void CUILogIn::ServerInfoUpdate()
{
	if (NULL == m_pList_Server) return;

	m_pList_Server->ResetContent();
	if (!m_ListServerInfos.empty())
	{
		//sort(m_ListServerInfos.begin(), m_ListServerInfos.end(), not2(__GameServerInfo()));
		int iSize = m_ListServerInfos.size();
		for (int i = 0; i < iSize; i++)
		{
			m_pList_Server->AddString(m_ListServerInfos[i].szName);
		}
	}
}

void CUILogIn::Tick()
{
	CN3UIBase::Tick();

	if (m_pGroup_ServerList)
	{
		if (m_bOpenningNow) // 위에서 아래로 스르륵...열려야 한다면..
		{
			POINT ptCur = m_pGroup_ServerList->GetPos();
			RECT rc = m_pGroup_ServerList->GetRegion();
			float fHeight = (float)(rc.bottom - rc.top);

			float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
			fDelta *= (fHeight - m_fMoveDelta) / fHeight;
			if (fDelta < 2.0f) fDelta = 2.0f;
			m_fMoveDelta += fDelta;

			int iYLimit = 0;
			ptCur.y = (int)(m_fMoveDelta - fHeight);
			if (ptCur.y >= iYLimit) // 다열렸다!!
			{
				ptCur.y = iYLimit;
				m_bOpenningNow = false;
			}

			m_pGroup_ServerList->SetPos(ptCur.x, ptCur.y);
		}
	}
}

void CUILogIn::OpenServerList()
{
	if (m_bOpenningNow || NULL == m_pGroup_ServerList) return;

	// 스르륵 열린다!!
	m_pGroup_ServerList->SetVisible(true);
	RECT rc = m_pGroup_ServerList->GetRegion();
	m_pGroup_ServerList->SetPos(0, -(rc.bottom - rc.top));

	m_fMoveDelta = 0;
	m_bOpenningNow = true;
}

void CUILogIn::SetVisibleLogInUIs(bool bEnable)
{
	if (m_pGroup_LogIn) m_pGroup_LogIn->SetVisible(bEnable); // 로그인을 숨긴다..

	// 로그인한 계정의 구분에 따라 UI 만지기...
	if (m_pText_Rights) m_pText_Rights->SetVisible(false);
	if (m_pImg_MGameLogo) m_pImg_MGameLogo->SetVisible(false);
	if (m_pImg_DaumLogo) m_pImg_DaumLogo->SetVisible(false);

	if (false == bEnable)
	{
		if (LIC_MGAME == CGameProcedure::s_eLogInClassification)
		{
			if (m_pText_Rights && m_pImg_MGameLogo)
			{
				// 아래쪽 중단으로 맞춘다..
				RECT rcView = { 0, 0, (int)s_CameraData.vp.Width, (int)s_CameraData.vp.Height };
				int iX = (rcView.right - (m_pText_Rights->GetWidth() + m_pImg_MGameLogo->GetWidth())) / 2;
				int iY = rcView.bottom - m_pText_Rights->GetHeight() - 20;
				m_pText_Rights->SetPos(iX, iY);
				RECT rc = m_pText_Rights->GetRegion();
				m_pImg_MGameLogo->SetPos(rc.right, rcView.bottom - m_pImg_MGameLogo->GetHeight() - 20);

				m_pText_Rights->SetVisible(true);
				m_pImg_MGameLogo->SetVisible(true);
			}
		}
		else if (LIC_DAUM == CGameProcedure::s_eLogInClassification)
		{
			if (m_pText_Rights && m_pImg_DaumLogo)
			{
				// 아래쪽 중단으로 맞춘다..
				RECT rcView = { 0, 0, (int)s_CameraData.vp.Width, (int)s_CameraData.vp.Height };
				int iX = (rcView.right - (m_pText_Rights->GetWidth() + m_pImg_DaumLogo->GetWidth())) / 2;
				int iY = rcView.bottom - m_pText_Rights->GetHeight() - 20;
				m_pText_Rights->SetPos(iX, iY);
				RECT rc = m_pText_Rights->GetRegion();
				m_pImg_DaumLogo->SetPos(rc.right, rcView.bottom - m_pImg_DaumLogo->GetHeight() - 20);

				m_pText_Rights->SetVisible(true);
				m_pImg_DaumLogo->SetVisible(true);
			}
		}
		else
		{
			if (m_pText_Rights)
			{
				RECT rcView = { 0, 0, (int)s_CameraData.vp.Width, (int)s_CameraData.vp.Height };
				int iX = (rcView.right - m_pText_Rights->GetWidth()) / 2;
				int iY = rcView.bottom - m_pText_Rights->GetHeight() - 20;
				m_pText_Rights->SetPos(iX, iY);

				m_pText_Rights->SetVisible(true);
			}
		}
	}
}

void CUILogIn::RecalcGradePos()
{
	if (m_pImg_GradeLogo) // 이용등급 표시
	{
		RECT rc = m_pImg_GradeLogo->GetRegion();
		int iX = s_CameraData.vp.Width - (rc.right - rc.left + 10);
		int iY = 10;
		m_pImg_GradeLogo->SetPos(iX, iY);
	}
}

bool CUILogIn::OnKeyPress(int iKey)
{
	if (!m_bLogIn)
	{
		switch (iKey)
		{
		case SDL_SCANCODE_TAB://DIK_TAB:
			FocusCircular();
			return true;
			//		case DIK_NUMPADENTER:
			//		case DIK_RETURN:
			//			CGameProcedure::s_pProcLogIn->MsgSend_AccountLogIn(LIC_KNIGHTONLINE);
			//			return true;
		}
	}
	else if (!m_bOpenningNow && m_pGroup_ServerList && m_pGroup_ServerList->IsVisible())
	{
		switch (iKey)
		{
		case SDL_SCANCODE_UP://DIK_UP:
		{
			if (NULL == m_pList_Server) return false;

			int iIndex = m_pList_Server->GetCurSel();

			if (iIndex > 0) m_pList_Server->SetCurSel(iIndex - 1);
			int iCnt = m_pList_Server->GetCount();
		}
		return true;
		case SDL_SCANCODE_DOWN://DIK_DOWN:
		{
			if (NULL == m_pList_Server) return false;

			int iIndex = m_pList_Server->GetCurSel();
			int iCnt = m_pList_Server->GetCount();

			if (iCnt - iIndex > 1) m_pList_Server->SetCurSel(iIndex + 1);
		}
		return true;
		case SDL_SCANCODE_KP_ENTER://DIK_NUMPADENTER:
		case SDL_SCANCODE_RETURN://DIK_RETURN:
			ReceiveMessage(m_pBtn_Connect, UIMSG_BUTTON_CLICK);
			return true;
		}
	}

	return CN3UIBase::OnKeyPress(iKey);
}
