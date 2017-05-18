// N3River2.h: interface for the CN3River2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3RIVER2_H__2CED45C5_8866_42BC_9677_9084A455DDFF__INCLUDED_)
#define AFX_N3RIVER2_H__2CED45C5_8866_42BC_9677_9084A455DDFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

#define MAX_RIVER_TEX	32

class CN3River2 : public CN3Base  
{
public:
	CN3River2();
	virtual ~CN3River2();

	struct __VertexRiver
	{
	public:
		float x,y,z;
		float nx,ny,nz;
		D3DCOLOR	color;
		float u,v, u2,v2;
		void Set(float sx, float sy,float sz, float snx, float sny, float snz, D3DCOLOR scolor, float su, float sv, float su2, float sv2)
		{
			x = sx, y = sy, z = sz;
			nx = snx, y = sny, z = sny;
			color = scolor;
			u = su, v = sv;
			u2 = su2, v2 = sv2;
		}
	};

	struct _RIVER_DIFF
	{
		float	fDiff;
		float	fWeight;
	};

	struct _RIVER_INFO
	{
		int iVC;
		int iIC;
		__VertexRiver		*pVertices;
		uint16_t				*pwIndex;
		_RIVER_DIFF			*pDiff;

		BOOL				m_bTick2Rand;
		__Vector3			m_vCenterPo;	//	강의 중간지점
		float				m_fRadius;		//	강의 지름

		_RIVER_INFO() {
			pVertices = NULL; pwIndex = NULL; pDiff = NULL;
		}
		~_RIVER_INFO() {
			if (pVertices)
				delete[] pVertices, pVertices = NULL;
			if (pwIndex)
				delete[] pwIndex, pwIndex = NULL;
			if (pDiff)
				delete[] pDiff, pDiff = NULL;
		}
	};

protected:
	_RIVER_INFO			*m_pRiverInfo;
	int					m_iRiverCount;

	CN3Texture			*m_pTexRiver[MAX_RIVER_TEX];
	CN3Texture			*m_pTexWave;


	float				m_fTexIndex;
	void				UpdateWaterPositions();
public:
	virtual bool	Load(HANDLE hFile);
	void			Render();
	void			Tick();
	void			Init(const std::string& TexPath);
};

#endif // !defined(AFX_N3RIVER2_H__2CED45C5_8866_42BC_9677_9084A455DDFF__INCLUDED_)
