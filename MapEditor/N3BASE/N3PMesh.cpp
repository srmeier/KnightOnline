// N3PMesh.cpp: implementation of the CN3PMesh class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3PMesh.h"
#include "N3PMeshInstance.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3PMesh::CN3PMesh()
{
	m_dwType |= OBJ_MESH_PROGRESSIVE;

#ifdef _USE_VERTEXBUFFER
	m_pVB = NULL;
	m_pIB = NULL;
#else
	m_pVertices = NULL;
	m_pVertices2 = NULL;
	m_pIndices = NULL;
#endif
	m_pCollapses = NULL;
	m_pAllIndexChanges = NULL;
	m_pLODCtrlValues = NULL;

	m_iNumCollapses = 0;
	m_iTotalIndexChanges = 0;
	m_iMaxNumVertices = 0; m_iMaxNumIndices = 0;
	m_iMinNumVertices = 0; m_iMinNumIndices = 0;
	m_iLODCtrlValueCount = 0;

	m_vMin.Set(0,0,0);
	m_vMax.Set(0,0,0);
	m_fRadius = 0.0f;
}

CN3PMesh::~CN3PMesh()
{
#ifdef _USE_VERTEXBUFFER
	if (m_pVB && m_pVB->Release() == 0) m_pVB = NULL;
	if (m_pIB && m_pIB->Release() == 0) m_pIB = NULL;
#else
	delete [] m_pVertices; m_pVertices = NULL;
	delete [] m_pVertices2; m_pVertices2 = NULL;
	delete [] m_pIndices; m_pIndices = NULL;
#endif
	delete [] m_pCollapses; m_pCollapses = NULL;
	delete [] m_pAllIndexChanges; m_pAllIndexChanges = NULL;
	delete [] m_pLODCtrlValues;	m_pLODCtrlValues = NULL;
}

void CN3PMesh::Release()
{
#ifdef _USE_VERTEXBUFFER
	if (m_pVB) { m_pVB->Release(); m_pVB = NULL;}
	if (m_pIB) { m_pIB->Release(); m_pIB = NULL;}
#else
	if (m_pVertices) { delete[] m_pVertices; m_pVertices = NULL;}
	if (m_pVertices2) { delete[] m_pVertices2; m_pVertices2 = NULL;}
	if (m_pIndices) { delete[] m_pIndices; m_pIndices = NULL;}
#endif
	if (m_pCollapses) {	delete[] m_pCollapses; m_pCollapses = NULL;}
	if (m_pAllIndexChanges)	{ delete[] m_pAllIndexChanges; m_pAllIndexChanges = NULL;}
	if (m_pLODCtrlValues) {	delete [] m_pLODCtrlValues;	m_pLODCtrlValues = NULL;}

	m_iNumCollapses = 0;
	m_iTotalIndexChanges = 0;
	m_iMaxNumVertices = 0; m_iMaxNumIndices = 0;
	m_iMinNumVertices = 0; m_iMinNumIndices = 0;
	m_iLODCtrlValueCount = 0;

	m_vMin.Set(0,0,0);
	m_vMax.Set(0,0,0);
	m_fRadius = 0.0f;

	CN3BaseFileAccess::Release();
}

