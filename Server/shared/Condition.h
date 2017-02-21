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
	uint32 Wait(time_t timeout);
	uint32 Wait();
	void Signal();
	void Broadcast();

private:
	int m_nLockCount;

#if IsWinDef
	std::condition_variable m_condition;
	std::mutex m_lock;
#endif
#if IsUnixDef
	pthread_cond_t m_condition;
	Guard m_lock;
#endif
	
};
