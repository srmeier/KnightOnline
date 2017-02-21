#pragma once

#if IsWinDef

void OnSignal(int s);
void HookSignals(Condition * notifier);
void UnhookSignals();

#endif

#if IsUnixDef

#include "Condition.h"

void OnSignal(int signum, siginfo_t *info, void * ptr);
void HookSignals(Condition * notifier);
void UnhookSignals();
#endif