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

//-----------------------------------------------------------------------------
//
// Shutdown with error - fatal error
void sys_errorFatal (const char *sourcefile, int sourceline, const char *text, ...)
//-----------------------------------------------------------------------------
{
	static		char textBuffer[MAX_STRING_SIZE];
	static		char tempBuffer[MAX_STRING_SIZE];
	va_list		args;

	va_start (args, text);
	vsnprintf (textBuffer, MAX_STRING_SIZE, text, args);
	va_end (args);

	if (strlen (textBuffer) > MAX_STRING_SIZE - 1)
		strcpy (textBuffer, "The string passed to sys_errorFatal is too long.");

	if (strlen (sourcefile) + strlen (textBuffer) > MAX_STRING_SIZE - 4)
		strcpy (tempBuffer, "The string passed to sys_errorFatal is too long.");

	sprintf (tempBuffer, "File: [ %s ] Line: [ %i ] - %s", sourcefile, sourceline, textBuffer);

	if (true == fileLoggingOn)
		io_logToFile ("ERROR: %s", tempBuffer);
	else
		printf ("Couldn't print error to logfile [ %s ]", textBuffer);

	sys_shutDown();

	exit (-1);
}

//-----------------------------------------------------------------------------
//
// Shutdown the game
bool sys_shutDown()
//-----------------------------------------------------------------------------
{
//	al_destroy_mutex(mutexWayPointsReady);

	io_logToFile("Profile - Average time for tiles [ %3.6f ]", profileTimeTotal / profileTimeCount);

	sys_freePhysicsEngine();

	sys_executeScriptFunction ("as_shutdown", "");

	gam_freeTileTexMemory();

	gam_freeStarField();

	al_destroy_font (builtInFont);

	al_shutdown_font_addon();

	sys_releaseSound();

	gam_saveScoreTable((char *)SCORE_TABLE_FILENAME);

	al_destroy_display (display);

	io_closeLogFile();

	sys_cleanupScriptEngine(false);

	return true;
}
