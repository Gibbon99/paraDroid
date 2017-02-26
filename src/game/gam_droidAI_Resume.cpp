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

//#define DEBUG_RESUME 1

//-----------------------------------------------------------------------------
//
// Checks to see if the droid needs to return to patrolling
int ai_isNotPatrolling ( int whichDroid )
//-----------------------------------------------------------------------------
{
	if ( shipLevel[currentLevel].droid[whichDroid].isNotPatrolling == true )
		{
			return AI_RESULT_FAILED;
		}
	return AI_RESULT_SUCCESS;
}

//-----------------------------------------------------------------------------
// Reset resume flags for when found destination tile or changing level
void ai_resetResumeFlags ( int whichDroid )
//-----------------------------------------------------------------------------
{
	shipLevel[currentLevel].droid[whichDroid].isNotPatrolling = false;
	shipLevel[currentLevel].droid[whichDroid].onResumeDestTile = false;
	shipLevel[currentLevel].droid[whichDroid].aStarInitDone = false;

	shipLevel[currentLevel].droid[whichDroid].destSet = false;
	shipLevel[currentLevel].droid[whichDroid].aStarInitDone = false;
	shipLevel[currentLevel].droid[whichDroid].onHealingTile = false;
	shipLevel[currentLevel].droid[whichDroid].foundHealingTile = false;
	shipLevel[currentLevel].droid[whichDroid].aStarDestinationFound = false;
}

//-----------------------------------------------------------------------------
//
// Has droid made it to the resume destination
int ai_onResumeDest ( int whichDroid )
//-----------------------------------------------------------------------------
{
	if ( true == shipLevel[currentLevel].droid[whichDroid].onResumeDestTile )
		{
			ai_resetResumeFlags(whichDroid);
			return AI_RESULT_RUNNING;
		}
	else
		{
//		con_print(true, true, "Droid [ %i ] still returning to resume", whichDroid);
			return AI_RESULT_FAILED;
		}
}

//-----------------------------------------------------------------------------
//
// Work out the AStar path to the destination
int ai_reachedResumeDest ( int whichDroid )
//-----------------------------------------------------------------------------
{
	cpVect nearestWP;
	cpVect droidWorldPosTiles;

	if ( false == shipLevel[currentLevel].droid[whichDroid].destSet )
		{
			if ( false == shipLevel[currentLevel].droid[whichDroid].aStarInitDone )
				{
					nearestWP = ai_findNearestTile ( whichDroid, TILE_TYPE_NEAREST_WP );

					nearestWP.y += (TILE_SIZE / 2);

					nearestWP.x = ( int ) nearestWP.x / TILE_SIZE;
					nearestWP.y = ( int ) nearestWP.y / TILE_SIZE;

					if ( true == gam_AStarIsTileSolid ( ( nearestWP.y * shipLevel[currentLevel].levelDimensions.x ) + nearestWP.x ) )
					{
						printf("ERROR: Found a solid tile for nearest waypoint.\n");
						return AI_RESULT_FAILED;
					}

					droidWorldPosTiles.x = ( int ) shipLevel[currentLevel].droid[whichDroid].worldPos.x / TILE_SIZE;
					droidWorldPosTiles.y = ( ( int ) shipLevel[currentLevel].droid[whichDroid].worldPos.y + ( SPRITE_SIZE / 2 ) ) / TILE_SIZE;

					shipLevel[currentLevel].droid[whichDroid].aStarPathIndex = gam_AStarRequestNewPath ( droidWorldPosTiles, nearestWP, whichDroid );
					if (shipLevel[currentLevel].droid[whichDroid].aStarPathIndex < 0)
					{
						con_print(true, false, "Error: Couldn't get a valid path index,");
						return AI_RESULT_FAILED;
					}

					shipLevel[currentLevel].droid[whichDroid].aStarInitDone = true;
				}

			if ( true == gam_AStarIsPathReady ( shipLevel[currentLevel].droid[whichDroid].aStarPathIndex ) )
				{
					shipLevel[currentLevel].droid[whichDroid].destSet = true;

					shipLevel[currentLevel].droid[whichDroid].currentAStarIndex = gam_AStarGetNumWaypoints ( shipLevel[currentLevel].droid[whichDroid].aStarPathIndex );
					//
					// Point to first entry
					shipLevel[currentLevel].droid[whichDroid].currentAStarIndex++;
					shipLevel[currentLevel].droid[whichDroid].ai_moveMode = AI_MODE_ASTAR;
					ai_getNextWaypoint ( whichDroid );
					return AI_RESULT_RUNNING;
				}
			return AI_RESULT_FAILED; // Now start moving
		}

	if ( shipLevel[currentLevel].droid[whichDroid].ai_moveMode == AI_PATHFIND_END )
		{
#ifdef DEBUG_RESUME
			con_print ( true, true, "Returned to RESUME location [ %i ]", whichDroid );
#endif
			return AI_RESULT_SUCCESS;
		}
	else
		{
			if ( AI_RESULT_SUCCESS == ai_moveDroidToWaypoint ( whichDroid ) )
				{
					shipLevel[currentLevel].droid[whichDroid].ai_moveMode = AI_MODE_ASTAR;
					if ( AI_RESULT_FAILED == ai_getNextWaypoint ( whichDroid ) )
						{
							shipLevel[currentLevel].droid[whichDroid].ai_moveMode = AI_MODE_WAYPOINT;
							shipLevel[currentLevel].droid[whichDroid].onResumeDestTile = true;
							shipLevel[currentLevel].droid[whichDroid].destSet = false;
							shipLevel[currentLevel].droid[whichDroid].aStarInitDone = false;
							gam_AStarRemovePath ( shipLevel[currentLevel].droid[whichDroid].aStarPathIndex, false );
						}
				}
			return AI_RESULT_RUNNING;
		}

	return AI_RESULT_RUNNING;
}
