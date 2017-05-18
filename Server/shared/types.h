#pragma once

#if _WIN32
#define INLINE __forceinline
#else
#define INLINE __attribute__((always_inline))
#endif

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
