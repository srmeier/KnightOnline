#pragma once
#if IsWinDef
uint32 THREADCALL NpcThreadProc(void * lpParam /* CNpcThread ptr */);
uint32 THREADCALL ZoneEventThreadProc(void * lpParam /* = nullptr */);
#endif
#if IsUnixDef
void * NpcThreadProc(void * lpParam /* CNpcThread ptr */);
void * ZoneEventThreadProc(void * lpParam /* = nullptr */);
#endif



typedef std::set<CNpc *> NpcSet;

class CNpc;
class CNpcThread  
{
public:
	CNpcThread();
	void AddNPC(CNpc * pNpc);
	void RemoveNPC(CNpc * pNpc);
	virtual ~CNpcThread();

public:
	NpcSet m_pNpcs;

#if IsWinDef
	std::recursive_mutex m_lock;
#endif
#if IsUnixDef
	pthread_mutex_t * m_lock;
#endif
	
	Thread m_thread;
};
