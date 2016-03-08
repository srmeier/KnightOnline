#pragma once

void StartConsoleInputThread();
void CleanupConsoleInputThread();
uint32 THREADCALL ConsoleInputThread(void * lpParam);