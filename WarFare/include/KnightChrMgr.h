// KnightChrMgr.h: interface for the CKnightChrMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KNIGHTCHRMGR_H__00D0C1D6_95E8_44A5_BE2B_C8A670F5A9B8__INCLUDED_)
#define AFX_KNIGHTCHRMGR_H__00D0C1D6_95E8_44A5_BE2B_C8A670F5A9B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SharedMem.h"

class CKnightChrMgr  
{
	char	m_sIDAndPW[32];
	int		m_iActionNo;

public:
	CKnightChrMgr(HWND hWnd);
	virtual ~CKnightChrMgr();

public:
	CSharedMemQueue	Smq;

	void RCommand(char rbuf[], int leng);
	void TCommand(char);

	LONG OnReceiveSmq(UINT WParam, LONG LParam);

	void SendActionCommand(int iAc);
};

#endif // !defined(AFX_KNIGHTCHRMGR_H__00D0C1D6_95E8_44A5_BE2B_C8A670F5A9B8__INCLUDED_)
