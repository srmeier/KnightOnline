// UILoadingDlg.h: interface for the UILoadingDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UILOADINGDLG_H__81E8BA13_2261_4A3D_9A94_BF0E7C49C4DD__INCLUDED_)
#define AFX_UILOADINGDLG_H__81E8BA13_2261_4A3D_9A94_BF0E7C49C4DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"

class CN3UIProgress;

class CUILoadingDlg     : public CN3UIBase  
{
public:
	bool Load(HANDLE hFile);
	CUILoadingDlg();
	virtual ~CUILoadingDlg();

	virtual void	Release();

	CN3UIProgress* GetProcCtrl(char* );
};

#endif // !defined(AFX_UILOADINGDLG_H__81E8BA13_2261_4A3D_9A94_BF0E7C49C4DD__INCLUDED_)
