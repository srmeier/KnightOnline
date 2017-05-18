#pragma once

#include <stdint.h>
#include <inttypes.h>

#ifdef _WIN32
#define INLINE __attribute__((always_inline))
#else
#define INLINE __forceinline
#endif

#ifdef _WIN32
#define STRCASECMP _stricmp
#define STRNCASECMP _strnicmp
#define I64FMT "%016I64X"
#define I64FMTD "%I64u"
#define SI64FMTD "%I64d"
#define atoll _atoi64
#define strdup _strdup
#define strtok_r strtok_s
#elif defined(PRIu64) && defined(PRId64)
#define STRCASECMP strcasecmp
#define STRNCASECMP strncasecmp
#define I64FMT "%016" PRIx64
#define I64FMTD "%" PRIu64
#define SI64FMTD "%" PRId64
#else
#define STRCASECMP strcasecmp
#define STRNCASECMP strncasecmp
#define I64FMT "%016llX"
#define I64FMTD "%llu"
#define SI64FMTD "%lld"
#endif

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
