// Map.h: interface for the CMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DMAP_H__986E02B6_E5A3_43CF_B1D7_A7135551933D__INCLUDED_)
#define AFX_3DMAP_H__986E02B6_E5A3_43CF_B1D7_A7135551933D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3VMesh.h"
#include "My_3DStruct.h"

class CRegion;

class C3DMap
{
public:
	bool ObjectIntersect( float x, float z, float y );
	float GetHeight( float x, float z );
	void SetRegion();
	void LoadTerrain( FILE* hFile );
	bool LoadMap( FILE* hFile );
	C3DMap();
	virtual ~C3DMap();

private:
	CN3VMesh*	m_pVMesh;
	CRegion**	m_ppRegion;

	int			m_nMapSize;		// Grid Unit ex) 4m
	float		m_fUnitDist;	// i Grid Distance
	int			m_nVMeshCount;
	__Vector3*	m_vNormal;
	float*		m_fHeight;

	int			m_nXRegion;
	int			m_nZRegion;
};

#endif // !defined(AFX_3DMAP_H__986E02B6_E5A3_43CF_B1D7_A7135551933D__INCLUDED_)
