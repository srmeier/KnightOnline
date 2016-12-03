//////////////////////////////////////////////////
//	Commented By : Lynus
//	Commented On 2001-04-12 오후 8:42:02
//
//	CN3SndObjStream class (N3SndObjStream.h)
//
//	End Of Comment (By Lynus On 2001-04-12 오후 8:42:02 )
//////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3SndObjStream.h"
#include "N3SndMgr.h"
#include "N3Base.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CN3SndObjStream::CN3SndObjStream()
{
	m_WaveSize = NULL;
	
	m_CurrState = 0;
	m_CurrPos = 0;
	m_PrevState = 0;

	hMMIO = NULL;
	m_hWaveMem = NULL;
	m_pWaveMem = NULL;
}

CN3SndObjStream::~CN3SndObjStream()
{
	Release();
}

bool CN3SndObjStream::Create(const std::string& szFN, e_SndType eType)
{
	Release();

	if(NULL == s_lpDS) return false;
	if(SNDTYPE_STREAM != eType) return false;
	if(!LoadWave(szFN.c_str())) return false;

	m_szFileName = szFN;

	DWORD nBlockAlign = m_WaveFormat.nBlockAlign;
	DWORD BlockPerSec = m_WaveFormat.nSamplesPerSec * nBlockAlign; 

	m_PlayTime = m_PastTime = m_WaveSize/BlockPerSec;
	m_FinalByte = m_WaveSize%BlockPerSec;

	m_BlockSize = BlockPerSec;
	m_BufferSize = m_BlockSize*2;

	ZeroMemory(&m_dsbd, sizeof(m_dsbd));
	m_dsbd.dwSize = sizeof(DSBUFFERDESC);
    m_dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLVOLUME;
	m_dsbd.dwBufferBytes = m_BufferSize;
	m_dsbd.lpwfxFormat = &m_WaveFormat;

	if(m_lpDSBuff) m_lpDSBuff->Release();

	HRESULT	hResult;
	hResult = s_lpDS->CreateSoundBuffer(&m_dsbd, &m_lpDSBuff, NULL);
	if(FAILED(hResult)) return false;

	m_hWaveMem = ::GlobalAlloc(GHND, m_BlockSize);

	InitWriteBuffer();

	m_ePlayState = SNDSTATE_STOP;

	return true;
}


BOOL CN3SndObjStream::LoadWave(LPCSTR pFileName)
{
	hMMIO = mmioOpen((LPSTR)pFileName,NULL,MMIO_READ|MMIO_ALLOCBUF);
	if(hMMIO==NULL) return FALSE;

	mmCkInfoRIFF.fccType = mmioFOURCC('W','A','V','E');

	MMRESULT mmResult;
	mmResult = mmioDescend(hMMIO, &mmCkInfoRIFF, NULL, MMIO_FINDRIFF);
	if(mmResult != MMSYSERR_NOERROR) return FALSE;

	mmCkInfoChunk.ckid = mmioFOURCC('f','m','t',' ');

	mmResult = mmioDescend(hMMIO, &mmCkInfoChunk, &mmCkInfoRIFF, MMIO_FINDCHUNK);
	if(mmResult != MMSYSERR_NOERROR) return FALSE;

	if(mmioRead(hMMIO, (LPSTR)&m_WaveFormat,sizeof(WAVEFORMATEX))==-1) 
		return FALSE;

	mmResult = mmioAscend(hMMIO, &mmCkInfoChunk,0);
	if(mmResult != MMSYSERR_NOERROR) return FALSE;

	mmCkInfoChunk.ckid = mmioFOURCC('d','a','t','a');
	mmResult = mmioDescend(hMMIO, &mmCkInfoChunk, &mmCkInfoRIFF, MMIO_FINDCHUNK);
	if(mmResult != MMSYSERR_NOERROR) return FALSE;

	m_WaveSize = mmCkInfoChunk.cksize;
	return TRUE;
}

BOOL CN3SndObjStream::InitWriteBuffer()
{	
	LPVOID	pSoundBlock1 = NULL;
	LPVOID	pSoundBlock2 = NULL;
	DWORD	byteSoundBlock1 = 0;
	DWORD	byteSoundBlock2 = 0;
	DWORD	Offset = 0;
	
	HRESULT hResult;
	//ReadStream();
	
	hResult = m_lpDSBuff->Lock(Offset,m_BlockSize*2,&pSoundBlock1,&byteSoundBlock1,
							&pSoundBlock2,&byteSoundBlock2,0);
	if(FAILED(hResult)) return FALSE;

	if(mmioRead(hMMIO, (LPSTR)pSoundBlock1, m_BlockSize*2)==-1) return FALSE;

	m_lpDSBuff->Unlock(pSoundBlock1,byteSoundBlock1,pSoundBlock2,byteSoundBlock2);

	return TRUE;
}

