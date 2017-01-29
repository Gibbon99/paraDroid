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
