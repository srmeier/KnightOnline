#include "stdafx.h"
#include "RWLock.h"

RWLock::RWLock()
{ 
	_readers = _writers = 0; 
}

void RWLock::AcquireReadLock()
{
	_cond.BeginSynchronized();
	_readers++;
	_cond.EndSynchronized();
}

void RWLock::ReleaseReadLock()
{
	_cond.BeginSynchronized();
	if (!(--_readers))
		if(_writers)
			_cond.Signal();
	_cond.EndSynchronized();
}

void RWLock::AcquireWriteLock()
{
	_cond.BeginSynchronized();
	_writers++;
	if (_readers)
		_cond.Wait();
}

void RWLock::ReleaseWriteLock()
{
	if (--_writers)
		_cond.Signal();
	_cond.EndSynchronized();
}