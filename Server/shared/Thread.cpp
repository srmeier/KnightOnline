#include "stdafx.h"

#if  IsWinDef
Thread::Thread()
{
}

Thread::Thread(lpfnThreadFunc lpThreadFunc, void * lpParam /*= nullptr*/)
{
	start(lpThreadFunc, lpParam);
}

bool Thread::start(lpfnThreadFunc lpThreadFunc, void * lpParam /*= nullptr*/)
{
	try
	{
		m_thread = std::thread(lpThreadFunc, lpParam);
		return true;
	}
	catch (std::exception & ex)
	{
#ifdef _DEBUG
		printf("Caught thread exception: %s\n", ex.what());
		ASSERT(0); /* fix me */
#endif
		return false;
	}
}

bool Thread::waitForExit()
{
	try
	{
		// Ensure it's started & hasn't exited yet.
		if (m_thread.joinable())
			m_thread.join();

		return true;
	}
	catch (std::exception & ex)
	{
#ifdef _DEBUG
		printf("Caught thread exception: %s\n", ex.what());
		ASSERT(0); /* fix me */
#endif
		return false;
	}
}

#endif	//End of #ifdef for Windows specific code

#if IsUnixDef

#include "Thread.h"
/*	@brief Thread constructer.When an Thread object is created, we are 
 *			firstly initializing the attribute object of the thread, so that
 *			in case we are not going to use the default attribute object
 *			we can edit it.
 */			

Thread::Thread()
{
	m_initResult = pthread_attr_init(&m_attr);
}
Thread::Thread(void *(*lpThreadFunc)(void *), void * lpParam)
{
	start(lpThreadFunc, lpParam);
}

/* @brief Starts a new thread
 * @param lpThreadFunc The function that the thread will execute
 * @param lpParam the argument that the lpThreadFunction gets
 *
 * \example it will be added...
 */
bool Thread::start(void *(*lpThreadFunc)(void *), void * lpParam){

	try
	{	
		m_thread = pthread_create(&m_threadID, &m_attr, lpThreadFunc, lpParam);
		if (!m_thread)
		{
			return true;
		}else if (m_thread == EAGAIN){
			printf("The system is lack the necessary resources to create another thread!\n");
			return false;
		}else if(m_thread == EINVAL){
			printf("The value specified by attribute object in Thread class, m_attr, is invalid.\n");
			return false;
		}
		
	}
	catch (std::exception & ex)
	{
#ifdef _DEBUG
		printf("Caught thread exception: %s\n", ex.what());
		ASSERT(0); /* fix me */
#endif
		return false;
	}
	
	return false;
}

bool Thread::waitForExit()
{
	//Note that, this function will wait for the last thread that is created.
	//I'm not sure whether we are using more than one thread at once, but 
	//if we are, we can edit this later on. 
	try
	{
		// Ensure it's started
		if (!m_thread)
			m_threadJoinResult = pthread_join(m_threadID, (void **)&m_threadResult);
			//Do we need to process the m_threadResult's value ?
			//I haven't seen a thread function that return other than zero
		pthread_attr_getdetachstate(&m_attr, &detachstate);
		if (!m_threadJoinResult || (detachstate == PTHREAD_CREATE_DETACHED))
		{
			return true;
		}else
		{
			printf("A joinable thread has ended before Thread::waitForExit() function is called with the return value, m_threadJoinResult: %d\n", m_threadJoinResult);
			return false;
		}
	}
	catch (std::exception & ex)
	{
#ifdef _DEBUG
		printf("Caught thread exception: %s\n", ex.what());
		ASSERT(0); // fix me 
#endif
		return false;
	}
}


#endif //End of #ifdef for Unix specific code
