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
#include "../../hdr/game/gam_doors.h"

_doorTrigger		doorTrigger[MAX_NUM_OF_DOORS];		// pointer to memory to hold door trigger information
int				    numDoorsOnLevel;					// how many doors are on this level
float				doorFrameDelay;

// ----------------------------------------------------------------------------
//
// Check door trigger areas against sprite positions
void gam_doorCheckTriggerAreas()
// ----------------------------------------------------------------------------
{
	int i = 0;
	int j = 0;

	if (0 == numDoorsOnLevel)
		return;		// no doors on this level to draw

	for (i = 0; i != numDoorsOnLevel; i++)
	{
		if ((   playerWorldMiddlePos.x  > doorTrigger[i].topLeft.x)
			&& (playerWorldMiddlePos.y  > doorTrigger[i].topLeft.y)
			&& (playerWorldMiddlePos.x  < doorTrigger[i].botRight.x)
			&& (playerWorldMiddlePos.y  < doorTrigger[i].botRight.y))
		{	// player sprite is inside a trigger area
			doorTrigger[i].inUse = true;
		}
		else
		{
			doorTrigger[i].inUse = false;	// this will reset all the doors the player is not in
		}
	}
	//
	// now check all the enemy sprites against the doors
	//
	for (i = 0; i != numDoorsOnLevel; i++)
	{
		for (j = 0; j != shipLevel[currentLevel].numDroids; j++)
		{
			if (true == shipLevel[currentLevel].droid[j].isAlive)
			{
				if ((shipLevel[currentLevel].droid[j].worldPos.x + (SPRITE_SIZE / 2) > doorTrigger[i].topLeft.x) &&
					(shipLevel[currentLevel].droid[j].worldPos.y + (SPRITE_SIZE / 2) > doorTrigger[i].topLeft.y) &&
					(shipLevel[currentLevel].droid[j].worldPos.x + (SPRITE_SIZE / 2) < doorTrigger[i].botRight.x) &&
					(shipLevel[currentLevel].droid[j].worldPos.y + (SPRITE_SIZE / 2) < doorTrigger[i].botRight.y))
				{
					doorTrigger[i].inUse = true;
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
//
// Play a door sound - left or right and gain depending on distance from player
void gam_playDoorSound(int whichDoor)
// ----------------------------------------------------------------------------
{
	if (doorTrigger[whichDoor].topLeft.x < playerWorldMiddlePos.x)
		sys_playSound(SND_DOOR, SND_PAN_LEFT, ALLEGRO_PLAYMODE_ONCE);
	else
		sys_playSound(SND_DOOR, SND_PAN_RIGHT, ALLEGRO_PLAYMODE_ONCE);
}

// ----------------------------------------------------------------------------
//
// Process all the doors that are currently inUse
void gam_doorProcessActions()
// ----------------------------------------------------------------------------
{
	int i;

	for (i = 0; i < numDoorsOnLevel; i++)
	{
		if (true == doorTrigger[i].inUse)
		{
			doorTrigger[i].nextFrame += doorTrigger[i].frameDelay * thinkInterval;
			if (doorTrigger[i].nextFrame > 1.0f)
			{
				doorTrigger[i].nextFrame = 0.0f;
				switch (doorTrigger[i].currentFrame)
				{
				case DOOR_ACROSS:
					doorTrigger[i].currentFrame = DOOR_ACROSS_OPEN_1;
					gam_playDoorSound(i);
					break;
				case DOOR_ACROSS_OPEN_1:
					doorTrigger[i].currentFrame = DOOR_ACROSS_OPEN_2;
					break;
				case DOOR_ACROSS_OPEN_2:
					doorTrigger[i].currentFrame = DOOR_ACROSS_OPENED;
					break;
				case DOOR_ACROSS_OPENED:
					doorTrigger[i].currentFrame = DOOR_ACROSS_OPENED;
					break;
				case DOOR_ACROSS_CLOSING_1:
					doorTrigger[i].currentFrame = DOOR_ACROSS_OPENED;
					break;
				case DOOR_ACROSS_CLOSING_2:
					doorTrigger[i].currentFrame = DOOR_ACROSS_CLOSING_1;
					break;


				case DOOR_UP:
					doorTrigger[i].currentFrame = DOOR_UP_OPEN_1;
					gam_playDoorSound(i);
					break;
				case DOOR_UP_OPEN_1:
					doorTrigger[i].currentFrame = DOOR_UP_OPEN_2;
					break;
				case DOOR_UP_OPEN_2:
					doorTrigger[i].currentFrame = DOOR_UP_OPENED;
					break;
				case DOOR_UP_OPENED:
					doorTrigger[i].currentFrame = DOOR_UP_OPENED;
					break;
				case DOOR_UP_CLOSING_1:
					doorTrigger[i].currentFrame = DOOR_UP_OPENED;
					break;
				case DOOR_UP_CLOSING_2:
					doorTrigger[i].currentFrame = DOOR_UP_CLOSING_1;
					break;

				}	// end of switch statement
			}	// end of nextFrame test
		}	// end of inUse is true text
		else
		{	// trigger is not in use
			doorTrigger[i].nextFrame += doorTrigger[i].frameDelay * thinkInterval;
			if (doorTrigger[i].nextFrame > 1.0f)
			{
				doorTrigger[i].nextFrame = 0.0f;
				switch (doorTrigger[i].currentFrame)
				{
				case DOOR_ACROSS_OPENED:
					gam_playDoorSound(i);
					doorTrigger[i].currentFrame = DOOR_ACROSS_CLOSING_1;
					break;
				case DOOR_ACROSS_CLOSING_1:
					doorTrigger[i].currentFrame = DOOR_ACROSS_CLOSING_2;
					break;
				case DOOR_ACROSS_CLOSING_2:
					doorTrigger[i].currentFrame = DOOR_ACROSS_CLOSED;
					break;

				case DOOR_UP_OPENED:
					gam_playDoorSound(i);
					doorTrigger[i].currentFrame = DOOR_UP_CLOSING_1;
					break;
				case DOOR_UP_CLOSING_1:
					doorTrigger[i].currentFrame = DOOR_UP_CLOSING_2;
					break;
				case DOOR_UP_CLOSING_2:
					doorTrigger[i].currentFrame = DOOR_UP_CLOSED;
					break;
				}
			}
		}
		shipLevel[currentLevel].tiles[doorTrigger[i].mapPos] = doorTrigger[i].currentFrame;
	}	// end of for each door loop
}

// ----------------------------------------------------------------------------
//
// Show door trigger area
void gam_doorDebugTrigger()
// ----------------------------------------------------------------------------
{
cpVect topLeft, bottomRight;

	for (int i = 0; i != numDoorsOnLevel; i++)
	{
		topLeft = sys_worldToScreen(doorTrigger[i].topLeft, TILE_SIZE);
		bottomRight = sys_worldToScreen(doorTrigger[i].botRight, TILE_SIZE);

		al_draw_rectangle(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, al_map_rgb_f(0.0f, 1.0f, 0.0f), 2);
	}
}

// ----------------------------------------------------------------------------
//
// Setup each door doorTrigger for this level
void gam_doorTriggerSetup()
// ----------------------------------------------------------------------------
{

	int 	i;
	float 	sourceX = 0;
	float 	sourceY = 0;
	int 	destX = 0;
	int 	destY = 0;
	int 	currentTile = 0;
	int 	doorCounter = 0;
	cpVect	center;
	cpFloat	width, height;

	for (i = 0; i != shipLevel[currentLevel].levelDimensions.x * shipLevel[currentLevel].levelDimensions.y; i++)
	{
		currentTile = shipLevel[currentLevel].tiles[(((int)sourceY * (shipLevel[currentLevel].levelDimensions.x)) + (int)sourceX)];
		if (currentTile < 0)
			sys_errorFatal(__FILE__, __LINE__, "Tile found in sysDoordoorTriggerSetup is below 0. loop [ %i ] Exiting...", i);

		if	(
			(DOOR_ACROSS == currentTile) ||
			(DOOR_ACROSS_OPEN_1 == currentTile) ||
			(DOOR_ACROSS_OPEN_2 == currentTile) ||
			(DOOR_ACROSS_OPENED == currentTile) ||
			(DOOR_ACROSS_CLOSING_1 == currentTile) ||
			(DOOR_ACROSS_CLOSING_2 == currentTile) ||
			(DOOR_ACROSS_CLOSED == currentTile) ||
			(DOOR_UP == currentTile) ||
			(DOOR_UP_OPEN_1 == currentTile) ||
			(DOOR_UP_OPEN_2 == currentTile) ||
			(DOOR_UP_OPENED == currentTile) ||
			(DOOR_UP_CLOSING_1 == currentTile) ||
			(DOOR_UP_CLOSING_2 == currentTile) ||
			(DOOR_UP_CLOSED == currentTile)
			)

		{
			doorTrigger[doorCounter].frameDelay = doorFrameDelay;
			doorTrigger[doorCounter].nextFrame = 0.0f;
			doorTrigger[doorCounter].mapPos = i;
			doorTrigger[doorCounter].mapX = destX;
			doorTrigger[doorCounter].mapY = destY;

			if ((DOOR_UP == currentTile) || (DOOR_UP_OPEN_1 == currentTile) || (DOOR_UP_OPEN_2 == currentTile) || (DOOR_UP_OPENED == currentTile) || (DOOR_UP_CLOSING_1 == currentTile) || (DOOR_UP_CLOSING_2 == currentTile) || (DOOR_UP_CLOSED == currentTile))
			{
				doorTrigger[doorCounter].currentFrame = DOOR_UP_CLOSED;	// reset to default frame

				doorTrigger[doorCounter].topLeft.x = (sourceX * TILE_SIZE) - TILE_SIZE;
				doorTrigger[doorCounter].topLeft.y = (sourceY * TILE_SIZE) - (TILE_SIZE / 2);

				doorTrigger[doorCounter].topRight.x = (sourceX * TILE_SIZE) + (TILE_SIZE * 2);
				doorTrigger[doorCounter].topRight.y = (sourceY * TILE_SIZE) - (TILE_SIZE / 2);

				doorTrigger[doorCounter].botLeft.x = (sourceX * TILE_SIZE) - TILE_SIZE;
				doorTrigger[doorCounter].botLeft.y = (sourceY * TILE_SIZE) + (TILE_SIZE) + (TILE_SIZE / 2);

				doorTrigger[doorCounter].botRight.x = (sourceX * TILE_SIZE) + (TILE_SIZE * 2);
				doorTrigger[doorCounter].botRight.y = (sourceY * TILE_SIZE) + (TILE_SIZE) + (TILE_SIZE / 2);

				center.x = (sourceX * TILE_SIZE) + (TILE_SIZE / 2);
				center.y = (sourceY * TILE_SIZE) + (TILE_SIZE / 2);
				width = TILE_SIZE / 3;
				height = TILE_SIZE;
			}
			else
			{
				doorTrigger[doorCounter].currentFrame = DOOR_ACROSS;		// reset to default frame
				doorTrigger[doorCounter].topLeft.x = (sourceX * TILE_SIZE) - (TILE_SIZE / 2);
				doorTrigger[doorCounter].topLeft.y = (sourceY * TILE_SIZE) - (TILE_SIZE);

				doorTrigger[doorCounter].topRight.x = (sourceX * TILE_SIZE) + (TILE_SIZE) + (TILE_SIZE / 2);
				doorTrigger[doorCounter].topRight.y = (sourceY * TILE_SIZE) - (TILE_SIZE);

				doorTrigger[doorCounter].botLeft.x = (sourceX * TILE_SIZE) - (TILE_SIZE / 2);
				doorTrigger[doorCounter].botLeft.y = (sourceY * TILE_SIZE) + (TILE_SIZE * 2);

				doorTrigger[doorCounter].botRight.x = (sourceX * TILE_SIZE) + (TILE_SIZE) + (TILE_SIZE / 2);
				doorTrigger[doorCounter].botRight.y = (sourceY * TILE_SIZE) + (TILE_SIZE * 2);

				center.x = (sourceX * TILE_SIZE) + (TILE_SIZE / 2);
				center.y = (sourceY * TILE_SIZE);
				width = TILE_SIZE;
				height = TILE_SIZE / 3;
			}
			sys_createDoorSensor(doorCounter, width, height, center);
			doorCounter++;
		}	// end of if statement

		destX++;
		sourceX++;
		if (destX == (shipLevel[currentLevel].levelDimensions.x))
		{
			destX = 0;
			sourceX = 0;
			destY++;
			sourceY++;
		}
	}
	numDoorsOnLevel = doorCounter;
}
