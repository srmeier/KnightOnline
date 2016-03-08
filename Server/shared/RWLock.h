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
	volatile unsigned int _readers;
	volatile unsigned int _writers;
}; 
