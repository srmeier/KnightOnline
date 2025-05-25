#ifndef __EVENT_H_
#define __EVENT_H_

#include "STLMap.h"
#include "EVENT_DATA.h"

//-----------------------------------------------------------------------------
typedef CSTLMap<EVENT_DATA> EventDataArray;

class EVENT {
public:
	int m_Zone;
	EventDataArray m_arEvent;

public:
	void Init(void);
	void DeleteAll(void);
	void Parsing(char* pBuf);
	bool LoadEvent(int zone);

public:
	EVENT(void) {}
	virtual ~EVENT(void) { DeleteAll(); }
};

#endif
