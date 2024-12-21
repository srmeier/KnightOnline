// DialogPartBase.cpp : implementation file
//

#include "stdafx.h"
#include "N3FXE.h"
#include "DialogPartBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPartBase dialog


CDialogPartBase::CDialogPartBase(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPartBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPartBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogPartBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPartBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPartBase, CDialog)
	//{{AFX_MSG_MAP(CDialogPartBase)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPartBase message handlers
