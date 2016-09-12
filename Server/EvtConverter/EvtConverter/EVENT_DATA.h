#ifndef __EVENT_DATA_H_
#define __EVENT_DATA_H_

#include <list>

#include "EXEC.h"
#include "LOGIC_ELSE.h"

//-----------------------------------------------------------------------------
typedef std::list<EXEC*> ExecArray;
typedef std::list<LOGIC_ELSE*> LogicElseArray;

//-----------------------------------------------------------------------------
class EVENT_DATA {
public:
	bool m_bDup;
	int m_EventNum;
	ExecArray m_arExec;
	bool m_bUsedByAnother;
	LogicElseArray m_arLogicElse;

public:
	EVENT_DATA(void) {
		m_bDup = false;
		m_bUsedByAnother = false;
	}

	virtual ~EVENT_DATA(void) {
		EXEC* pExec = NULL;
		LOGIC_ELSE* pLogicElse = NULL;

		while(m_arExec.size()) {
			pExec = m_arExec.front();
			if(pExec) delete pExec;
			m_arExec.pop_front();
		}

		m_arExec.clear();

		while(m_arLogicElse.size()) {
			pLogicElse = m_arLogicElse.front();
			if(pLogicElse) delete pLogicElse;
			m_arLogicElse.pop_front();
		}

		m_arLogicElse.clear();
	}
};

#endif
