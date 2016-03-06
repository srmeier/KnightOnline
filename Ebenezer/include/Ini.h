// Ini.h: interface for the CIni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INI_H__51A75906_827F_44DE_B612_F34750B4A1C8__INCLUDED_)
#define AFX_INI_H__51A75906_827F_44DE_B612_F34750B4A1C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIni  
{
public:
	char m_szFileName[200];
	int m_nError;
	char m_strError[200];
public:
	CIni();
	virtual ~CIni();

public:
	int GetProfileInt(char* lpAppName,
							  char* lpKeyName,
							  int nDefault);
	char* GetProfileString(char* lpAppName,
							  char* lpKeyName,
							  char* lpDefault);
	int SetProfileInt(char* lpAppName,
							  char* lpKeyName,
							  int nDefault);
	int SetProfileString(char* lpAppName,
							  char* lpKeyName,
							  char* lpDefault);
	bool SetPath(const char* filename);
};

#endif // !defined(AFX_INI_H__51A75906_827F_44DE_B612_F34750B4A1C8__INCLUDED_)
