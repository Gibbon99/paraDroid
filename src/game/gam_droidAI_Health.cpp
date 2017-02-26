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

//#define AI_HEALTH_DEBUG 1

/*stayHealthy (SELECTOR - stop on success)

- ai_isDroidHealthy (SUCCESS - health is OK)
- findHealingTile (SUCCESS - found a tile) - (RUNNING - aStar) (FAILURE - no tile on level)
- moveToLocation (SUCCESS - reached tile) (RUNNING - still moving) (FAILURE - ??)
- healDamage (SUCCESS - health restored ) (RUNNING - still healing)
*/
//-----------------------------------------------------------------------------
//
// Droid checks his health status
int ai_isDroidHealthy ( int whichDroid )
//-----------------------------------------------------------------------------
{
	//
	// Put in some tests like - percentage of max and current
	//
	// Maybe Battle droids fight until lower health level than non combatands
	//
	if ( ( shipLevel[currentLevel].droid[whichDroid].currentHealth < 30 ) && ( 0 != healing[0].numOnLevel ) )
		{
			return AI_RESULT_FAILED;
		}
	else
		{
			return AI_RESULT_SUCCESS;
		}
}

//-----------------------------------------------------------------------------
//
// Reset health flags for when found healing tile or changing level
void ai_resetHealthFlags ( int whichDroid )
//-----------------------------------------------------------------------------
{
	shipLevel[currentLevel].droid[whichDroid].isNotPatrolling = true;
	shipLevel[currentLevel].droid[whichDroid].onResumeDestTile = false;
	shipLevel[currentLevel].droid[whichDroid].destSet = false;
	shipLevel[currentLevel].droid[whichDroid].aStarInitDone = false;
	shipLevel[currentLevel].droid[whichDroid].onHealingTile = false;
	shipLevel[currentLevel].droid[whichDroid].foundHealingTile = false;
	shipLevel[currentLevel].droid[whichDroid].aStarDestinationFound = false;
}

//-----------------------------------------------------------------------------
//
// Is the droid currently located on a healing tile
int ai_onHealingTile ( int whichDroid )
//-----------------------------------------------------------------------------
{
	//
	// If droid over tile ??
	//
	if ( true == shipLevel[currentLevel].droid[whichDroid].onHealingTile )
		{
#ifdef AI_HEALTH_DEBUG
			con_print ( true, false, "Droid [ %i ] fully healed", whichDroid );
#endif
			shipLevel[currentLevel].droid[whichDroid].currentHealth = dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].maxHealth;
			//
			// Find way back to origin
			//
			ai_resetHealthFlags(whichDroid);
			return AI_RESULT_RUNNING;
		}
	else
		{

			//			con_print ( true, false, "[ %i ] - Droid still not on healing tile", whichDroid );
			return AI_RESULT_FAILED;
		}
}

//-----------------------------------------------------------------------------
//
// Find the nearest healing tile
int ai_reachedHealingTile ( int whichDroid )
//-----------------------------------------------------------------------------
{
	cpVect healingTileLocation;
	cpVect	droidWorldPosTiles;

	if ( false == shipLevel[currentLevel].droid[whichDroid].foundHealingTile )
		{
			if ( false == shipLevel[currentLevel].droid[whichDroid].aStarInitDone )
				{
					debugAStarIndex = whichDroid;

					shipLevel[currentLevel].droid[whichDroid].aStarInitDone = true;	// Reset as well
					healingTileLocation = ai_findNearestTile ( whichDroid, TILE_TYPE_HEALING );

					if (healingTileLocation.x == -1)
					{
						// had a problem finding a healing tile
						printf("Error: Couldn't find healing tile to use.\n");
						return AI_RESULT_FAILED;
					}

					healingTileLocation.x = ( int ) healingTileLocation.x / TILE_SIZE;
					healingTileLocation.y = ( int ) healingTileLocation.y / TILE_SIZE;

#ifdef AI_HEALTH_DEBUG
					con_print ( true, false, "[ %i ] - Starting AStar to find nearest healing tile", whichDroid );
#endif
					droidWorldPosTiles.x = ( int ) shipLevel[currentLevel].droid[whichDroid].worldPos.x / TILE_SIZE;
					droidWorldPosTiles.y = ( ( int ) shipLevel[currentLevel].droid[whichDroid].worldPos.y + ( SPRITE_SIZE / 2 ) ) / TILE_SIZE;

					shipLevel[currentLevel].droid[whichDroid].aStarPathIndex = gam_AStarRequestNewPath ( droidWorldPosTiles, healingTileLocation, whichDroid );
					if (shipLevel[currentLevel].droid[whichDroid].aStarPathIndex < 0)
					{
						con_print(true, false, "Error: Couldn't get a valid path index,");
						return AI_RESULT_FAILED;
					}
				}

			//
			// Create the aStar until we reach the healing tile
			//
			// Ask if the thread has finished creating the path and waypoints
			if ( true == gam_AStarIsPathReady ( shipLevel[currentLevel].droid[whichDroid].aStarPathIndex ) )
				{
#ifdef AI_HEALTH_DEBUG
					con_print ( true, false, "[ %i ] - Have not found the AStar destination", whichDroid );
#endif
					shipLevel[currentLevel].droid[whichDroid].foundHealingTile = true;
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
// This is run each time until the droid reaches the healing tile
//
	if ( shipLevel[currentLevel].droid[whichDroid].ai_moveMode == AI_PATHFIND_END )
		{
#ifdef AI_HEALTH_DEBUG
			con_print ( true, true, "[ %i ] - Droid has reached healing tile.", whichDroid );
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
							shipLevel[currentLevel].droid[whichDroid].onHealingTile = true;
							shipLevel[currentLevel].droid[whichDroid].foundHealingTile = false;
							shipLevel[currentLevel].droid[whichDroid].aStarInitDone = false;
							//if (shipLevel[currentLevel].droid[whichDroid].aStarPathIndex > -1)
								gam_AStarRemovePath ( shipLevel[currentLevel].droid[whichDroid].aStarPathIndex, false );
						}
				}
			return AI_RESULT_RUNNING;
		}

	return AI_RESULT_RUNNING;
}
