// EventManager.h: interface for the CEventManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTMANAGER_H__7FB7F7FD_67B8_4F22_AE3E_C41581C16561__INCLUDED_)
#define AFX_EVENTMANAGER_H__7FB7F7FD_67B8_4F22_AE3E_C41581C16561__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEventCell
{
public:
	short	m_sEventType;
	RECT	m_Rect;

	void	Load(HANDLE hFile);
	CEventCell();
	virtual ~CEventCell();

};

typedef std::list<CEventCell*>		EventList;
typedef EventList::iterator			EventItor;

class CEventManager  
{
private:
	EventList	m_lstEvents;
	short		m_sEventType;
	RECT		m_rcEvent;

public:
	void Behavior(short sEventType, short sPreEventType);
	bool PtInRect(int x, int z, RECT rc);
	short SetPos(float fX, float fZ);
	void Release();
	bool LoadFromFile(const char* szFileName);
	CEventManager();
	virtual ~CEventManager();

};

#endif // !defined(AFX_EVENTMANAGER_H__7FB7F7FD_67B8_4F22_AE3E_C41581C16561__INCLUDED_)
