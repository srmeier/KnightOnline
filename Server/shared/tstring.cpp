#include "stdafx.h"
#include "tstring.h"
#include <stdarg.h>
#include <cctype>
#include <functional>
#include <algorithm>

static constexpr char WhitespaceChars[] = " \t\n\r\f\v";

void _string_format(const std::string fmt, std::string * result, va_list args)
{
	char buffer[1024];
	_vsnprintf(buffer, sizeof(buffer), fmt.c_str(), args);
	*result = buffer;
}

std::string string_format(const std::string fmt, ...)
{
	std::string result;
	va_list ap;

	va_start(ap, fmt);
	_string_format(fmt, &result, ap);
	va_end(ap);

	return result;
}

// trim from end
std::string& rtrim(std::string& s)
{
	s.erase(s.find_last_not_of(WhitespaceChars) + 1);
	return s;
}

// trim from start
std::string& ltrim(std::string& s)
{
	s.erase(0, s.find_first_not_of(WhitespaceChars));
	return s;
}

void tstrcpy(char *dst, size_t len, std::string & src)
{
	memset(dst, 0x00, len);
	memcpy(dst, src.c_str(), src.length() > len ? len : src.length());
}

#define _tstrcpy(dst, src) tstrcpy(dst, sizeof(dst), src)

#ifdef UNICODE
string_format(const tstring fmt, ...)
{
	TCHAR buffer[1024];
	va_list ap;

	va_start(ap, fmt);
	_vsntprintf(buffer, sizeof(buffer), fmt.c_str(), ap);
	va_end(ap);

	return buffer;
}

tstring & rtrim(tstring &s) 
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(safe_isspace))).base(), s.end());
	return s;
}

void tstrcpy(TCHAR *dst, size_t len, tstring & src)
{
	memset(dst, 0x00, len);
	memcpy(dst, src.c_str(), src.length() > len ? len : src.length());
}
#endif