bool CN3PMesh::Load(HANDLE hFile)
{
	CN3BaseFileAccess::Load(hFile);

	DWORD dwNum;
	ReadFile(hFile, &m_iNumCollapses, sizeof(m_iNumCollapses), &dwNum, NULL);
	ReadFile(hFile, &m_iTotalIndexChanges, sizeof(m_iTotalIndexChanges), &dwNum, NULL);

	ReadFile(hFile, &m_iMaxNumVertices, sizeof(int), &dwNum, NULL);
	ReadFile(hFile, &m_iMaxNumIndices , sizeof(int), &dwNum, NULL);
	ReadFile(hFile, &m_iMinNumVertices, sizeof(int), &dwNum, NULL);
	ReadFile(hFile, &m_iMinNumIndices , sizeof(int), &dwNum, NULL);

	HRESULT hr = Create(m_iMaxNumVertices, m_iMaxNumIndices);
	__ASSERT(SUCCEEDED(hr), "Failed to create progressive mesh");
#ifdef _USE_VERTEXBUFFER
	if (m_iMaxNumVertices>0)
	{
//		HRESULT hr = m_lpD3DDev->CreateVertexBuffer(m_iMaxNumVertices*sizeof(__VertexT1),
//										D3DUSAGE_DYNAMIC, FVF_VNT1, D3DPOOL_MANAGED, &m_pVB);
//		if (FAILED(hr)) return hr;
		BYTE* pByte;
		hr = m_pVB->Lock(0, 0, &pByte, 0);
		if (FAILED(hr)) return false;
		ReadFile(hFile, pByte, m_iMaxNumVertices*sizeof(__VertexT1), &dwNum, NULL);
		m_pVB->Unlock();
	}
	if (m_iMaxNumIndices>0)
	{
//		HRESULT hr = m_lpD3DDev->CreateIndexBuffer(m_iMaxNumIndices*sizeof(WORD),
//										D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB);
//		if (FAILED(hr)) return hr;
		BYTE* pByte;
		hr = m_pIB->Lock(0, 0, &pByte, 0);
		if (FAILED(hr)) return false;
		ReadFile(hFile, pByte, m_iMaxNumIndices*sizeof(WORD), &dwNum, NULL);
		m_pIB->Unlock();
	}
#else
	if (m_iMaxNumVertices>0)
	{
//		m_pVertices = new __VertexT1[m_iMaxNumVertices];
		ReadFile(hFile, m_pVertices, m_iMaxNumVertices*sizeof(__VertexT1), &dwNum, NULL);
	}

	if (m_iMaxNumIndices>0)
	{
//		m_pIndices = new WORD[m_iMaxNumIndices];
		ReadFile(hFile, m_pIndices, m_iMaxNumIndices*sizeof(WORD), &dwNum, NULL);
	}
#endif

	if (m_iNumCollapses>0)
	{
		m_pCollapses = new __EdgeCollapse[m_iNumCollapses+1];	// +1을 한 이유 : PMeshInstance::SplitOne() 함수에서 부득이하게 포인터가 경계선을 가르키게 해야 하는 경우가 있어서.
		ReadFile(hFile, m_pCollapses, m_iNumCollapses*sizeof(__EdgeCollapse), &dwNum, NULL);
		ZeroMemory(m_pCollapses + m_iNumCollapses, sizeof(__EdgeCollapse));	// 위의 +1을 한이유와 같음. 만약을 대비해 마지막 데이타를 초기화 해둠

		bool bFixed = false;
		for(int i = 0; i < m_iNumCollapses; i++)
		{
			if(m_pCollapses[i].iIndexChanges < 0)
			{
				m_pCollapses[i].iIndexChanges = 0;
				bFixed = true;
			}
		}
#ifdef _DEBUG
		if(bFixed)
			::MessageBox(s_hWndBase, "잘못된 Progressive Mesh 수정", m_szName.c_str(), MB_OK);
#endif
	}
	if (m_iTotalIndexChanges>0)
	{
		m_pAllIndexChanges = new int[m_iTotalIndexChanges];
		ReadFile(hFile, m_pAllIndexChanges, m_iTotalIndexChanges*sizeof(int), &dwNum, NULL);
	}

	__ASSERT(m_pLODCtrlValues == NULL && m_iLODCtrlValueCount == 0, "Invalid Level of detail control value");
	ReadFile(hFile, &m_iLODCtrlValueCount, sizeof(m_iLODCtrlValueCount), &dwNum, NULL);
	if (m_iLODCtrlValueCount>0)
	{
		m_pLODCtrlValues = new __LODCtrlValue[m_iLODCtrlValueCount];
		ReadFile(hFile, m_pLODCtrlValues, m_iLODCtrlValueCount*sizeof(__LODCtrlValue), &dwNum, NULL);
	}

	FindMinMax();

	return true;
}

