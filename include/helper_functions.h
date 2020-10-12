#pragma once
#include "simstruc.h"
#include <string>

/**
** Helper functions to print out messages or errors, and to create exceptions from PLC errors.
**/
template<typename ... Args> inline std::string FormatText(const char* const message, const Args... args)
{
	static char buffer[200];
	_snprintf_s(buffer, _countof(buffer), _TRUNCATE, message, args...);
	return std::string(buffer);
}
template<typename ... Args> inline std::string FormatPLCError(const ERuntimeErrorCode srec, const char* const message, const Args... args)
{
	return FormatText("PLC-Error %ls (%d): ", GetNameOfErrorCode(srec), srec) + FormatText(message, args...);
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

/* Reference time to be mapped to time 0.0 in Simulink */
static const SYSTEMTIME PLC_SIMTIME_BASE =
{
  2000,	// WORD wYear;
  1,	// WORD wMonth;
  0,	// WORD wDayOfWeek;
  1,	// WORD wDay;
  0,	// WORD wHour;
  0,	// WORD wMinute;
  0,	// WORD wSecond;
  0		// WORD wMilliseconds;
};

/* Convert SYSTEMTIME to ULONGLONG (number of 100-nanosecond intervals since January 1, 1601 (UTC)) */
static ULONGLONG SystemTimeToULongLong(SYSTEMTIME s)
{
	FILETIME t;
	if (!SystemTimeToFileTime(&s, &t))
	{
		::PrintText("Error: Could not convert SYSTEMTIME to FILETIME (error code %d).", GetLastError());
		return 0;
	}
	ULARGE_INTEGER i;
	i.LowPart = t.dwLowDateTime;
	i.HighPart = t.dwHighDateTime;
	return i.QuadPart;
}

/* Convert ULONGLONG (number of 100-nanosecond intervals since January 1, 1601 (UTC)) to SYSTEMTIME */
static SYSTEMTIME ULongLongToSystemTime(ULONGLONG u)
{
	ULARGE_INTEGER i;
	i.QuadPart = u;
	FILETIME t;
	t.dwLowDateTime = i.LowPart;
	t.dwHighDateTime = i.HighPart;
	SYSTEMTIME s;
	if (!FileTimeToSystemTime(&t, &s))
	{
		::PrintText("Could not convert FILETIME to SYSTEMTIME (error code %d).", GetLastError());
		ZeroMemory(&s, sizeof(s));
		return s;
	}
	return s;
}

/* Convert Simulink time (time_T) to PLCSim time, using the PLCSim base time */
static SYSTEMTIME ConvertSimulinkTimeToPLCSimTime(time_T simulinkTime)
{
	ULONGLONG plcsimBlockTime = (ULONGLONG)(simulinkTime * 10000000);
	ULONGLONG basePlcsimTimeULL = SystemTimeToULongLong(PLC_SIMTIME_BASE);
	return ULongLongToSystemTime(plcsimBlockTime + basePlcsimTimeULL);
}

/* Convert PLCSim time to SYSTEMTIME Simulink time (time_T), using the PLCSim base time */
static time_T ConvertPLCSimTimeToSimulinkTime(SYSTEMTIME plcsimTime)
{
	ULONGLONG plcsimTimeULL = SystemTimeToULongLong(plcsimTime);
	ULONGLONG basePlcsimTimeULL = SystemTimeToULongLong(PLC_SIMTIME_BASE);
	ULONGLONG plcsimBlockTime = plcsimTimeULL - basePlcsimTimeULL;
	return ((time_T)plcsimBlockTime) / 10000000;
}

/* Invert the byte order of a byte array, non-destructive */
inline static void InvertByteOrder(BYTE inbytes[], BYTE outbytes[], int_T numbytes)
{
	for (int_T b = 0; b < numbytes; ++b)
	{
		outbytes[b] = inbytes[numbytes - 1 - b];
	}
}

/* Invert the byte order of a byte array, in place */
inline static void InvertByteOrder(BYTE inoutbytes[], int_T numbytes)
{
	for (int_T b1 = 0; b1 < (numbytes / 2); ++b1)
	{
		int_T b2 = numbytes - 1 - b1;
		inoutbytes[b1] ^= inoutbytes[b2];
		inoutbytes[b2] ^= inoutbytes[b1];
		inoutbytes[b1] ^= inoutbytes[b2];
	}
}


inline static boolean_T IsCompatibleIODataType(DTypeId dataType)
{
	boolean_T isAcceptable =
		(dataType == SS_DOUBLE ||
			dataType == SS_SINGLE ||
			dataType == SS_INT8 ||
			dataType == SS_UINT8 ||
			dataType == SS_INT16 ||
			dataType == SS_UINT16 ||
			dataType == SS_INT32 ||
			dataType == SS_UINT32 ||
			dataType == SS_BOOLEAN);

	return isAcceptable;
}
