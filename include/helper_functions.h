#pragma once
#include "simstruc.h"
#include <string>

#define ShutdownWhenTerminating		FALSE
#define ExecuteRunNextCycleLoop		1
#define UseTimespanMode				0	

template<typename ... Args> inline std::string FormatText(const char* const message, const Args... args)
{
	static char buffer[200];
	_snprintf_s(buffer, _countof(buffer), _TRUNCATE, message, args...);
	return std::string(buffer);
}
template<typename ... Args> inline std::string FormatPLCError(const ERuntimeErrorCode srec, const char* const message, const Args... args)
{
	return FormatText("SPS-Error %ls (%d): ", GetNameOfErrorCode(srec), srec) + FormatText(message, args...);
}
template<typename ... Args> inline std::exception CreateException(const char* const message, const Args... args)
{
	return std::exception(FormatText(message, args...).c_str());
}

template<typename ... Args>inline  std::exception CreatePLCException(const ERuntimeErrorCode srec, const char* const message, const Args... args)
{
	return std::exception(FormatPLCError(srec, message, args...).c_str());
}
template<typename ... Args> inline void PrintText(const char* const message, const Args... args)
{
	ssPrintf((FormatText(message, args...)+"\n").c_str());
}
template<typename ... Args> inline void PrintPLCError(const ERuntimeErrorCode srec, const char* const message, const Args... args)
{
	ssPrintf((FormatPLCError(srec, message, args...) + "\n").c_str());
}