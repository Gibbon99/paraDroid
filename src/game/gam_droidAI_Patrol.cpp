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

#include "../../hdr/sys_globals.h"
#include "../../hdr/game/gam_droidAI.h"

//-----------------------------------------------------------------------------
//
// Process all Droid movements
int ai_getNextWaypoint ( int whichDroid )
//-----------------------------------------------------------------------------
{
	//
	// Get the next waypoint index pointer
	switch ( shipLevel[currentLevel].droid[whichDroid].ai_moveMode )
		{
		case AI_MODE_WAYPOINT:

			switch ( shipLevel[currentLevel].droid[whichDroid].wayPointDirection )
				{
				case WAYPOINT_UP:
					if ( shipLevel[currentLevel].droid[whichDroid].wayPointIndex < shipLevel[currentLevel].numWaypoints - 1 )
						shipLevel[currentLevel].droid[whichDroid].wayPointIndex++;
					else
						shipLevel[currentLevel].droid[whichDroid].wayPointIndex = 0;

					break;

				case WAYPOINT_DOWN:
					shipLevel[currentLevel].droid[whichDroid].wayPointIndex--;

					if ( shipLevel[currentLevel].droid[whichDroid].wayPointIndex < 0 )
						shipLevel[currentLevel].droid[whichDroid].wayPointIndex = shipLevel[currentLevel].numWaypoints - 1;

					break;
				}

			//
			// Save current destination in case we need to reverse
			//
			shipLevel[currentLevel].droid[whichDroid].previousWaypoints =
			    shipLevel[currentLevel].droid[whichDroid].destinationCoords;
			//
			// Get new destination to work out direction vector
			//
			shipLevel[currentLevel].droid[whichDroid].destinationCoords.x =
			    shipLevel[currentLevel].wayPoints[shipLevel[currentLevel].droid[whichDroid].wayPointIndex].x;
			shipLevel[currentLevel].droid[whichDroid].destinationCoords.y =
			    shipLevel[currentLevel].wayPoints[shipLevel[currentLevel].droid[whichDroid].wayPointIndex].y;

			break;

		case AI_MODE_ASTAR:
			shipLevel[currentLevel].droid[whichDroid].currentAStarIndex--; // Move index to next waypoint

			if ( shipLevel[currentLevel].droid[whichDroid].currentAStarIndex < 0 )
				{
//					con_print ( true, true, "Droid has reached destination" );
					shipLevel[currentLevel].droid[whichDroid].ai_moveMode = AI_PATHFIND_END;
					return AI_RESULT_FAILED; // No more waypoints
				}

			//
			// Get new destination to work out direction vector
			//
			shipLevel[currentLevel].droid[whichDroid].destinationCoords.x =
			    path[shipLevel[currentLevel].droid[whichDroid].aStarPathIndex]
			    .wayPoints[shipLevel[currentLevel].droid[whichDroid].currentAStarIndex]
			    .x;
			shipLevel[currentLevel].droid[whichDroid].destinationCoords.y =
			    path[shipLevel[currentLevel].droid[whichDroid].aStarPathIndex]
			    .wayPoints[shipLevel[currentLevel].droid[whichDroid].currentAStarIndex]
			    .y;
			break;
		}

	return AI_RESULT_SUCCESS;
}

//-----------------------------------------------------------------------------
//
// Can this droid make a move this turn
//
// Will fail if it is currently in a collision with either another droid or
// the player
// TODO: Add in player collision
int ai_canMove ( int whichDroid )
//-----------------------------------------------------------------------------
{
	//
	// Droid can always move as it's ignoring any collisions with other droids
	//
	if ( true == shipLevel[currentLevel].droid[whichDroid].ignoreCollisions )
		{
			return AI_RESULT_SUCCESS;
		}

	if ( true == shipLevel[currentLevel].droid[whichDroid].hasCollided )
		{
			return AI_RESULT_FAILED;
		}
	else
		{
			return AI_RESULT_SUCCESS;
		}

	return AI_RESULT_SUCCESS;
}

