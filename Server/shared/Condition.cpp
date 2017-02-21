#include "stdafx.h"
#include "Condition.h"

Condition::Condition() : m_nLockCount(0)
{
	#if IsUnixDef
	pthread_cond_init(&m_condition, NULL);
	#endif
}

void Condition::BeginSynchronized()
{
	m_lock.lock();
	++m_nLockCount;
}

void Condition::EndSynchronized()
{
	--m_nLockCount;
	m_lock.unlock();
}



#if IsWinDef

uint32 Condition::Wait(time_t timeout)
{
	std::unique_lock<std::mutex> lock(m_lock);
	m_condition.wait_for(lock, std::chrono::milliseconds(timeout));
	return 0;
}

uint32 Condition::Wait()
{
	std::unique_lock<std::mutex> lock(m_lock);
	m_condition.wait(lock);
	return 0;
}

void Condition::Signal()
{
	m_condition.notify_one();
}

void Condition::Broadcast()
{
	m_condition.notify_all();
}

#endif

#if IsUnixDef

/*	@brief Blocks the thread that calls the function until the time is up or
 *			the condition variable is signalled.
 *
 * 	@param timeout Its unit is in seconds
 */
uint32 Condition::Wait(time_t timeout)
{
	struct timespec * abstime;
	abstime->tv_nsec = (timeout % 1000) * 1000000;	
	pthread_cond_timedwait(&m_condition, &m_lock.m_target, (timespec *)&abstime->tv_nsec);
	return 0;
}

uint32 Condition::Wait()//Should we unlock the mutex also ?
{
	pthread_cond_wait(&m_condition, &m_lock.m_target);
	return 0;
}

void Condition::Signal()
{
	pthread_cond_signal(&m_condition);
}

void Condition::Broadcast()
{
	pthread_cond_broadcast(&m_condition);
}

#endif



Condition::~Condition()
{
}
