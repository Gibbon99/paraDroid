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

//#define AI_FLEE_DEBUG 1

//------------------------------------------
//
// Droid will flee when it has low health, and there is no healing tile on the level.
//
// Flee direction is based on it's position to the left/right of the player worldpos.
//
// Get the left/right extreme tile from level, and look along the Y for a passable tile.
//
// Set aStar destination to that location
//
//---------------------------------------------

//-----------------------------------------------------------------------------
//
// Droid checks his health status and number of healing tiles
int ai_isDroidHealthyFlee ( int whichDroid )
//-----------------------------------------------------------------------------
{
	//
	// Put in some tests like - percentage of max and current
	//
	// Maybe Battle droids fight until lower health level than non combatands
	//
	int badHealthLevel = 0;

	badHealthLevel = dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].maxHealth * badHealthFactor;

	if ( (shipLevel[currentLevel].droid[whichDroid].currentHealth < badHealthLevel) 
		&& ( 0 == healing[0].numOnLevel )  
		&& ( shipLevel[currentLevel].numWaypoints > 6 ) )
		{
			return AI_RESULT_FAILED;
		}
	else
		{
			return AI_RESULT_SUCCESS;
		}
}

//-----------------------------------------------------------------------------
// Reset flee flags for when found destination tile or changing level
void ai_resetFleeFlags ( int whichDroid )
//-----------------------------------------------------------------------------
{
	shipLevel[currentLevel].droid[whichDroid].isNotPatrolling = true;
	shipLevel[currentLevel].droid[whichDroid].onResumeDestTile = false;
	shipLevel[currentLevel].droid[whichDroid].destSet = false;
	shipLevel[currentLevel].droid[whichDroid].aStarInitDone = false;
	shipLevel[currentLevel].droid[whichDroid].onFleeTile = false;
	shipLevel[currentLevel].droid[whichDroid].foundFleeTile = false;
	shipLevel[currentLevel].droid[whichDroid].aStarDestinationFound = false;
}

//-----------------------------------------------------------------------------
//
// Has the droid found the flee tile
int ai_onFleeTile ( int whichDroid )
//-----------------------------------------------------------------------------
{
	//
	// If droid over tile ??
	//
	if ( true == shipLevel[currentLevel].droid[whichDroid].onFleeTile )
		{
#ifdef AI_FLEE_DEBUG
			con_print ( true, false, "Droid [ %i ] found Flee tile destination", whichDroid );
#endif
			shipLevel[currentLevel].droid[whichDroid].currentHealth += 5;
			if ( shipLevel[currentLevel].droid[whichDroid].currentHealth > dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].maxHealth )
				shipLevel[currentLevel].droid[whichDroid].currentHealth = dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].maxHealth;

			//
			// Find way back to origin
			//
			ai_resetFleeFlags(whichDroid);
			return AI_RESULT_RUNNING;
		}
	else
		{
			return AI_RESULT_FAILED;
		}
}


//-----------------------------------------------------------------------------
//
// Find the nearest flee tile
int ai_reachedFleeTile ( int whichDroid )
//-----------------------------------------------------------------------------
{
	cpVect 	fleeTileLocation;
	cpVect	droidWorldPosTiles;

	if ( false == shipLevel[currentLevel].droid[whichDroid].foundFleeTile )
		{
			if ( false == shipLevel[currentLevel].droid[whichDroid].aStarInitDone )
				{
					debugAStarIndex = whichDroid;

					shipLevel[currentLevel].droid[whichDroid].aStarInitDone = true;	// Reset as well
					fleeTileLocation = ai_findNearestTile ( whichDroid, TILE_TYPE_FLEE );

#ifdef AI_FLEE_DEBUG
					con_print ( true, true, "Found flee tile World [ %3.2f %3.2f ]", fleeTileLocation.x, fleeTileLocation.y );
#endif

					fleeTileLocation.x = ( int ) fleeTileLocation.x / TILE_SIZE;
					fleeTileLocation.y = ( int ) fleeTileLocation.y / TILE_SIZE;

#ifdef AI_FLEE_DEBUG
					con_print ( true, true, "Found flee tile Tile [ %3.2f %3.2f ]", fleeTileLocation.x, fleeTileLocation.y );
					con_print ( true, false, "[ %i ] - Starting AStar to find nearest flee tile", whichDroid );
#endif
					droidWorldPosTiles.x = ( int ) shipLevel[currentLevel].droid[whichDroid].worldPos.x / TILE_SIZE;
					droidWorldPosTiles.y = ( ( int ) shipLevel[currentLevel].droid[whichDroid].worldPos.y + ( SPRITE_SIZE / 2 ) ) / TILE_SIZE;

					shipLevel[currentLevel].droid[whichDroid].aStarPathIndex = gam_AStarRequestNewPath ( droidWorldPosTiles, fleeTileLocation, whichDroid );
					if (shipLevel[currentLevel].droid[whichDroid].aStarPathIndex < 0)
					{
						con_print(true, false, "Error: Couldn't get a valid path index,");
						return AI_RESULT_FAILED;	//TODO: Check this is the right code to return
					}
				}

			//
			// Create the aStar until we reach the flee tile
			//
			// Ask if the thread has finished creating the path and waypoints
			if ( true == gam_AStarIsPathReady ( shipLevel[currentLevel].droid[whichDroid].aStarPathIndex ) )
				{
#ifdef AI_FLEE_DEBUG
					con_print ( true, false, "[ %i ] - Have not found the AStar destination", whichDroid );
#endif
					shipLevel[currentLevel].droid[whichDroid].foundFleeTile = true;
					shipLevel[currentLevel].droid[whichDroid].currentAStarIndex = gam_AStarGetNumWaypoints ( shipLevel[currentLevel].droid[whichDroid].aStarPathIndex );

					//
					// Put pointer at the start of the list
					shipLevel[currentLevel].droid[whichDroid].currentAStarIndex++;

					shipLevel[currentLevel].droid[whichDroid].ai_moveMode = AI_MODE_ASTAR;
					ai_getNextWaypoint ( whichDroid );

					return AI_RESULT_RUNNING;
				}
			return AI_RESULT_FAILED; // Now start moving
		}

//
// This is run each time until the droid reaches the flee tile
//
	if ( shipLevel[currentLevel].droid[whichDroid].ai_moveMode == AI_PATHFIND_END )
		{
#ifdef AI_FLEE_DEBUG
			con_print ( true, true, "[ %i ] - Droid has reached flee tile.", whichDroid );
			if (debugAStarIndex == whichDroid)
				debugAStarIndex = -1;
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
							shipLevel[currentLevel].droid[whichDroid].ai_moveMode = AI_PATHFIND_END;
							// Reset ready for next time
							shipLevel[currentLevel].droid[whichDroid].onFleeTile = true;
							shipLevel[currentLevel].droid[whichDroid].foundFleeTile = false;
							shipLevel[currentLevel].droid[whichDroid].aStarInitDone = false;
							gam_AStarRemovePath ( shipLevel[currentLevel].droid[whichDroid].aStarPathIndex, false );
						}
				}
			return AI_RESULT_RUNNING;
		}
	return AI_RESULT_RUNNING;
}
