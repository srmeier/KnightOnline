// N3BaseFileAccess.h: interface for the CN3BaseFileAccess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3BASEFILEACCESS_H__C99953BD_12BE_4B37_823F_4F4B2379FF74__INCLUDED_)
#define AFX_N3BASEFILEACCESS_H__C99953BD_12BE_4B37_823F_4F4B2379FF74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include "N3Base.h"
#include <string>

class CN3BaseFileAccess : public CN3Base
{
protected:
	std::string		m_szFileName; // Base Path 를 제외한 로컬 경로 + 파일 이름

public:
	int m_iLOD; // 로딩할때 쓸 LOD

public:
	const std::string& FileName() const { return m_szFileName; } // Full Path
	void FileNameSet(const std::string& szFileName);

	bool LoadFromFile(); // 파일에서 읽어오기.
	virtual bool LoadFromFile(const std::string& szFileName); // 파일에서 읽어오기.
	virtual bool Load(HANDLE hFile); // 핸들에서 읽어오기..

	virtual bool SaveToFile(); // 현재 파일 이름대로 저장.
	virtual bool SaveToFile(const std::string& szFileName); // 새이름으로 저장.
	virtual bool Save(HANDLE hFile); // 핸들을 통해 저장..

public:
	void Release();

	CN3BaseFileAccess();
	virtual ~CN3BaseFileAccess();
};

#endif // !defined(AFX_N3BASEFILEACCESS_H__C99953BD_12BE_4B37_823F_4F4B2379FF74__INCLUDED_)
