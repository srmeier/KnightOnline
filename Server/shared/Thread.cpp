#include "stdafx.h"

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
