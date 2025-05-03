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

	CStatic m_staticText; //bilgi mesajı
	CListCtrl m_ListCtrl; //list
	CEdit m_editCell;
	CButton m_btnOk;
	CButton m_btnSave;

	CTblEditorBase* m_pTblBase;

	afx_msg void OnFileOpen();

	//for editting
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

	CEdit m_Edit;

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

	afx_msg void OnBnClickedSaveButton();
	afx_msg void OnBnClickedBtnAddRow();

	DECLARE_MESSAGE_MAP()
public:
};
