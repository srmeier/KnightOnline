#pragma once

void StartConsoleInputThread();
void CleanupConsoleInputThread();
uint32_t THREADCALL ConsoleInputThread(void * lpParam);
