/*
*/

//#include "StdAfx.h"
#include "UIChat.h"
#include "GameEng.h"
#include "resource.h"
#include "N3SndMgr.h"
#include "N3UIEdit.h"
#include "PacketDef.h"
#include "APISocket.h"
#include "PlayerMySelf.h"
#include "GameProcMain.h"
#include "N3WorldManager.h"
#include "../Server/shared/Ini.h"

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
	lstrcat(szIniPath, "Option.ini");

	CIni * s_CIni = new CIni(szIniPath); // Lets read Option.ini

	// NOTE: what is the texture quality?
	CN3Base::s_Options.iTexLOD_Chr = s_CIni->GetInt("Texture", "LOD_Chr", 0);
	if(CN3Base::s_Options.iTexLOD_Chr < 0) CN3Base::s_Options.iTexLOD_Chr = 0;
	if(CN3Base::s_Options.iTexLOD_Chr >= 2) CN3Base::s_Options.iTexLOD_Chr = 1;
	
	// NOTE: what is the texture quality?
	CN3Base::s_Options.iTexLOD_Shape = s_CIni->GetInt("Texture", "LOD_Shape", 0);
	if(CN3Base::s_Options.iTexLOD_Shape < 0) CN3Base::s_Options.iTexLOD_Shape = 0;
	if(CN3Base::s_Options.iTexLOD_Shape >= 2) CN3Base::s_Options.iTexLOD_Shape = 1;

	// NOTE: what is the texture quality?
	CN3Base::s_Options.iTexLOD_Terrain = s_CIni->GetInt("Texture", "LOD_Terrain", 0);
	if(CN3Base::s_Options.iTexLOD_Terrain < 0) CN3Base::s_Options.iTexLOD_Terrain = 0;
	if(CN3Base::s_Options.iTexLOD_Terrain >= 2) CN3Base::s_Options.iTexLOD_Terrain = 1;

	// NOTE: should we use shadows?
	CN3Base::s_Options.iUseShadow = s_CIni->GetInt("Shadow", "Use", 1);

	// NOTE: what is the screen resolution?
	CN3Base::s_Options.iViewWidth = s_CIni->GetInt("ViewPort", "Width", 1024);
	CN3Base::s_Options.iViewHeight = s_CIni->GetInt("ViewPort", "Height", 600);

	/*
	//Test
	if(CN3Base::s_Options.iViewWidth == 1024) CN3Base::s_Options.iViewHeight = 600;
	else if(1280 == CN3Base::s_Options.iViewWidth) CN3Base::s_Options.iViewHeight = 1024;
	else if(1600 == CN3Base::s_Options.iViewWidth) CN3Base::s_Options.iViewHeight = 1200;
	else {
		CN3Base::s_Options.iViewWidth = 1024;
		CN3Base::s_Options.iViewHeight = 600;
	}*/

	// NOTE: what is the viewport's color depth?
	CN3Base::s_Options.iViewColorDepth = s_CIni->GetInt("ViewPort", "ColorDepth", 16);
	if(CN3Base::s_Options.iViewColorDepth != 16 && CN3Base::s_Options.iViewColorDepth != 32)
		CN3Base::s_Options.iViewColorDepth = 32;

	// NOTE: what is the viewport's draw distance?
	CN3Base::s_Options.iViewDist = s_CIni->GetInt("ViewPort", "Distance", 512);
	if(CN3Base::s_Options.iViewDist < 256) CN3Base::s_Options.iViewDist = 256;
	if(CN3Base::s_Options.iViewDist > 512) CN3Base::s_Options.iViewDist = 512;

	// NOTE: what is the distance for sound events?
	CN3Base::s_Options.iEffectSndDist = s_CIni->GetInt("Sound", "Distance", 48);
	if(CN3Base::s_Options.iEffectSndDist < 20) CN3Base::s_Options.iEffectSndDist = 20;
	if(CN3Base::s_Options.iEffectSndDist > 48) CN3Base::s_Options.iEffectSndDist = 48;

	// NOTE: is sound enabled?
	CN3Base::s_Options.bSndEnable = s_CIni->GetBool("Sound", "Enable", true);

	// NOTE: is sound duplicated?
	CN3Base::s_Options.bSndDuplicated = s_CIni->GetBool("Sound", "Duplicate", false);

	// NOTE: should we show the window cursor?
	CN3Base::s_Options.bWindowCursor = s_CIni->GetBool("Cursor", "WindowCursor", true);

	delete s_CIni;

	srand((uint32_t) time(NULL));

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "ER: %s\n", SDL_GetError());
		Sleep(1000 * 5);//If the user can't read the error, there is no point in warning them.
		return false;
	}

	// TEMP: until we can get off windows
	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
	//

	if(TTF_Init() == -1) {
		fprintf(stderr, "ER: %s\n", TTF_GetError());
		Sleep(1000 * 5);
		return false;
	}

	int flags = IMG_INIT_JPG|IMG_INIT_PNG;
	if((IMG_Init(flags)&flags) != flags) {
		fprintf(stderr, "ER: %s\n", IMG_GetError());
		Sleep(1000 * 5);
		return false;
	}

	flags = MIX_INIT_OGG|MIX_INIT_MP3;
	if((Mix_Init(flags)&flags) != flags) {
		fprintf(stderr, "ER: %s\n", Mix_GetError());
		Sleep(1000 * 5);
		return false;
	}

	if(SDLNet_Init() == -1) {
		fprintf(stderr, "ER: %s\n", SDLNet_GetError());
		Sleep(1000 * 5);
		return false;
	}

	SDL_Window* s_pSDLWindow = SDL_CreateWindow(
		"KnightOnline",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		CN3Base::s_Options.iViewWidth,
		CN3Base::s_Options.iViewHeight,
		SWP_SHOWWINDOW | SDL_WINDOW_INPUT_FOCUS
	);
	
	

	if(s_pSDLWindow == NULL) {
		fprintf(stderr, "ER: %s\n", SDL_GetError());
		Sleep(1000 * 5);
		return false;
	}

	CGameProcedure::s_bWindowed = true;

	// NOTE: allocate the static members
	CGameProcedure::StaticMemberInit(s_pSDLWindow);

	// NOTE: set the games current procedure to s_pProcLogIn
	CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcLogIn);

	/*
	// TESTING ----------
	CGameBase::s_pPlayer->m_InfoBase.eRace = RACE_KA_ARKTUAREK;
	CGameBase::s_pPlayer->m_InfoBase.eNation = NATION_KARUS;
	CGameBase::s_pPlayer->m_InfoBase.eClass = CLASS_KA_WARRIOR;
	CGameBase::s_pPlayer->m_InfoExt.iZoneInit = 0x01;
	CGameBase::s_pPlayer->m_InfoExt.iZoneCur = 1;
	CGameBase::s_pPlayer->IDSet(-1, "testing", 0xffffffff);
	CGameBase::s_pPlayer->m_InfoBase.iAuthority == AUTHORITY_MANAGER;

	__TABLE_PLAYER_LOOKS* pLooks = CGameBase::s_pTbl_UPC_Looks.Find(CGameBase::s_pPlayer->m_InfoBase.eRace);
	CGameBase::s_pPlayer->InitChr(pLooks);

	CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcMain);
	//CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcCharacterSelect);
	// TESTING -----------
	*/

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