//-----------------------------------------------------------------------------
//
// Process movements for the passed in droid
// Work out heading vector and velocity
//
// Return AI_RESULT_SUCCESS when we have reached the destination
//
int ai_moveDroidToWaypoint ( int whichDroid )
//-----------------------------------------------------------------------------
{
	float wayPointDistance;

	wayPointDistance = cpvdist ( shipLevel[currentLevel].droid[whichDroid].destinationCoords, shipLevel[currentLevel].droid[whichDroid].worldPos );

	if ( wayPointDistance < wayPointDestinationSize + 20 )
		{
			shipLevel[currentLevel].droid[whichDroid].currentSpeed = dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].maxSpeed * 0.5f;
		}
	else
		{
			// Acclerate faster on higher alert levels
			switch (currentAlertLevel)
			{
				case ALERT_GREEN_TILE:
					shipLevel[currentLevel].droid[whichDroid].currentSpeed += dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].accelerate * thinkInterval;
					break;
					
				case ALERT_YELLOW_TILE:
					shipLevel[currentLevel].droid[whichDroid].currentSpeed += (dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].accelerate * 1.2f)  * thinkInterval;
					break;
					
				case ALERT_RED_TILE:
					shipLevel[currentLevel].droid[whichDroid].currentSpeed += (dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].accelerate * 1.5f)  * thinkInterval;
					break;
			}
			
			if ( shipLevel[currentLevel].droid[whichDroid].currentSpeed > dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].maxSpeed )
				shipLevel[currentLevel].droid[whichDroid].currentSpeed = dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].maxSpeed;
		}

	shipLevel[currentLevel].droid[whichDroid].destDirection =
	    sys_getDirection ( shipLevel[currentLevel].droid[whichDroid].destinationCoords, shipLevel[currentLevel].droid[whichDroid].worldPos );

	shipLevel[currentLevel].droid[whichDroid].velocity = cpvnormalize ( shipLevel[currentLevel].droid[whichDroid].destDirection );
	shipLevel[currentLevel].droid[whichDroid].velocity = cpvmult ( shipLevel[currentLevel].droid[whichDroid].velocity, shipLevel[currentLevel].droid[whichDroid].currentSpeed );
	//
	// Move the droid
	//
	if ( cpFalse == cpBodyIsSleeping ( shipLevel[currentLevel].droid[whichDroid].body ) )
		{
			if ( cpTrue == cpSpaceContainsBody ( space, shipLevel[currentLevel].droid[whichDroid].body ) )
				{
					cpBodySetForce ( shipLevel[currentLevel].droid[whichDroid].body, shipLevel[currentLevel].droid[whichDroid].velocity );
				}
		}
	//
	// See if the droid has reached it's destination
	//
	//
	// How far between current and destination position
	//
	// If it's less then we have reached the waypoint destination
	if ( wayPointDistance < wayPointDestinationSize )
		{
			shipLevel[currentLevel].droid[whichDroid].velocity = cpvzero;
			cpBodySetVelocity ( shipLevel[currentLevel].droid[whichDroid].body, shipLevel[currentLevel].droid[whichDroid].velocity );
			return AI_RESULT_SUCCESS;
		}
	else
		{
			return AI_RESULT_RUNNING; // still hasn't made it to the destination
		}
}

//-----------------------------------------------------------------------------
//
// This droid has collided - can it reverse direction and move
//
int ai_canReverseDirection ( int whichDroid )
//-----------------------------------------------------------------------------
{
	if ( shipLevel[currentLevel].droid[whichDroid].droidType <
	        shipLevel[currentLevel].droid[shipLevel[currentLevel].droid[whichDroid].collidedWith].droidType )
		{
			switch ( shipLevel[currentLevel].droid[whichDroid].wayPointDirection )
				{
				case WAYPOINT_DOWN:
					shipLevel[currentLevel].droid[whichDroid].wayPointDirection = WAYPOINT_UP;
					break;

				case WAYPOINT_UP:
					shipLevel[currentLevel].droid[whichDroid].wayPointDirection = WAYPOINT_DOWN;
					break;
				}

			ai_getNextWaypoint ( whichDroid );
		}

	return AI_RESULT_SUCCESS;
}
