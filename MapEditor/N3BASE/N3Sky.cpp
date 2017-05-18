// N3Sky.cpp: implementation of the CN3Sky class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxBase.h"
#include "N3Sky.h"
#include "N3PMesh.h"
#include "N3PMeshInstance.h"
#include "N3Texture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Sky::CN3Sky()
{
//	m_pTex = NULL;
	m_SkyColor.ChangeColor(0xff081021);	//밤
	m_FogColor.ChangeColor(0xff102942);	//밤
}

CN3Sky::~CN3Sky()
{
//	if (m_pTex) {s_MngTex.Delete(m_pTex);	m_pTex = NULL;}
}

void CN3Sky::Release()
{
	CN3Base::Release();
//	m_SkyColor.ChangeColor(0xFF5284DE);	//낮
//	m_FogColor.ChangeColor(0xFFB5C6DE);	//낮
	m_SkyColor.ChangeColor(0xff081021);	//밤
	m_FogColor.ChangeColor(0xff102942);	//밤
//	if (m_pTex) {s_MngTex.Delete(m_pTex);	m_pTex = NULL;}
}

void CN3Sky::Tick()
{
	m_SkyColor.Tick();
	m_FogColor.Tick();
	int i;
	D3DCOLOR FogColor = m_FogColor.GetCurColor();
	for (i=0; i<4; ++i)
	{
		m_vFronts[i].color = (m_vFronts[i].color&0xff000000) | (FogColor&0x00ffffff);
		m_Bottom[i].color = FogColor;
	}
}

void CN3Sky::Render()
{
    // Set up a rotation matrix to orient the billboard towards the camera.
	__Matrix44 matWorld;
	__Vector3 vDir = s_CameraData.vEye - s_CameraData.vAt;	// 카메라의 방향
	if (0.0f == vDir.x) matWorld.Identity();
	else if( vDir.x > 0.0f ) matWorld.RotationY(-atanf(vDir.z/vDir.x) - (D3DX_PI * 0.5f));
	else  matWorld.RotationY(-atanf(vDir.z/vDir.x) + (D3DX_PI * 0.5f));
	s_lpD3DDev->SetTransform( D3DTS_WORLD, &matWorld );

	// Set the texture stage states
	s_lpD3DDev->SetTexture(0, NULL);
	s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );

	// Render the skybox
	s_lpD3DDev->SetVertexShader(FVF_XYZCOLOR);
	// 밑판 그리기
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_Bottom, sizeof(m_Bottom[0]));
	// 옆판 그리기
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_vFronts, sizeof(m_vFronts[0]));
}

void CN3Sky::Init()
{
	Release();
	
	// 노을 진 경우를 예로 든다....
	const float fWidth		= 3.5f; // 전면 판의 폭
	const float fTopY		= 0.5f; // 전면 판의 높이..  노을져서 붉으스름하게 된 부분의 위 높이 - 이 위부터는 하늘이다..
	const float fBottomY	= 0.1f; // 전면 판의 바닥 높이..  노을져서 붉으스름하게 된 부분의 아래 - 이 아래는 순수한 안개색이다..
	const float fDistance	= 1.5f; // 전면 판의 바닥 길이..
	const D3DCOLOR color = m_FogColor.GetCurColor();
	m_vFronts[0].Set( fWidth,	fTopY,		fDistance,		0x00ffffff&color);
	m_vFronts[1].Set( fWidth,	fBottomY,	fDistance,		color);
	m_vFronts[2].Set(-fWidth,	fBottomY,	fDistance,		color);
	m_vFronts[3].Set(-fWidth,	fTopY,		fDistance,		0x00ffffff&color);

	const float fBottomOffset = -5.0f;	// 밑면 판의 바닥 높이
	m_Bottom[0].Set( fWidth,	fBottomY,	fDistance,		color);
	m_Bottom[1].Set( fWidth,	fBottomOffset,	fDistance,	color);
	m_Bottom[2].Set(-fWidth,	fBottomOffset,	fDistance,	color);
	m_Bottom[3].Set(-fWidth,	fBottomY,	fDistance,		color);
}
