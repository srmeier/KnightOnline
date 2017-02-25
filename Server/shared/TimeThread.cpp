#include "stdafx.h"
#include <time.h>
#include "TimeThread.h"

/*
*	@brief	The function that sets the return value
*			according to the OS that the server is running.
*			Make sure that the argument x is passed in the type of void.
*
*	@example	return RETURN_THREAD((void *)2);
*/
#if IsWinDef
uint32 INLINE RETURN_THREAD(void * x)
#endif
#if IsUnixDef
void INLINE * RETURN_THREAD(void * x)
#endif
{
#if IsWinDef
	return *(uint32 *)x;
#endif
#if IsUnixDef
	return x;
#endif
}

time_t UNIXTIME; // update this routinely to avoid the expensive time() syscall!
tm g_localTime;

static Thread s_timeThread;
static bool g_bRunningThread;

void StartTimeThread()
{
	UNIXTIME = time(nullptr); // update it first, just to ensure it's set when we need to use it.
	g_localTime = *localtime(&UNIXTIME);
	s_timeThread.start(TimeThread);
}

void CleanupTimeThread()
{
	printf("Waiting for time thread to shutdown...");
	g_bRunningThread = false;
	s_timeThread.waitForExit();
	printf(" done.\n");
}

#if IsWinDef
uint32 THREADCALL TimeThread(void * lpParam)
#endif
#if IsUnixDef
void * TimeThread(void * lpParam)
#endif
{

	g_bRunningThread = true;
	while (g_bRunningThread)
	{
		time_t t = time(nullptr);
		if (UNIXTIME != t)
		{
			UNIXTIME = t;
			g_localTime = *localtime(&t);
		}

		#if IsWinDef
		sleep(1000);	// might need to run it twice a second 
		// to be sure it does in fact update somewhat accurately.. depends on the use cases.
		#endif
		#if IsUnixDef
		sleep(1);
		#endif

	}

	return RETURN_THREAD((void *)0); 
}
