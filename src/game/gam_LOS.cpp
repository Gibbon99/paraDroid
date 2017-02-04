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

float		g_LineOfSightStep;
bool		allDroidsVisible;

//-----------------------------------------------------
//
// See if point passed by X and Y is a solid tile
//
// Returns 1 if solid, 0 if passable
bool gam_LOSCalled ( int pass_x, int pass_y )
//-----------------------------------------------------
{
	int tile;

	pass_x = pass_x / TILE_SIZE;
	pass_y = pass_y / TILE_SIZE;

	tile = shipLevel[currentLevel].tiles[ ( pass_y * ( shipLevel[currentLevel].levelDimensions.x ) + pass_x )];

	if ( tile > NO_PASS_TILE )
		return false;		// not on a solid tile - can see through
	else
		return true;		// tile is solid - set flag to true
}

//-----------------------------------------------------
// Get passed x and y for player and on screen enemy
// draw a line until hit a solid tile or reach enemy.
//
// If solid is hit, set visible to 0 and exit rightaway
void lvl_doLineSight ( cpVect startPos, cpVect destPos, int whichDroid )
//-----------------------------------------------------
{
	float		distance;

	cpVect		vectDirection;
	cpVect		vectVector;
//	cpVect		currentDirection;
	cpVect		currentPosition;

	vectDirection = sys_getVectorDirection ( startPos, destPos );

	vectVector = cpvnormalize ( vectDirection );

	distance = sys_getDistance ( startPos, destPos );
	distance /= g_LineOfSightStep;

	currentPosition.x = startPos.x;
	currentPosition.y = startPos.y;

	for ( int i = 0; i != distance; i++ )
		{
			currentPosition.x += vectVector.x * g_LineOfSightStep;
			currentPosition.y += vectVector.y * g_LineOfSightStep;

			if ( true == gam_LOSCalled ( currentPosition.x, currentPosition.y ) )
				{
					shipLevel[currentLevel].droid[whichDroid].visibleToPlayer = false;
					return; // Early out - hit a tile before reaching the droid - can't be seen
				}
		}
}

//-----------------------------------------------------
//
// Master routine to check visiblity of all valid sprites
void lvl_LOS()
//-----------------------------------------------------
{
	cpVect tempVect;

	int tempCount = 0;

	LOSTimeStart = al_get_time();

	for ( tempCount = 0; tempCount != shipLevel[currentLevel].numDroids; tempCount++ )
		{
			if ( true == shipLevel[currentLevel].droid[tempCount].isAlive )
				{
					shipLevel[currentLevel].droid[tempCount].visibleToPlayer = true;
					//
					// TODO: Check - how is this being set
					//
					if ( ( true == isnan ( shipLevel[currentLevel].droid[tempCount].worldPos.x ) ) || ( true == isnan ( shipLevel[currentLevel].droid[tempCount].worldPos.y ) ) )
						{
							con_print ( true, false, "Droid [ %i ] worldPos is NAN - frame [ %l ]", tempCount, frameCount );

							shipLevel[currentLevel].droid[tempCount].worldPos.x = shipLevel[currentLevel].wayPoints[shipLevel[currentLevel].droid[tempCount].wayPointIndex].x;
							shipLevel[currentLevel].droid[tempCount].worldPos.y = shipLevel[currentLevel].wayPoints[shipLevel[currentLevel].droid[tempCount].wayPointIndex].y;

							break;
						}
					//
					// Check if it is even visible on the screen
					if ( true == sys_visibleOnScreen ( shipLevel[currentLevel].droid[tempCount].worldPos, SPRITE_SIZE ) )
						{


							tempVect = shipLevel[currentLevel].droid[tempCount].worldPos;

							tempVect.y += ( TILE_SIZE / 2 );

							lvl_doLineSight ( playerWorldMiddlePos, tempVect, tempCount );

							if ( true == allDroidsVisible )
								shipLevel[currentLevel].droid[tempCount].visibleToPlayer = true;
						}
				}
		}

	LOSTimeStop = al_get_time();
}

//-----------------------------------------------------
//
// Debug LOS lines
void lvl_debugLOS()
//-----------------------------------------------------
{
	cpVect screenPos;
	cpVect screenPlayerWorldPos;

	for ( int i = 0; i != shipLevel[currentLevel].numDroids; i++ )
		{
			screenPos = sys_worldToScreen ( shipLevel[currentLevel].droid[i].worldPos, DROID_BODY_SIZE );
			screenPlayerWorldPos = sys_worldToScreen ( playerWorldMiddlePos, DROID_BODY_SIZE );

			al_draw_line ( screenPlayerWorldPos.x, screenPlayerWorldPos.y, screenPos.x + TILE_SIZE, screenPos.y + TILE_SIZE, al_map_rgb_f ( 1.0f, 1.0f, 1.0f ), 2 );
		}
}
