#pragma once

#include "Condition.h"

void OnSignal(int s);
void HookSignals(Condition * notifier);
void UnhookSignals();