BOOL CN3SndObjStream::WriteBuffer()
{	
	LPVOID	pSoundBlock1 = NULL;
	LPVOID	pSoundBlock2 = NULL;
	DWORD	byteSoundBlock1 = 0;
	DWORD	byteSoundBlock2 = 0;
	DWORD	Offset;
	
	HRESULT hResult;
	//ReadStream();
	//if(m_CurrState==0) Offset = 2*m_BlockSize; 
	//else Offset = (m_CurrState-1)*m_BlockSize;
	if(m_CurrState==0) Offset = m_BlockSize; 
	else Offset = (m_CurrState-1)*m_BlockSize;
	
	hResult = m_lpDSBuff->Lock(Offset,m_BlockSize,&pSoundBlock1,&byteSoundBlock1,
							&pSoundBlock2,&byteSoundBlock2,0);
	if(FAILED(hResult)) return FALSE;

	if(m_PastTime>=2)
	{
		if(mmioRead(hMMIO, (LPSTR)pSoundBlock1, m_BlockSize)==-1) return FALSE;
	}
	else 
	{
		FillMemory((LPSTR)pSoundBlock1, m_BlockSize, (BYTE)(m_WaveFormat.wBitsPerSample == 8 ? 128:0));
		if(m_PastTime==1)
		{
			//mmioSeek(hMMIO, m_FinalByte, SEEK_END);
			mmioRead(hMMIO, (LPSTR)pSoundBlock1, m_FinalByte);
		}
	}

	m_lpDSBuff->Unlock(pSoundBlock1,byteSoundBlock1,pSoundBlock2,byteSoundBlock2);

	return TRUE;
}

void CN3SndObjStream::RealPlay()
{
//^^	if(m_PlayState==0) Reset();

//^^	m_ePlayState = SNDSTATE_PLAY;
	
	if(m_lpDSBuff)
	{
		if(m_ePlayState==SNDSTATE_DELAY)
		{
			m_lpDSBuff->SetCurrentPosition(0);
			m_lpDSBuff->Play(0,0,DSBPLAY_LOOPING);			
		}

		m_lpDSBuff->GetCurrentPosition(&m_CurrPos,NULL);
		m_PrevState = m_CurrState;
		m_CurrState = m_CurrPos/m_BlockSize;
		if(m_CurrState != m_PrevState)
		{
			m_PastTime--;
			if(m_PastTime<0)
			{
				if(m_bIsLoop==false)
				{
					//tick에 어케 적용할 것인가..
					m_PastTime = m_PlayTime;
					Stop();
					return;
				}
				else if(m_bIsLoop==true)
				{
					//tick에 어케 적용할 것인가..
					//Stop();
					//m_lpDSBuff->Stop();
					Play();
					return;
				}				
			}
			WriteBuffer();
		}
	}
}

void CN3SndObjStream::Reset()
{
	m_PastTime = m_PlayTime;
	mmioSeek(hMMIO, 44, SEEK_SET);

	m_CurrPos = 0;
	m_CurrState = 0;
	InitWriteBuffer();
}

void CN3SndObjStream::Release()
{
	m_fTmpSecPerFrm = 0;
	if(m_lpDSBuff) HRESULT hr = m_lpDSBuff->Stop();
	m_fFadeOutTime = 0;
	m_ePlayState = SNDSTATE_STOP;

	CN3SndObj::Release();

	if(hMMIO) 
	{
		mmioClose(hMMIO,0);
		hMMIO = NULL;
	}
	if(m_hWaveMem)
	{
		::GlobalFree(m_hWaveMem);
		m_hWaveMem = NULL;
	}
}


//
//	음악이 플레이되고 있을때 streamming시키기..
//
void CN3SndObjStream::Tick()
{
	if(m_ePlayState == SNDSTATE_STOP) return;

	m_fTmpSecPerFrm += CN3Base::s_fSecPerFrm;
	
	if(m_ePlayState==SNDSTATE_DELAY && m_fTmpSecPerFrm >= m_fStartDelayTime)
	{
		m_fTmpSecPerFrm = 0;
		Reset();
		RealPlay();
		m_ePlayState = SNDSTATE_FADEIN;		
	}

	if(m_ePlayState==SNDSTATE_FADEIN) 
	{
		if(m_fTmpSecPerFrm >= m_fFadeInTime)
		{
			m_fTmpSecPerFrm = 0;
			m_ePlayState = SNDSTATE_PLAY;
			SetVolume(m_iMaxVolume);
		}
		else
		{
			int vol = 0;			
			if(m_fFadeInTime>0.0f) vol = ((m_fTmpSecPerFrm/m_fFadeInTime)*(float)m_iMaxVolume);
			SetVolume(vol);
		}
		RealPlay();
	}

	if(m_ePlayState==SNDSTATE_PLAY)
	{
		RealPlay();
	}
	if(m_ePlayState==SNDSTATE_FADEOUT)
	{
		if(m_fTmpSecPerFrm >= m_fFadeOutTime || m_PastTime<0)
		{
			m_fTmpSecPerFrm = 0;
			SetVolume(0);

			HRESULT hr = m_lpDSBuff->Stop();
			m_ePlayState = SNDSTATE_STOP;
		}
		else
		{
			//볼륨 점점 작게....
			int vol = 0;
			if(m_fFadeOutTime>0.0f)  vol = (((m_fFadeOutTime - m_fTmpSecPerFrm)/m_fFadeOutTime)*(float)m_iMaxVolume);
			SetVolume(vol);
			RealPlay();
		}
	}
}

/*
//
// 똑같다.
//
void CN3SndObjStream::Play(float delay, float fFadeInTime)
{
	m_fFadeInTime = fFadeInTime;
	m_fFadeOutTime = 0.0f;
	m_fStartDelayTime = delay;
	m_fTmpSecPerFrm = 0.0f;
	m_ePlayState = SNDSTATE_DELAY;
}


//
// 똑같다.
//
void CN3SndObjStream::Stop(float fFadeOutTime)
{
	if( m_lpDSBuff == NULL ) return;

	m_fTmpSecPerFrm = 0;
	m_fFadeOutTime = fFadeOutTime;
	m_ePlayState = SNDSTATE_FADEOUT;
	return;
}
*/
// end of N3SndObjStream.cpp