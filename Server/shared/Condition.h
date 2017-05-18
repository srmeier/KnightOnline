#pragma once

#include <condition_variable>
#include <deque>

class Condition
{
public:
	Condition();
	~Condition();
	void BeginSynchronized();
	void EndSynchronized();
	uint32_t Wait(time_t timeout);
	uint32_t Wait();
	void Signal();
	void Broadcast();

private:
	int m_nLockCount;

	std::condition_variable m_condition;
	std::mutex m_lock;
};
