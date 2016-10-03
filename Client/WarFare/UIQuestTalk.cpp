// UIQuestTalk.cpp: implementation of the CUIQuestTalk class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "GameDef.h"
#include "GameBase.h"
#include "N3UIString.h"
#include "N3UIButton.h"
#include "GameProcedure.h"
#include "UIQuestTalk.h"
#include "UIManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIQuestTalk::CUIQuestTalk()
{
	m_pTextTalk	= NULL;
	m_pBtnOk	= NULL;

	m_iNumTalk	= 0;
	m_iCurTalk	= 0;
}

CUIQuestTalk::~CUIQuestTalk()
{
}

void CUIQuestTalk::Open(DataPack *pDataPack, int &iOffset)
{
	m_iNumTalk = 0;
	m_iCurTalk = 0;

	// NOTE(srmeier): two -1s before text ids
	int index = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
	index = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);

	for(int i=0;i<MAX_STRING_TALK;i++)
	{
		m_szTalk[i] = "";

		index = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		__TABLE_QUEST_TALK* pTbl_Quest_Talk = CGameBase::s_pTbl_QuestTalk->Find(index);
		if(pTbl_Quest_Talk)
		{
			m_szTalk[i] = pTbl_Quest_Talk->szTalk;
			m_iNumTalk++;
		}
	}

	m_pTextTalk->SetString(m_szTalk[m_iCurTalk]);
	SetVisible(true);
}

bool CUIQuestTalk::ReceiveMessage(CN3UIBase *pSender, DWORD dwMsg)
{
	if( dwMsg == UIMSG_BUTTON_CLICK )
	{
		if(pSender == m_pBtnOk)
		{
			m_iCurTalk++;
			if(m_iCurTalk>=m_iNumTalk)
			{
				m_iCurTalk = 0;
				SetVisible(false);
			}
			else
			{
				m_pTextTalk->SetString(m_szTalk[m_iCurTalk]);
			}
		}
	}

	return true;
}

bool CUIQuestTalk::Load(HANDLE hFile)
{
	if(CN3UIBase::Load(hFile)==false) return false;

	m_pTextTalk	= (CN3UIString*)(this->GetChildByID("Text_Talk"));	__ASSERT(m_pTextTalk, "NULL UI Component!!!");
//	m_pBtnOk	= (CN3UIButton*)(this->GetChildByID("Btn_Ok"));		__ASSERT(m_pBtnOk, "NULL UI Component!!!");
	m_pBtnOk	= (CN3UIButton*)(this->GetChildByID("btn_Ok_center"));		__ASSERT(m_pBtnOk, "NULL UI Component!!!");

	// NOTE(srmeier): new stuff
	CN3UIButton* m_pBtnClose = (CN3UIButton*)(this->GetChildByID("btn_close"));
	if (m_pBtnClose) {
		m_pBtnClose->SetVisible(false);
	}

	CN3UIButton* m_pBtnUpperEvent = (CN3UIButton*)(this->GetChildByID("btn_UpperEvent"));
	if (m_pBtnUpperEvent) {
		m_pBtnUpperEvent->SetVisible(false);
	}

	CN3UIButton* m_pBtnNext = (CN3UIButton*)(this->GetChildByID("btn_Next"));
	if (m_pBtnNext) {
		m_pBtnNext->SetVisible(false);
	}

	CN3UIButton* m_pBtnOkRight = (CN3UIButton*)(this->GetChildByID("btn_Ok_right"));
	if (m_pBtnOkRight) {
		m_pBtnOkRight->SetVisible(false);
	}

	CN3UIButton* m_pBtnPre = (CN3UIButton*)(this->GetChildByID("btn_Pre"));
	if (m_pBtnPre) {
		m_pBtnPre->SetVisible(false);
	}

	return true;
}

bool CUIQuestTalk::OnKeyPress(int iKey)
{
	switch(iKey)
	{
	case DIK_ESCAPE:
		SetVisible(false);
		return true;
	case DIK_RETURN:
		ReceiveMessage(m_pBtnOk, UIMSG_BUTTON_CLICK);
		return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}

void CUIQuestTalk::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if(bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}
