// N3FXShape.h: interface for the CN3FXShape class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __N3FXSHAPE_H__
#define __N3FXSHAPE_H__

#pragma warning(disable : 4786)

#include "N3TransformCollision.h"
#include "N3Texture.h"
#include "N3FXPMesh.h"
#include "N3FXPMeshInstance.h"

#include <vector>

class CN3FXSPart : public CN3BaseFileAccess
{
friend class CN3FXShape;

public:
	__Material	m_Mtl;					// Material
	__Vector3	m_vPivot;				// Local 축
	__Matrix44	m_WorldMtx;				// World Matrix.. Shape Loading 때 미리 계산해야 좋다..		
	BOOL		m_bOutOfCameraRange;	// Camera 범위 바깥에 있음...

	float		m_fTexFPS;				// Texture Animation Interval;
	bool		m_bTexLoop;

	CN3FXShape*	m_pRefShape;

protected:
	std::vector<CN3Texture*>	m_TexRefs;		// Texture Reference Pointers
	CN3FXPMeshInstance			m_FXPMInst;		// Progressive Mesh Instance

	float						m_fTexIndex;	// Current Texture Index.. Animation 시킬때 필요한 인덱스이다.. float 로 해서 텍스처 에니메이션 제어한다.

public:
	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);
	void Duplicate(CN3FXSPart* pSrc);
		
	int			TexCount() { return m_TexRefs.size(); }
	CN3Texture* Tex(int iIndex);	
	void		TexAlloc(int m_nCount);
	CN3Texture*	TexSet(int iIndex, const std::string& szFN);
	void		TexSet(int iIndex, CN3Texture* pTex);

	__Vector3 Min() { if(m_FXPMInst.GetMesh()) return m_FXPMInst.GetMesh()->Min() * m_WorldMtx; else return __Vector3(0,0,0); } // 월드 상의 최소값
	__Vector3 Max() { if(m_FXPMInst.GetMesh()) return m_FXPMInst.GetMesh()->Max() * m_WorldMtx; else return __Vector3(0,0,0); } // 월드 상의 최대값
	float	Radius() { if(m_FXPMInst.GetMesh()) return m_FXPMInst.GetMesh()->Radius(); else return 0.0f; }


	CN3FXPMesh*	Mesh() { return m_FXPMInst.GetMesh(); }
	__VertexXyzColorT1* GetColorVertices() { return m_FXPMInst.GetVertices(); }
	void	SetColor(DWORD dwColor = 0xffffffff) { m_FXPMInst.SetColor(dwColor); }
	bool	MeshSet(const std::string& szFN);
	void	Tick(const __Matrix44& mtxParent);
	void	Render();
	void	Release();
	
	CN3FXSPart();
	virtual ~CN3FXSPart();
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CN3FXShape
//
class CN3FXShape : public CN3TransformCollision
{
public:
	std::vector<CN3FXSPart*>	m_Parts; // Part Data Pointer Linked List

	__Matrix44		m_mtxParent;
	__Matrix44		m_mtxFinalTransform;

	DWORD			m_dwSrcBlend;
	DWORD			m_dwDestBlend;
	BOOL			m_bAlpha;

	DWORD			m_dwZEnable;
	DWORD			m_dwZWrite;
	DWORD			m_dwLight;
	DWORD			m_dwDoubleSide;
	
public:
	void			SetPartsMtl(BOOL bAlpha, DWORD dwSrcBlend, DWORD dwDestBlend, DWORD dwZEnable, DWORD dwZWrite, DWORD dwLight, DWORD dwDoubleSide);
	__Vector3		CenterPos();

	void			SetCurrFrm(float fFrm);
	float			GetCurrFrm();
	float			GetWholeFrm() { return m_fFrmWhole; }

	void			FindMinMax();

	CN3FXSPart*		Part(int iIndex) { if(iIndex < 0 || iIndex >= m_Parts.size()) return NULL; return m_Parts[iIndex]; }
	CN3FXSPart*		PartAdd() { CN3FXSPart* pPart = new CN3FXSPart(); m_Parts.push_back(pPart); return pPart; }
	int				PartCount() { return m_Parts.size(); }
	void			PartDelete(int iIndex);
	
	bool	Load(HANDLE hFile);
	bool	Save(HANDLE hFile);
	void	Duplicate(CN3FXShape* pSrc);

	void	Tick(float fFrm = FRAME_SELFPLAY);
	void	Render();
	void	Release();		

public:
	void SetMaxLOD();
	CN3FXShape();
	virtual ~CN3FXShape();
};

#endif // !defined(AFX_N3Shape_h__INCLUDED_)
