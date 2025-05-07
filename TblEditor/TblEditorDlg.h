#pragma once

#include <vector>
#include <string>
#include <map>

#include "TblListCtrl.h"

class CTblEditorBase;
class CTblEditorDlg : public CDialogEx
{
// Construction
public:
	CTblEditorDlg(CWnd* pParent = nullptr);	// standard constructor
	~CTblEditorDlg() override;

	CStatic			m_staticText; // status message
	CTblListCtrl	m_ListCtrl;
	CEdit			m_editCell;
	CEdit			m_Edit;

	CTblEditorBase*	m_pTblBase;

	CString			m_strLoadedPath;

	bool			m_bIsFileLoaded;
	bool			m_bIsFileModified;

	// for editing
	int				m_iEditItem;
	int				m_iEditSubItem;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TBLEDITOR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void InsertRows(const std::map<int, std::vector<CString>>& datas);
	void LoadTable(const CString& path);
	bool SaveTable(const CString& savePath, bool bShowConfirmation);

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnAddRow();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnMenuExit();
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
public:
};