#ifdef _N3TOOL
bool CN3PMesh::Save(HANDLE hFile)
{
	CN3BaseFileAccess::Save(hFile);

	DWORD dwNum;
	WriteFile(hFile, &m_iNumCollapses, sizeof(m_iNumCollapses), &dwNum, NULL);
	WriteFile(hFile, &m_iTotalIndexChanges, sizeof(m_iTotalIndexChanges), &dwNum, NULL);

	WriteFile(hFile, &(m_iMaxNumVertices), sizeof(int), &dwNum, NULL);
	WriteFile(hFile, &(m_iMaxNumIndices), sizeof(int), &dwNum, NULL);
	WriteFile(hFile, &(m_iMinNumVertices), sizeof(int), &dwNum, NULL);
	WriteFile(hFile, &(m_iMinNumIndices), sizeof(int), &dwNum, NULL);

#ifdef _USE_VERTEXBUFFER
	if (m_iMaxNumVertices>0)
	{
		HRESULT hr;
		BYTE* pByte;
		hr = m_pVB->Lock(0, 0, &pByte, D3DLOCK_READONLY);
		if (FAILED(hr)) return false;
		WriteFile(hFile, pByte, m_iMaxNumVertices*sizeof(__VertexT1), &dwNum, NULL);
		m_pVB->Unlock();
	}
	if (m_iMaxNumIndices>0)
	{
		HRESULT hr;
		BYTE* pByte;
		hr = m_pIB->Lock(0, 0, &pByte, D3DLOCK_READONLY);
		if (FAILED(hr)) return false;
		WriteFile(hFile, pByte, m_iMaxNumIndices*sizeof(WORD), &dwNum, NULL);
		m_pIB->Unlock();
	}
#else
	if (m_iMaxNumVertices>0) WriteFile(hFile, m_pVertices, m_iMaxNumVertices*sizeof(__VertexT1), &dwNum, NULL);
	if (m_iMaxNumIndices>0) WriteFile(hFile, m_pIndices, m_iMaxNumIndices*sizeof(WORD), &dwNum, NULL);
#endif

	if (m_iNumCollapses>0)
	{
		for(int i = 0; i < m_iNumCollapses; i++)
			if(m_pCollapses[i].iIndexChanges < 0) m_pCollapses[i].iIndexChanges = 0; // 저장..
		WriteFile(hFile, m_pCollapses, m_iNumCollapses*sizeof(__EdgeCollapse), &dwNum, NULL);
	}
	if (m_iTotalIndexChanges>0) WriteFile(hFile, m_pAllIndexChanges, m_iTotalIndexChanges*sizeof(m_pAllIndexChanges[0]), &dwNum, NULL);

	WriteFile(hFile, &m_iLODCtrlValueCount, sizeof(m_iLODCtrlValueCount), &dwNum, NULL);
	if (m_iLODCtrlValueCount>0) WriteFile(hFile, m_pLODCtrlValues, m_iLODCtrlValueCount*sizeof(__LODCtrlValue), &dwNum, NULL);

	return true;
}
#endif // end of _N3TOOL

void CN3PMesh::FindMinMax()
{
	if(m_iMaxNumVertices <= 0)
	{
		m_vMin.Zero();
		m_vMax.Zero();
		m_fRadius = 0;
		return;
	}

	// 최소, 최대 점을 찾는다.
	m_vMin.Set(FLT_MAX, FLT_MAX, FLT_MAX);
	m_vMax.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);

#ifdef _USE_VERTEXBUFFER
	__ASSERT(m_pVB && m_pIB, "Vertex buffer is NULL or Index buffer is NULL!");
	HRESULT hr;
	BYTE* pByte;
	hr = m_pVB->Lock(0, 0, &pByte, D3DLOCK_READONLY);
	__VertexT1* pVertices = (__VertexT1*)pByte;
	for(int i = 0; i < m_iMaxNumVertices; i++)
	{
		if(pVertices[i].x < m_vMin.x) m_vMin.x = pVertices[i].x;
		if(pVertices[i].y < m_vMin.y) m_vMin.y = pVertices[i].y;
		if(pVertices[i].z < m_vMin.z) m_vMin.z = pVertices[i].z;
		if(pVertices[i].x > m_vMax.x) m_vMax.x = pVertices[i].x;
		if(pVertices[i].y > m_vMax.y) m_vMax.y = pVertices[i].y;
		if(pVertices[i].z > m_vMax.z) m_vMax.z = pVertices[i].z;
	}
	m_pVB->Unlock();
