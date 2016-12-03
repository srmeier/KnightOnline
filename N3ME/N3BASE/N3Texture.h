// N3Texture7.h: interface for the CN3Texture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3Texture_h__INCLUDED_)
#define AFX_N3Texture_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3BaseFileAccess.h"
#include <string>

class CN3Texture : public CN3BaseFileAccess
{
public:
	typedef struct __DXT_HEADER
	{
		char szID[4]; // "NTF"숫자 - Noah Texture File Ver. ?.0
		int nWidth;
		int nHeight;
		D3DFORMAT Format; // 0 - 압축 안함 1 ~ 5 : D3DFMT_DXT1 ~ D3DFMT_DXT5
		BOOL bMipMap; // Mip Map ??
	} __DxtHeader;

protected:
	__DXT_HEADER m_Header;
	LPDIRECT3DTEXTURE8 m_lpTexture;

public:
	void				UpdateRenderInfo();
	bool				LoadFromFile(const std::string& szFileName);
	bool				Load(HANDLE hFile);
	bool				SkipFileHandle(HANDLE hFile);

#ifdef _N3TOOL
	bool				GenerateMipMap(LPDIRECT3DSURFACE8 lpSurf = NULL); // NULL 이면 0 레벨의 서피스로부터 생성..
	bool				Convert(D3DFORMAT Format, int nWidth = 0, int nHeight = 0, BOOL bGenerateMipMap = TRUE);
//#ifdef _N3TOOL
	bool				SaveToFile(); // 현재 파일 이름대로 저장.
	bool				SaveToFile(const std::string& szFileName); // 새이름으로 저장.
	bool				Save(HANDLE hFile);
	bool				SaveToBitmapFile(const std::string& szFN); // 24비트 비트맵 파일로 저장..
	bool				CreateFromSurface(LPDIRECT3DSURFACE8 lpSurf, D3DFORMAT Format, BOOL bGenerateMipMap);
#endif // end of _N3TOOL

	DWORD				Width() { return m_Header.nWidth; }
	DWORD				Height() { return m_Header.nHeight; }
	D3DFORMAT			PixelFormat() { return m_Header.Format; }
	int					MipMapCount() { if(NULL == m_lpTexture) return 0; else return m_lpTexture->GetLevelCount(); }
	
	bool				Create(int nWidth, int nHeight, D3DFORMAT Format, BOOL bGenerateMipMap); // 장치에 맞게 생성
	LPDIRECT3DTEXTURE8	Get() { return m_lpTexture; }
	operator LPDIRECT3DTEXTURE8 () { return m_lpTexture; }
	
	void Release();
	CN3Texture();
	virtual ~CN3Texture();
};

#endif // !defined(AFX_N3Texture_h__INCLUDED_)
