// Copyright � 2017, Christiaan Bakker, All rights reserved.
#include "Logging.h"

#include <iostream>
#include <chrono>

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Windows.h"

#ifdef GetCurrentTime
#undef GetCurrentTime
#endif
#endif

inline void BreakDebugger()
{
#ifdef _WIN32
	if( IsDebuggerPresent() )
	{
		__debugbreak();
	}
#endif
};

namespace Log
{
	static const int nMaximumLogMessageLength = 8192;

	void Event( const char* Format, ... )
	{
		va_list Arguments;
		va_start( Arguments, Format );
		CLog::Get().Event( Format, Arguments );
		va_end( Arguments );
	}

	void Event( LogSeverity Severity, const char* Format, ... )
	{
		va_list Arguments;
		va_start( Arguments, Format );
		CLog::Get().Event( Severity, Format, Arguments );
		va_end( Arguments );
	}

	CLog::CLog( const char* LogName )
	{
		strcpy_s( Name, LogName );

		char LogFileName[256];
		sprintf_s( LogFileName, "Shatter%s.log", Name );
		LogOutputStream.open( LogFileName );

		Timer.Start();
	}

	CLog::CLog()
	{
		Name[0] = '\0';
		LogOutputStream.open( "ShatterGlobalLog.log" );

		Timer.Start();
	}

	CLog::~CLog()
	{
		Timer.Stop();
		LogOutputStream.close();
	}

	void CLog::Print( const char* Format, va_list Arguments )
	{
		char FullMessage[nMaximumLogMessageLength];
		vsprintf_s( FullMessage, Format, Arguments );

		char LogMessage[nMaximumLogMessageLength];
		if( Name[0] != '\0' )
		{
			sprintf_s( LogMessage, "%s: %s", Name, FullMessage );
		}
		else
		{
			strcpy_s( LogMessage, FullMessage );
		}

		CLog& GlobalInstance = CLog::Get();
		if( this != &GlobalInstance )
		{
			GlobalInstance.PrintDirect( LogMessage );
		}

#if !defined( ConsoleWindowDisabled )
		printf( LogMessage );
#else
		if( IsDebuggerPresent() )
		{
			OutputDebugString( LogMessage );
		}
#endif

		PrintDirect( LogMessage );
	}

	void CLog::PrintDirect( const char* Message )
	{
		if( LogOutputStream.is_open() )
		{
			char TimeCode[64];
			sprintf_s( TimeCode, "%.3fs", static_cast<float>( Timer.GetElapsedTimeMilliseconds() ) * 0.001f );
			LogOutputStream << TimeCode << ": " << Message;
		}
	}

	void CLog::Event( const char* Format, ... )
	{
		va_list Arguments;
		va_start( Arguments, Format );
		Event( Format, Arguments );
		va_end( Arguments );
	}

	void CLog::Event( LogSeverity Severity, const char* Format, ... )
	{
		va_list Arguments;
		va_start( Arguments, Format );
		Event( Severity, Format, Arguments );
		va_end( Arguments );
	}

	void CLog::Event( const char* Format, va_list Arguments )
	{
		Print( Format, Arguments );
	}

	void CLog::Event( LogSeverity Severity, const char* Format, va_list Arguments )
	{
		Print( Format, Arguments );

		if( Severity >= Error )
		{
			BreakDebugger();
		}

		if( Severity >= Fatal )
		{
			exit( EXIT_FAILURE );
		}
	}

}
