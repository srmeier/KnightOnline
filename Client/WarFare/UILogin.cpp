// UILogIn.cpp: implementation of the CUILogIn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "UILogIn.h"
#include "GameProcLogIn.h"

#include "N3UIEdit.h"
#include "N3UIButton.h"
#include "N3UIString.h"
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
	
	m_pGroup_Notice_1 = NULL;
	m_pGroup_Notice_2 = NULL;
	m_pGroup_Notice_3 = NULL;

	m_pBtn_NoticeOK_1 = NULL;
	m_pBtn_NoticeOK_2 = NULL;
	m_pBtn_NoticeOK_3 = NULL;

	m_pGroup_ServerList = NULL;
	m_pGroup_LogIn = NULL;

	m_pStr_Premium = NULL;

	m_pText_Rights = NULL;
	m_pImg_MGameLogo = NULL;
	m_pImg_DaumLogo = NULL;
	m_pImg_GradeLogo = NULL;

	m_iSelectedServerIndex = -1;

	for (size_t i = 1; i <= MAX_SERVERS; i++)
	{
		m_pServer_Group[i - 1] = NULL;
		m_pArrow_Group[i - 1] = NULL;
	} 

	m_bOpenningNow = false; // 위에서 아래로 스르륵...열려야 한다면..
	m_bNoticeScreen = false; 
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
			PostQuitMessage(0);	// 종료...
			return true;
		}
		else if (pSender == m_pBtn_Option) // 옵션..
		{
			std::string szMsg;
			CGameBase::GetText(IDS_CONFIRM_EXECUTE_OPTION, &szMsg);
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
		else if (pSender == m_pBtn_NoticeOK_1 || pSender == m_pBtn_NoticeOK_2 ||
				 pSender == m_pBtn_NoticeOK_3)
		{
			this->OpenServerList();
		}
	}
	else if (UIMSG_STRING_LDCLICK == dwMsg) // double click on string
	{

		for (int i = 0; i < MAX_SERVERS; ++i)
		{
			if (m_pServer_Group[i] == nullptr) continue;

			CN3UIString* pStr = (CN3UIString*) (m_pServer_Group[i]->GetChildByID("List_Server"));
			if (pSender == pStr)
			{
				m_iSelectedServerIndex = i;
				CGameProcedure::s_pProcLogIn->ConnectToGameServer();
				return true;
			}
		}

	}
	else if (UIMSG_STRING_LCLICK == dwMsg) //change color on left click
	{

		for (int i = 0; i < MAX_SERVERS; ++i)
		{
			if (m_pServer_Group[i] == nullptr) continue;

			CN3UIString* pStr = (CN3UIString*) (m_pServer_Group[i]->GetChildByID("List_Server"));
			if (pStr) pStr->SetColor(0xFFFFFFFF); //white
		}

		for (int i = 0; i < MAX_SERVERS; ++i)
		{
			if (m_pServer_Group[i] == nullptr) continue;

			CN3UIString* pStr = (CN3UIString*) (m_pServer_Group[i]->GetChildByID("List_Server"));
			if (pSender == pStr)
			{
				pStr->SetColor(0xFFFFFF00); // A=255, R=255, G=255, B=0, yellow 
				m_iSelectedServerIndex = i;
				return true;
			}
		}
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

	return false;
}

