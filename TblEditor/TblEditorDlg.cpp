
// TblEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "framework.h"
#include "TblEditor.h"
#include "TblEditorDlg.h"
#include "afxdialogex.h"
#include "Resource.h"
#include "TblEditorBase.h"

#ifdef min
#undef min
#endif

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_COLUMN_WIDTH 300  // En fazla 300 piksel genişliğe izin ver

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTblEditorDlg dialog



CTblEditorDlg::CTblEditorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TBLEDITOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTblEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl); // IDC_LIST1 ile m_ListCtrl'ü bağlar
	DDX_Control(pDX, IDC_EDIT1, m_editCell);
	DDX_Control(pDX, IDC_BUTTON1, m_btnOk); // Change button
	DDX_Control(pDX, IDC_BUTTON2, m_btnSave); // OK butonunu bağla

}

BEGIN_MESSAGE_MAP(CTblEditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, &CTblEditorDlg::OnFileOpen)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CTblEditorDlg::OnNMDblclkListCtrl)  // Double click to select list item for editting
	ON_EN_KILLFOCUS(IDC_EDIT1, &CTblEditorDlg::OnEnKillfocusEditCell)  // EditBox Killfocus
	ON_BN_CLICKED(IDC_BUTTON1, &CTblEditorDlg::OnBnClickedOkButton) // Button message
	ON_BN_CLICKED(IDC_BUTTON2, &CTblEditorDlg::OnBnClickedSaveButton) // Button message
	ON_BN_CLICKED(IDC_BUTTON3, &CTblEditorDlg::OnBnClickedBtnAddRow) // Editting row
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CTblEditorDlg::OnNMDblclkList) //Double click to edit
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTblEditorDlg message handlers

BOOL CTblEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ListCtrl stil ayarları
	m_ListCtrl.SetExtendedStyle(
		m_ListCtrl.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_SUBITEMIMAGES |
		LVS_EX_ONECLICKACTIVATE
	);

	


	// Load Menu
	CMenu* pMenu = new CMenu();
	if (pMenu->LoadMenu(IDR_MENU1)) // IDR_MENU1
	{
		SetMenu(pMenu);
	}

	m_staticText.SubclassDlgItem(IDC_STATIC, this);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{

		pSysMenu->AppendMenu(MF_SEPARATOR);

		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}



	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	
	


	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CTblEditorDlg::OnFileOpen()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST,
		_T("Tbl Files (*.tbl)|*.tbl|All Files (*.*)|*.*||"));


	if (dlg.DoModal() == IDOK)
	{
		//get file full path
		CString path = dlg.GetPathName();

		//get file extension
		CString extension = dlg.GetFileExt();
		extension.MakeLower();

		if (extension != _T("tbl"))
		{
			AfxMessageBox(IDS_ERROR_WRONG_FILE_FORMAT, MB_ICONERROR);
			return;
		}

		CTblEditorDlg::LoadTable(path);

	}

}

