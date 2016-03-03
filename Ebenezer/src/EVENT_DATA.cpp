// EVENT_DATA.cpp: implementation of the EVENT_DATA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Ebenezer.h"
#include "Define.h"
#include "EVENT_DATA.h"
#include "EXEC.h"
#include "LOGIC_ELSE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EVENT_DATA::EVENT_DATA()
{

}

EVENT_DATA::~EVENT_DATA()
{
	EXEC* pExec = NULL;
	LOGIC_ELSE* pLogicElse = NULL;

	while(m_arExec.size()) {
		pExec = m_arExec.front();
		if( pExec )
			delete pExec;
		m_arExec.pop_front();
	}
	m_arExec.clear();

	while(m_arLogicElse.size()) {
		pLogicElse = m_arLogicElse.front();
		if( pLogicElse )
			delete pLogicElse;
		m_arLogicElse.pop_front();
	}
	m_arLogicElse.clear();
}
