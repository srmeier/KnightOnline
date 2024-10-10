#ifndef __N3ENG_H_
#define __N3ENG_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_net.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#include "ddraw.h"
#include "N3Base.h"
#include "N3BaseFileAccess.h"

//-----------------------------------------------------------------------------
/*!
This class is used for initializing and configuring DirectX
*/
class CN3Eng : public CN3Base
{
public:
	typedef struct __D3DDEV_INFO
	{
		char szDeviceName[128];
		char szDeviceDesc[128];
		int  nAdapter;
		int  nDevice;
		
		D3DDEVTYPE      DevType;
		int             nModeCount;
		D3DDISPLAYMODE* pModes;
	} __D3DDevInfo;

protected:
	int           m_nModeActive;
	int           m_nAdapterCount;
	__D3DDEV_INFO m_DeviceInfo;

public:
	LPDIRECT3D9  m_lpD3D;
	LPDIRECTDRAW m_lpDD;

public:
	void Release(void);
	void SetViewPort(RECT& pRC);
	void SetDefaultEnvironment(void);
	void LookAt(__Vector3& vEye, __Vector3& vAt, __Vector3& vUp);
	bool Reset(bool bWindowed, Uint32 dwWidth, Uint32 dwHeight, Uint32 dwBPP);
	void SetProjection(float fNear, float fFar, float fLens, float fAspect);

	static void ClearAuto(RECT* pRC = NULL);
	static void ClearZBuffer(const RECT* pRC = NULL);
	static void Clear(D3DCOLOR crFill, RECT* pRC = NULL);
	static void Present(HWND hWnd, RECT* pRC = NULL);

	bool Init(
		BOOL bWindowed, SDL_Window* pWindow,
		uint32_t dwWidth, uint32_t dwHeight, uint32_t dwBPP, BOOL bUseHW
	);

	BOOL FindDepthStencilFormat(
		UINT iAdapter, D3DDEVTYPE DeviceType,
		D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat
	);

public:
	CN3Eng(void);
	virtual ~CN3Eng(void);
};

#endif
