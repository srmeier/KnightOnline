#include "stdafx.h"
#include "TblEditorDoc.h"
#include "TblEditorView.h" // TODO: remove this

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CTblEditorDoc, CDocument)

CTblEditorDoc::CTblEditorDoc()
{
}

BOOL CTblEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_Tbl.Release();
	m_strLoadedPath.Empty();

	UpdateAllViews(nullptr, HINT_DOCUMENT_LOADED);
	return TRUE;
}

BOOL CTblEditorDoc::OnOpenDocument(
	LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	m_Tbl.Release();
	m_strLoadedPath.Empty();

	CString errorMsg;
	if (!m_Tbl.LoadFile(lpszPathName, errorMsg))
	{
		AfxMessageBox(errorMsg, MB_ICONERROR);
		return FALSE;
	}

	UpdateAllViews(nullptr, HINT_DOCUMENT_LOADED);
	return TRUE;
}

BOOL CTblEditorDoc::OnSaveDocument(
	LPCTSTR lpszPathName)
{
	CDocument::OnSaveDocument(lpszPathName);

	// TODO: Have the list memory backed, so we're not relying on fetching data
	// from the list -- we're just modifying & saving the data directly.
	// Then we don't need to awkwardly access the view here.
	POSITION pos = GetFirstViewPosition();
	if (pos == nullptr)
		return FALSE;

	CTblEditorView* pView
		= static_cast<CTblEditorView*>(GetNextView(pos));
	if (pView == nullptr)
		return FALSE;

	if (!pView->SaveTable(lpszPathName))
		return FALSE;

	return TRUE;
}
