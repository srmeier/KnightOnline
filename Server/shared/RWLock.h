#pragma once

#include "Condition.h"

class RWLock
{
public: 
	RWLock();
	void AcquireReadLock();
	void ReleaseReadLock();
	void AcquireWriteLock();
	void ReleaseWriteLock();

private:
	Condition _cond;
	volatile uint32_t _readers;
	volatile uint32_t _writers;
}; 
