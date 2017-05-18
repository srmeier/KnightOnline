// N3SndEng.cpp: implementation of the CN3SndEng class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3SndEng.h"
#include "N3SndObj.h"
#include "N3Snd3dObj.h"
#include <DSound.h>
#include "WaveFile.h"
#include "LogWriter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3SndEng::CN3SndEng()
{
	m_pDS = NULL;

	m_pDSListener = NULL;			// 3D listener object
	m_Tick = false;
}

CN3SndEng::~CN3SndEng()
{
	Release();
}


//
//	Init
//	엔진 초기화...
//
bool CN3SndEng::Init( HWND  hWnd, DWORD dwCoopLevel, DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate )
{
    HRESULT hr;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

	if(m_pDS)
	{
		m_pDS->Release();
		m_pDS = NULL;
	}

    // Create IDirectSound using the primary sound device
    if( FAILED( hr = DirectSoundCreate8( NULL, &m_pDS, NULL ) ) )
        return false;

    // Set DirectSound coop level 
    if( FAILED( hr = m_pDS->SetCooperativeLevel( hWnd, dwCoopLevel ) ) )
        return false;
    
    // Set primary buffer format
    SetPrimaryBufferFormat( dwPrimaryChannels, dwPrimaryFreq, dwPrimaryBitRate );

	// Set listener 
	Get3DListenerInterface(&m_pDSListener);
	m_dsListenerParams.dwSize = sizeof(DS3DLISTENER);
	m_pDSListener->GetAllParameters( &m_dsListenerParams );
	m_pDSListener->SetRolloffFactor( DS3D_DEFAULTROLLOFFFACTOR/2, DS3D_IMMEDIATE);
	m_pDSListener->SetDopplerFactor(0, DS3D_DEFERRED);
	m_Tick = true;
	
    return true;
}


//
//
//
bool CN3SndEng::SetPrimaryBufferFormat( DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate )
{
    HRESULT             hr;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

    if( m_pDS == NULL )
        return false;

    // Get the primary buffer 
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D | DSBCAPS_MUTE3DATMAXDISTANCE;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
       
    if( FAILED( hr = m_pDS->CreateSoundBuffer( &dsbd, &pDSBPrimary, NULL ) ) ) return false;

    WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
    wfx.wFormatTag      = WAVE_FORMAT_PCM; 
    wfx.nChannels       = (WORD) dwPrimaryChannels; 
    wfx.nSamplesPerSec  = dwPrimaryFreq; 
    wfx.wBitsPerSample  = (WORD) dwPrimaryBitRate; 
    wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    if( FAILED( hr = pDSBPrimary->SetFormat(&wfx) ) ) return false;

	//pDSBPrimary->SetVolume(0);

	if(pDSBPrimary)
	{
		pDSBPrimary->Release();
		pDSBPrimary = NULL;
	}

    return true;
}


//
//	Release..
//
void CN3SndEng::Release()
{
	if(m_pDSListener)
	{
		m_pDSListener->Release();
		m_pDSListener = NULL;
	}
	if(m_pDS)
	{
		m_pDS->Release();
		m_pDS = NULL;
	}
}


//
//
//
bool CN3SndEng::LoadSource(LPSOUNDSOURCE pSrc)
{
	if(pSrc->Type==SNDTYPE_STREAM) return true;
	
	CWaveFile	WaveFile;
	HRESULT hr = WaveFile.Open(pSrc->szFN.c_str(), NULL, 1);	//#define WAVEFILE_READ   1
	if(FAILED(hr))
	{
#ifdef _N3GAME
		CLogWriter::Write("CN3SndEng::LoadSource - WaveFile Open Failed.. (%)", pSrc->szFN.c_str());
#endif
		return false;
	}

	pSrc->Size = WaveFile.GetSize();

	DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
	dsbd.dwBufferBytes   = pSrc->Size;
	dsbd.lpwfxFormat     = WaveFile.m_pwfx;
	dsbd.dwFlags		 = DSBCAPS_CTRLVOLUME | DSBCAPS_STATIC;

	if(pSrc->Type==SNDTYPE_3D)	//3D 음원..
	{
		dsbd.dwFlags         = DSBCAPS_CTRL3D | DSBCAPS_MUTE3DATMAXDISTANCE | DSBCAPS_STATIC;
		dsbd.guid3DAlgorithm = DS3DALG_HRTF_FULL;
	}
	
	hr = m_pDS->CreateSoundBuffer( &dsbd, &(pSrc->pDSBuff), NULL );
	if(FAILED(hr))
	{
#ifdef _N3GAME
		CLogWriter::Write("CN3SndEng::LoadSource - CreateSoundBuffer Failed.. (%)", pSrc->szFN.c_str());
#endif
		return false;
	}

    if(!FillBufferWithSound( pSrc, &WaveFile))
	{
#ifdef _N3GAME
		CLogWriter::Write("CN3SndEng::LoadSource - FillBufferWithSound Failed.. (%)", pSrc->szFN.c_str());
#endif
		return false;
	}

	if(pSrc->pDSBuff) pSrc->pDSBuff->SetCurrentPosition(0);

	return true;
}


