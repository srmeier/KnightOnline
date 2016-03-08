#include "stdafx.h"
#include "Condition.h"
#include "signal_handler.h"
#include <signal.h>

Condition * g_hNotifier;
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
