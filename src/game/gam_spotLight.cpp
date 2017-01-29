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

vector<_spotLight>		spotLight;
#define DEGTORAD (ALLEGRO_PI/180.0)


//------------------------------------------------------------------
//
// Draw all the spotlights visible on the screen
void gam_drawSpotLights()
//------------------------------------------------------------------
{
	#define DISTANCE_DEFAULT	2000.0f

	cpVect 			drawPosition;
	ALLEGRO_COLOR	spotLightTint;
	float			distanceToPlayer, tempDistance;
	int				op, src, dst;

	if (spotLight.size() == 0)
		return;	// Nothing to draw

	distanceToPlayer = DISTANCE_DEFAULT;

	switch (currentAlertLevel)
	{
		case ALERT_GREEN_TILE:
			spotLightTint = al_map_rgba(1,129,1,128);
			break;
		case ALERT_YELLOW_TILE:
			spotLightTint = al_map_rgba(255,255,0,128);
			break;
		case ALERT_RED_TILE:
			spotLightTint = al_map_rgba(255,0,0,128);
			break;
	}

	al_get_blender(&op, &src, &dst);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

	for (int i = 0; i != spotLight.size(); i++)
	{
		if (true == sys_visibleOnScreen(spotLight[i].worldPosition, spotLight[i].bitmapWidth))
		{
			tempDistance = cpvdist(playerWorldMiddlePos, spotLight[i].worldPosition);
			if ( tempDistance < distanceToPlayer)
				distanceToPlayer = tempDistance;		// Remember the nearest alert tile

			drawPosition = sys_worldToScreen(spotLight[i].worldPosition, spotLight[i].bitmapWidth);

			al_draw_tinted_rotated_bitmap(image[spotLightImg].image, spotLightTint, spotLight[i].srcPosition.x, spotLight[i].srcPosition.y, drawPosition.x, drawPosition.y - 10, spotLight[i].rotateAngle * DEGTORAD, 0);
		}
	}
	//
	// See if there is an alert tile affecting the volume
	alertLevelDistance = 0.01;
	if (distanceToPlayer < DISTANCE_DEFAULT)
	{
		alertLevelDistance = distanceToPlayer - 35.0f;
		alertLevelDistance * 0.5f;
		alertLevelDistance = alertLevelDistance / 100.0f;
		alertLevelDistance = 1.0f - alertLevelDistance;

		if (alertLevelDistance < 0.0f)
			alertLevelDistance = 0.01f;
		if (alertLevelDistance > 1.0f)
			alertLevelDistance = 1.0f;
	}

	al_set_blender(op, src, dst);
}

//------------------------------------------------------------------
//
// Animate all the alert spotlights
void gam_animateSpotLights()
//------------------------------------------------------------------
{
	if (spotLight.size() == 0)	// No Alert tiles on this level
		return;

	float	spotLightSpeed;

	switch (currentAlertLevel)
	{
		case ALERT_GREEN_TILE:
			spotLightSpeed = 10.0f;
			break;
		case ALERT_YELLOW_TILE:
			spotLightSpeed = 30.0f;
			break;
		case ALERT_RED_TILE:
			spotLightSpeed = 60.0f;
			break;
	}

	for (int i = 0; i != spotLight.size(); i++)
	{
		spotLight[i].rotateSpeedCount += spotLightSpeed * thinkInterval;
		if (spotLight[i].rotateSpeedCount > 1.0f)
		{
			spotLight[i].rotateAngle++;
			if (spotLight[i].rotateAngle > 359)
				spotLight[i].rotateAngle = 0;
			spotLight[i].rotateSpeedCount = 0;
		}
	}
}

//------------------------------------------------------------------
//
// Find all the alert tiles on the current level
void gam_findAllAlertTiles(bool clearVector)
//------------------------------------------------------------------
{
	_spotLight		tempSpotLight;
	int				countX, countY, currentTile;

	if (true == clearVector)
	{
		spotLight.clear();	// Leaves with size of 0
	}

	countX = 0; countY = 0;

	for (int i = 0; i != shipLevel[currentLevel].levelDimensions.x * shipLevel[currentLevel].levelDimensions.y; i++)
		{
			currentTile = shipLevel[currentLevel].tiles[i];

			if (currentTile < 0)
				currentTile = 0;

			switch (currentTile)
				{
				case ALERT_GREEN_TILE:
				case ALERT_YELLOW_TILE:
				case ALERT_RED_TILE:
					tempSpotLight.worldPosition.x = (countX * TILE_SIZE) + (TILE_SIZE / 2);
					tempSpotLight.worldPosition.y = (countY * TILE_SIZE) + (TILE_SIZE / 2);
					tempSpotLight.rotateAngle = rand() % 359;
					tempSpotLight.rotateSpeedCount = 0.0f;
					tempSpotLight.bitmapWidth = al_get_bitmap_width(image[spotLightImg].image);
					tempSpotLight.bitmapHeight = al_get_bitmap_height(image[spotLightImg].image);
					tempSpotLight.srcPosition.x = tempSpotLight.bitmapWidth - 5;
					tempSpotLight.srcPosition.y = tempSpotLight.bitmapHeight / 2;

					spotLight.push_back(tempSpotLight);
					break;
				}

			countX++;

			if (countX == shipLevel[currentLevel].levelDimensions.x)
				{
					countX = 0;
					countY++;
				}
		}
}