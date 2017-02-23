#pragma once

void StartTimeThread();
void CleanupTimeThread();

#if IsWinDef
uint32 THREADCALL TimeThread(void * lpParam);
#endif
#if IsUnixDef
void * TimeThread(void * lpParam);
#endif

extern time_t UNIXTIME;
extern tm g_localTime;

/*

#if IsWinDef

#endif
#if IsUnixDef

#endif

*/