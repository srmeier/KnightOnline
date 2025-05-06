#pragma once

#include <wincrypt.h>

class CWinCrypt
{
public:
	constexpr const static TCHAR Provider[]	= MS_ENHANCED_PROV;
	constexpr const static char Cipher[]	= "owsd9012%$1as!wpow1033b%!@%12";

	inline BOOL IsLoaded() const {
		return m_bIsLoaded;
	}

	CWinCrypt();
	~CWinCrypt();
	bool Load();
	void Release();

	BOOL ReadFile(
		HANDLE hFile,
		LPVOID lpBuffer,
		DWORD nNumberOfBytesToRead,
		LPDWORD lpNumberOfBytesRead,
		LPOVERLAPPED lpOverlapped);

protected:
	BOOL ReadFileImpl(
		HANDLE hFile,
		LPVOID lpBuffer,
		DWORD nNumberOfBytesToRead,
		LPDWORD lpNumberOfBytesRead,
		LPOVERLAPPED lpOverlapped);

protected:
	BOOL		m_bIsLoaded;
	HCRYPTPROV	m_hCryptProvider;
	HCRYPTHASH	m_hCryptHash;
	HCRYPTKEY	m_hCryptKey;
};
