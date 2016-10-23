// UIHelp.cpp: implementation of the CUIHelp class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "UIHelp.h"
#include "GameProcedure.h"
#include "UIManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIHelp::CUIHelp()
{
	m_pBtn_Close = NULL;
	m_pBtn_Prev = NULL;
	m_pBtn_Next = NULL;
	for(int i = 0; i < MAX_HELP_PAGE; i++) m_pPages[i] = NULL;
}

CUIHelp::~CUIHelp()
{
}

bool CUIHelp::Load(HANDLE hFile)
{
	if(false == CN3UIBase::Load(hFile)) return false;

	int iPageCount = 0;
	for(int i = 0; i < MAX_HELP_PAGE; i++)
	{
		char szID[32]; sprintf(szID, "Page%d", i);
		m_pPages[i] = this->GetChildByID(szID);
		if(m_pPages[i])
		{
			m_pPages[i]->SetVisible((0 == iPageCount ? true : false));
			iPageCount++;
		}
	}

	m_pBtn_Close = GetChildByID("Btn_Close");
	m_pBtn_Prev = GetChildByID("Btn_Left");
	m_pBtn_Next = GetChildByID("Btn_Right");

	return true;
}

bool CUIHelp::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)					
	{
		int iPage = -1;
		for(int i = 0; i < MAX_HELP_PAGE; i++)
		{
			if(m_pPages[i] && m_pPages[i]->IsVisible())
			{
				iPage = i;
				break;
			}
		}

		int iPagePrev = iPage;
		
		if(pSender == m_pBtn_Prev)
		{
			iPage--;
			if(iPage < 0) iPage = 0;
		}
		else if(pSender == m_pBtn_Next)
		{
			iPage++;
			if(iPage >= MAX_HELP_PAGE) iPage = 0;
		}
		else if(pSender == m_pBtn_Close) this->SetVisible(false);

		if(iPagePrev != iPage)
		{
			for(int i = 0; i < MAX_HELP_PAGE; i++)
			{
				if(NULL == m_pPages[i]) continue;

				m_pPages[i]->SetVisible(false);
				if(i == iPage) m_pPages[i]->SetVisible(true);
			}
		}
	}

	return false;
}

void CUIHelp::Release()
{
	CN3UIBase::Release();

	m_pBtn_Close = NULL;
	m_pBtn_Prev = NULL;
	m_pBtn_Next = NULL;
	for(int i = 0; i < MAX_HELP_PAGE; i++) m_pPages[i] = NULL;
}

bool CUIHelp::OnKeyPress(int iKey)
{
	switch(iKey)
	{
	case SDL_SCANCODE_PAGEUP://DIK_PRIOR:
		ReceiveMessage(m_pBtn_Prev, UIMSG_BUTTON_CLICK);
		return true;
	case SDL_SCANCODE_PAGEDOWN://DIK_NEXT:
		ReceiveMessage(m_pBtn_Next, UIMSG_BUTTON_CLICK);
		return true;
	case SDL_SCANCODE_ESCAPE://DIK_ESCAPE:
		ReceiveMessage(m_pBtn_Close, UIMSG_BUTTON_CLICK);
		return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}

void CUIHelp::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if(bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}