bool CUILogIn::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;

	m_pGroup_LogIn = GetChildByID("Group_LogIn"); __ASSERT(m_pGroup_LogIn, "NULL UI Component!!");

	
	RECT rc = m_pGroup_LogIn->GetRegion();
	int iX = ((int) s_CameraData.vp.Width - (rc.right - rc.left)) / 2;
	int iY = ((int) s_CameraData.vp.Height - (rc.bottom - rc.top)) / 2;
	
	//TODO: this is trivial, login group
	//not centered so it has been pushed 180 more
	m_pGroup_LogIn->SetPos(iX - 180, iY);
	m_pGroup_LogIn->SetVisible(true);


	m_pBtn_LogIn = (CN3UIButton*)m_pGroup_LogIn->GetChildByID("btn_ok");		__ASSERT(m_pBtn_LogIn, "NULL UI Component!!");
	m_pBtn_Cancel = (CN3UIButton*)m_pGroup_LogIn->GetChildByID("btn_cancel");	__ASSERT(m_pBtn_Cancel, "NULL UI Component!!");
	m_pBtn_Option = (CN3UIButton*)m_pGroup_LogIn->GetChildByID("btn_option");	__ASSERT(m_pBtn_Option, "NULL UI Component!!");
	m_pBtn_Join = (CN3UIButton*)m_pGroup_LogIn->GetChildByID("btn_homepage");	__ASSERT(m_pBtn_Join, "NULL UI Component!!");

	m_pEdit_id = (CN3UIEdit*)m_pGroup_LogIn->GetChildByID("Edit_ID");			__ASSERT(m_pEdit_id, "NULL UI Component!!");
	m_pEdit_pw = (CN3UIEdit*)m_pGroup_LogIn->GetChildByID("Edit_PW");			__ASSERT(m_pEdit_pw, "NULL UI Component!!");

	//m_pImg_GradeLogo = m_pGroup_LogIn->GetChildByID("Img_Grade");	__ASSERT(m_pImg_GradeLogo, "NULL UI Component!!");

	//m_pText_Rights = GetChildByID("Text_Rights");	__ASSERT(m_pText_Rights, "NULL UI Component!!");
	//m_pImg_MGameLogo = GetChildByID("Img_MGame");	__ASSERT(m_pImg_MGameLogo, "NULL UI Component!!");
	//m_pImg_DaumLogo = GetChildByID("Img_Daum");	__ASSERT(m_pImg_DaumLogo, "NULL UI Component!!");

	//if (m_pText_Rights) m_pText_Rights->SetVisible(false);
	//if (m_pImg_MGameLogo) m_pImg_MGameLogo->SetVisible(false);
	//if (m_pImg_DaumLogo) m_pImg_DaumLogo->SetVisible(false);

	//get notice boxes
	m_pGroup_Notice_1 = GetChildByID("Group_Notice_1");		__ASSERT(m_pGroup_Notice_1, "NULL UI Component!!");
	m_pGroup_Notice_2 = GetChildByID("Group_Notice_2");		__ASSERT(m_pGroup_Notice_2, "NULL UI Component!!");
	m_pGroup_Notice_3 = GetChildByID("Group_Notice_3");		__ASSERT(m_pGroup_Notice_3, "NULL UI Component!!");
	
	m_pBtn_NoticeOK_1 = (CN3UIButton*) m_pGroup_Notice_1->GetChildByID("btn_ok");
	__ASSERT(m_pBtn_NoticeOK_1, "NULL UI Component!!");
	m_pBtn_NoticeOK_2 = (CN3UIButton*) m_pGroup_Notice_2->GetChildByID("btn_ok");
	__ASSERT(m_pBtn_NoticeOK_2, "NULL UI Component!!");
	m_pBtn_NoticeOK_3 = (CN3UIButton*) m_pGroup_Notice_3->GetChildByID("btn_ok");
	__ASSERT(m_pBtn_NoticeOK_3, "NULL UI Component!!");

	//set notice boxes' visibility as false
	m_pGroup_Notice_1->SetVisible(false);
	m_pGroup_Notice_2->SetVisible(false);
	m_pGroup_Notice_3->SetVisible(false);

	m_pStr_Premium = (CN3UIString*) GetChildByID("premium");
	__ASSERT(m_pStr_Premium, "NULL UI Component!!");
	m_pStr_Premium->SetVisible(false);

	m_pGroup_ServerList = GetChildByID("Group_ServerList_01");	
	__ASSERT(m_pGroup_ServerList, "NULL UI Component!!");
	
	m_pGroup_ServerList->SetVisible(false);

	
	//get List_Server (structure : Group_ServerList_01 -> server_20 -> List_Server )
	for (size_t i = 1; i <= MAX_SERVERS; i++)
	{
		std::string strPath = "server_" + std::to_string(i);
		CN3UIBase* pChild = m_pGroup_ServerList->GetChildByID(strPath);

		std::string strPathImage = "img_arrow" + std::to_string(i);
		CN3UIBase* pChildImage = m_pGroup_ServerList->GetChildByID(strPathImage);

			
		if (!pChild || !pChildImage)
			continue; 
		
		m_pServer_Group[i - 1] = pChild;
		m_pArrow_Group[i - 1] = pChildImage;

	}

	m_pBtn_Connect = (CN3UIButton*)m_pGroup_ServerList->GetChildByID("Btn_Connect");	
	__ASSERT(m_pBtn_Connect, "NULL UI Component!!");
		
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

	if (m_pBtn_Connect) m_pBtn_Connect->SetState(eState1);

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
	if (iIndex >= m_ListServerInfos.size())
		return false;

	GSI = m_ListServerInfos[iIndex];
	return true;
}

