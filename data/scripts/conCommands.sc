
#include "data/scripts/commonDefines.h"

const int	CON_COMMAND	= 1;
const int	CON_USAGE	= 2;
const int	CON_FUNC	= 3;
const int	CON_PARAM   = 4;

//-----------------------------------------------------------------------------
//
// Toggle if LOS is always true or not
void as_setDroidVisibility(int &in param1)
//-----------------------------------------------------------------------------
{
	string testParam;

	testParam = param1;

	sys_printConStr("Passed in ", testParam);

	if (0 == param1)
	{
		allDroidsVisible = false;
		sys_printConStr("LOS visibility is", " false");
	}
	else
	{
		allDroidsVisible = true;
		sys_printConStr("LOS visibility is", " true");
	}
}

//-----------------------------------------------------------------------------
//
// Change the number of steps used in LOS routine
//
// 1 = is every one pixel
void as_setStepsLOS(int &in param1)
//-----------------------------------------------------------------------------
{
	if ((param1 < 0) || (param1 > 32))
	{
	sys_printConStr("Error:", "Parameter is out of range");
	return;
	}
	g_LineOfSightStep = param1;
}

//-----------------------------------------------------------------------------
//
// Change to a new ship level - default to lift 0
void as_changeToLevel(int &in param1)
//-----------------------------------------------------------------------------
{
	string tempParam;

	tempParam = param1;

	sys_printConStr("Passed in ", tempParam);

	if ((param1 < 0) || (param1 > NUM_OF_LEVELS) || (param1 == 7))
	{
		sys_printConStr("Error: ", "Parameter is invalid");
		return;
	}
	gam_changeToLevel(param1);
}

//-----------------------------------------------------------------------------
//
// Toggle the state of the collision detection
bool as_toggleTileCollision(int &in param1)
//-----------------------------------------------------------------------------
{
	string tempParam;

	tempParam = param1;

	sys_printConStr("Passed in ", tempParam);

	if (0 == param1)
	{
		useCollisionDetection = false;
		sys_printConStr("Tile Collision set to", " false");
	}
	else
	{
		useCollisionDetection = true;
		sys_printConStr("Tile collision set to", " true");
	}
	return true;
}

//-----------------------------------------------------------------------------
//
// Script console command - quit program
bool as_conCommandQuit()
//-----------------------------------------------------------------------------
{
	gameIsRunning = false;

	return true;
}

//-----------------------------------------------------------------------------
//
// Script console command - exit program
bool as_conCommandExit()
//-----------------------------------------------------------------------------
{
	gameIsRunning = false;

	return true;
}

//-----------------------------------------------------------------------------
//
// Add all the script commands to console commands
void as_addAllScriptCommands()
//-----------------------------------------------------------------------------
{
//	bool con_addScriptCommand(string command, string usage, string funcPtr, bool setParam)

	sys_addScriptCommand("quit", 			"Quit the program", 		"bool as_conCommandQuit()", 				false);
	sys_addScriptCommand("exit", 			"Quit the program", 		"bool as_conCommandQuit()", 				false);
	sys_addScriptCommand("tilecollision", 	"Toggle tile collision", 	"bool as_toggleTileCollision(int &in)", 	true);
	sys_addScriptCommand("changetolevel", 	"Change to new deck", 		"void as_changeToLevel(int &in)", 			true);
	sys_addScriptCommand("lineofsightsteps","Steps for LOS", 			"void as_setStepsLOS(int &in)", 			true);
	sys_addScriptCommand("droidsvisible", 	"Toggle droid visibility", 	"void as_setDroidVisibility(int &in)",	 	true);

    numConsoleCommands = 6;
}

