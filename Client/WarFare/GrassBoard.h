// N3Board2DGrass.h: interface for the GrassBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRASSBOARD_H__D53F0EC4_B777_49CD_BEE8_071AD4A1680E__INCLUDED_)
#define AFX_GRASSBOARD_H__D53F0EC4_B777_49CD_BEE8_071AD4A1680E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Transform.h"

const DWORD BOARD_X				= 0;
const DWORD BOARD_Y				= 1;
const DWORD BOARD_Z				= 2;
const DWORD BOARD_XYZ			= 3;

class CN3Terrain;

class CGrassBoard : public CN3Transform
{
protected:
	typedef struct Grass_Info
	{
		__Matrix44	mtxWorld;
		__Vector3	vPos;	//	풀의 위치(지도상의)
		DWORD		dwAlpColor;	//	풀의 알파
		int			iTexIndex;	//	풀의 인덱스
	}Grass_Info;
//	__VertexXyzColorT1 m_vRects[4];
	Grass_Info		m_sGrassInfo[20];

	int				m_nTexIndex;
	unsigned char	m_ucTexIndex;	//	그림인덱스
	unsigned char	m_ucTexNum;	//	풀의 갯수

//	float			m_fLeftPo;
//	float			m_fTopPo;
	__Vector3		m_vCenterPo;

public:
	DWORD m_dwBoardType; // Board Type

	float	m_fBrightmin;	//	unit full bright lengs
	float	m_fBrightmax;	//	unit shadow lengs	

	BOOL	m_bCamOut;	//	카메라의 범위를 벋어나 랜더 할 필요 없음(TRUE)

	BOOL	m_bGroundInfo;	//	현재 셀이 풀을 그릴수 있는지 확인

public:
//	void TexSelectNum(int Texindex,unsigned char TexOrgIndex) {m_nTexIndex = Texindex, m_usTexIndex = TexOrgIndex;}
//	BOOL ThisTexIsHave(unsigned char TexIndex) { return TexIndex & m_usTexIndex;}

	void Init(__Vector3 vPos, DWORD dwBoardType);
	void Tick(CN3Terrain* pTerrain);
	void Render(CN3Texture** ppTex);

	void ReCalcMatrix();

	bool Load(HANDLE hFile);
	void LoadFromFile(int iTexIndex,unsigned char ucTexOrgIndex,__Vector3 vPos);

	void SetBrightInit(float min,float max) {m_fBrightmin = min,m_fBrightmax = max - min;};
	DWORD SetBrightLevel(float Level);
	
	void SetInfo(__Vector3 vBoardPosion,unsigned short usData);
//	WORD GetLeft() {return m_vCenterPo.x;}
//	WORD GetTop()  {return m_vCenterPo.z;}
	__Vector3	GetPo() {return m_vCenterPo;}

#ifdef _N3TOOL
	bool Save(HANDLE hFile);
#endif // end of #ifdef _N3TOOL

	void Release();

	CGrassBoard();
	virtual ~CGrassBoard();

protected:
	void FindGrassIndex(const unsigned char uCGrassMngOrder,int* pnInputGrass,int& nGrassTotNum);

};

#endif // !defined(AFX_GRASSBOARD_H__D53F0EC4_B777_49CD_BEE8_071AD4A1680E__INCLUDED_)