#else
	for(int i = 0; i < m_iMaxNumVertices; i++)
	{
		if(m_pVertices[i].x < m_vMin.x) m_vMin.x = m_pVertices[i].x;
		if(m_pVertices[i].y < m_vMin.y) m_vMin.y = m_pVertices[i].y;
		if(m_pVertices[i].z < m_vMin.z) m_vMin.z = m_pVertices[i].z;

		if(m_pVertices[i].x > m_vMax.x) m_vMax.x = m_pVertices[i].x;
		if(m_pVertices[i].y > m_vMax.y) m_vMax.y = m_pVertices[i].y;
		if(m_pVertices[i].z > m_vMax.z) m_vMax.z = m_pVertices[i].z;
	}
#endif

	// 최대 최소값을 갖고 반지름 계산한다..
	m_fRadius  = (m_vMax - m_vMin).Magnitude() * 0.5f;
}

#ifdef _N3TOOL
void CN3PMesh::CopyMesh(CN3PMesh* pSrcPMesh)
{
	Release();

	HRESULT hr;
	
	__ASSERT(pSrcPMesh, "Source progressive mesh pointer is NULL");
	
	m_iNumCollapses			= pSrcPMesh->m_iNumCollapses;
	m_iTotalIndexChanges	= pSrcPMesh->m_iTotalIndexChanges;
	m_iMaxNumVertices		= pSrcPMesh->m_iMaxNumVertices;
	m_iMaxNumIndices		= pSrcPMesh->m_iMaxNumIndices;
	m_iMinNumVertices		= pSrcPMesh->m_iMinNumVertices;
	m_iMinNumIndices		= pSrcPMesh->m_iMinNumIndices;
	m_vMin					= pSrcPMesh->m_vMin;
	m_vMax					= pSrcPMesh->m_vMax;
	m_fRadius				= pSrcPMesh->m_fRadius;
	m_iLODCtrlValueCount	= pSrcPMesh->m_iLODCtrlValueCount;

	if (m_iTotalIndexChanges>0)
	{
		m_pAllIndexChanges = new int[m_iTotalIndexChanges];

		CopyMemory(m_pAllIndexChanges, pSrcPMesh->m_pAllIndexChanges, sizeof(int)*m_iTotalIndexChanges);
	}

	if (m_iNumCollapses>0)
	{
		m_pCollapses = new __EdgeCollapse[m_iNumCollapses+1];	// +1을 한 이유 : PMeshInstance::SplitOne() 함수에서 부득이하게 포인터가 경계선을 가르키게 해야 하는 경우가 있어서.
		CopyMemory(m_pCollapses, pSrcPMesh->m_pCollapses, sizeof(__EdgeCollapse)*m_iNumCollapses);
		ZeroMemory(m_pCollapses + m_iNumCollapses, sizeof(__EdgeCollapse));	// 위의 +1을 한이유와 같음. 만약을 대비해 마지막 데이타를 초기화 해둠
	}

	hr = Create(m_iMaxNumVertices, m_iMaxNumIndices);
	__ASSERT(SUCCEEDED(hr), "Failed to create progressive mesh");
#ifdef _USE_VERTEXBUFFER
	if (m_iMaxNumVertices>0)
	{
		BYTE* pDestByte, *pSrcByte;
		hr = m_pVB->Lock(0, 0, &pDestByte, 0);
		hr = pSrcPMesh->m_pVB->Lock(0, 0, &pSrcByte, D3DLOCK_READONLY);
		CopyMemory(pDestByte, pSrcByte, m_iMaxNumVertices*sizeof(__VertexT1));
		hr = pSrcPMesh->m_pVB->Unlock();
		m_pVB->Unlock();
	}
	if (m_iMaxNumIndices>0)
	{
		BYTE* pDestByte, *pSrcByte;
		hr = m_pIB->Lock(0, 0, &pDestByte, 0);
		hr = pSrcPMesh->m_pIB->Lock(0, 0, &pSrcByte, D3DLOCK_READONLY);
		CopyMemory(pDestByte, pSrcByte, m_iMaxNumIndices*sizeof(WORD));
		pSrcPMesh->m_pIB->Unlock();
		m_pIB->Unlock();
	}
#else
	if (m_iMaxNumVertices>0)
	{
//		m_pVertices = new __VertexT1[m_iMaxNumVertices];
		CopyMemory(m_pVertices, pSrcPMesh->m_pVertices, sizeof(__VertexT1)*m_iMaxNumVertices);
	}

	if (m_iMaxNumIndices>0)
	{
//		m_pIndices = new WORD[m_iMaxNumIndices];
		CopyMemory(m_pIndices, pSrcPMesh->m_pIndices, sizeof(WORD)*m_iMaxNumIndices);
	}
#endif
	if (m_iLODCtrlValueCount>0)
	{
		m_pLODCtrlValues = new __LODCtrlValue[m_iLODCtrlValueCount];
		CopyMemory(m_pLODCtrlValues, pSrcPMesh->m_pLODCtrlValues, sizeof(__LODCtrlValue)*m_iLODCtrlValueCount);
	}

	m_szName = pSrcPMesh->m_szName;
}
#endif // end of _N3TOOL

