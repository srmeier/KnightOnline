#pragma once

void StartTimeThread();
void CleanupTimeThread();

uint32_t THREADCALL TimeThread(void * lpParam);

extern time_t UNIXTIME;
extern tm g_localTime;
