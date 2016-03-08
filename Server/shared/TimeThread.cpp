#include "stdafx.h"
#include <time.h>
#include "TimeThread.h"

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

uint32 THREADCALL TimeThread(void * lpParam)
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

		sleep(1000);	// might need to run it twice a second 
		// to be sure it does in fact update somewhat accurately.. depends on the use cases.
	}

	return 0;
}
