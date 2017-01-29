#include "../../hdr/sys_globals.h"
#include "../../hdr/system/sys_sound.h"

//------------------------------------------------------------
//
// Show physics shapes on screen
bool sys_debugPhysicsShapes(string param1)
//------------------------------------------------------------
{
	debugDrawPhysics = atoi(param1.c_str());

	return true;
}

//------------------------------------------------------------
//
// Display the game version
bool sys_printGameVersion()
//------------------------------------------------------------
{
	con_print (true, true, "Version [ %s ]", gameVersion.c_str());

	return true;
}

//------------------------------------------------------------
//
// Print debug about the particles
bool sys_debugParticles()
//------------------------------------------------------------
{
	par_debugParticles();

	return true;
}

//------------------------------------------------------------
//
// Show the pathfinding structure
bool sys_debugPathFinding()
//------------------------------------------------------------
{
	gam_AStarDebugPaths ();

	return true;
}

//------------------------------------------------------------
//
// Set the droid to debug aStar
bool sys_setAStarDebugIndex(string param1)
//------------------------------------------------------------
{
	debugAStarIndex = atoi(param1.c_str());

	return true;
}

//------------------------------------------------------------
//
// Help - show available console commands
bool sys_conCommandHelp()
//------------------------------------------------------------
{
  int i = 0;

  for (i = 0; i != conCurrentNumCommands; i++)
    {
      con_print (true, true, "[ %s ] - [ %s ]", conCommands[i].command.c_str(), conCommands[i].usage.c_str());
    }

	con_print(true, true, "Script functions [ %i ]", numScriptFunctions);
  return true;
}

//------------------------------------------------------------
//
// Display number of healing tiles on current level
bool sys_conShowNumHealingTiles()
//------------------------------------------------------------
{
    if (0 == healing[0].numOnLevel)
        con_print(true, true, "Num healing tiles [ %i ]", healing[0].numOnLevel);

    for (int i = 0; i != healing[0].numOnLevel; i++)
    {
        con_print(true, true, "Tile [ %i ] Pos [ %4.2f %4.2f ]", i, healing[i].worldPosition.x, healing[i].worldPosition.y);
    }

	return true;
}

//------------------------------------------------------------
//
// Display loaded sound sample information
bool sys_conSoundInfo()
//------------------------------------------------------------
{
	for (int i = 0; i != NUM_SOUNDS; i++)
	{
		con_print(true, true, "Loaded [ %i ] - [ %s ] - [ %s ]", i, sound[i].fileName, sound[i].loadedOk == true ? "true" : "false");
	}
	return true;
}

//------------------------------------------------------------
//
// Display loaded images information
bool sys_conImageInfo()
//------------------------------------------------------------
{
	for (int i = 0; i != numImages; i++)
	{
	    con_print(true, true, "Loaded [ %i ] [ %s ] - W [ %i ] H [ %i ] [ %s ]", i, image[i].fileName,
               al_get_bitmap_width(image[i].image), al_get_bitmap_height(image[i].image), image[i].loadedOk == true ? "true" : "false");
	}
	return true;
}

//------------------------------------------------------------
//
// Restart the scripting engine
// TODO (dberry#1#): Function duplicated in sys_script.cpp line 902
void con_scriptEngineRestart()
//------------------------------------------------------------
{
    sys_restartScriptEngine();
	sys_executeScriptFunction ("as_initGameVars", "");
	sys_executeScriptFunction ("as_addAllScriptCommands", "");
}
