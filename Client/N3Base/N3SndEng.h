// N3SndEng.h: interface for the CN3SndEng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3SNDENG_H__0205273B_6697_4AB9_A33F_218534153AB3__INCLUDED_)
#define AFX_N3SNDENG_H__0205273B_6697_4AB9_A33F_218534153AB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3SndDef.h"

class CN3SndObj;

class CN3SndEng  
{
protected:
	LPDIRECTSOUND8			m_pDS;

	LPDIRECTSOUND3DLISTENER m_pDSListener;			// 3D listener object
	DS3DLISTENER            m_dsListenerParams;		// Listener properties

	bool					m_Tick;

protected:
	HRESULT RestoreBuffer( LPDIRECTSOUNDBUFFER pDSB, BOOL* pbWasRestored );
	bool SetPrimaryBufferFormat( uint32_t dwPrimaryChannels, uint32_t dwPrimaryFreq, uint32_t dwPrimaryBitRate );
	bool Get3DListenerInterface( LPDIRECTSOUND3DLISTENER* ppDSListener );

	bool FillBufferWithSound( LPSOUNDSOURCE pSrc, CWaveFile* pWaveFile);
	
public:
	LPDIRECTSOUND8*		GetDS() { return &m_pDS; }
	void SetDopplerFactor(D3DVALUE factor);
	
	void DuplicateBuff(LPSOUNDSOURCE pSrc, CN3SndObj* pDest, D3DVECTOR* pPos=NULL);
	
	void Tick();
	
	void SetRollOffFactor(D3DVALUE value, bool IsDeferred = true);
	void SetListenerOrientation(D3DVECTOR* pVAt, D3DVECTOR* pVUp, bool IsDeferred = true);
	void SetListenerPos(D3DVECTOR* pVPos, bool IsDeferred = true);
	
	bool LoadSource(LPSOUNDSOURCE pSrc);
	bool Init( HWND hWnd, uint32_t dwCoopLevel = DSSCL_PRIORITY, uint32_t dwPrimaryChannels = 2, uint32_t dwPrimaryFreq = 22050, uint32_t dwPrimaryBitRate = 16 );
	void Release();

	CN3SndEng();
	virtual ~CN3SndEng();

};

#endif // !defined(AFX_N3SNDENG_H__0205273B_6697_4AB9_A33F_218534153AB3__INCLUDED_)
