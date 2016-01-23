/*
*/

#include "StdAfx.h"
#include "UIChat.h"
#include "GameEng.h"
#include "Resource.h"
#include "N3SndMgr.h"
#include "N3UIEdit.h"
#include "PacketDef.h"
#include "APISocket.h"
#include "Warfaremain.h"
#include "PlayerMySelf.h"
#include "GameProcMain.h"
#include "KnightChrMgr.h"
#include "N3WorldManager.h"

#include "time.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_net.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#include "DFont.h"

//-----------------------------------------------------------------------------
int SDL_main(int argc, char** argv)
{	
	// NOTE: get the current directory and make it known to CN3Base
	char szPath[_MAX_PATH] = "";
	GetCurrentDirectory(_MAX_PATH, szPath);
	CN3Base::PathSet(szPath);

	// NOTE: we are anticipating an Options file to exist within this directory
	char szIniPath[_MAX_PATH] = "";
	lstrcpy(szIniPath, CN3Base::PathGet().c_str());
	lstrcat(szIniPath, "Option.Ini");

	// NOTE: what is the texture quality?
	CN3Base::s_Options.iTexLOD_Chr = GetPrivateProfileInt("Texture", "LOD_Chr", 0, szIniPath);
	if(CN3Base::s_Options.iTexLOD_Chr < 0) CN3Base::s_Options.iTexLOD_Chr = 0;
	if(CN3Base::s_Options.iTexLOD_Chr >= 2) CN3Base::s_Options.iTexLOD_Chr = 1;
	
	// NOTE: what is the texture quality?
	CN3Base::s_Options.iTexLOD_Shape = GetPrivateProfileInt("Texture", "LOD_Shape", 0, szIniPath);
	if(CN3Base::s_Options.iTexLOD_Shape < 0) CN3Base::s_Options.iTexLOD_Shape = 0;
	if(CN3Base::s_Options.iTexLOD_Shape >= 2) CN3Base::s_Options.iTexLOD_Shape = 1;

	// NOTE: what is the texture quality?
	CN3Base::s_Options.iTexLOD_Terrain = GetPrivateProfileInt("Texture", "LOD_Terrain", 0, szIniPath);
	if(CN3Base::s_Options.iTexLOD_Terrain < 0) CN3Base::s_Options.iTexLOD_Terrain = 0;
	if(CN3Base::s_Options.iTexLOD_Terrain >= 2) CN3Base::s_Options.iTexLOD_Terrain = 1;

	// NOTE: should we use shadows?
	CN3Base::s_Options.iUseShadow = GetPrivateProfileInt("Shadow", "Use", 1, szIniPath);

	// NOTE: what is the screen resolution?
	CN3Base::s_Options.iViewWidth = GetPrivateProfileInt("ViewPort", "Width", 1024, szIniPath);
	CN3Base::s_Options.iViewHeight = GetPrivateProfileInt("ViewPort", "Height", 768, szIniPath);	
	if(CN3Base::s_Options.iViewWidth == 1024) CN3Base::s_Options.iViewHeight = 768;
	else if(1280 == CN3Base::s_Options.iViewWidth) CN3Base::s_Options.iViewHeight = 1024;
	else if(1600 == CN3Base::s_Options.iViewWidth) CN3Base::s_Options.iViewHeight = 1200;
	else {
		CN3Base::s_Options.iViewWidth = 1024;
		CN3Base::s_Options.iViewHeight = 768;
	}

	// NOTE: what is the viewport's color depth?
	CN3Base::s_Options.iViewColorDepth = GetPrivateProfileInt("ViewPort", "ColorDepth", 16, szIniPath);
	if(CN3Base::s_Options.iViewColorDepth != 16 && CN3Base::s_Options.iViewColorDepth != 32)
		CN3Base::s_Options.iViewColorDepth = 32;

	// NOTE: what is the viewport's draw distance?
	CN3Base::s_Options.iViewDist = GetPrivateProfileInt("ViewPort", "Distance", 512, szIniPath);
	if(CN3Base::s_Options.iViewDist < 256) CN3Base::s_Options.iViewDist = 256;
	if(CN3Base::s_Options.iViewDist > 512) CN3Base::s_Options.iViewDist = 512;

	// NOTE: what is the distance for sound events?
	CN3Base::s_Options.iEffectSndDist = GetPrivateProfileInt("Sound", "Distance", 48, szIniPath);
	if(CN3Base::s_Options.iEffectSndDist < 20) CN3Base::s_Options.iEffectSndDist = 20;
	if(CN3Base::s_Options.iEffectSndDist > 48) CN3Base::s_Options.iEffectSndDist = 48;

	// NOTE: is sound enabled?
	int iSndEnable = GetPrivateProfileInt("Sound", "Enable", 1, szIniPath);
	CN3Base::s_Options.bSndEnable = (iSndEnable) ? true : false; // 사운드...

	// NOTE: is sound duplicated?
	int iSndDuplicate = GetPrivateProfileInt("Sound", "Duplicate", 0, szIniPath);
	CN3Base::s_Options.bSndDuplicated = (iSndDuplicate) ? true : false; // 사운드...

	// NOTE: should we show the window cursor?
	int iWindowCursor = GetPrivateProfileInt("Cursor", "WindowCursor", 1, szIniPath);
	CN3Base::s_Options.bWindowCursor = (iWindowCursor) ? true : false; // cursor...

	srand((unsigned int) time(NULL));

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "ER: %s\n", SDL_GetError());
		return false;
	}

	if(TTF_Init() == -1) {
		fprintf(stderr, "ER: %s\n", TTF_GetError());
		return false;
	}

	int flags = IMG_INIT_JPG|IMG_INIT_PNG;
	if((IMG_Init(flags)&flags) != flags) {
		fprintf(stderr, "ER: %s\n", IMG_GetError());
		return false;
	}

	flags = MIX_INIT_OGG|MIX_INIT_MP3;
	if((Mix_Init(flags)&flags) != flags) {
		fprintf(stderr, "ER: %s\n", Mix_GetError());
		return false;
	}

	if(SDLNet_Init() == -1) {
		fprintf(stderr, "ER: %s\n", SDLNet_GetError());
		return false;
	}

	SDL_Window* s_pSDLWindow = SDL_CreateWindow(
		"Knight Online",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		CN3Base::s_Options.iViewWidth,
		CN3Base::s_Options.iViewHeight,
		0
	);

	if(s_pSDLWindow == NULL) {
		fprintf(stderr, "ER: %s\n", SDL_GetError());
		return false;
	}

	CGameProcedure::s_bWindowed = true;

	// NOTE: allocate the static members
	CGameProcedure::StaticMemberInit(s_pSDLWindow);

	// NOTE: set the games current procedure to s_pProcLogIn
	//CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcLogIn);

	// TESTING ----------
	CGameBase::s_pPlayer->m_InfoBase.eRace = RACE_KA_ARKTUAREK;
	CGameBase::s_pPlayer->m_InfoBase.eNation = NATION_KARUS;
	CGameBase::s_pPlayer->m_InfoBase.eClass = CLASS_KA_WARRIOR;
	CGameBase::s_pPlayer->m_InfoExt.iZoneInit = 0x01;
	CGameBase::s_pPlayer->m_InfoExt.iZoneCur = 204;
	CGameBase::s_pPlayer->IDSet(-1, "testing", 0xffffffff);
	CGameBase::s_pPlayer->m_InfoBase.iAuthority == AUTHORITY_MANAGER;

	__TABLE_PLAYER_LOOKS* pLooks = CGameBase::s_pTbl_UPC_Looks->Find(CGameBase::s_pPlayer->m_InfoBase.eRace);
	CGameBase::s_pPlayer->InitChr(pLooks);

	CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcMain);
	//CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcCharacterSelect);
	// TESTING -----------

	BOOL bGotMsg = FALSE;
	MSG msg; memset(&msg, 0, sizeof(MSG));

	CGameBase::s_bRunning = true;

	while(CGameBase::s_bRunning) {
		CGameProcedure::TickActive();
		CGameProcedure::RenderActive();
	}

	CGameProcedure::StaticMemberRelease();

	return 0;
}
