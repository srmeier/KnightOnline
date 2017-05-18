// N3Board.cpp: implementation of the CN3Board class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3Texture.h"
#include "N3Board.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Board::CN3Board()
{
	m_dwType |= OBJ_BOARD;

	memset(m_vRects, 0, sizeof(m_vRects));
	m_Mtl.Init();
	m_fTexFPS = 30.0f;
	m_fTexIndex = 0.0f;
	m_dwBoardType = BOARD_XYZ;

}

CN3Board::~CN3Board()
{
	int iTC = m_TexRefs.size();
	if(iTC > 0)
	{
		for(int i = 0; i < iTC; i++) this->s_MngTex.Delete(&m_TexRefs[i]);
		m_TexRefs.clear();
	}
}

void CN3Board::Release()
{
	memset(m_vRects, 0, sizeof(m_vRects));
	m_Mtl.Init();
	m_fTexFPS = 30.0f;
	m_fTexIndex = 0.0f;
	m_dwBoardType = BOARD_XYZ;

	int iTC = m_TexRefs.size();
	if(iTC > 0)
	{
		for(int i = 0; i < iTC; i++) this->s_MngTex.Delete(&m_TexRefs[i]);
		m_TexRefs.clear();
	}
	
	CN3Transform::Release();
}

void CN3Board::Tick(float fFrm)
{
	// 회전 시킨다..
	if(m_dwBoardType == BOARD_Y)
	{
		__Vector3 vDir = s_CameraData.vEye - m_vPos;
		if( vDir.x > 0.0f ) m_Matrix.RotationY(-atanf(vDir.z/vDir.x) - (D3DX_PI * 0.5f));
		else m_Matrix.RotationY(-atanf(vDir.z/vDir.x) + (D3DX_PI * 0.5f));
		m_Matrix.PosSet(m_vPos);
	}
	else if(m_dwBoardType == BOARD_XYZ)
	{
		m_Matrix = s_CameraData.mtxViewInverse;
		m_Matrix.PosSet(m_vPos);
	}
	
	// Texture Animation...
	int iTC = m_TexRefs.size();
	if(iTC > 1)
	{
		m_fTexIndex += CN3Base::s_fSecPerFrm * m_fTexFPS;
		if(m_fTexIndex >= iTC) m_fTexIndex -= (iTC * m_fTexIndex) / iTC; // 정수로 나누면 소숫점만 남기게 된다??(하여튼 비슷해~)
	}
}

void CN3Board::Render()
{
	LPDIRECT3DTEXTURE8 lpTex = NULL;
	int iTC = m_TexRefs.size();
	if(iTC > 0 && m_fTexIndex < iTC) lpTex = m_TexRefs[(int)m_fTexIndex]->Get();

	s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);

	static DWORD dwAlpha, dwFog, dwCull;
	if(m_Mtl.nRenderFlags & RF_ALPHABLENDING) // Alpha 사용
	{
		s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
		if(TRUE != dwAlpha) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND,   m_Mtl.dwSrcBlend);
		s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND,  m_Mtl.dwDestBlend);
	}
	if(m_Mtl.nRenderFlags & RF_NOTUSEFOG) // Fog 무시..
	{
		s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
		if(TRUE == dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}
	if(m_Mtl.nRenderFlags & RF_DOUBLESIDED) // Render Flags - 
	{
		s_lpD3DDev->GetRenderState(D3DRS_CULLMODE, &dwCull);
		s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	s_lpD3DDev->SetTexture(0, lpTex);
	s_lpD3DDev->SetMaterial(&m_Mtl);
	s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP,   m_Mtl.dwColorOp);
	s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, m_Mtl.dwColorArg1);
	s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, m_Mtl.dwColorArg2);

	s_lpD3DDev->SetVertexShader(FVF_VNT1);
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_vRects, sizeof(__VertexT1));

	if((m_Mtl.nRenderFlags & RF_ALPHABLENDING) && FALSE == dwAlpha)	s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if((m_Mtl.nRenderFlags & RF_NOTUSEFOG) && TRUE == dwFog) 		s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, TRUE); // 안개 사용하지 않는다..
	if((m_Mtl.nRenderFlags & RF_DOUBLESIDED) && D3DCULL_NONE != dwCull) 		s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, dwCull);
}