void CTblEditorDlg::InsertRows(const std::map<int, std::vector<std::string>>& datas) {
	m_ListCtrl.DeleteAllItems();
	int nColumnCount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();
	for (int i = nColumnCount - 1; i >= 0; --i)
		m_ListCtrl.DeleteColumn(i);

	size_t columnCount = CTblEditorBase::memory_DataTypes.size();
	std::vector<int> columnWidths(columnCount, 0); // Her sütun için genişlik

	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(m_ListCtrl.GetFont());

	// Sütun başlıklarını ekle ve ilk genişliği ölç
	for (size_t i = 0; i < columnCount; ++i) {
		CString headerText;
		headerText.Format(_T("Column %d"), static_cast<int>(i + 1));
		CSize size = dc.GetTextExtent(headerText);
		columnWidths[i] = size.cx + 20;  // +20 padding

		m_ListCtrl.InsertColumn(static_cast<int>(i), headerText, LVCFMT_LEFT, 100); // şimdilik 100
	}

	// Satırları ekle ve içerikleri ölç
	for (const auto& it : datas) {
		int rowIndex = it.first;
		const std::vector<std::string>& columns = it.second;

		CString firstCol = columns.empty() ? _T("") : CString(columns[0].c_str());
		int nItem = m_ListCtrl.InsertItem(rowIndex, firstCol);

		CSize size = dc.GetTextExtent(firstCol);
		if (size.cx + 10 > columnWidths[0])
			columnWidths[0] = std::min<int>(size.cx + 10, static_cast<int>(MAX_COLUMN_WIDTH));

		for (size_t i = 1; i < columns.size() && i < columnCount; ++i) {
			CString text(columns[i].c_str());
			m_ListCtrl.SetItemText(nItem, static_cast<int>(i), text);

			CSize size = dc.GetTextExtent(text);
			if (size.cx + 10 > columnWidths[i])
				columnWidths[i] = std::min<int>(size.cx + 10, static_cast<int>(MAX_COLUMN_WIDTH));
		}
	}

	dc.SelectObject(pOldFont); // Fontu geri yükle

	// Gerçek genişlikleri uygula
	for (size_t i = 0; i < columnCount; ++i) {
		m_ListCtrl.SetColumnWidth(static_cast<int>(i), columnWidths[i]);
	}
}

void CTblEditorDlg::LoadTable(const CString& path)
{
	
	if (CTblEditorBase::LoadFile(path) == true)
	{
		std::map<int, std::vector<std::string>> result;
		result = CTblEditorBase::Data;
		InsertRows(result);

		//fill list than save data to compare later.
		int nItemCount = m_ListCtrl.GetItemCount();
		int nSubItemCount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();

		m_OriginalData.clear();
		m_OriginalData.resize(nItemCount);

		for (int i = 0; i < nItemCount; ++i)
		{
			m_OriginalData[i].resize(nSubItemCount);
			for (int j = 0; j < nSubItemCount; ++j)
			{
				m_OriginalData[i][j] = m_ListCtrl.GetItemText(i, j);
			}
		}


		m_ListCtrl.ModifyStyle(0, LVS_EDITLABELS);

	}
	else
	{
		AfxMessageBox(IDS_ERROR_FILE_NOT_OPENED, MB_ICONERROR);
	}
	

	
	
}







void CTblEditorDlg::OnNMDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
	if (pNMItemActivate->iItem != -1)
	{
		

		

		// Load text from cell to edit controll
		CString strText = m_ListCtrl.GetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem);
		m_editCell.SetWindowText(strText);
		m_editCell.SetFocus();

		m_iEditItem = pNMItemActivate->iItem;
		m_iEditSubItem = pNMItemActivate->iSubItem;
	}

	*pResult = 0;
}

void CTblEditorDlg::OnEnKillfocusEditCell()
{
	CString strText;
	m_editCell.GetWindowText(strText); // Get text from edit box

	// Update cell in Clist with text
	m_ListCtrl.SetItemText(m_iEditItem, m_iEditSubItem, strText);
}


void CTblEditorDlg::OnBnClickedOkButton()
{
	// Get text from edit controll
	CString strText;
	m_editCell.GetWindowText(strText);

	// Update Clist with new text
	m_ListCtrl.SetItemText(m_iEditItem, m_iEditSubItem, strText);

	// Zorla listeyi güncelle
	m_ListCtrl.RedrawItems(m_iEditItem, m_iEditItem);
	m_ListCtrl.UpdateWindow();
}


