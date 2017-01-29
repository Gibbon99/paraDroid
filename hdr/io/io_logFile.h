
#pragma once

extern bool    fileLoggingOn;

// open the log file for writing
// returns TRUE/FALSE for operation
bool io_openLogFile(const char *logFileName);

// start the log file - if possible
// set fileLoggingOn to true
void io_startLogFile(const char *logFileName);

// log the current time and date to the log file
void io_logTimeToFile();

// if the log file is open - close it
void io_closeLogFile();

// Log output to file on disk
void io_logToFile (const char *format, ...);
