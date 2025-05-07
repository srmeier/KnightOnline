#pragma once

#include <vector>
#include <string>
#include <map>

class CTblEditorBase;
class CTblEditorDlg : public CDialogEx
{
// Construction
public:
	CTblEditorDlg(CWnd* pParent = nullptr);	// standard constructor
	~CTblEditorDlg() override;

	CStatic		m_staticText; // status message
	CListCtrl	m_ListCtrl;
	CEdit		m_editCell;
	CEdit		m_Edit;

	CTblEditorBase* m_pTblBase;

	// for editing
	int m_iEditItem;
	int m_iEditSubItem;


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TBLEDITOR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	std::vector<std::vector<CString>> m_OriginalData;
	void SetupColumns(const std::vector<CString>& columnNames);
	void InsertRows(const std::map<int, std::vector<CString>>& datas);
	void LoadTable(const CString& path);

// Implementation
protected:
	HICON m_hIcon;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnNMDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnKillfocusEditCell();
	afx_msg void OnBnClickedOkButton();

	afx_msg void OnBnClickedBtnAddRow();

	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnExit();

	DECLARE_MESSAGE_MAP()
public:
};
