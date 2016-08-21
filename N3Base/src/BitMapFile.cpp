// BitMapFile.cpp: implementation of the CBitMapFile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "BitMapFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitMapFile::CBitMapFile()
{
	m_pPixels = NULL;
	Release();
}

CBitMapFile::~CBitMapFile()
{
	Release();
}

void CBitMapFile::Release()
{
	memset(&m_bmfHeader, 0, sizeof(m_bmfHeader));
	memset(&m_bmInfoHeader, 0, sizeof(m_bmInfoHeader));
	::GlobalFree(m_pPixels); // 실제 픽셀 데이터
	m_pPixels = NULL;
}

bool CBitMapFile::Load(HANDLE hFile)
{
	this->Release(); // 일단 다 해제하고..

	DWORD dwRWC = 0;

	// 파일 헤더 읽기
	ReadFile(hFile, &m_bmfHeader, sizeof(m_bmfHeader), &dwRWC, NULL);

	// bmp 파일임을 나타내는 "BM"마커 확인
	if (m_bmfHeader.bfType != 0x4D42)
	{
		MessageBox(::GetActiveWindow(), "원본 파일이 bitmap파일이 아닙니다.", "error", MB_OK);
		return FALSE;
	}

	// BITMAPINFOHEADER 얻기
	ReadFile(hFile, &m_bmInfoHeader, sizeof(m_bmInfoHeader), &dwRWC, NULL);

	// 픽셀당 비트 수 확인
	WORD wBitCount = m_bmInfoHeader.biBitCount;
	if (24 != wBitCount || m_bmInfoHeader.biWidth <= 0 || m_bmInfoHeader.biHeight <= 0)		// 24비트 bmp가 아니면 return해 버린다.
	{
		MessageBox(::GetActiveWindow(), "원본 bitmap이 너비, 높이에 이상이 있거나 24bit파일이 아닙니다.", "error", NULL);
		return FALSE;
	}

	// 실제 이미지의 메모리상에 잡힌 가로 길이 (24bit)
	int iRealWidth = ((int)((m_bmInfoHeader.biWidth*3 + 3)/4))*4;	

	// 새로 만들 이미지 메모리 할당
	int iDIBSize = iRealWidth * m_bmInfoHeader.biHeight;

	if ((m_pPixels = ::GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, iDIBSize )) == NULL )
	{
		MessageBox(::GetActiveWindow(), "메모리를 할당하지 못했습니다.", "error", MB_OK);
		return FALSE;
	}

	// 픽셀을 읽는다..
	for(int y = m_bmInfoHeader.biHeight - 1; y >= 0; y--) // 비트맵은 위아래가 거꾸로 있다..
	{
		ReadFile(hFile, (BYTE*)m_pPixels + y * iRealWidth, iRealWidth, &dwRWC, NULL);
	}

	return TRUE;
}

void* CBitMapFile::Pixels(int x, int y)
{
	if(24 != m_bmInfoHeader.biBitCount) return NULL;

	int nPitch = this->Pitch();
	int nPixelSize = 3;
	return (char*)m_pPixels + y * nPitch + x * nPixelSize;
}

bool CBitMapFile::Save(HANDLE hFile)
{
	DWORD dwRWC = 0;

	// 파일 헤더 쓰기
	WriteFile(hFile, &m_bmfHeader, sizeof(m_bmfHeader), &dwRWC, NULL);

	// BITMAPINFOHEADER 쓰기
	WriteFile(hFile, &m_bmInfoHeader, sizeof(m_bmInfoHeader), &dwRWC, NULL);

	// 실제 이미지의 메모리상에 잡힌 가로 길이 (24bit)
	int iRealWidth = this->Pitch();

	// 픽셀을 저장한다...
	for(int y = m_bmInfoHeader.biHeight - 1; y >= 0; y--) // 비트맵은 위아래가 거꾸로 있다..
	{
		WriteFile(hFile, (BYTE*)m_pPixels + y * iRealWidth, iRealWidth, &dwRWC, NULL);
	}

	return true;
}

