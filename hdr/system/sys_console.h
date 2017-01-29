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

#define NUM_CON_LINES			100
#define NUM_MAX_CON_COMMANDS	40

#define CON_COMMAND_HOST		1
#define CON_COMMAND_SCRIPT		2

typedef struct
{
	int		    type;
	string		command;
	string		usage;
	string		scriptFunc;
	ExternFunc	conFunc;
} _conCommand;

extern vector<_conCommand>			conCommands;
extern int							conCurrentNumCommands;

typedef struct
{
	string		      conLine;
	ALLEGRO_COLOR     conLineColor;
} _conLine;

extern ALLEGRO_COLOR currentConLineColor;

extern _conLine		conLines[NUM_CON_LINES];
extern _conLine		conPrevCommands[NUM_MAX_CON_COMMANDS];
extern _conLine		conCurrentLine;
extern _conLine		conCurrentPrompt;

extern int		      conCurrentCharCount;
extern int		      conBackSpaceDown;

#define CON_CURSOR_MAX_COUNT	100

extern bool	 	conCursorIsOn;
extern float		conCursorCount;

extern int			conStartY;
extern float		conBackSpaceDelay;

extern int		    conHistoryPtr;			// Which history command are we at
extern int			conNumInHistory;		// How many commands in history buffer currently

// Draw the console screen
void con_drawScreen(bool updateNow, float frameInterval);

// Add a line to the console
void con_print(bool fileLog, bool updateNow, const char *printText, ...);

// Print to console from the scripts - INT version
void sys_scriptPrintInt(string *msgText, int *intParam);

// Print to console from the scripts - String version
void sys_scriptPrintStr(string *msgText, string *msgParam);

// Init the console
void con_initConsole();

// Process a entered command
void con_processCommand(string comLine);

// Add a command to the console command list
bool con_addCommand(string command, string usage, ExternFunc functionPtr);

// Display the current prompt
void con_processBackspaceKey(float frameInterval);

// Process the cursor
void con_processCursor(float frameInterval);

// Autocompletion for console commands
void con_completeCommand(string lookFor);

// Pop a command from the history buffer
void con_popHistoryCommand();

// Add a line to the console and update the display
void con_printUpdate (bool fileLog, const char *printText, ...);

// Set the current color for printing lines to the console
void con_setColor (float red, float green, float blue, float alpha);

// Add a script function to the console commands
void con_addScriptCommand(string command, string usage, string funcPtr, bool setParam);

// Add all the host based console commands here
void con_addConsoleCommands();

//-----------------------------------------------------------------
//
// Commands run from console
//
//-----------------------------------------------------------------

// Help - show available console commands
bool sys_conCommandHelp();
