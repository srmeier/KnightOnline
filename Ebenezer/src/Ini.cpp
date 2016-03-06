// Ini.cpp: implementation of the CIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ini.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIni::CIni()
{

}

CIni::~CIni()
{

}

int CIni::GetProfileInt(char* lpAppName,
							  char* lpKeyName,
							  int nDefault)
{
	if (m_szFileName[0] == '\0') {
		m_nError = 0X01;
		strcpy(m_strError, "File이 Set되지 않았습니다.");
		return -1;
	}

	GetPrivateProfileInt(lpAppName,lpKeyName,nDefault,m_szFileName);
	char tmpDefault[500];
	if (!GetPrivateProfileString(lpAppName,lpKeyName,"",tmpDefault,500,m_szFileName)) {
		sprintf(tmpDefault,"%d",nDefault);
		WritePrivateProfileString(lpAppName,lpKeyName,tmpDefault,m_szFileName);
		return nDefault;
	}

	return GetPrivateProfileInt(lpAppName,lpKeyName,nDefault,m_szFileName);
}

char* CIni::GetProfileString(char* lpAppName,
							  char* lpKeyName,
							  char* lpDefault)
{
	static char retString[200]="";
	if (m_szFileName[0] == '\0') {
		m_nError = 0X01;
		strcpy(m_strError, "File이 Set되지 않았습니다.");
		strcpy(retString , "");
		return retString;
	}
	
	char tmpReturn[500];
	DWORD ret=GetPrivateProfileString(lpAppName,lpKeyName,"",tmpReturn,500,m_szFileName);
	if (!ret) {
		WritePrivateProfileString(lpAppName,lpKeyName,lpDefault,m_szFileName);
		strcpy(tmpReturn,lpDefault);
	}

	strcpy(retString , tmpReturn);

	return retString;

}

bool CIni::SetPath(const char* filename)
{
	char Buf[256], Path[256];
	char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

	//::GetModuleFileName(AfxGetApp()->m_hInstance, Buf, 256);
	//_splitpath(Buf,drive,dir,fname,ext);
	//strcpy(Path, drive);
	//strcat(Path, dir);		
	char IniPath[256]="";
	strcpy(IniPath, "./");//Path);
	wsprintf(IniPath, "%s%s", IniPath, filename);
	//IniPath += filename;
	strcpy(m_szFileName,(LPCTSTR)IniPath);
	return true;
}

int CIni::SetProfileInt(char* lpAppName,
							  char* lpKeyName,
							  int nDefault)
{
	int nRet  = -1;
	if (m_szFileName[0] == '\0') {
		m_nError = 0X01;
		strcpy(m_strError, "File이 Set되지 않았습니다.");
		return nRet;
	}

	char tmpDefault[500];
	memset(tmpDefault, 0x00, 500);
	sprintf(tmpDefault,"%d",nDefault);
	nRet = WritePrivateProfileString(lpAppName,lpKeyName,tmpDefault,m_szFileName);

	return nRet;
}

int CIni::SetProfileString(char* lpAppName,
							  char* lpKeyName,
							  char* lpDefault)
{
	int nRet  = -1;
	if (m_szFileName[0] == '\0') {
		m_nError = 0X01;
		strcpy(m_strError, "File이 Set되지 않았습니다.");
		return nRet;
	}

	nRet = WritePrivateProfileString(lpAppName, lpKeyName, lpDefault,m_szFileName);

	return nRet;
}