bool CUILogIn::ServerInfoGetCur(__GameServerInfo& GSI)
{
	GSI.Init();
	
	if (m_iSelectedServerIndex < 0 || m_iSelectedServerIndex >= m_ListServerInfos.size())
		return false;

	return this->ServerInfoGet(m_iSelectedServerIndex, GSI);
}

void CUILogIn::ServerInfoUpdate()
{
	
	if (!m_ListServerInfos.empty())
	{
		//sort(m_ListServerInfos.begin(), m_ListServerInfos.end(), not2(__GameServerInfo()));
		int iSize = m_ListServerInfos.size();

		//show ui of existing servers
		int iMaxPlayerCount = 3000;
		int iNumUserForLine = 3000 / 12;
		int iNumLines = 1;

		for (size_t i = 0; i < iSize; i++)
		{
			if (m_pServer_Group[i])
			{

				CN3UIString* pStr = (CN3UIString*) (m_pServer_Group[i]->GetChildByID("List_Server"));
				__ASSERT(pStr, "NULL UI Component!!");

				pStr->SetString(m_ListServerInfos[i].szName);
				m_pServer_Group[i]->SetVisible(true); 
				m_pArrow_Group[i]->SetVisible(true);

				//hide number of lines with respect to user number
				iNumLines = m_ListServerInfos[i].iConcurrentUserCount / iNumUserForLine;

				if (iNumLines < 1) //minimum 1 lines
					iNumLines = 1;

				if (iNumLines > 12) //uif file has max 12 lines 
					iNumLines = 12;

				//ids of lines set as 1,2,3 ... 12 in .uif file
				for (size_t j = iNumLines + 1; j <= 12; j++)
					m_pServer_Group[i]->GetChildByID(std::to_string(j))->SetVisible(false);

			}
		}

		//hide ui of extra servers
		for (size_t i = iSize; i < MAX_SERVERS; i++)
		{
			if (m_pServer_Group[i])
			{
				m_pServer_Group[i]->SetVisible(false);
				m_pArrow_Group[i]->SetVisible(false);
			}
		}
	 
		//TODO: Show Premium info if user have premium

	}

	
}

