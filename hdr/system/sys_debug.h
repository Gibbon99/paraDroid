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

extern int					debugCurrentCircuitType;

// Time taken to run the LOS function
extern double				LOSTimeStart;
extern double				LOSTimeStop;

// Set which droid to debug for aStar
extern int					debugAStarIndex;

// Display debug values
void sys_displayDebug();

// Show waypoint lines
void gam_debugShowWaypoints();

// Debug line segments from tiles
void gam_debugLineSegments();

// Show stats and states for droids on level
void sys_debugDroidStats();

//	Show the droids destination point
void gam_debugShowDestination();

// Show middle of the screen
void sys_displayScreenMiddle();

// Show the tile grid
void sys_debugShowTileGrid();