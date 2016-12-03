// N3RiverPatch.cpp: implementation of the CN3RiverPatch class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3RiverPatch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3RiverPatch::CN3RiverPatch()
{
	m_pRivers = NULL;
	Release();
}

CN3RiverPatch::~CN3RiverPatch()
{
	Release();
}

void CN3RiverPatch::Release()
{
	m_iRiverCount = 0;
	if (m_pRivers) {delete [] m_pRivers; m_pRivers = NULL;}
}

bool CN3RiverPatch::Load(HANDLE hFile)
{
	Release();
	DWORD dwNum;
	ReadFile(hFile, &m_iRiverCount, sizeof(m_iRiverCount), &dwNum, NULL);
	__River* pRivers = CreateRiver(m_iRiverCount);
	int i;
	for (i=0; i<m_iRiverCount; ++i)
	{
		__River* pR = pRivers+i;
		ReadFile(hFile, &(pR->iRiverID), sizeof(pR->iRiverID), &dwNum, NULL);
		ReadFile(hFile, &(pR->iVertexCount), sizeof(pR->iVertexCount), &dwNum, NULL);
		if (pR->iVertexCount>0)
		{
			pR->pVertices = new __VertexRiver[pR->iVertexCount];
			ReadFile(hFile, pR->pVertices, sizeof(__VertexRiver)*pR->iVertexCount, &dwNum, NULL);
		}
	}
	return true;
}

bool CN3RiverPatch::Save(HANDLE hFile)
{
	DWORD dwNum;
	WriteFile(hFile, &m_iRiverCount, sizeof(m_iRiverCount), &dwNum, NULL);
	int i;
	for (i=0; i<m_iRiverCount; ++i)
	{
		__River* pR = m_pRivers+i;
		WriteFile(hFile, &(pR->iRiverID), sizeof(pR->iRiverID), &dwNum, NULL);
		WriteFile(hFile, &(pR->iVertexCount), sizeof(pR->iVertexCount), &dwNum, NULL);
		if (pR->iVertexCount>0)
		{
			WriteFile(hFile, pR->pVertices, sizeof(__VertexRiver)*pR->iVertexCount, &dwNum, NULL);
		}
	}
	return true;
}

__River* CN3RiverPatch::CreateRiver(int iRiverCount)
{
	if (iRiverCount <= 0) return NULL;
	Release();
	m_iRiverCount = iRiverCount;
	m_pRivers = new __River[m_iRiverCount];

	return m_pRivers;
}

__River* CN3RiverPatch::GetRiverByID(int iRiverID)
{
	int i;
	for (i=0; i<m_iRiverCount; ++i)
	{
		if (m_pRivers[i].iRiverID == iRiverID)
		{
			return (m_pRivers + i);
		}
	}
	return NULL;
}