void CN3Board::Init(__Vector3 vPos, DWORD dwBoardType, float fW, float fH)
{
	m_vPos = vPos;
	m_dwBoardType = dwBoardType;

	__Vector3 vNormal(0.0f, 0.0f, -1.0f);
	m_vRects[0].Set(-fW/2.0f, fH, 0, vNormal.x, vNormal.y, vNormal.z, 0.0f, 0.0f);
	m_vRects[1].Set( fW/2.0f, fH, 0, vNormal.x, vNormal.y, vNormal.z, 1.0f, 0.0f);
	m_vRects[2].Set( fW/2.0f,  0, 0, vNormal.x, vNormal.y, vNormal.z, 1.0f, 1.0f);
	m_vRects[3].Set(-fW/2.0f,  0, 0, vNormal.x, vNormal.y, vNormal.z, 0.0f, 1.0f);
}

bool CN3Board::Load(HANDLE hFile)
{
	return CN3Transform::Load(hFile);
}

#ifdef _N3TOOL
bool CN3Board::Save(HANDLE hFile)
{
	return CN3Transform::Save(hFile);
}
#endif // end of #ifdef _N3TOOL

void CN3Board::TexAlloc(int nCount)
{
	if(nCount <= 0) return;

	int iTC = m_TexRefs.size();
	if(iTC > 0)
	{
		for(int i = 0; i < iTC; i++) s_MngTex.Delete(&m_TexRefs[i]);
		m_TexRefs.clear();
	}

	m_TexRefs.assign(nCount, NULL);
}

void CN3Board::TexSet(int index, const std::string& szFN)
{
	if(index < 0 || index >= m_TexRefs.size()) return;

	s_MngTex.Delete(&m_TexRefs[index]);
	m_TexRefs[index] = s_MngTex.Get(szFN);
}

void CN3Board::LoadFromText(const std::string& szFName)
{
	Release();

	FILE* stream = fopen(szFName.c_str(), "r");
	__ASSERT(stream, "지정한 파일을 찾을 수 없습니다.");
	
	int result, i, iCount;
	char szBoardType[64]="";	__Vector3 vPos;	float fWidth, fHeight;
	result = fscanf(stream, "Position = %f %f %f\n", &(vPos.x), &(vPos.y), &(vPos.z));	__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Size = %f %f\n", &fWidth, &fHeight);	__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Rotation Axis = %s\n", szBoardType);		__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Frame per Sec = %f\n", &m_fTexFPS);			__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");

	result = fscanf(stream, "Render Flag = %d\n", &m_Mtl.nRenderFlags);		__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Source Blend = %d\n", &m_Mtl.dwSrcBlend);		__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Dest Blend = %d\n", &m_Mtl.dwDestBlend);		__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	// 안개, culling 옵션은 우선 정하지 말자.

	result = fscanf(stream, "Texture Count = %d\n", &iCount);		__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");

	if (iCount>0)
	{
		char szTexFName[_MAX_PATH];
		TexAlloc(iCount);
		for (i=0; i<iCount; ++i)
		{
			result = fscanf(stream, "Texture Name = %s\n", &szTexFName);		__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
			TexSet(i, szTexFName);
		}
	}

	fclose(stream);

	if (lstrcmpi(szBoardType, "X") == 0) m_dwBoardType = BOARD_X;
	else if (lstrcmpi(szBoardType, "Y") == 0) m_dwBoardType = BOARD_Y;
	else if (lstrcmpi(szBoardType, "Z") == 0) m_dwBoardType = BOARD_Z;
	else if (lstrcmpi(szBoardType, "XYZ") == 0) m_dwBoardType = BOARD_XYZ;
	Init(vPos, m_dwBoardType, fWidth, fHeight);
}