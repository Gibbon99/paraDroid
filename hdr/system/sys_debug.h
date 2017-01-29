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