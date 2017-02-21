#pragma once

#if  IsWinDef

typedef uint32 (THREADCALL lpfnThreadFunc)(void *);

class Thread
{
public:
	Thread();
	Thread(lpfnThreadFunc lpThreadFunc, void * lpParam = nullptr);

	INLINE bool isStarted() { return m_thread.joinable(); }

	bool start(lpfnThreadFunc lpThreadFunc, void * lpParam = nullptr);
	bool waitForExit();

	virtual ~Thread() {}

protected:
	std::thread m_thread;

private:
	Thread(const Thread & other); /* disable copy constructor */
};

#endif	//End of #ifdef for Windows specific code

#if IsUnixDef

#include <pthread.h>

class Thread
{
public:
	Thread();
	Thread(void *(*lpThreadFunc)(void *), void * lpParam = NULL);

	//bool isStarted() { return ((m_thread == 0) ? true:false);}
	bool isStarted() { return (m_thread==0);}

	bool start(void *(*lpThreadFunc)(void *), void * lpParam = NULL);
	bool waitForExit();


	virtual ~Thread() {}

protected:
	int m_thread;
	pthread_t m_threadID;
	pthread_attr_t m_attr;
	int detachstate;
	int m_initResult;
	int m_threadResult;
	int m_threadJoinResult;

private:
	Thread(const Thread & other); /* disable copy constructor */
};

#endif //End of #ifdef for Unix specific code
