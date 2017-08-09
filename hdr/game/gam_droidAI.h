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

#define		AI_PATROL			   	0
#define		AI_PATHFIND			   	1	// path find to a location
#define		AI_ATTACK			   	2
#define		AI_FLEE				   	3
#define		AI_PATHFIND_START	   	4
#define		AI_PATHFIND_END			5
#define		AI_MODE_CONTINUE_PATH	6
//#define		AI_WAYPOINT_UP		   	6
//#define		AI_WAYPOINT_DOWN	   	7
#define		AI_NO_ACTION		   	8
#define     AI_FOLLOW_PATH			9     // Follow the path found from aStar
#define     AI_CHECK_HEALTH			10
#define     AI_EVAL_NEXT_OPTION  	11

#define 	AI_MODE_WAYPOINT		0
#define 	AI_MODE_ASTAR			1

#define     AI_STATE_PATROL			0
#define     AI_STATE_HEALTH			1

const int	AI_NODE_SEQUENCE 		= 1;	// Continue until one fails
const int	AI_NODE_SELECTOR 		= 2;	// Stop on success
const int	AI_NODE_EXECUTE 		= 3;	// Run this node for a result
const int	AI_NODE_ALWAYS_SUCCEED	= 4;	// Always return AI_RESULT_SUCCEED;

const int	AI_RESULT_INVALID		= 0;
const int	AI_RESULT_SUCCESS		= 1;
const int	AI_RESULT_FAILED		= 2;
const int	AI_RESULT_RUNNING		= 3;

const int	TILE_TYPE_HEALING		= 1;
const int	TILE_TYPE_NEAREST_WP 	= 2;
const int	TILE_TYPE_FLEE			= 3;

typedef struct
{
	string			nodeName;
	int				nodeType;
	int				functionResult;
	int				parentIndex;
	bool			hasChildren;
	int				numChildren;
	int				childVisitedCounter;
	vector<int>		childIndex;
	ExternFunc		functionName;
} _aiNode;

extern	vector<_aiNode>		aiTree;
extern bool					backToAIRoot;

#include "gam_droidAI_Resume.h"

//--------------------------------------------------------------------
//
// Functions for aiTree ( Behaviour Tree )
//
//--------------------------------------------------------------------

// Setup the nodes for the AI behavioural tree
void ai_setupAITree();

// Walk the aiTree
void ai_walkTree(string startNode, int whichDroid);

// Debug the aiTree
void ai_debugTree(string startNode);

// Add a child node to the named parent node
bool ai_addNode(int nodeType, string parentNode, string nodeName, ExternFunc functionPtr);

extern float	wayPointDestinationSize;
extern float	minChangeDirectionSpeed;
extern bool 	useDroidAcceleration;

// Clear the hasCollided flag before running physics
void drd_clearHadCollidedFlag();

extern float		droidBeenShotValue;
extern float		witnessShootValue;
extern float		witnessTransferValue;

extern float		ai_beenShot;
extern float		ai_healthAmount;
extern float		ai_playerVisible;
extern float		ai_witnessShoot;
extern float		ai_witnessTransfer;
extern float		ai_greenFactor;
extern float		ai_yellowFactor;
extern float		ai_redFactor;

//--------------------------------------------------------------------
//
// Functions run by the aiTree - processing Droid actions
//
//--------------------------------------------------------------------
//
// Process all Droid movements
int ai_getNextWaypoint(int whichDroid);

// Droid checks his health status
int ai_healthIsOk(int whichDroid);

// Can this droid make a move this turn
//
// Will fail if it is currently in a collision with either another droid or
// the player
int ai_canMove(int whichDroid);

// This droid has collided - can it reverse direction and move
int ai_canReverseDirection(int whichDroid);

// Find the nearest tile type
cpVect ai_findNearestTile(int whichDroid, int tileType);

//---------------------------------------------------------
//
// AI Routines for droid fleeing
//
//---------------------------------------------------------

// Find the nearest flee tile
int ai_reachedFleeTile (int whichDroid);

// Has the droid found the flee tile
int ai_onFleeTile (int whichDroid);

// Droid checks his health status and number of healing tiles
int ai_isDroidHealthyFlee (int whichDroid);

// Reset flee flags for when found destination tile or changing level
void ai_resetFleeFlags ( int whichDroid );

//---------------------------------------------------------
//
// AI Routines for health check
//
//---------------------------------------------------------

// Droid checks his health status
int ai_isDroidHealthy(int whichDroid);

// Find the closet healing tile to the current position
int ai_findNearestHealingTile(int whichDroid);

int ai_reachedHealingTile(int whichDroid);

// Is the droid currently located on a healing tile
int ai_onHealingTile(int whichDroid);

// Reset health flags for when found healing tile or changing level
void ai_resetHealthFlags ( int whichDroid );

//---------------------------------------------------------
//
// AI Routines for a droid to return to a location
//
//---------------------------------------------------------

// Check to see if droid needs to return to an origin location
int ai_originNeedToRun(int whichDroid);

// Check to see if droid needs to return to an origin location
int ai_returnToOrigin(int whichDroid);

// Has the droid found a AStar path back to the origin yet
int ai_foundOriginPosition(int whichDroid);

// Continue working out the path back to the origin
int ai_originFoundPath(int whichDroid);

// Reset resume flags for when found destination tile or changing level
void ai_resetResumeFlags ( int whichDroid );

//---------------------------------------------------------
//
// AI Routines for a droid to shoot and react
//
//---------------------------------------------------------

// Flag if a droid witnesses the player shooting another droid
void gam_processWitnessShooting();

// Work out the chance to shoot for this round
void gam_findChanceToShoot(int whichDroid);

// Should the Droid shoot or not
int ai_notShoot ( int whichDroid );

// Locate the player position - see if the droid can take the shot or not
int ai_findBulletDest ( int whichDroid );

// Take the shot
int ai_shootBullet ( int whichDroid );

// Flag if a droid witnesses the player transferring into another droid
void gam_processWitnessTransfer();

//-------------------------------------------------------------

// Process the no collision counter for each droid
void ai_processCollisionCounter(int whichDroid);

// Process the AI for each enemy droid
void drd_processDroidAI(int whichLevel, float delayInterval);

// Swap the direction of travel - get previous waypoint
void gam_swapWaypointDirection(int whichLevel, int whichDroid1, int whichDroid2);

// Fire at the player
void gam_fireAtPlayer(int whichLevel, int whichDroid);

// Set this droids AI state to no action for a defined period of time
void drd_setAINoAction(int whichDroid, float timeLength);

// Find the cloest healing tile to the current position
int ai_findNearestHealingTile(int whichDroid);
