//////////////////////////////////////////////////
//	Commented By : Lynus
//	Commented On 2001-04-12 오후 5:09:28
//
//	CStreamSoundObj class (SoundObj.h)
//
//	End Of Comment (By Lynus On 2001-04-12 오후 5:09:28 )
//////////////////////////////////////////////////

#ifndef __STREAMSOUNDOBJ_H_
#define __STREAMSOUNDOBJ_H_

#include "N3SndObj.h"

class CN3SndEng;

class CStreamSoundObj : public CN3SndObj
{
	private:
		int					m_PlayTime;		//wave파일의 전체 플레이 타임.
		int					m_FinalByte;
				
		DSBUFFERDESC		m_dsbd;
		uint32_t				m_BufferSize;
		uint32_t				m_BlockSize;	//buffer를 초단위로 나눈 것.
		
		int					m_CurrState;	//현재 어느 블럭에 있는지..
		int					m_PrevState;	//지금 바로 전에 어느 블럭에 있었는지..
		uint32_t				m_CurrPos;		//현재 버퍼에서의 위치 
		int					m_PastTime;		//전체 플레이 타임에서 남은 시간

		//related wave data
		WAVEFORMATEX		m_WaveFormat;
		uint32_t				m_WaveSize;
		LPSTR				m_pWaveMem;
		HGLOBAL				m_hWaveMem;
		MMCKINFO			mmCkInfoRIFF;
		MMCKINFO			mmCkInfoChunk;
		HMMIO				hMMIO;
		
	public:
		BOOL Create(CN3SndEng* pEng);

//		void Play(float delay = 0.0f, float fFadeInTime = 0.0f);
//		void Stop(float fFadeOutTime = 0.0f);		
		
		void Tick();		
		void Release();
		
		CStreamSoundObj();
		virtual ~CStreamSoundObj();
				
	private:
		void RealPlay();
		
		//void Stop();

		BOOL LoadWave(LPCSTR pFileName);
		BOOL WriteBuffer();
		BOOL InitWriteBuffer();
		void Reset();
};

#endif // #ifndef __STREAMSOUNDOBJ_H_