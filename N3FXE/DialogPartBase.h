#if !defined(AFX_DIALOGPARTBASE_H__A53F3A27_A0F5_447D_A923_E921D1DD5148__INCLUDED_)
#define AFX_DIALOGPARTBASE_H__A53F3A27_A0F5_447D_A923_E921D1DD5148__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogPartBase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPartBase dialog

class CDialogPartBase : public CDialog
{
// Construction
public:
	CDialogPartBase(CWnd* pParent = NULL);   // standard constructor

	class CMainFrame*			m_pRefFrm;
	class CDlgEditScript*		m_pRefBundle;

// Dialog Data
	//{{AFX_DATA(CDialogPartBase)
	enum { IDD = IDR_MAINFRAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPartBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogPartBase)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPARTBASE_H__A53F3A27_A0F5_447D_A923_E921D1DD5148__INCLUDED_)
