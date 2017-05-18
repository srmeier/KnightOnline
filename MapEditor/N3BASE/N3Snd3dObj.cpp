// N3Snd3dObj.cpp: implementation of the CN3Snd3dObj class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "N3Snd3dObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Snd3dObj::CN3Snd3dObj()
{
	m_pDSBuff = NULL;
	m_pDS3DBuff = NULL;
}

CN3Snd3dObj::~CN3Snd3dObj()
{
	Release();
}


//
//
//
void CN3Snd3dObj::Release()
{
	CN3SndObj::Release();

	if(m_pDS3DBuff)
	{
		m_pDS3DBuff->Release();
		m_pDS3DBuff = NULL;
	}	
}


//
//
//
void CN3Snd3dObj::Init()
{
	Release();
	CN3SndObj::Init();
	
	m_pDS3DBuff = NULL;
}


//
//
//
void CN3Snd3dObj::SetPos(const D3DVECTOR* pvPos)
{
    if( m_pDS3DBuff && pvPos ) 
		HRESULT hr = m_pDS3DBuff->SetPosition(pvPos->x, pvPos->y, pvPos->z, DS3D_IMMEDIATE );
}


//
//
//
void CN3Snd3dObj::SetMaxDistance(D3DVALUE max)
{
	if( m_pDS3DBuff )	m_pDS3DBuff->SetMaxDistance(max, DS3D_IMMEDIATE);
}


//
//
//
void CN3Snd3dObj::SetMinDistance(D3DVALUE min)
{
	if( m_pDS3DBuff )	m_pDS3DBuff->SetMinDistance(min, DS3D_IMMEDIATE);
}


//
//
//
void CN3Snd3dObj::SetConeOutSizeVolume(LONG vol)
{
	if( m_pDS3DBuff )	m_pDS3DBuff->SetConeOutsideVolume(vol, DS3D_IMMEDIATE);
}


//
//
//
void CN3Snd3dObj::SetConeOrientation(D3DVECTOR* pDir)
{
	if( m_pDS3DBuff )	m_pDS3DBuff->SetConeOrientation(pDir->x, pDir->y, pDir->z, DS3D_IMMEDIATE);
}


