// UIQuestMenu.cpp: implementation of the CUIQuestMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameDef.h"
#include "GameBase.h"
#include "N3UIString.h"
#include "N3UIImage.h"
#include "N3UITooltip.h"
#include "GameProcedure.h"
#include "UIQuestMenu.h"
#include "UIManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIQuestMenu::CUIQuestMenu()
{
	m_pTextTitle	= NULL;
	m_pTextSample	= NULL;
	m_pImageLeft	= NULL;
	m_pImageCenter	= NULL;
	m_pImageRight	= NULL;
	m_pImageBLeft	= NULL;
	m_pImageBCenter	= NULL;
	m_pImageBRight	= NULL;

	m_iMenuCnt		= 0;

	for(int i=0;i<MAX_STRING_MENU;i++)
	{
		m_pTextMenu[i]   = NULL;
	}
	
}

CUIQuestMenu::~CUIQuestMenu()
{
	InitBase();
}

void CUIQuestMenu::InitBase()
{
	m_iMenuCnt = 0;

	for(int i=1;i<MAX_STRING_MENU;i++)
	{
		if(m_pTextMenu[i]) delete m_pTextMenu[i]; m_pTextMenu[i] = NULL;
	}
}

bool CUIQuestMenu::Load(HANDLE hFile)
{
	if(CN3UIBase::Load(hFile)==false) return false;

	m_pTextSample	= (CN3UIString*)(this->GetChildByID("Text_Menu"));	__ASSERT(m_pTextSample, "NULL UI Component!!!");
	m_pTextTitle	= (CN3UIString*)(this->GetChildByID("Text_Title"));	__ASSERT(m_pTextTitle, "NULL UI Component!!!");

	m_pImageLeft	= (CN3UIImage*)(this->GetChildByID("img_lmenu"));	__ASSERT(m_pImageLeft	, "NULL UI Component!!!");
	m_pImageCenter	= (CN3UIImage*)(this->GetChildByID("img_cmenu"));	__ASSERT(m_pImageCenter	, "NULL UI Component!!!");
	m_pImageRight	= (CN3UIImage*)(this->GetChildByID("img_rmenu"));	__ASSERT(m_pImageRight	, "NULL UI Component!!!");
	m_pImageBLeft	= (CN3UIImage*)(this->GetChildByID("img_lBottom"));	__ASSERT(m_pImageBLeft	, "NULL UI Component!!!");
	m_pImageBCenter	= (CN3UIImage*)(this->GetChildByID("img_cBottom"));	__ASSERT(m_pImageBCenter, "NULL UI Component!!!");
	m_pImageBRight	= (CN3UIImage*)(this->GetChildByID("img_rBottom"));	__ASSERT(m_pImageBRight	, "NULL UI Component!!!");

	if(m_pImageLeft)
		m_rcLImage = m_pImageLeft->GetRegion();
	if(m_pImageRight)
		m_rcRImage = m_pImageRight->GetRegion();
	if(m_pImageCenter)
		m_rcCImage = m_pImageCenter->GetRegion();

	return true;
}

void CUIQuestMenu::Render()
{
	if (!m_bVisible) return;	// 보이지 않으면 자식들을 render하지 않는다.

	for(UIListReverseItor itor = m_ReSizeChildren.rbegin(); m_ReSizeChildren.rend() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		pChild->Render();

		CN3UIBase* pCUI = NULL;
		pCUI = pChild->m_pChildUI;
		while(pCUI)
		{
			pCUI->Render();
			pCUI = pCUI->m_pChildUI;
		}
	}
}

