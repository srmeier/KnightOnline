// KnightChrMgr.cpp: implementation of the CKnightChrMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KnightChrMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKnightChrMgr::CKnightChrMgr(HWND hWnd)
{
#ifdef _N3_KNIGHT_CHR
	Smq.CreateSmq(TRUE);
//	Smq.CreateSmq(FALSE);
	Smq.SetHwnd(hWnd);	
	if (Smq.IsPartner()) TCommand('R');
#endif
}

CKnightChrMgr::~CKnightChrMgr()
{
#ifdef _N3_KNIGHT_CHR
	TCommand('D');
#endif
}


//////////////////////////////////////////////////////////////////////

void CKnightChrMgr::RCommand(char rbuf[], int leng) 
{
#ifdef _N3_KNIGHT_CHR
	switch(rbuf[0]) {
		case 'C':	
//			m_pStatusView.AddString(rbuf);
			memcpy(m_sIDAndPW, rbuf, sizeof(char)*32);
			TCommand('c');
			break;
	}
#endif
}


//////////////////////////////////////////////////////////////////////

void CKnightChrMgr::TCommand(char Cmd) 
{
#ifdef _N3_KNIGHT_CHR
	char	Tbuf[100];
	switch(Cmd) {
		case 'R':	Tbuf[0] = Cmd; Tbuf[1] = 0;
					Smq.PutData(Tbuf, 1);
					break;

		case 'c':	Tbuf[0] = Cmd; Tbuf[1] = 1;
					Smq.PutData(Tbuf, 2);
					break;

		case 'A':	
					sprintf(Tbuf, "A%c", m_iActionNo);
					Smq.PutData(Tbuf, 2);
					break;

		case 'D':	sprintf(Tbuf, "D-Game Inform");
					Smq.PutData(Tbuf, strlen(Tbuf));
					break;
	}
#endif
}


//////////////////////////////////////////////////////////////////////

LONG CKnightChrMgr::OnReceiveSmq(UINT WParam, LONG LParam)
{
#ifdef _N3_KNIGHT_CHR
//	UpdateData(TRUE);
	int		count = WParam;
	char rData[4096];
	Smq.GetReadData(rData,count);
	rData[count] = 0;
	RCommand(rData,count);
#endif
	return 1;
}

void CKnightChrMgr::SendActionCommand(int iAc)
{
#ifdef _N3_KNIGHT_CHR
	m_iActionNo = iAc;
	TCommand('A');
#endif
}

