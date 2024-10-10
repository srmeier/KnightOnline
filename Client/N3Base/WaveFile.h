//////////////////////////////////////////////////
//	Commented By : Lynus
//	Commented On 2001-04-12 오후 1:51:00
//
//	CWave Class (WaveFile.h)
//
//	End Of Comment (By Lynus On 2001-04-12 오후 1:51:00 )
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//	Coded By : Lynus
//	Coded On 2001-04-12 오후 1:50:14
//

#ifndef __WAVE_H_
#define __WAVE_H_

#include <Windows.h>
#include "shared/types.h"

#include <mmsystem.h>
#include <mmreg.h>
#include "dxerr.h"

#define WAVEFILE_READ	1
#define WAVEFILE_WRITE	0

class CWaveFile
{
public:
    WAVEFORMATEX* m_pwfx;        // Pointer to WAVEFORMATEX structure
    HMMIO         m_hmmio;       // MM I/O handle for the WAVE
    MMCKINFO      m_ck;          // Multimedia RIFF chunk
    MMCKINFO      m_ckRiff;      // Use in opening a WAVE file
    uint32_t      m_dwSize;      // The size of the wave file
    MMIOINFO      m_mmioinfoOut;
    uint32_t      m_dwFlags;
    BOOL          m_bIsReadingFromMemory;
    uint8_t*      m_pbData;
    uint8_t*      m_pbDataCur;
    ULONG         m_ulDataSize;

protected:
    HRESULT ReadMMIO();
    HRESULT WriteMMIO( WAVEFORMATEX *pwfxDest );

public:
    CWaveFile();
    ~CWaveFile();

    HRESULT Open( LPCSTR strFileName, WAVEFORMATEX* pwfx, uint32_t dwFlags );
    HRESULT OpenFromMemory( uint8_t* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, uint32_t dwFlags );
    HRESULT Close();

    HRESULT Read( uint8_t* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead );
    HRESULT Write( UINT nSizeToWrite, uint8_t* pbData, UINT* pnSizeWrote );

    uint32_t   GetSize();
    HRESULT ResetFile();
    WAVEFORMATEX* GetFormat() { return m_pwfx; };
};

#endif // #ifndef __WAVE_H_
//	End Of Code (By Lynus On 2001-04-12 오후 1:50:14 )
//////////////////////////////////////////////////