DWORD CUIQuestMenu::MouseProc(DWORD dwFlags, const POINT &ptCur, const POINT &ptOld)
{
	DWORD dwRet = UI_MOUSEPROC_NONE;
	if (!m_bVisible) return dwRet;

	// UI 움직이는 코드
	if (UI_STATE_COMMON_MOVE == m_eState)
	{
		if (dwFlags&UI_MOUSE_LBCLICKED)
		{
			SetState(UI_STATE_COMMON_NONE);
		}
		else
		{
			MoveOffset(ptCur.x - ptOld.x, ptCur.y - ptOld.y);
		}
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	if(false == IsIn(ptCur.x, ptCur.y))	// 영역 밖이면
	{
		if(false == IsIn(ptOld.x, ptOld.y))
		{
			return dwRet;// 이전 좌표도 영역 밖이면 
		}
		dwRet |= UI_MOUSEPROC_PREVINREGION;	// 이전 좌표는 영역 안이었다.
	}
	else
	{
		// tool tip 관련
		if (s_pTooltipCtrl) s_pTooltipCtrl->SetText(m_szToolTip);
	}
	dwRet |= UI_MOUSEPROC_INREGION;	// 이번 좌표는 영역 안이다.

	if(m_pChildUI && m_pChildUI->IsVisible())
		return dwRet;

	// child에게 메세지 전달
	for(UIListItor itor = m_ReSizeChildren.begin(); m_ReSizeChildren.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		DWORD dwChildRet = 0;

		dwChildRet = pChild->MouseProc(dwFlags, ptCur, ptOld);
		if( pChild->IsVisible() && UI_TYPE_STRING == pChild->UIType() )
		{
			if(pChild->IsIn(ptCur.x, ptCur.y) && (dwFlags & UI_MOUSE_LBCLICKED) )	
			{
				dwChildRet |= UI_MOUSEPROC_DONESOMETHING;
			}

			if(pChild->IsIn(ptCur.x, ptCur.y) && (dwFlags & UI_MOUSE_LBDBLCLK) )	
			{
				dwChildRet |= UI_MOUSEPROC_DONESOMETHING;
			}
		}

		if (UI_MOUSEPROC_DONESOMETHING & dwChildRet)
		{	// 이경우에는 먼가 포커스를 받은 경우이다.
			// (아래 코드는 dialog를 관리하는 곳에서 해야 한다. 따라서 막아놓음)
//			m_Children.erase(itor);			// 우선 리스트에서 지우고
//			m_Children.push_front(pChild);	// 맨앞에 넣는다. 그리는 순서를 맨 나중에 그리도록 하려고

			dwRet |= (UI_MOUSEPROC_CHILDDONESOMETHING|UI_MOUSEPROC_DONESOMETHING);
			return dwRet;
		}
	}

	// UI 움직이는 코드
	if (UI_STATE_COMMON_MOVE != m_eState && 
			PtInRect(&m_rcMovable, ptCur) && (dwFlags&UI_MOUSE_LBCLICK) )
	{
		SetState(UI_STATE_COMMON_MOVE);
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	return dwRet;
}

bool CUIQuestMenu::ReceiveMessage(CN3UIBase *pSender, DWORD dwMsg)
{
	if( dwMsg == UIMSG_STRING_LCLICK )
	{
		for(int i=0;i<MAX_STRING_MENU;i++)
		{
			if(pSender == m_pTextMenu[i])
			{
				MsgSend_SelectMenu(i);
				SetVisible(false);
				return true;
			}
		}
	}

	return true;
}

BOOL CUIQuestMenu::MoveOffset(int iOffsetX, int iOffsetY)
{
	if (0 == iOffsetX && 0 == iOffsetY) return FALSE;

	m_rcLImage.left += iOffsetX;		m_rcLImage.top += iOffsetY;
	m_rcLImage.right += iOffsetX;		m_rcLImage.bottom += iOffsetY;

	m_rcRImage.left += iOffsetX;		m_rcRImage.top += iOffsetY;
	m_rcRImage.right += iOffsetX;		m_rcRImage.bottom += iOffsetY;

	m_rcCImage.left += iOffsetX;		m_rcCImage.top += iOffsetY;
	m_rcCImage.right += iOffsetX;		m_rcCImage.bottom += iOffsetY;

	// ui 영역
	m_rcRegion.left += iOffsetX;		m_rcRegion.top += iOffsetY;
	m_rcRegion.right += iOffsetX;		m_rcRegion.bottom += iOffsetY;

	// movable 영역
	if(	m_rcMovable.right - m_rcMovable.left != 0 &&
		m_rcMovable.bottom - m_rcMovable.top != 0 )
	{
		m_rcMovable.left += iOffsetX;		m_rcMovable.top += iOffsetY;
		m_rcMovable.right += iOffsetX;		m_rcMovable.bottom += iOffsetY;
	}

	// children 좌표 갱신
	CN3UIBase* pCUI = NULL; // Child UI...
	for(UIListItor itor = m_ReSizeChildren.begin(); m_ReSizeChildren.end() != itor; ++itor)
	{
		pCUI = (*itor);
		__ASSERT(pCUI, "child UI pointer is NULL!");
		pCUI->MoveOffset(iOffsetX, iOffsetY);
	}
	return TRUE;
}

void CUIQuestMenu::MsgSend_SelectMenu(int index)
{
	BYTE byBuff[10];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_QUEST_SELECT);
	CAPISocket::MP_AddDword(byBuff, iOffset, index);
	CGameProcedure::s_pSocket->Send(byBuff, iOffset);
}

void CUIQuestMenu::Open(DataPack *pDataPack, int &iOffset)
{
	InitBase();

	if(m_pTextSample == NULL) return;
	if(m_pImageCenter == NULL || m_pImageLeft == NULL || m_pImageRight == NULL)	return;
	if(m_pImageBRight == NULL || m_pImageBLeft == NULL || m_pImageBCenter == NULL) return;


	std::string szTitle;
	std::string szMenu[MAX_STRING_MENU];

	int iNpcID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	int index = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);

	__TABLE_QUEST_TALK* pTbl_Quest_Talk = CGameBase::s_pTbl_QuestTalk->Find(index);
	if(pTbl_Quest_Talk == NULL) return;

	szTitle = pTbl_Quest_Talk->szTalk;
	m_pTextTitle->SetString(szTitle);

	m_iMenuCnt = 0;

	for(int j=0;j<MAX_STRING_MENU;j++)
	{
		int iMenu = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		if( iMenu >= 0 )
		{
			__TABLE_QUEST_MENU* pTbl_Quest_Menu = CGameBase::s_pTbl_QuestMenu->Find(iMenu);
			if(pTbl_Quest_Menu)
			{
				szMenu[j] = pTbl_Quest_Menu->szMenu;
			}

			m_iMenuCnt++;
		}
	}

	if(m_iMenuCnt==0) return;

	SetVisible(true);

	m_pImageLeft->SetRegion(m_rcLImage);
	m_pImageRight->SetRegion(m_rcRImage);
	m_pImageCenter->SetRegion(m_rcCImage);

	m_ReSizeChildren.clear();
	for(UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		m_ReSizeChildren.push_back(pChild);
	}

	int iTW, iTH, iIW, iIH;
	RECT rcImage, rcText, rcTemp;
	rcImage = m_pImageCenter->GetRegion();
	rcText	= m_pTextSample->GetRegion();

	iTW = rcText.right - rcText.left;
	iTH = rcText.bottom - rcText.top;
	iIW = rcImage.right - rcImage.left;
	iIH = rcImage.bottom - rcImage.top;

	if(m_iMenuCnt>0)
	{
		m_pTextMenu[0] = m_pTextSample;
		m_pTextMenu[0]->SetString(szMenu[0]);
	}

	//string 생성
	for(int i=1;i<m_iMenuCnt;i++)
	{
		m_pTextMenu[i] = new CN3UIString;
		__ASSERT(m_pTextMenu[i], "NULL UI Component!!!");

		if(m_pTextMenu[i])
		{
			*(m_pTextMenu[i]) = *m_pTextSample;
			m_pTextMenu[i]->SetParent(this);
			m_pTextMenu[i]->SetString(szMenu[i]);

			rcText.top = rcImage.top + i*iIH;
			rcText.bottom = rcText.top + iTH;

			m_pTextMenu[i]->SetRegion(rcText);
			m_ReSizeChildren.push_front(m_pTextMenu[i]);
		}
	}

	//ui resize
	rcText = m_pTextMenu[m_iMenuCnt-1]->GetRegion();

	rcImage = m_pImageRight->GetRegion();
	rcImage.bottom = rcText.bottom;
	m_pImageRight->SetRegion(rcImage);

	rcImage = m_pImageLeft->GetRegion();
	rcImage.bottom = rcText.bottom;
	m_pImageLeft->SetRegion(rcImage);

	rcImage = m_pImageCenter->GetRegion();
	rcImage.bottom = rcText.bottom;
	m_pImageCenter->SetRegion(rcImage);

	rcTemp = m_pImageBRight->GetRegion();
	iIH = rcTemp.bottom - rcTemp.top;
	rcTemp.top = rcText.bottom;
	rcTemp.bottom = rcTemp.top + iIH;
	m_pImageBRight->SetRegion(rcTemp);

	rcTemp = m_pImageBLeft->GetRegion();
	iIH = rcTemp.bottom - rcTemp.top;
	rcTemp.top = rcText.bottom;
	rcTemp.bottom = rcTemp.top + iIH;
	m_pImageBLeft->SetRegion(rcTemp);

	rcTemp = m_pImageBCenter->GetRegion();
	iIH = rcTemp.bottom - rcTemp.top;
	rcTemp.top = rcText.bottom;
	rcTemp.bottom = rcTemp.top + iIH;
	m_pImageBCenter->SetRegion(rcTemp);


	//이건 기본 유아이의 영역을 다시 설정해줌...
	const int iWidth = CN3Base::s_CameraData.vp.Width;
	const int iHeight = CN3Base::s_CameraData.vp.Height;
	int iX = 0, iY = 0;

	rcText = GetRegion();
	rcText.bottom = rcTemp.bottom;
	SetRegion(rcText);
	iX = (iWidth - (rcText.right - rcText.left))/2;
	iY = (iHeight - (rcText.bottom - rcText.top))/2;
	SetPos(iX, iY);
}

bool CUIQuestMenu::OnKeyPress(int iKey)
{
	switch(iKey)
	{
	case DIK_ESCAPE:
		SetVisible(false);
		return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}

void CUIQuestMenu::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if(bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}
