// MachineMng.h: interface for the CMachineMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MACHINEMNG_H__61657DDC_3561_4BCB_BCDD_658B2E9AC77F__INCLUDED_)
#define AFX_MACHINEMNG_H__61657DDC_3561_4BCB_BCDD_658B2E9AC77F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMachineBase;

#include "GameBase.h"

typedef std::list<CMachineBase*>::iterator it_Machine;

class CMachineMng : public CGameBase  
{
public:
	CMachineMng();
	virtual ~CMachineMng();

// Attributes
public:
protected:
	std::list<CMachineBase*>	m_Machines;

// Operations
public:
//	void			ReceiveReplyMsg(ReplyMsg& RPMsg);
	virtual void	Release();
	void			Render();
	void			Tick();
protected:
//	void			AddMachine(ReplyMsg& RPMsg);
	CMachineBase*	GetMachine(const std::string& szID);
};


#endif // !defined(AFX_MACHINEMNG_H__61657DDC_3561_4BCB_BCDD_658B2E9AC77F__INCLUDED_)
