#include "stdafx.h"
#include "Condition.h"

Condition::Condition() : m_nLockCount(0)
{
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

Condition::~Condition()
{
}