//
//
//
bool CN3SndEng::FillBufferWithSound(LPSOUNDSOURCE pSrc, CWaveFile* pWaveFile)
{
    HRESULT hr; 
    VOID*   pDSLockedBuffer      = NULL; // Pointer to locked buffer memory
    DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
    DWORD   dwWavDataRead        = 0;    // Amount of data read from the wav file 

    if( pSrc->pDSBuff == NULL ) return false;

    if( FAILED( hr = RestoreBuffer( pSrc->pDSBuff, NULL ) ) ) 
        return false;

    // Lock the buffer down
    if( FAILED( hr = pSrc->pDSBuff->Lock( 0, pSrc->Size, &pDSLockedBuffer, &dwDSLockedBufferSize, NULL, NULL, 0L ) ) )
        return false;

	pWaveFile->ResetFile();

    if( FAILED( hr = pWaveFile->Read( (BYTE*) pDSLockedBuffer, dwDSLockedBufferSize, &dwWavDataRead ) ) )
		return false;

    if( dwWavDataRead == 0 )
    {
        // Wav is blank, so just fill with silence
        FillMemory( (BYTE*) pDSLockedBuffer, dwDSLockedBufferSize, (BYTE)(pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
    }
    else if( dwWavDataRead < dwDSLockedBufferSize )
    {
       // just fill in silence 
        FillMemory( (BYTE*) pDSLockedBuffer + dwWavDataRead, 
                    dwDSLockedBufferSize - dwWavDataRead, 
                    (BYTE)(pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
    }

    // Unlock the buffer, we don't need it anymore.
    pSrc->pDSBuff->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );

    return true;
}


//
//
//
HRESULT CN3SndEng::RestoreBuffer( LPDIRECTSOUNDBUFFER pDSB, BOOL* pbWasRestored )
{
    HRESULT hr;

    if( pDSB == NULL )
        return CO_E_NOTINITIALIZED;
    if( pbWasRestored )
        *pbWasRestored = FALSE;

    DWORD dwStatus;
    if( FAILED( hr = pDSB->GetStatus( &dwStatus ) ) )
        return S_FALSE;

    if( dwStatus & DSBSTATUS_BUFFERLOST )
    {
        // Since the app could have just been activated, then
        // DirectSound may not be giving us control yet, so 
        // the restoring the buffer may fail.  
        // If it does, sleep until DirectSound gives us control.
        do 
        {
            hr = pDSB->Restore();
            if( hr == DSERR_BUFFERLOST )
                Sleep( 10 );
        }
        while( hr = pDSB->Restore() );

        if( pbWasRestored != NULL )
            *pbWasRestored = TRUE;

        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}


//
//
//
bool CN3SndEng::Get3DListenerInterface( LPDIRECTSOUND3DLISTENER* ppDSListener )
{
    HRESULT             hr;
    DSBUFFERDESC        dsbdesc;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

    if( ppDSListener == NULL )
        return false;
    if( m_pDS == NULL )
        return false;

    *ppDSListener = NULL;

    // Obtain primary buffer, asking it for 3D control
    ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_MUTE3DATMAXDISTANCE | DSBCAPS_CTRL3D;
    if( FAILED( hr = m_pDS->CreateSoundBuffer( &dsbdesc, &pDSBPrimary, NULL ) ) )
        return false;

    if( FAILED( hr = pDSBPrimary->QueryInterface( IID_IDirectSound3DListener, 
                                                  (VOID**)ppDSListener ) ) )
    {
       if(pDSBPrimary)
	   {
		   pDSBPrimary->Release();
		   pDSBPrimary = NULL;
	   }		   
	   return false;
    }

    if(pDSBPrimary)
	{
		pDSBPrimary->Release();
		pDSBPrimary = NULL;
	}

    return true;
}


//
//
//
void CN3SndEng::SetListenerPos(D3DVECTOR* pVPos, bool IsDeferred)
{
	if(IsDeferred)
	{
		m_pDSListener->SetPosition(pVPos->x, pVPos->y, pVPos->z, DS3D_DEFERRED);
		m_Tick = true;
		return;
	}
	else m_pDSListener->SetPosition(pVPos->x, pVPos->y, pVPos->z, DS3D_IMMEDIATE);
}


//
//
//
void CN3SndEng::SetListenerOrientation(D3DVECTOR* pVAt, D3DVECTOR* pVUp, bool IsDeferred)
{
	if(IsDeferred)
	{
		m_pDSListener->SetOrientation(pVAt->x, pVAt->y, pVAt->z, pVUp->x, pVUp->y, pVUp->z, DS3D_DEFERRED);
		m_Tick = true;
		return;
	}
	else m_pDSListener->SetOrientation(pVAt->x, pVAt->y, pVAt->z, pVUp->x, pVUp->y, pVUp->z, DS3D_IMMEDIATE);
}


//
//
//
void CN3SndEng::Tick()
{
	if(m_Tick)
	{
		m_pDSListener->CommitDeferredSettings();
		m_Tick = false;
	}
}


//
//
//
void CN3SndEng::SetRollOffFactor(D3DVALUE value, bool IsDeferred)
{
	if(IsDeferred)
	{
		m_pDSListener->SetRolloffFactor( value, DS3D_DEFERRED);
		m_Tick = true;
		return;
	}
	else m_pDSListener->SetRolloffFactor( value, DS3D_IMMEDIATE);
}


//
//
//
void CN3SndEng::DuplicateBuff(LPSOUNDSOURCE pSrc, CN3SndObj* pDest, D3DVECTOR* pPos)
{
	HRESULT hr;
	if(pDest->GetType()==SNDTYPE_2D)
	{
		if(pSrc->pDSBuff)
		{
			if(pDest->m_pDSBuff) pDest->m_pDSBuff->Release();
			hr = m_pDS->DuplicateSoundBuffer(pSrc->pDSBuff, &(pDest->m_pDSBuff));
		}

	}
	if(pDest->GetType()==SNDTYPE_3D)
	{
		if(pSrc->pDSBuff)
		{
			if(pDest->m_pDSBuff) pDest->m_pDSBuff->Release();
			hr = m_pDS->DuplicateSoundBuffer(pSrc->pDSBuff, &(pDest->m_pDSBuff));

			LPDIRECTSOUNDBUFFER* p2dBuff = pDest->GetBuff();
			DS3DBUFFER* pBuffParam = ((CN3Snd3dObj*)pDest)->Get3DBuffParams();
			LPDIRECTSOUND3DBUFFER* ppDS3DBuff = ((CN3Snd3dObj*)pDest)->Get3DBuff();

			if( (*p2dBuff) == NULL ) return;

			(*p2dBuff)->QueryInterface( IID_IDirectSound3DBuffer, (VOID**)ppDS3DBuff );

			pBuffParam->dwSize = sizeof(DS3DBUFFER);
			hr = (*ppDS3DBuff)->GetAllParameters( pBuffParam );

			memcpy( &(pBuffParam->vPosition), pPos, sizeof(D3DVECTOR) );
			
			if( (*ppDS3DBuff) )
			{
				((CN3Snd3dObj*)pDest)->SetMaxDistance(256);
				hr = (*ppDS3DBuff)->SetAllParameters( pBuffParam, DS3D_DEFERRED );
				m_Tick = true;
			}
		}
	}
}


//
//
//
void CN3SndEng::SetDopplerFactor(D3DVALUE factor)
{
	m_pDSListener->SetDopplerFactor(factor, DS3D_DEFERRED);
	m_Tick = true;
}