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