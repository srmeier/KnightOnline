#pragma once

#include <string>
#include <map>
#include <vector>

enum DATA_TYPE : int32_t
{
	DT_NONE,
	DT_CHAR,
	DT_BYTE,
	DT_SHORT,
	DT_WORD,
	DT_INT,
	DT_DWORD,
	DT_STRING,
	DT_FLOAT,
	DT_DOUBLE
};

class CTblEditorBase
{
public:
	CTblEditorBase();
	virtual ~CTblEditorBase();
	bool LoadFile(const CString& path);
	bool SaveFile(const CString& path, const std::map<int, std::vector<CString>>& newData);

protected:
	void LoadRowData(HANDLE hFile);

public: // TODO: fix me
	std::map<int, std::vector<CString>> m_Rows;
	std::vector<DATA_TYPE> m_DataTypes;
};