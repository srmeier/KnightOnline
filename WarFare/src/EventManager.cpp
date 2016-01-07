// EventManager.cpp: implementation of the CEventManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventManager.h"
#include "GameProcedure.h"
#include "PlayerMySelf.h"
#include "N3FXMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define EVENT_TYPE_POISON	3

CEventCell::CEventCell()
{
	m_sEventType = 1;
	memset(&m_Rect, 0x00, sizeof(RECT));
}

CEventCell::~CEventCell()
{

}

void CEventCell::Load(HANDLE hFile)
{
	DWORD dwNum;
	ReadFile(hFile, &m_Rect, sizeof(RECT), &dwNum, NULL);
	ReadFile(hFile, &m_sEventType, sizeof(short), &dwNum, NULL);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEventManager::CEventManager()
{
	Release();
}

CEventManager::~CEventManager()
{
	Release();
}


bool CEventManager::LoadFromFile(const char* szFileName)
{
	Release();

	HANDLE hGevFile = CreateFile(szFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hGevFile)
	{
		return false;
	}

	DWORD dwNum;
	int nEventCellCount = 0;
	ReadFile(hGevFile, &nEventCellCount, sizeof(int), &dwNum, NULL);

	for(int i = 0; i < nEventCellCount ; i++)
	{
		CEventCell* pEventCell = new CEventCell();
		pEventCell->Load(hGevFile);
		m_lstEvents.push_back(pEventCell);
	}

	CloseHandle(hGevFile);
	return true;
}

void CEventManager::Release()
{
	m_sEventType = -1;
	memset(&m_rcEvent, 0x00, sizeof(RECT));

	EventItor it;
	for(it=m_lstEvents.begin(); it!=m_lstEvents.end(); it++)
	{
		CEventCell* pEventCell = (*it);
		if(pEventCell) delete pEventCell;
	}
	m_lstEvents.clear();
}

short CEventManager::SetPos(float fX, float fZ)
{
	int x = (int)fX;
	int y = (int)fZ;

	if(PtInRect(x, y, m_rcEvent))
		return m_sEventType;

	EventItor it;
	for(it=m_lstEvents.begin(); it!=m_lstEvents.end(); it++)
	{
		CEventCell* pEventCell = (*it);
		if(pEventCell)
		{
			if(PtInRect(x, y, pEventCell->m_Rect))
			{
				if(m_sEventType != pEventCell->m_sEventType)
				{
					Behavior(pEventCell->m_sEventType, m_sEventType);
				}
				m_rcEvent = pEventCell->m_Rect;
				m_sEventType = pEventCell->m_sEventType;
				return pEventCell->m_sEventType;
			}
		}
	}

	if(m_sEventType != -1)
	{
		Behavior(-1, m_sEventType);
		m_sEventType = -1;
		memset(&m_rcEvent, 0x00, sizeof(RECT));
	}

	return m_sEventType;
}

bool CEventManager::PtInRect(int x, int z, RECT rc)
{
	if(x < rc.left)		return false;
	if(x > rc.right)	return false;
	if(z < rc.top)		return false;
	if(z > rc.bottom)	return false;

	return true;
}

void CEventManager::Behavior(short sEventType, short sPreEventType)
{
	switch(sPreEventType)
	{
	case EVENT_TYPE_POISON:
		{
			int iID = CGameProcedure::s_pPlayer->IDNumber();
			int iFX = FXID_REGION_POISON;
			CGameProcedure::s_pFX->Stop(iID, iID, iFX, -1, true);
		}
		break;
	}

	switch(sEventType)
	{
	case EVENT_TYPE_POISON:
		{
			int iID = CGameProcedure::s_pPlayer->IDNumber();
			int iFX = FXID_REGION_POISON;
			CGameProcedure::s_pFX->TriggerBundle(iID, 0, iFX, iID, -1, FX_BUNDLE_REGION_POISON);
		}
		break;
	}
}
