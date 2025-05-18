#pragma once

#include "TblEditorBase.h"

class CTblEditorDoc : public CDocument
{
protected: // create from serialization only
	CTblEditorDoc();
	DECLARE_DYNCREATE(CTblEditorDoc)

public:
	static constexpr LPARAM HINT_DOCUMENT_LOADED = 0x100000;

	inline bool IsLoaded() const {
		return m_strLoadedPath.IsEmpty();
	}

	inline const CString& GetLoadedPath() const {
		return m_strLoadedPath;
	}

	inline CTblEditorBase& GetTbl() {
		return m_Tbl;
	}

	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

protected:
	CTblEditorBase	m_Tbl;
	CString			m_strLoadedPath;
};
