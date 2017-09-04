// UIStateBar.cpp: implementation of the CUICurtail class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "GameDef.h"
#include "UICurtail.h"
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

CUICurtail::CUICurtail()
{
	m_bOpenningNow = false; // 열리고 있다..
	m_bClosingNow = false;	// 닫히고 있다..
	m_fMoveDelta = 0.0f; // 부드럽게 열리고 닫히게 만들기 위해서 현재위치 계산에 부동소수점을 쓴다..
}

CUICurtail::~CUICurtail()
{
	//SavvyNik - need to make sure to delete any pointers
}

bool CUICurtail::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;

	m_pBtn_cancel = (CN3UIButton*)(this->GetChildByID("btn_cancel"));		__ASSERT(m_pBtn_cancel, "NULL UI Component!!!");

	return true;
}

void CUICurtail::Release()
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

void CUICurtail::Render()
{
	if (false == m_bVisible) return;

	CN3UIBase::Render();
}

void CUICurtail::Tick()
{
	
}


bool CUICurtail::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (NULL == pSender) return false;

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender->m_szID == "btn_cancel")
			SetVisible(false);
	}
	
	return false;
}

bool CUICurtail::OnKeyPress(int iKey)
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

void CUICurtail::Open()
{
	// 스르륵 열린다!!
	SetVisible(true);
	this->SetPos(CN3Base::s_CameraData.vp.Width, 10);
	m_fMoveDelta = 0;
	m_bOpenningNow = true;
	m_bClosingNow = false;

	m_iRBtnDownOffs = -1;
}


void CUICurtail::Close()
{
	//SetVisible(false); 
	RECT rc = this->GetRegion();
	this->SetPos(CN3Base::s_CameraData.vp.Width - (rc.right - rc.left), 10);
	m_fMoveDelta = 0;
	m_bOpenningNow = false;
	m_bClosingNow = true;

	m_iRBtnDownOffs = -1;
}

void CUICurtail::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if (bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}