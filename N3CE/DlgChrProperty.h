#if !defined(AFX_DLGCHRPROPERTY_H__44297C60_64E5_4263_9814_AEA9AF4BC07A__INCLUDED_)
#define AFX_DLGCHRPROPERTY_H__44297C60_64E5_4263_9814_AEA9AF4BC07A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChrProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgChrProperty dialog
#include "PropertyList.h"
#include "Resource.h"

class CDlgChrProperty : public CDialog
{
// Construction
public:
	void UpdateInfo();
	CDlgChrProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgChrProperty)
	enum { IDD = IDD_CHR_PROPERTY };
	CComboBox	m_CBChrPart; // Character Part Number
	CComboBox	m_CBChrPlug; // Chraracter Plug Number
	CPropertyList	m_LPMaterial;
	CPropertyList	m_LPChr;
	CPropertyList	m_LPCPart;
	CPropertyList	m_LPCPlug;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgChrProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgChrProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCbPartShape();
	afx_msg void OnSelchangeCbPartCharacter();
	afx_msg void OnSelchangeCbPlugCharacter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHRPROPERTY_H__44297C60_64E5_4263_9814_AEA9AF4BC07A__INCLUDED_)
