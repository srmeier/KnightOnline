#pragma once

#include <vector>
#include <string>
#include <map>
// CTblEditorDlg dialog
class CTblEditorDlg : public CDialogEx
{
// Construction
public:
	CTblEditorDlg(CWnd* pParent = nullptr);	// standard constructor
	
	CStatic m_staticText; //bilgi mesajı
	CListCtrl m_ListCtrl; //list
	CEdit m_editCell;
	CButton m_btnOk;
	CButton m_btnSave;


	afx_msg void OnFileOpen();
	
	//for editting
	int m_iEditItem;
	int m_iEditSubItem;
	

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TBLEDİTOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	std::vector<std::vector<CString>> m_OriginalData;
	void SetupColumns(const std::vector<CString>& columnNames);
	void InsertRows(const std::map<int, std::vector<std::string>>& datas);
	void LoadTable(const CString& path);
	enum class TableType
	{
		None,
		Texts,
		Zones,
		// İstersen başka tablolar ekleyebilirsin
	};
	TableType m_eCurrentTableType = TableType::None;
	
	CEdit m_Edit;

	enum DATA_TYPE { DT_NONE, DT_CHAR, DT_BYTE, DT_SHORT, DT_WORD, DT_INT, DT_DWORD, DT_STRING, DT_FLOAT, DT_DOUBLE };


// Implementation
protected:
	HICON m_hIcon;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	afx_msg void OnNMDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnKillfocusEditCell();
	afx_msg void OnBnClickedOkButton();

	afx_msg void OnBnClickedSaveButton();

	afx_msg void CTblEditorDlg::OnBnClickedBtnAddRow();
	void CTblEditorDlg::OnNMDblclkList(NMHDR* pNMHDR, LRESULT* pResult);


	DECLARE_MESSAGE_MAP()
public:
};
