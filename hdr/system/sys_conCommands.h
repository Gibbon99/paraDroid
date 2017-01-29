#pragma once

// Help - show available console commands
bool sys_conCommandHelp();

// Display loaded images information
bool sys_conImageInfo();

// Restart the scripting engine
void con_scriptEngineRestart();

// Display loaded sound sample information
bool sys_conSoundInfo();

// Display number of healing tiles on current level
bool sys_conShowNumHealingTiles();

// Set the droid to debug aStar
bool sys_setAStarDebugIndex(string param1);

// Print debug about the particles
bool sys_debugParticles();

// Show the pathfinding structure
bool sys_debugPathFinding();

// Display the game version
bool sys_printGameVersion();

// Show physics shapes on screen
bool sys_debugPhysicsShapes(string param1);