void CUILogIn::Tick()
{
	CN3UIBase::Tick();

	if (m_pGroup_ServerList)
	{
		if (m_bOpenningNow) // slide effect opening, from top to bottom
		{
			TRACE("TEST___");
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

void CUILogIn::AddNoticeText(std::string strNoticeText)
{
	//TODO: needs improvement	

	std::string input = strNoticeText;
	std::string pieces[10];
	int count = 0;

	size_t BOX_START_LEN = 3; // '#', 0x00, '\n'
	size_t BOX_END_LEN = 5;   // '#', 0x00, '\n', 0x00, '\n'

	size_t searchPos = 0;
	while (count + 1 < 10)
	{
		size_t boxStart = input.find("#\0\n", searchPos);
		if (boxStart == std::string::npos)
			break;

		// title is inbetween searchPos and boxStart
		std::string title = input.substr(searchPos, boxStart - searchPos);

		size_t messageStart = boxStart + BOX_START_LEN;
		size_t boxEnd = input.find("#\0\n\0\n", messageStart);
		if (boxEnd == std::string::npos)
			break;

		std::string message = input.substr(messageStart, boxEnd - messageStart);

		pieces[count++] = title;
		pieces[count++] = message;

		// jump to next block
		searchPos = boxEnd + BOX_END_LEN;
	}

	int iReqNoticeBox = (count + 1) / 2;

	if (iReqNoticeBox <= 1)
	{
		CN3UIBase* pNoticeBox = GetChildByID("Group_Notice_1");
		__ASSERT(pNoticeBox, "NULL UI Component!!");

		CN3UIString* pNoticeTextTitle = (CN3UIString*) pNoticeBox->GetChildByID("text_notice_name_01");
		__ASSERT(pNoticeTextTitle, "NULL UI Component!!");

		pNoticeTextTitle->SetString(pieces[0]);

		CN3UIString* pNoticeTextContent = (CN3UIString*) pNoticeBox->GetChildByID("text_notice_01");
		__ASSERT(pNoticeTextContent, "NULL UI Component!!");

		pNoticeTextContent->SetString(pieces[1]);

		pNoticeBox->SetVisible(true);
	}
	else if (iReqNoticeBox == 2)
	{
		CN3UIBase* pNoticeBox = GetChildByID("Group_Notice_2");
		__ASSERT(pNoticeBox, "NULL UI Component!!");

		CN3UIString* pNoticeTextTitle1 = (CN3UIString*) pNoticeBox->GetChildByID("text_notice_name_01");
		__ASSERT(pNoticeTextTitle1, "NULL UI Component!!");

		pNoticeTextTitle1->SetString(pieces[0]);

		CN3UIString* pNoticeTextContent1 = (CN3UIString*) pNoticeBox->GetChildByID("text_notice_01");
		__ASSERT(pNoticeTextContent1, "NULL UI Component!!");

		pNoticeTextContent1->SetString(pieces[1]);

		CN3UIString* pNoticeTextTitle2 = (CN3UIString*) pNoticeBox->GetChildByID("text_notice_name_02");
		__ASSERT(pNoticeTextTitle2, "NULL UI Component!!");
		
		pNoticeTextTitle2->SetString(pieces[2]);

		CN3UIString* pNoticeTextContent2 = (CN3UIString*) pNoticeBox->GetChildByID("text_notice_02");
		__ASSERT(pNoticeTextContent2, "NULL UI Component!!");

		pNoticeTextContent2->SetString(pieces[3]);

		pNoticeBox->SetVisible(true);
	}
	else if (iReqNoticeBox == 3)
	{
		CN3UIBase* pNoticeBox = GetChildByID("Group_Notice_3");
		__ASSERT(pNoticeBox, "NULL UI Component!!");

		CN3UIString* pNoticeTextTitle1 = (CN3UIString*) pNoticeBox->GetChildByID("text_notice_name_01");
		__ASSERT(pNoticeTextTitle1, "NULL UI Component!!");

		pNoticeTextTitle1->SetString(pieces[0]);

		CN3UIString* pNoticeTextContent1 = (CN3UIString*) pNoticeBox->GetChildByID("text_notice_01");
		__ASSERT(pNoticeTextContent1, "NULL UI Component!!");

		pNoticeTextContent1->SetString(pieces[1]);

		CN3UIString* pNoticeTextTitle2 = (CN3UIString*) pNoticeBox->GetChildByID("text_notice_name_02");
		__ASSERT(pNoticeTextTitle2, "NULL UI Component!!");

		pNoticeTextTitle2->SetString(pieces[2]);

		CN3UIString* pNoticeTextContent2 = (CN3UIString*) pNoticeBox->GetChildByID("text_notice_02");
		__ASSERT(pNoticeTextContent2, "NULL UI Component!!");

		pNoticeTextContent2->SetString(pieces[3]);

		CN3UIString* pNoticeTextTitle3 = (CN3UIString*) pNoticeBox->GetChildByID("text_notice_name_03");
		__ASSERT(pNoticeTextTitle3, "NULL UI Component!!");

		pNoticeTextTitle3->SetString(pieces[4]);

		CN3UIString* pNoticeTextContent3 = (CN3UIString*) pNoticeBox->GetChildByID("text_notice_03");
		__ASSERT(pNoticeTextContent3, "NULL UI Component!!");

		pNoticeTextContent3->SetString(pieces[5]);

		pNoticeBox->SetVisible(true);
	}
	else //skip notice page , notice = 0
	{
		m_bNoticeScreen = false;
		OpenServerList();
	}

}

void CUILogIn::OpenNoticePage()
{
	if (m_bNoticeScreen) return;

	//set position of notice boxes
	//if it is done in Load function, they are not centered.
	RECT rc = m_pGroup_Notice_1->GetRegion();
	int iX = ((int) s_CameraData.vp.Width - (rc.right - rc.left)) / 2;
	int iY = ((int) s_CameraData.vp.Height - (rc.bottom - rc.top)) / 2;
	m_pGroup_Notice_1->SetPos(iX, iY);

	rc = m_pGroup_Notice_2->GetRegion();
	iX = ((int) s_CameraData.vp.Width - (rc.right - rc.left)) / 2;
	iY = ((int) s_CameraData.vp.Height - (rc.bottom - rc.top)) / 2;
	m_pGroup_Notice_2->SetPos(iX, iY);

	rc = m_pGroup_Notice_3->GetRegion();
	iX = ((int) s_CameraData.vp.Width - (rc.right - rc.left)) / 2;
	iY = ((int) s_CameraData.vp.Height - (rc.bottom - rc.top)) / 2;
	m_pGroup_Notice_3->SetPos(iX, iY);


	m_bNoticeScreen = true;
}

void CUILogIn::OpenServerList()
{
	if (m_bOpenningNow || m_pGroup_ServerList == NULL) return;

	//close all notice boxes
	m_pGroup_Notice_1->SetVisible(false);
	m_pGroup_Notice_2->SetVisible(false);
	m_pGroup_Notice_3->SetVisible(false);

	// 스르륵 열린다!! = open without sound
	m_pGroup_ServerList->SetVisible(true);
	RECT rc = m_pGroup_ServerList->GetRegion();
	int iX = ((int) s_CameraData.vp.Width - (rc.right - rc.left)) / 2;
	int iY = ((int) s_CameraData.vp.Height - (rc.bottom - rc.top)) / 2;
	m_pGroup_ServerList->SetPos(iX, iY);
	
	GetChildByID("premium")->SetVisible(true);

	m_fMoveDelta = 0;
	m_bOpenningNow = true;
	m_bNoticeScreen = false;
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
		case DIK_TAB:
			FocusCircular();
			return true;
			//		case DIK_NUMPADENTER:
			//		case DIK_RETURN:
			//			CGameProcedure::s_pProcLogIn->MsgSend_AccountLogIn(LIC_KNIGHTONLINE);
			//			return true;
		}
	}
	else if (m_pGroup_ServerList && m_pGroup_ServerList->IsVisible())
	{

		switch (iKey)
		{
		case DIK_UP:
		{
			m_iSelectedServerIndex--;
			
			uint8_t iServerCount = m_ListServerInfos.size();

			if (m_iSelectedServerIndex < 0)
				m_iSelectedServerIndex = iServerCount - 1;

			//change its color 
			CN3UIString* pStrServer = (CN3UIString*) m_pServer_Group[m_iSelectedServerIndex]->GetChildByID("List_Server");
			__ASSERT(pStrServer, "NULL UI Component!!");

			this->ReceiveMessage(pStrServer, UIMSG_STRING_LCLICK);
			
		}
		return true;
		case DIK_DOWN:
		{
			m_iSelectedServerIndex++;

			uint8_t iServerCount = m_ListServerInfos.size();

			if (m_iSelectedServerIndex >= iServerCount)
				m_iSelectedServerIndex = 0;

			//change its color 
			CN3UIString* pStrServer = (CN3UIString*) m_pServer_Group[m_iSelectedServerIndex]->GetChildByID("List_Server");
			__ASSERT(pStrServer, "NULL UI Component!!");

			this->ReceiveMessage(pStrServer, UIMSG_STRING_LCLICK);
		}
		return true;
		case DIK_NUMPADENTER:
		case DIK_RETURN:
		{
		  
			//select the first server if user presses enter at server select screen
			if (m_iSelectedServerIndex == -1)
			{
				m_iSelectedServerIndex = 0;
				
				//change its color 
				CN3UIString* pStrServer = (CN3UIString*) m_pServer_Group[0]->GetChildByID("List_Server");
				__ASSERT(pStrServer, "NULL UI Component!!");

				this->ReceiveMessage(pStrServer, UIMSG_STRING_LCLICK);

				
			}

		}
		return true;
		}
	}
	else if (m_bNoticeScreen)
	{
		switch (iKey)
		{
			case DIK_RETURN:
			  ReceiveMessage(m_pBtn_NoticeOK_1, UIMSG_BUTTON_CLICK);
			return true;
		}
	}

	return CN3UIBase::OnKeyPress(iKey);
}
