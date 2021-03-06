/*
This file is part of paraDroid.

    paraDroid is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    paraDroid is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with paraDroid.  If not, see <http://www.gnu.org/licenses/>.

Copyright 2017 David Berry
*/

#include "../../hdr/sys_globals.h"

#include <time.h>

#include <string>
#include <cstdarg>
#include <memory>

//#define APPEND_LOGFILE	1

bool    fileLoggingOn;

//--------------------------------------------------------
//
// Logfile routines
//
// Create a text logfile to track application messages
//
//--------------------------------------------------------

static FILE *logFile;

//--------------------------------------------------------
// open the log file for writing
// returns TRUE/FALSE for operation
bool io_openLogFile ( const char *logFileName )
//--------------------------------------------------------
{
#ifdef APPEND_LOGFILE
	logFile = fopen ( logFileName, "at" );
#else
	logFile = fopen ( logFileName, "wt" );
#endif

	if ( NULL == logFile )
		return false;	// return failure
	else
		return true;
}

//--------------------------------------------------------
// start the log file - if possible
// set fileLoggingOn to true
void io_startLogFile ( const char *logFileName )
//--------------------------------------------------------
{
	fileLoggingOn = false;

	if ( !io_openLogFile ( logFileName ) )
		{
			printf ("Unable to create logfie. Check file permissions or disk space.\n");
			fileLoggingOn = false;
		}
	else
		{
			fileLoggingOn = true;
			io_logToFile ( "-----------------------------------------------------------------------------\n\n" );
			io_logToFile ( "Logfile: Log file opened:" );
			io_logTimeToFile();
			io_logToFile ( "Logfile: Logfile started..." );
		}
}

//--------------------------------------------------------
// log the current time and date to the log file
void io_logTimeToFile()
//--------------------------------------------------------
{
	char tmptime[MAX_STRING_SIZE];
	char tmpdate[MAX_STRING_SIZE];

#if defined (WIN32)
	_strtime ( tmptime );
	_strdate ( tmpdate );
#else
	time_t	currentTime;
	struct tm *localTime;

	// Get current time
	currentTime = time ( NULL );

	// Convert it to local time
	localTime = localtime ( &currentTime );

	sprintf ( tmptime, "%02i:%02i:%02i", localTime->tm_hour, localTime->tm_min, localTime->tm_sec );
	sprintf ( tmpdate, "%02i-%02i-%02i", localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900 );
#endif

	io_logToFile ( "%s\t%s", tmpdate, tmptime );
}

//--------------------------------------------------------
// if the log file is open - close it
void io_closeLogFile()
//--------------------------------------------------------
{
	if ( fileLoggingOn == true )
		{
			io_logToFile ( "Logfile: Log file closed:" );
			io_logTimeToFile();
			io_logToFile ( "-----------------------------------------------------------------------------\n\n" );
#if defined (WIN32)
			fclose ( logFile );
#else
			fclose ( logFile );
#endif
			fileLoggingOn = false;
		}
}

//--------------------------------------------------------
// Log output to file on disk
void io_logToFile ( const char* format, ... )
//--------------------------------------------------------
{
	int logTimeStamp = 0;
	string logTimeText;

	//
	// check if filelogging is actually enabled
	//
	if ( fileLoggingOn == false )
		return;

	va_list args;

	va_start ( args, format );

	if ( true == al_is_system_installed ( ) )
		{
			logTimeStamp = al_get_time();
		}
	else
		logTimeStamp = 0;

	sprintf ( ( char * ) logTimeText.c_str(), "[ %5d ] : ", logTimeStamp );
	fprintf ( logFile, "%s", logTimeText.c_str() );

	vfprintf ( logFile, format, args );

	fputc ( '\r\n', logFile );

	va_end ( args );

	fflush ( logFile );
}
