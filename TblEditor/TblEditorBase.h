#pragma once

#include <string>
#include <map>
#include <vector>

#include <N3Base/N3TableBase.h>

using DATA_TYPE = TBL_DATA_TYPE;

class CTblEditorBase
{
public:
	CTblEditorBase();
	virtual ~CTblEditorBase();
	bool LoadFile(const CString& path, CString& errorMsg);
	bool SaveFile(const CString& path, const std::map<int, std::vector<CStringA>>& newData);
	CString GetColumnDefault(int iColNo) const;
	CString GetColumnName(int iColNo) const;
	DATA_TYPE GetColumnType(int iColNo) const;

protected:
	bool LoadRowData(HANDLE hFile);

public: // TODO: fix me
	std::map<int, std::vector<CStringA>> m_Rows;
	std::vector<DATA_TYPE> m_DataTypes;
};
