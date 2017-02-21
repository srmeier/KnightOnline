#include "stdafx.h"
#include "Condition.h"
#include "signal_handler.h"
#include <signal.h>

Condition * g_hNotifier;

#if IsWinDef

void HookSignals(Condition * notifier)
{
	g_hNotifier = notifier;

	signal(SIGINT, OnSignal);
	signal(SIGTERM, OnSignal);
	signal(SIGABRT, OnSignal);
	signal(SIGBREAK, OnSignal);
}

void OnSignal(int s)
{
	switch (s)
	{
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
	case SIGBREAK:
		g_hNotifier->BeginSynchronized();
		g_hNotifier->Signal();
		g_hNotifier->EndSynchronized();
		break;
	}
	

	signal(s, OnSignal);
}

void UnhookSignals()
{
	signal(SIGINT, 0);
	signal(SIGTERM, 0);
	signal(SIGABRT, 0);
	signal(SIGBREAK, 0);	
	
}

#endif

#if IsUnixDef
struct sigaction act;
void HookSignals(Condition * notifier)
{
	g_hNotifier = notifier;

	memset(&act, 0, sizeof(act));
	act.sa_sigaction = &OnSignal;
	act.sa_flags = SA_SIGINFO;

	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
	sigaction(SIGABRT, &act, NULL);
	sigaction(SIGHUP, &act, NULL);
}

void OnSignal(int signum, siginfo_t *info, void * ptr)
{
	switch (signum)
	{
	case SIGINT:
		g_hNotifier->BeginSynchronized();
		g_hNotifier->Signal();
		g_hNotifier->EndSynchronized();
		break;
	case SIGTERM:
	case SIGABRT:
	case SIGHUP:
		g_hNotifier->BeginSynchronized();
		g_hNotifier->Signal();
		g_hNotifier->EndSynchronized();
		break;
	}	

	//signal(signum, OnSignal); //Why ? but lets do in case it is neeeded
	sigaction(signum, &act, NULL);
}

void UnhookSignals()
{
	printf("Unhooking the signals...\n");
	act.sa_sigaction = NULL;
	//I'm not sure whether we have to do the followings again.
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
	sigaction(SIGABRT, &act, NULL);
	sigaction(SIGHUP, &act, NULL);

}

#endif