bool CBitMapFile::SaveRectToFile(const std::string& szFN, RECT rc)
{
	if(szFN.empty()) return false;

	if(rc.right <= rc.left) return false;
	if(rc.bottom <= rc.top) return false;
	if(rc.left < 0) rc.left = 0;
	if(rc.top < 0) rc.top = 0;
	if(rc.right > m_bmInfoHeader.biWidth) rc.right = m_bmInfoHeader.biWidth;
	if(rc.bottom > m_bmInfoHeader.biHeight) rc.bottom = m_bmInfoHeader.biHeight;

	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;

	if (nWidth <=0 || nHeight <=0)
	{
		MessageBox(::GetActiveWindow(), "가로 세로가 0이하인 bitmap으로 저장할수 없습니다.", "error", MB_OK);
		return FALSE;
	}

	DWORD dwRWC = 0;
	HANDLE hFile = ::CreateFile(szFN.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// 쓰기 모드로 파일 열기
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(::GetActiveWindow(), "원본 bitmap을 열 수 없습니다.", "error", MB_OK);
		return false;
	}

	// 실제 이미지의 메모리상에 잡힌 가로 길이 (24bit)
	int iRealWidthDest = ((int)((nWidth*3 + 3)/4))*4;	
	int iDestDIBSize = sizeof(BITMAPINFOHEADER) + iRealWidthDest * nHeight;

	// 새로 만들 이미지 file header 정보 채우기
	BITMAPFILEHEADER bmfHeaderDest = m_bmfHeader;
	bmfHeaderDest.bfType = 0x4D42; // "BM"
	bmfHeaderDest.bfSize = sizeof(bmfHeaderDest) + iDestDIBSize;
	bmfHeaderDest.bfOffBits = sizeof(bmfHeaderDest) + sizeof(BITMAPINFOHEADER);

	// 새로 만들 이미지 bitmap info header 정보 채우기
	BITMAPINFOHEADER bmInfoHeaderDest = m_bmInfoHeader;
	bmInfoHeaderDest.biSize = sizeof(bmInfoHeaderDest);
	bmInfoHeaderDest.biWidth = nWidth;
	bmInfoHeaderDest.biHeight = nHeight;
	bmInfoHeaderDest.biPlanes = 1;
	bmInfoHeaderDest.biSizeImage = iRealWidthDest * nHeight;

	// 파일 헤더 쓰기
	WriteFile(hFile, &bmfHeaderDest, sizeof(bmfHeaderDest), &dwRWC, NULL);

	// BITMAPINFOHEADER 쓰기
	WriteFile(hFile, &bmInfoHeaderDest, sizeof(bmInfoHeaderDest), &dwRWC, NULL);

	// 픽셀을 저장한다...
	int iRealWidth = ((int)((m_bmInfoHeader.biWidth*3 + 3)/4))*4;	
	for(int y = rc.bottom - 1; y >= rc.top; y--)
	{
		void* pPixelDest = ((byte*)m_pPixels) + iRealWidth * y + (rc.left * 3);
		WriteFile(hFile, pPixelDest, iRealWidthDest, &dwRWC, NULL); // 라인 쓰기..
	}

	CloseHandle(hFile);
	return true;
}

bool CBitMapFile::LoadFromFile(const char* pszFN)
{
	if(NULL == pszFN || lstrlen(pszFN) <= 0)
		return false;

	DWORD dwRWC = 0;
	HANDLE hFile = ::CreateFile(pszFN, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(INVALID_HANDLE_VALUE == hFile)
		return false;

	bool bSuccess =	this->Load(hFile);

	CloseHandle(hFile);

	return bSuccess;
}

bool CBitMapFile::SaveToFile(const char* pszFN)
{
	if(NULL == pszFN || lstrlen(pszFN) <= 0)
		return false;

	DWORD dwRWC = 0;
	HANDLE hFile = ::CreateFile(pszFN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
		return false;

	this->Save(hFile);

	CloseHandle(hFile);
	return true;
}

bool CBitMapFile::Create(int nWidth, int nHeight, int nBPP)
{
	if(nWidth <= 0 || nHeight <= 0) return false;
	this->Release(); // 일단 다 해제하고..

	if(24 != nBPP) return FALSE;

	int iRealWidth = ((nWidth*3 + 3)/4)*4; // 실제 이미지의 메모리상에 잡힌 가로 길이 (24bit)
	int iDIBSize = iRealWidth * nHeight; // 새로 만들 이미지 메모리 할당

	if ((m_pPixels = ::GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, iDIBSize )) == NULL )
	{
		MessageBox(::GetActiveWindow(), "메모리를 할당하지 못했습니다.", "error", MB_OK);
		return FALSE;
	}

	memset(m_pPixels, 0, iDIBSize);

	// 새로 만들 이미지 file header 정보 채우기
	m_bmfHeader.bfType = 0x4D42; // "BM"
	m_bmfHeader.bfSize = sizeof(m_bmfHeader) + iDIBSize;
	m_bmfHeader.bfOffBits = sizeof(m_bmfHeader) + sizeof(BITMAPINFOHEADER);

	// 새로 만들 이미지 bitmap info header 정보 채우기
	m_bmInfoHeader.biSize = sizeof(m_bmInfoHeader);
	m_bmInfoHeader.biWidth = nWidth;
	m_bmInfoHeader.biHeight = nHeight;
	m_bmInfoHeader.biBitCount = nBPP;
	m_bmInfoHeader.biPlanes = 1;
	m_bmInfoHeader.biSizeImage = iRealWidth * nHeight;

	return TRUE;
}
