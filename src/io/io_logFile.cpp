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

bool    fileLoggingOn;

//--------------------------------------------------------
//
// Logfile routines
//
// Create a text logfile to track application messages
//
//--------------------------------------------------------

static ALLEGRO_FILE		*logFile;			// file handle to the logfile

//--------------------------------------------------------
// open the log file for writing
// returns TRUE/FALSE for operation
bool io_openLogFile ( const char *logFileName )
//--------------------------------------------------------
{
#ifdef APPEND_LOGFILE
	logFile = al_fopen ( logFileName, "at" );
#else
	logFile = al_fopen ( logFileName, "wt" );
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
//		ErrorNormal ((char *)__FILE__, __LINE__, (char *)"Logfile: Couldn't create the logfile - check file permissions or disk space.");
			fileLoggingOn = false;
		}
	else
		{
			fileLoggingOn = true;
			io_logToFile ( "-----------------------------------------------------------------------------" );
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
			_close ( logFile );
#else
			al_fclose ( logFile );
#endif
			fileLoggingOn = false;
		}
}

/*


#include <stdio.h>
#include <stdarg.h>

int vfprintf(FILE *file, const char *format, va_list arguments);

Description

Sends formatted output from the arguments to the file. See section printf.

Return Value

The number of characters written.

void my_errmsg(char *format, ...)
{
  va_list arg;

  va_start(arg, format);
  fprintf(stderr, "my_errmsg: ");
  vfprintf(stderr, format, arg);
  va_end(arg);
}

void log_log(FILE * logfile, char * format, ...)
{
   va_list ap;
   if (!logfile) return;
   va_start(ap, format);
   vfprintf(logfile, format, ap);
   va_end(ap);
   fflush(logfile);
 }
*/

//--------------------------------------------------------
// Log output to file on disk
void io_logToFile ( const char *format, ... )
//--------------------------------------------------------
{
	va_list		args;
	char		logText[MAX_STRING_SIZE];

	static		char	staticLogText[MAX_STRING_SIZE];
	static 		int    	staticCount = 0;

//	char		tmpTime[32];
	//
	// check if filelogging is actually enabled
	//
	if ( fileLoggingOn == false )
		return;
	//
	// check and make sure we don't overflow our string buffer
	//
	if ( strlen ( format ) >= MAX_STRING_SIZE )
		{
//		ErrorNormal ((char *)__FILE__, __LINE__, (char *)"String passed to logfile too long", (MAX_STRING_SIZE - 10), strlen(format) - (MAX_STRING_SIZE - 10));
			printf ( "Format greater than string size.\n" );
			return;
		}
	//
	// get out the passed in parameters
	//
	va_start ( args, format );
	vsnprintf ( logText, MAX_STRING_SIZE, format, args );
	va_end ( args );
	//
	// put a linefeed onto the end of the text line
	// and write it to the current line and file
	strlcat ( logText, "\n", sizeof ( "\n" ) );
	if ( strcmp ( staticLogText, logText ) == 0 )
		{
			staticCount++;
		}
	else
		{
			if ( EOF == al_fputs ( logFile, logText ) )
				printf ( "Unable to write to logfile.\n" );
			if (staticCount > 0)
			{
				al_fputs (logFile, "Message repeats\n");
				staticCount = 0;
			}
		}

	strcpy ( staticLogText, logText );

	al_fflush ( logFile );
}
