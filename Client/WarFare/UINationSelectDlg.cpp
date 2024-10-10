// UINationSelectDlg.cpp: implementation of the CUINationSelectDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UINationSelectDlg.h"
#include "GameProcNationSelect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUINationSelectDlg::CUINationSelectDlg()
{
	m_pProcNationSelectRef = NULL;
}

CUINationSelectDlg::~CUINationSelectDlg()
{
	m_pBtnKarus = NULL;
	m_pBtnElmorad = NULL;
	m_pBtnBack = NULL;
}

bool CUINationSelectDlg::Load(HANDLE hFile)
{
	bool bSuccess = CN3UIBase::Load(hFile);

	m_pBtnKarus = this->GetChildByID("btn_karus_selection");	__ASSERT(m_pBtnKarus, "NULL UI Component!!");
	m_pBtnElmorad = this->GetChildByID("btn_elmo_selection");	__ASSERT(m_pBtnElmorad, "NULL UI Component!!"); // 
	m_pBtnBack = this->GetChildByID("btn_back");	__ASSERT(m_pBtnElmorad, "NULL UI Component!!"); // 
	RECT rc = this->GetRegion();
	int iX = ((int)s_CameraData.vp.Width - (rc.right - rc.left))/2;
	int iY = ((int)s_CameraData.vp.Height - (rc.bottom - rc.top))/2;
	this->SetPos(iX, iY);

	return bSuccess;
}

bool CUINationSelectDlg::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if(NULL == pSender) return false;

	if( dwMsg == UIMSG_BUTTON_CLICK )
	{
		if ( pSender == m_pBtnKarus )	// Karus
		{
			if(m_pProcNationSelectRef) m_pProcNationSelectRef->MsgSendNationSelect(NATION_KARUS);
		}
		else
		if ( pSender == m_pBtnElmorad )	// Elmorad
		{
			if(m_pProcNationSelectRef) m_pProcNationSelectRef->MsgSendNationSelect(NATION_ELMORAD);
		}
		else
		if ( pSender == m_pBtnBack ) // Back
		{
			CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcLogIn); // 캐릭터 선택 프로시저로 한다..
		}
	}

	return true;
}

