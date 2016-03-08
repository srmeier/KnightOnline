#include "stdafx.h"
#include "ConsoleInputThread.h"
#include <conio.h>

static Thread s_consoleInputThread;

void StartConsoleInputThread()
{
	s_consoleInputThread.start(ConsoleInputThread, nullptr);
}

void CleanupConsoleInputThread()
{
	// The thread is still pretty primitive; there's no way to signal the thread to end
	// as it's blocking on fgets(). Need to fix this up so that we can wait for the thread.
	// Currently we close the thread when a read error occurs (ctrl-c causes a read error, exiting does not).
	s_consoleInputThread.waitForExit();
}

uint32 THREADCALL ConsoleInputThread(void * lpParam)
{
	size_t i = 0;
	size_t len;
	char cmd[300];

	while (g_bRunning)
	{
		if (!_kbhit())
		{
			sleep(100);
			continue;
		}

		// Read in single line from stdin
		memset(cmd, 0, sizeof(cmd)); 
		if (fgets(cmd, sizeof(cmd), stdin) == nullptr)
		{
			printf("Console input thread closing...\n");
			break;
		}

		if (!g_bRunning)
			break;

		len = strlen(cmd);
		for (i = 0; i < len; i++)
		{
			if (cmd[i] == '\n' || cmd[i] == '\r')
				cmd[i] = '\0';
		}

		g_pMain->HandleConsoleCommand(cmd);
	}

	return 0;
}