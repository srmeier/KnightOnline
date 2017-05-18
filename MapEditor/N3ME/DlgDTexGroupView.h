#if !defined(AFX_DLGDTEXGROUPVIEW_H__F3662140_F843_4AEF_9FA4_9812A085ED8B__INCLUDED_)
#define AFX_DLGDTEXGROUPVIEW_H__F3662140_F843_4AEF_9FA4_9812A085ED8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDTexGroupView.h : header file
//
class CDTexGroupMng;
class CDTexGroup;
class CN3Texture;
/////////////////////////////////////////////////////////////////////////////
// CDlgDTexGroupView dialog

class CDlgDTexGroupView : public CDialog
{
// Construction
public:
	float					m_fTexSurfaceSize;
	CN3Texture*				m_pPrevTex;
	RECT					m_PrevTexRect;

	int						m_iRadioState; //	0 ÀÚµ¿Âï±â 2 °­Á¦Âï±â 3 °­Á¦Ç®Å¸ÀÏ Âï±â

public:
	CDTexGroupMng* m_pDTexGroupMng;

	void SetGroupMng(CDTexGroupMng* pDTexGroupMng) { m_pDTexGroupMng = pDTexGroupMng; }
	void AddGroup(CDTexGroup* pGroup);
	void ResetAll();

	CDlgDTexGroupView(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDTexGroupView)
	enum { IDD = IDD_DTEX_GROUP };
	CSliderCtrl	m_SliderBrushSize;
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDTexGroupView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDTexGroupView)
	afx_msg void OnSelchangedTreeDtexGroup(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTileauto();
	afx_msg void OnTilemanuel();
	afx_msg void OnTilemanuelNomix();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDTEXGROUPVIEW_H__F3662140_F843_4AEF_9FA4_9812A085ED8B__INCLUDED_)
