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

#define NODE_UP			0x0
#define NODE_DOWN		0x1
#define NODE_LEFT		0x2
#define NODE_RIGHT		0x4

typedef struct
{
	bool walkable;
	cpVect tileLocation;
	int parent;
	int g_movementCost;
	int h_estMoveCost;
	int f_score;
} _pathNode;

typedef struct
{
	int					whichDroid;		// Who owns this path
	int					whichLevel;		// Which level is this droid on
	bool 				pathReady;
	bool 				wayPointsReady;
	bool				inUse;
	cpVect 				startTile;
	cpVect 				destTile;
	int 				currentNodePtrClosedList;
	vector<_pathNode> 	openNodes;
	vector<_pathNode> 	closedNodes;
	vector<_pathNode> 	foundPath;
	vector<cpVect> 		wayPoints;
} _nodeList;

extern vector<_nodeList> 	path;
extern int					numAStarPaths;		// Set from Script
extern ALLEGRO_MUTEX		*mutexWayPointsReady;

// Allocate new structure if required or reuse existing one
// Start thread process
int gam_AStarRequestNewPath (cpVect start, cpVect destination, int whichDroid);

// Check if the current thread pathfinding is finished
bool gam_AStarIsPathReady (int whichPath);

// Clear coord memory and flag ready for reuse
void gam_AStarRemovePath (int whichPath, bool resetAll);

// Return how many waypoints there are
int gam_AStarGetNumWaypoints(int whichPath);

// show waypoint lines
void gam_AStarDebugWayPoints (int whichPath);

// Show path structure information
void gam_AStarDebugPaths ();

// Is a tile considered 'solid' or not
bool gam_AStarIsTileSolid ( int tileIndex );

// Remove any running threads
void gam_resetAllPaths();

// Process all the currently active paths
void gam_AStarProcessPaths();