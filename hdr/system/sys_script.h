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