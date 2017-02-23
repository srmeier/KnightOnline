#pragma once

#define IsUnixDef defined(__APPLE__)||defined(__gnu_linux__)||defined(__MACH__)||defined(Macintosh)||defined(__linux__)
#define IsWinDef _WIN32 // defined(_WIN32)||defined(_WIN64) //
/****************************************************/
//Under Construction
/*#if IsWinDef
uint32 INLINE RETURN_THREAD(void *x){return *(uint32 *)x;}
#endif
#if IsUnixDef
void* INLINE RETURN_THREAD(void *x){return x;}
#endif*/

#if  IsWinDef

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <queue>

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define THREADCALL WINAPI
//uint32 inline RETURN_THREAD(uint32 r_Value){return r_Value;}
#define STRCASECMP _stricmp

#include <random>

#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>



class Guard
{
public:
	Guard(std::recursive_mutex& mutex) : target(mutex)  { target.lock(); }
	Guard(std::recursive_mutex* mutex) : target(*mutex) { target.lock(); }
	~Guard() { target.unlock(); }

protected:
	std::recursive_mutex& target;
};


#define sleep(ms) Sleep(ms)

#endif	//End of #ifdef for Windows specific defines & includes

/****************************************************/

#if IsUnixDef //Start of #ifdef for Unix specific defines % includes

#define nullptr NULL

//void inline * RETURN_THREAD(void * r_Value){return r_Value;}

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>


/* @brief The mutex system, for the time being we are using default 
 *		  attribitues for the mutex.
 *
 */
class Guard
{
public:
	Guard();
	Guard(pthread_mutex_t * target);
	void lock();
	void unlock();
	~Guard();

	pthread_mutex_t * m_target;

protected:
	int m_lockReturnValue;
	int m_unlockReturnValue;
};


#endif //End of #ifdef for Unix specific defines & includes

/****************************************************/

#define I64FMT "%016I64X"
#define I64FMTD "%I64u"
#define SI64FMTD "%I64d"

#if defined(_DEBUG) || defined(DEBUG)
#	include <cassert>
#	include "DebugUtils.h"

#	define ASSERT assert
#	define TRACE FormattedDebugString

//	Enables tracing to stdout. 
//	Preferable with the VS debugger (is thrown in the "output" window), but
//	it can be spammy otherwise (especially if you don't need it enabled).
#	define USE_SQL_TRACE

//	Ensure both typically used debug defines behave as intended
#	ifndef DEBUG
#		define DEBUG
#	endif

#	ifndef _DEBUG
#		define _DEBUG
#	endif

#else
#	define ASSERT 
#	define TRACE 
#endif

// Ignore the warning "nonstandard extension used: enum '<enum name>' used in qualified name"
// Sometimes it's necessary to avoid collisions, but aside from that, specifying the enumeration helps make code intent clearer.
#pragma warning(disable: 4482)

#define STR(str) #str
#define STRINGIFY(str) STR(str)


#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

// define compiler-specific types
#include "types.h"


#include <memory>
#include <map>
#include <list>
#include <vector>

#include "tstring.h"
#include "globals.h"
#include "Atomic.h"
#include "Thread.h"
#include "Network.h"
#include "TimeThread.h"
#include "Compress.h"

