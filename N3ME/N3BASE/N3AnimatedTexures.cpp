// N3AnimatedTexures.cpp: implementation of the CN3AnimatedTexures class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "N3Texture.h"
#include "N3AnimatedTexures.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3AnimatedTexures::CN3AnimatedTexures()
{
	m_fTexIndex = 0.0f;
	m_fTexFPS = 30.0f;
}

CN3AnimatedTexures::~CN3AnimatedTexures()
{
	int iTC = m_TexRefs.size();
	for(int i = 0; i < iTC; i++) s_MngTex.Delete(&m_TexRefs[i]);
}

void CN3AnimatedTexures::Release()
{
	m_fTexIndex = 0.0f;
	m_fTexFPS = 30.0f;

	int iTC = m_TexRefs.size();
	for(int i = 0; i < iTC; i++) s_MngTex.Delete(&m_TexRefs[i]);
	m_TexRefs.clear();
}

bool CN3AnimatedTexures::Load(HANDLE hFile)
{
	CN3BaseFileAccess::Load(hFile);

	DWORD dwRWC;

	ReadFile(hFile, &m_fTexFPS, 4, &dwRWC, NULL);
	int iTC = 0;
	ReadFile(hFile, &iTC, 4, &dwRWC, NULL);
	if(!m_TexRefs.empty())
	{
		int iTCPrev = m_TexRefs.size();
		for(int i = 0; i < iTCPrev; i++) s_MngTex.Delete(&m_TexRefs[i]);
		m_TexRefs.clear();
	}
	
	int nL = 0;
	char szFN[256] = "";
	m_TexRefs.assign(iTC, NULL); // Texture Pointer Pointer 할당..
	for(int i = 0; i < iTC; i++) // Texture Count 만큼 파일 이름 읽어서 텍스처 부르기..
	{
		ReadFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL > 0)
		{
			ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL; // 텍스처 파일 이름..
			m_TexRefs[i] = s_MngTex.Get(szFN);
		}
	}

	return true;
}

#ifdef _N3TOOL
bool CN3AnimatedTexures::Save(HANDLE hFile)
{
	CN3BaseFileAccess::Save(hFile);

	DWORD dwRWC;

	WriteFile(hFile, &m_fTexFPS, 4, &dwRWC, NULL);
	int iTC = m_TexRefs.size();
	WriteFile(hFile, &iTC, 4, &dwRWC, NULL);
	
	for(int i = 0; i < iTC; i++) // Texture Count 만큼 파일 이름 읽어서 텍스처 부르기..
	{
		nL = 0;
		if(m_TexRefs[i]) nL = m_TexRefs[i].size();
		WriteFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL > 0)
		{
			WriteFile(hFile, m_TexRefs[i]->FileName().c_str(), nL, &dwRWC, NULL); // 텍스처 파일 이름..
		}
	}

	return true;
}
#endif // #ifdef _N3TOOL

void CN3AnimatedTexures::Tick()
{
	if(m_fTexFPS <= 0 || m_TexRefs.empty()) return;

	int iTC = m_TexRefs.size();
	m_fTexIndex += CN3Base::s_fSecPerFrm * m_fTexFPS;
	if(m_fTexIndex >= iTC) m_fTexIndex -= (iTC * m_fTexIndex) / iTC; // 정수로 나누면 소숫점만 남기게 된다??(하여튼 비슷해~)
}
