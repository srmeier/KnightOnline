// GrassMng.h: interface for the CGrassMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRASSMNG_H__3B6D6FD3_4213_40EE_A8E9_F50412B49EC2__INCLUDED_)
#define AFX_GRASSMNG_H__3B6D6FD3_4213_40EE_A8E9_F50412B49EC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameBase.h"
//#include "GameProcedure.h"
#include <list>
#include "GrassBoard.h"

#define GRASS_MAX 11 //	(40/4(TILE_SIZE)) + 1
#define GRASS_TILENUM 11
#define GRASS_MAKENEW	1
#define	GRASS_MAKEUSE	2

class CGameProcedure;
class CN3Terrain;

//typedef std::list<class CGrassBoard*>::iterator it_GrassBoard;
class CGrassMng   : public CGameBase
{
protected:

//	std::list<class CGrassBoard*>	m_Grasses;
	CGrassBoard		m_pGrasses[GRASS_TILENUM][GRASS_TILENUM];	//	풀 뿌려지는 영역
//	__Vector3		m_pCount[GRASS_MAX];
	float			m_fChkRange[4];	//	카메라와의 위치검색용

//	__Vector3		m_vCamPo;
	bool			m_bChkZoneChange;
	unsigned short	m_usDrawIndex;

	CN3Texture*		m_txTexture[8];
#ifdef _DEBUG
	char			m_strFileName[8][_MAX_PATH];	//	나중에 디버거를 위해 그림의 이름을 백업 받는다
#endif
	int				m_iFileMaxNum;

protected:
	void	LoadFromFile(const char* szFileName,__Vector3 CamPo);
	int		SetFile(int iTexIndex,unsigned char ucTexOrgIndex,__Vector3 CamPo);
	float	RandomGenf(float max, float min);
	void	ChkThisZoneUseGrass(int nGrassUseOrder);

	void	FindGrassIndex(unsigned char GrassIndex,int& nFineGrass,unsigned char& ucFineIndex);
	void	FindGrassIndex(const unsigned char uCGrassMngOrder,int* pnInputGrass,unsigned char* GrassIndex,int& nGrassTotNum);

	void	ChkTileRange(float fCamX,float fCamZ);	//	타일간움직임 채크,tick
	void	FineNewTile(WORD* Tile,int& iCount,float* ChkRange,float* LargeRange);

public:
	void	Init(__Vector3 CamPo);
	void	Tick(CGameProcedure* pProc);
	void	Render();
	void	Release();

	bool	ChangeZone();

	bool	IsInRect(float fPoX,float fPoY,float* fRange);	//	영역내에 있는지 확인
public:
	CGrassMng();
	virtual ~CGrassMng();
};

#endif // !defined(AFX_GRASS_H__3B6D6FD3_4213_40EE_A8E9_F50412B49EC2__INCLUDED_)
