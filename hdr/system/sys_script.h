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

typedef struct
{
	string			scriptFunctionName;
	void			*hostFunctionPtr;
} _hostScriptFunctions;

extern _hostScriptFunctions hostVariables[];

extern int					numScriptFunctions;		// Number of functions declared in script
extern bool					scriptEngineStarted;
extern int                  numConsoleCommands;     // Number of script commands added from script - needed for malloc

// Start the script engine
bool sys_initScriptEngine();

// Clean up memory for script engine
void sys_cleanupScriptEngine(bool restart);

// Execute a function from the script
bool sys_executeScriptFunction (string functionName, string funcParam);

// Add a script console command to the executable list of functions
bool sys_addScriptConsoleFunction (string funcName,  string funcPtr, bool setParam);

// Restart the script engine
bool sys_restartScriptEngine();

// Call this from a running script to display the current
// variables in use
void sys_printVariables();

//! \brief List the variables we can change
//! \param Not required to pass anything in
bool sys_conListVariables();

//! \brief Get the value of a single variable
//! \param Pass in the name of the variable to query
bool sys_conGetVariable(string param1);

//! \brief Set the value of a single variable
//! \param Pass in the name and the new value
bool sys_conSetVariable(string param1, string param2);

void as_testObject();