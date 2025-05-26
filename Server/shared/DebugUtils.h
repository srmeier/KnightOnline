#pragma once

#if defined(_DEBUG) || defined(DEBUG)
void FormattedDebugString(const char* fmt, ...);

#	include <cassert>

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