HRESULT CN3PMesh::Create(int iNumVertices, int iNumIndices)
{
	m_iMaxNumVertices = iNumVertices;
	m_iMaxNumIndices = iNumIndices;

#ifdef _USE_VERTEXBUFFER
	HRESULT hr;
	if (m_pVB){ m_pVB->Release(); m_pVB = NULL;}
	if (m_pIB){ m_pIB->Release(); m_pIB = NULL;}

	if (m_iMaxNumVertices>0)
	{
		hr = m_lpD3DDev->CreateVertexBuffer(m_iMaxNumVertices*sizeof(__VertexT1),
										D3DUSAGE_DYNAMIC, FVF_VNT1, D3DPOOL_MANAGED, &m_pVB);
		if (FAILED(hr)) return hr;
	}

	if (m_iMaxNumIndices>0)
	{
		hr = m_lpD3DDev->CreateIndexBuffer(m_iMaxNumIndices*sizeof(WORD),
										D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB);
		if (FAILED(hr)) return hr;
	}
#else
	if (m_pVertices) {delete m_pVertices; m_pVertices = NULL;}
	if (m_pIndices) {delete m_pIndices; m_pIndices = NULL;}

	if (m_iMaxNumVertices>0)
	{
		m_pVertices = new __VertexT1[m_iMaxNumVertices];
	}
	if (m_iMaxNumIndices>0)
	{
		m_pIndices = new WORD[m_iMaxNumIndices];
	}
#endif
	return S_OK;
}

HRESULT CN3PMesh::GenerateSecondUV()
{
#ifdef _USE_VERTEXBUFFER
	HRESULT hr;
	if (m_pVB2){ m_pVB2->Release(); m_pVB2 = NULL;}

	if (m_iMaxNumVertices>0)
	{
		hr = m_lpD3DDev->CreateVertexBuffer(m_iMaxNumVertices*sizeof(__VertexT2),
										D3DUSAGE_DYNAMIC, FVF_VNT2, D3DPOOL_MANAGED, &m_pVB2);
		if (FAILED(hr)) return hr;

		__VertexT1* pVSrc = NULL; __VertexT2* pVDest = NULL;
		m_pVB->Lock(0, 0, (BYTE**)(&pVSrc), 0);
		m_pVB2->Lock(0, 0, (BYTE**)(&pVDest), 0);
		for(int i = 0; i < m_iMaxNumVertices; i++)
		{
			pVDest[i] = pVSrc[i];
			pVDest[i].tu2 = pVDest[i].tu;
			pVDest[i].tv2 = pVDest[i].tv;
		}
		m_pVB->Unlock();
		m_pVB2->Unlock();
	}

#else
	if (m_pVertices2) {delete m_pVertices2; m_pVertices2 = NULL;}

	if (m_iMaxNumVertices>0)
	{
		m_pVertices2 = new __VertexT2[m_iMaxNumVertices];

		for(int i = 0; i < m_iMaxNumVertices; i++)
		{
			m_pVertices2[i].x = m_pVertices[i].x;
			m_pVertices2[i].y = m_pVertices[i].y;
			m_pVertices2[i].z = m_pVertices[i].z;
			m_pVertices2[i].n = m_pVertices[i].n;
			m_pVertices2[i].tu = m_pVertices[i].tu;
			m_pVertices2[i].tv = m_pVertices[i].tv;
			m_pVertices2[i].tu2 = m_pVertices[i].tu;
			m_pVertices2[i].tv2 = m_pVertices[i].tv;
		}
	}
#endif
	return S_OK;
}

