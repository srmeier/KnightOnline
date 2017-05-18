#pragma once

#if _WIN32

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>

#if !defined(_N3GAME)
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#define THREADCALL WINAPI

#else

#define THREADCALL

#endif

#define STRCASECMP _stricmp

#include <random>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <queue>

class Guard
{
public:
	Guard(std::recursive_mutex& mutex) : target(mutex)  { target.lock(); }
	Guard(std::recursive_mutex* mutex) : target(*mutex) { target.lock(); }
	~Guard() { target.unlock(); }

protected:
	std::recursive_mutex& target;
};

#undef sleep

#if _WIN32
#define sleep(ms) Sleep(ms)
#else
#define sleep(ms) usleep(ms)
#endif

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
