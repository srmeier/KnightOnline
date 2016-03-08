#include "stdafx.h"
#include "../shared/signal_handler.h"

CServerDlg * g_pMain;
static Condition s_hEvent;

BOOL WINAPI _ConsoleHandler(DWORD dwCtrlType);

bool g_bRunning = true;

int main()
{
	SetConsoleTitle("AI Server for Knight Online v" STRINGIFY(__VERSION));

	// Override the console handler
	SetConsoleCtrlHandler(_ConsoleHandler, TRUE);

	HookSignals(&s_hEvent);

	// Start up the time updater thread
	StartTimeThread();

	g_pMain = new CServerDlg();

	// Startup server
	if (g_pMain->Startup())
	{
		printf("\nServer started up successfully!\n");

		// Wait until console's signaled as closing
		s_hEvent.Wait();
	}
	else
	{
		system("pause");
	}

	printf("Server shutting down, please wait...\n");

	// This seems redundant, but it's not. 
	// We still have the destructor for the dialog instance, which allows time for threads to properly cleanup.
	g_bRunning = false;

	delete g_pMain;

	CleanupTimeThread();
	UnhookSignals();

	return 0;
}

BOOL WINAPI _ConsoleHandler(DWORD dwCtrlType)
{
	s_hEvent.BeginSynchronized();
	s_hEvent.Signal();
	s_hEvent.EndSynchronized();
	sleep(10000); // Win7 onwards allows 10 seconds before it'll forcibly terminate
	return TRUE;
}
