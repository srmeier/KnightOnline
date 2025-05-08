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

	int				m_iStringCodePage;

	// for editing
	int				m_iEditItem;
	int				m_iEditSubItem;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TBLEDITOR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void RefreshTable();
	CString DecodeField(const CStringA& fieldA, int iColNo);
	void LoadTable(const CString& path);
	void BuildTableForSave(std::map<int, std::vector<CStringA>>& newRows);
	bool SaveTable(const CString& savePath, bool bShowConfirmation);
	void SetCodePage(int codepage);

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
	afx_msg void OnFileSaveAs();
	afx_msg void OnSetEncoding_Korean();
	afx_msg void OnSetEncoding_EnglishUS();
	afx_msg void OnSetEncoding_Turkish();
	afx_msg void OnMenuExit();
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
};
