// N3SndObj.cpp: implementation of the CN3SndObj class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "N3SndMgr.h"
#include "N3SndObj.h"
#include "../N3Base/N3Base.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3SndObj2D::CN3SndObj()
{
	m_pDSBuff = NULL;
	m_bIsLoop = false;
	m_iVol = -1;
	m_iType = SNDTYPE_2D;
	m_szFileName = "";
	m_pRefMgr = NULL;

	m_fFadeInTime = 0;
	m_fFadeOutTime = 0;
	m_fStartDelayTime = 0;
	m_fTmpSecPerFrm = 0;
	m_ePlayState = SNDSTATE_STOP;

	m_iMaxVolume = 100;

	m_bUse = false;
}

CN3SndObj2D::~CN3SndObj()
{
	Release();
}


//
//	Initialize....
//
void CN3SndObj2D::Init()
{
	Release();

	m_bIsLoop = false;
	m_iVol = -1;
	m_iType = SNDTYPE_2D;
	m_pRefMgr = NULL;
	m_szFileName = "";

	m_fStartDelayTime = 0;
	m_fTmpSecPerFrm = 0;
	m_ePlayState = SNDSTATE_STOP;
	m_fFadeInTime = 0;
	m_fFadeOutTime = 0;

	m_iMaxVolume = 100;
	
	m_bUse = false;
}


//
//	Release...
//
void CN3SndObj2D::Release()
{
	if(m_pDSBuff)
	{
		m_pDSBuff->Stop();
		m_pDSBuff->Release();
		m_pDSBuff = NULL;
	}
}


//
//	SetVolume...
//	range : [0,100]
//
void CN3SndObj2D::SetVolume(int Vol)
{
	if(m_pDSBuff)
	{
		m_iVol = Vol;
		if(Vol==0)
		{
			m_pDSBuff->SetVolume(-10000);
			return;
		}

		float fVol = (float)(Vol) / 100.0f;
		
		long dwVol = (long)(log10(fVol) * 3000);	//데시벨 관련 소리조절식..
		m_pDSBuff->SetVolume(dwVol);
	}
}


//
//
//
bool CN3SndObj2D::IsPlaying()
{
	if(m_ePlayState != SNDSTATE_STOP) return true;

	DWORD dwStatus = 0;
	m_pDSBuff->GetStatus( &dwStatus );
	if((dwStatus & DSBSTATUS_PLAYING)==DSBSTATUS_PLAYING) return true;
	
	return false;
}


//
//
//
void CN3SndObj2D::Tick()
{
	if(m_ePlayState == SNDSTATE_STOP) return;

	m_fTmpSecPerFrm += CN3Base::s_fSecPerFrm;
	
	if(m_ePlayState==SNDSTATE_DELAY && m_fTmpSecPerFrm >= m_fStartDelayTime)
	{
		m_fTmpSecPerFrm = 0;
		m_ePlayState = SNDSTATE_FADEIN;
		RealPlay();
	}

	if(m_ePlayState==SNDSTATE_FADEIN) 
	{
		if(m_fTmpSecPerFrm >= m_fFadeInTime)
		{
			m_fTmpSecPerFrm = 0;
			m_ePlayState = SNDSTATE_PLAY;
			if(this->m_iType!=SNDTYPE_3D) SetVolume(m_iMaxVolume);
		}
		else
		{
			int vol = 0;
			if(this->m_iType!=SNDTYPE_3D)
			{
				if(m_fFadeInTime>0.0f) vol = ((m_fTmpSecPerFrm/m_fFadeInTime)*(float)m_iMaxVolume);
				SetVolume(vol);
			}			
		}
	}

	if(m_ePlayState==SNDSTATE_PLAY)
	{
		if(!m_bIsLoop) m_ePlayState = SNDSTATE_STOP;		
	}
	if(m_ePlayState==SNDSTATE_FADEOUT)
	{
		if(m_fTmpSecPerFrm >= m_fFadeOutTime)
		{
			m_fTmpSecPerFrm = 0;
			m_ePlayState = SNDSTATE_STOP;
			if(this->m_iType!=SNDTYPE_3D) SetVolume(0);

			HRESULT hr = m_pDSBuff->Stop();
		}
		else
		{
			//볼륨 점점 작게....
			if(this->m_iType!=SNDTYPE_3D)
			{
				int vol = 0;
				if(m_fFadeOutTime>0.0f)  vol = (((m_fFadeOutTime - m_fTmpSecPerFrm)/m_fFadeOutTime)*(float)m_iMaxVolume);
				SetVolume(vol);
			}			
		}
	}
}


//
//
//
void CN3SndObj2D::Play(const D3DVECTOR* pvPos, float delay, float fFadeInTime, bool bImmediately)
{
	this->SetPos(pvPos);
	if(bImmediately) this->Stop();
	m_fFadeInTime = fFadeInTime;
	m_fFadeOutTime = 0;
	m_fStartDelayTime = delay;
	m_fTmpSecPerFrm = 0;
	m_ePlayState = SNDSTATE_DELAY;
}


//
//
//
void CN3SndObj2D::RealPlay()
{
    if(m_pDSBuff == NULL) return;

	DWORD dwStatus = 0;
	m_pDSBuff->GetStatus( &dwStatus );
	if((dwStatus & DSBSTATUS_PLAYING)==DSBSTATUS_PLAYING) return;

	HRESULT hr;
	hr = m_pDSBuff->SetCurrentPosition(0);

	if(m_bIsLoop) m_pDSBuff->Play(0,0,DSBPLAY_LOOPING);
	else hr |= m_pDSBuff->Play( 0, 0, 0 );

	return; 
}


//
//
//
void CN3SndObj2D::Stop(float fFadeOutTime)
{
	if( m_pDSBuff == NULL ) return;
	if( m_ePlayState == SNDSTATE_FADEOUT || m_ePlayState == SNDSTATE_STOP) return; 

	if(fFadeOutTime==0.0f)
	{
		m_fTmpSecPerFrm = 0;
		m_fFadeOutTime = fFadeOutTime;
		m_ePlayState = SNDSTATE_STOP;
		HRESULT hr = m_pDSBuff->Stop();
		return;
	}

	m_fTmpSecPerFrm = 0;
	m_fFadeOutTime = fFadeOutTime;
	m_ePlayState = SNDSTATE_FADEOUT;
	return;
}