#ifdef _N3TOOL
void CN3PMesh::LODCtrlSet(__LODCtrlValue *pLODCtrls, int nCount)
{
	m_iLODCtrlValueCount = 0;
	delete [] m_pLODCtrlValues; m_pLODCtrlValues = NULL;
	if(NULL == pLODCtrls || nCount <= 0) return;

	m_iLODCtrlValueCount = nCount;
	if(nCount > 0)
	{
		m_pLODCtrlValues = new __LODCtrlValue[nCount];
		memcpy(m_pLODCtrlValues, pLODCtrls, sizeof(__LODCtrlValue) * nCount);

		// 거리에 따라 정렬
		qsort(m_pLODCtrlValues, m_iLODCtrlValueCount, sizeof(__LODCtrlValue), SortByDistance);
	}
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
int CN3PMesh::SortByDistance(const void *pArg1, const void *pArg2)
{
	__LODCtrlValue *pObj1 = (__LODCtrlValue*)pArg1;
	__LODCtrlValue *pObj2 = (__LODCtrlValue*)pArg2;

	if(pObj1->fDist < pObj2->fDist) return -1;
	else if(pObj1->fDist > pObj2->fDist) return 1;
	else return 0;
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
void CN3PMesh::ReGenerateSmoothNormal()
{
	if(m_iMaxNumVertices <= 0) return;

	CN3PMeshInstance PMI(this);
	PMI.SetLODByNumVertices(m_iMaxNumVertices); // 최대 점으로 세팅하고..
	int nIC = PMI.GetNumIndices(); // Index Count...
	WORD* pwIndices = PMI.GetIndices(); // Index ...

	int* pnNs = new int[m_iMaxNumVertices];
	memset(pnNs, 0, 4 * m_iMaxNumVertices);
	__Vector3* pvNs = new __Vector3[m_iMaxNumVertices];
	memset(pvNs, 0, sizeof(__Vector3)*m_iMaxNumVertices);

	int nFC = nIC / 3;

	__Vector3 v0, v1, v2, vN(0,0,0);
	for(int i = 0; i < m_iMaxNumVertices; i++)
	{
		for(int j = 0; j < nFC; j++)
		{
			v0 = m_pVertices[pwIndices[j*3+0]];
			v1 = m_pVertices[pwIndices[j*3+1]];
			v2 = m_pVertices[pwIndices[j*3+2]];

			if(	m_pVertices[i] == v0 ||
				m_pVertices[i] == v1 ||
				m_pVertices[i] == v2 )
			{
				vN.Cross(v1 - v0, v2 - v1); // Normal 값을 계산하고...
				vN.Normalize();

				pnNs[i]++;
				pvNs[i] += vN;
			}
		}

		if(pnNs[i] > 0) m_pVertices[i].n = pvNs[i] / (float)pnNs[i];
	}

	delete [] pnNs;
	delete [] pvNs;
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
void CN3PMesh::ReGenerateSharpNormal()
{
	if(m_iMaxNumVertices <= 0) return;

	CN3PMeshInstance PMI(this);
	PMI.SetLODByNumVertices(m_iMaxNumVertices); // 최대 점으로 세팅하고..
	int nIC = PMI.GetNumIndices(); // Index Count...
	WORD* pwIndices = PMI.GetIndices(); // Index ...

	int nFC = nIC / 3;
	__Vector3 v0, v1, v2, vN(0,0,0);
	for(int j = 0; j < nFC; j++)
	{
		v0 = m_pVertices[pwIndices[j*3+0]];
		v1 = m_pVertices[pwIndices[j*3+1]];
		v2 = m_pVertices[pwIndices[j*3+2]];

		vN.Cross(v1 - v0, v2 - v1); // Normal 값을 계산하고...
		vN.Normalize();

		m_pVertices[pwIndices[j*3+0]].n = vN;
		m_pVertices[pwIndices[j*3+1]].n = vN;
		m_pVertices[pwIndices[j*3+2]].n = vN;
	}

}
#endif // end of _N3TOOL
