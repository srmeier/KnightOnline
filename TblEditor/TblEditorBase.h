#pragma once
#include <string>
#include <map>
#include <vector>

class CTblEditorBase 
{
public:
	CTblEditorBase();
	virtual ~CTblEditorBase();
	static bool LoadFile(const CString& path);
	static bool SaveFile(const CString& path, std::map<int, std::vector<std::string>> newData);
	static std::map<int, std::vector<std::string>> Data;
	static int memory_iDataTypeCount;
	static int memory_iRowCount;
	static void SetRowData(HANDLE hFile);
	enum DATA_TYPE { DT_NONE, DT_CHAR, DT_BYTE, DT_SHORT, DT_WORD, DT_INT, DT_DWORD, DT_STRING, DT_FLOAT, DT_DOUBLE };
	static std::vector<int> memory_DataTypes;

protected:
	std::vector<DATA_TYPE> m_DataTypes;

};