void CTblEditorDlg::OnBnClickedSaveButton() {

	if (m_ListCtrl.GetItemCount() == 0)
	{
		AfxMessageBox(IDS_ERROR_NO_DATA_FOUND, MB_ICONERROR);
		
		return;
	}

	

	CFileDialog saveDlg(FALSE, _T("tbl"), NULL, OFN_OVERWRITEPROMPT, _T("TBL Files (*.tbl)|*.tbl|All Files (*.*)|*.*||"), this);

	if (saveDlg.DoModal() == IDOK)
	{
		//get path
		CString savePath = saveDlg.GetPathName();

		std::map<int, std::vector<std::string>> newData;

		int nRowCount = m_ListCtrl.GetItemCount();
		int nColCount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();

		TRACE("ROW COUNT:%d", nRowCount);

		for (int row = 0; row < nRowCount; ++row)
		{
			//dont save wrongly edited lines, for example ID = empty
			CString strID = m_ListCtrl.GetItemText(row, 0);
			strID.Trim();
			if (strID.IsEmpty())
			{
				AfxMessageBox("Controll the list, one or more item has no ID");
				return;
			}
		}

		for (int row = 0; row < nRowCount; ++row)
		{

			


			std::vector<std::string> rowData;

			for (int col = 0; col < nColCount; ++col)
			{
				CString strText = m_ListCtrl.GetItemText(row, col);
				strText.Trim(); // Trim text

				// default -> empty string
				std::string value;

				// Trace memory_DataTypes if needed
				TRACE("memory_DataTypes içeriği:\n");
				for (size_t i = 0; i < CTblEditorBase::memory_DataTypes.size(); ++i)
				{
					TRACE("  [%zu] = %d\n", i, CTblEditorBase::memory_DataTypes[i]);
				}
			
				auto type = CTblEditorBase::memory_DataTypes[col];

				switch (type)
				{
				case DT_STRING:
					value = CT2A(strText);
					break;

				case DT_CHAR:
				case DT_BYTE:
				case DT_SHORT:
				case DT_WORD:
				case DT_INT:
				case DT_DWORD:
				{
					if (strText.IsEmpty())
						value = "0";
					else
						value = CT2A(strText);
					break;
				}

				case DT_FLOAT:
				case DT_DOUBLE:
				{
					if (strText.IsEmpty())
						value = "0.0";
					else
						value = CT2A(strText);
					break;
				}

				default:
					value = "0";
					break;
				}
				

				rowData.push_back(value);
			}

			newData[row] = rowData;
		}

		if (CTblEditorBase::SaveFile(savePath, newData) == true) //save successfull
		{
			AfxMessageBox(IDS_SAVE_SUCCESS, MB_ICONINFORMATION);
		}
		else //save failed
		{
			AfxMessageBox(IDS_SAVE_FAILED, MB_ICONERROR);
		}
		
		
		
		
		
		
	}
	
		
		
}

void CTblEditorDlg::OnBnClickedBtnAddRow()
{
	int colCount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();
	int selectedIndex = m_ListCtrl.GetSelectionMark();
	int insertIndex = (selectedIndex >= 0) ? selectedIndex + 1 : m_ListCtrl.GetItemCount();

	// İlk sütun için InsertItem gerekiyor (ilk sütun sadece InsertItem ile oluşturulabilir)
	m_ListCtrl.InsertItem(insertIndex, _T(""));

	// Diğer sütunları boş olarak ayarla
	for (int col = 1; col < colCount; ++col)
	{
		m_ListCtrl.SetItemText(insertIndex, col, _T(""));
	}

	// Yeni eklenen satırı seçili hale getir
	m_ListCtrl.SetItemState(insertIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ListCtrl.EnsureVisible(insertIndex, FALSE);
}


void CTblEditorDlg::OnNMDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItem = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int row = pNMItem->iItem;
	int col = pNMItem->iSubItem;

	if (row >= 0 && col >= 0)
	{
		CRect rect;
		m_ListCtrl.GetSubItemRect(row, col, LVIR_LABEL, rect);

		CString currentText = m_ListCtrl.GetItemText(row, col);

		m_Edit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			rect, &m_ListCtrl, 1234);

		m_Edit.SetWindowText(currentText);
		m_Edit.SetFocus();
		m_Edit.SetSel(0, -1);
	}

	*pResult = 0;
}





BOOL CTblEditorDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// Avoid closing program with enter and ESC keys
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTblEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTblEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}



// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTblEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



