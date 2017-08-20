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

#define TILE_LOOKUP 1 // Controls which way to look up tile texture coordinates

int		hudStatusState;
bool	indicatorStateOn = true;
float	indicatorAnimSpeed;
float	fadeSpeed;			// Setup from script

bool	processFadeValue = true;
int		currentFadeAction;
float	fadeValue = 1.0f;
int		modeAfterFadeOff;

int		tilePosX = 0;
int 	tilePosY = 0;
float 	pixelX = 0.0f;
float 	pixelY = 0.0f;

int 	numTileAcrossInTexture, numTilesDownInTexture;

int 	profileTileCalcSkipped;
int 	profileTotalTileDrawn;

double 	profileTimeStart;
double 	profileTimeEnd;
float 	profileTimeAverage = 0.0f;
float 	profileTimeCount = 0.0f;
float 	profileTimeTotal = 0.0f;

float 	statusTextX, statusTextY, scoreTextX;
int 	currentTunnel;

string 	statusText;

struct _tileTexCoords
{
	cpVect texCoord;
};

_tileTexCoords *tileTexCoords = NULL;

cpVect			beamOnPosition;
ALLEGRO_COLOR	beamOnColor;
float			beamOnRadius;
float			beamOnCurrentRadius;
float			beamOnDelay;
float			beamOnTimer;
float			beamOnAlphaStep;
float			beamOnAlpha;
static	bool	beamOnSetup = false;

//-----------------------------------------------------------------------------
//
// Reset beam on variables
void gam_resetBeamOn()
//-----------------------------------------------------------------------------
{
	beamOnSetup = false;
}

//-----------------------------------------------------------------------------
//
// Process beam on effect
void gam_processBeamOn (float thinkInterval)
//-----------------------------------------------------------------------------
{
	if (false == beamOnSetup)
	{
		beamOnSetup = true;
		beamOnPosition.x = winWidth / 2;
		beamOnPosition.y = winHeight / 2;
		beamOnRadius = 200.0f;
		beamOnCurrentRadius = beamOnRadius;
		beamOnDelay = 60.0f;
		beamOnTimer = 1.0f;
		beamOnAlphaStep = 1.0f / (beamOnRadius / 6.0f);
		beamOnAlpha = 0.0f;
	}

	beamOnTimer -= beamOnDelay * thinkInterval;
	if (beamOnTimer < 0.0f)
	{
		beamOnTimer = 1.0f;
		beamOnCurrentRadius -= 6.0f;
		beamOnAlpha += beamOnAlphaStep;
		if (beamOnAlpha > 0.9f)
			beamOnAlpha = 0.0f;

		beamOnColor = al_map_rgba_f(1.0f, 1.0f, 1.0f, beamOnAlpha);
		if (beamOnCurrentRadius < 0.0f)
		{
			beamOnCurrentRadius = beamOnRadius;
			beamOnAlpha = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//
// Draw the beam on effect - enable alpha blending
void gam_drawBeamOn()
//-----------------------------------------------------------------------------
{
	int op, src, dst;
			
	al_get_blender ( &op, &src, &dst );
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
		
	al_draw_circle(beamOnPosition.x, beamOnPosition.y, beamOnCurrentRadius, beamOnColor, 5.0f);
	
	al_set_blender ( op, src, dst );
}

//-----------------------------------------------------------------------------
//
// Draw the starfield
void gam_drawStarfield()
//-----------------------------------------------------------------------------
{
	al_draw_prim(stars, nullptr, nullptr, 0, numStars, ALLEGRO_PRIM_POINT_LIST);
}

//-----------------------------------------------------------------------------
//
// Setup a bitmap to hold graphics before blitting to screen
bool gam_setupFBO(int flags)
//-----------------------------------------------------------------------------
{
	al_set_new_bitmap_flags(flags);

	bitmapFBO = al_create_bitmap(winWidth, winHeight);
	if (NULL == bitmapFBO)
		{
			useBackingBitmap = false;
		}
	return true;
}

//-----------------------------------------------------------------------------
//
// Free memory for tileTexCoords
void gam_freeTileTexMemory()
//-----------------------------------------------------------------------------
{
	if (NULL != tileTexCoords)
		al_free(tileTexCoords);
}

//-----------------------------------------------------------------------------
//
// Change state to display status text in HUD
void gam_setHUDState(int newState)
//-----------------------------------------------------------------------------
{
	hudStatusState = newState;
}

//-----------------------------------------------------------------------------
//
// Draw the HUD and status text and current score
void gam_drawHud()
//-----------------------------------------------------------------------------
{
	gam_drawImage(hud, 0.5f, 0.5f, flagCenterX);

	sys_setFont(FONT_LARGE);
	sys_setFontColor(1.0f, 0.0f, 0.0f, 1.0f);

	switch (hudStatusState)
		{

			case HUD_STATE_CHARGING:
				statusText = gui_getString("charging");
//				strcpy(statusText, "Charging");
				break;

			case HUD_STATE_HEALING:
			  statusText = gui_getString("healing");
//				strcpy(statusText, "Healing");
				break;

			case HUD_STATE_HEALED:
			  statusText = gui_getString("healed");
//				strcpy(statusText, "Healing");
				break;

			case HUD_STATE_TRANSFER:
			  statusText = gui_getString("transfer");
//				strcpy(statusText, "Transfer");
				break;

			case HUD_STATE_TERMINAL:
				statusText = gui_getString("terminal");
//				strcpy(statusText, "Terminal");
				break;

			case HUD_STATE_MOVE:
				statusText = gui_getString("move");
//				strcpy(statusText, "Move");
				break;

			case HUD_STATE_PAUSED:
				statusText = gui_getString("paused");
//				strcpy(statusText, "Paused");
				break;

			case HUD_STATE_LIFT:
				statusText = gui_getString("lift");
//				strcpy(statusText, "Lift");
				break;

			case HUD_STATE_SHIP_VIEW:
				statusText = gui_getString("shipView");
//				strcpy(statusText, "Ship View");
				break;

			case HUD_STATE_DATABASE:
				statusText = gui_getString("database");
//				strcpy(statusText, "Database");
				break;

			case HUD_STATE_DECK_VIEW:
				statusText = gui_getString("deckView");
//				strcpy(statusText, "Deck View");
				break;

			case HUD_STATE_CAPTURED:
				statusText = gui_getString("captured");
//				strcpy(statusText, "Captured");
				break;

			case HUD_STATE_DEADLOCK:
				statusText = gui_getString("deadlock");
				break;

			case HUD_STATE_SELECT_TIME:
				sprintf((char *)statusText.c_str(), "Select - %i", selectSideCounter);
				break;

			case HUD_STATE_TIME_LEFT:
				sprintf((char *)statusText.c_str(), "Time left - %i", transferPlayCountLeft);
				break;

			case HUD_STATE_LOST:
				statusText = gui_getString("transferLostHUD");
				break;
				
			case HUD_STATE_BEAM_ON:
				statusText = gui_getString("beamOn");
				break;
		}

	sys_printStringExt(startStatusX, statusTextY, "%s", statusText.c_str());

	sys_printStringExt(startScoreX, statusTextY, "%s", displayScoreString);
}

//-----------------------------------------------------------------------------
//
// Return texture coords for passed in tile
cpVect gam_getTileTexCoords(int whichTile)
//-----------------------------------------------------------------------------
{
	return tileTexCoords[whichTile].texCoord;
}

//-----------------------------------------------------------------------------
//
// Setup up precalculated coords for the tiles
void gam_calcTileTexCoords()
//-----------------------------------------------------------------------------
{
	int totalNumTiles;
	//
	// How many tiles fit into the texture
	numTileAcrossInTexture = al_get_bitmap_width(image[tiles].image) / TILE_SIZE;
	numTilesDownInTexture = al_get_bitmap_height(image[tiles].image) / TILE_SIZE;

	totalNumTiles = numTileAcrossInTexture * numTilesDownInTexture;
	//
	// Get enough memory to hold all the coords
	tileTexCoords = (_tileTexCoords *) al_malloc(sizeof (_tileTexCoords) * totalNumTiles);
	if (NULL == tileTexCoords)
		sys_errorFatal(__FILE__, __LINE__, "ERROR: Could not get memory to hold texture coordinates.");

	for (int i = 0; i != totalNumTiles; i++)
		{
			tileTexCoords[i].texCoord.x = (float) (i % numTileAcrossInTexture) * TILE_SIZE;
			tileTexCoords[i].texCoord.y = (float) (i / numTilesDownInTexture) * TILE_SIZE;
			strlcpy(loadingStatus, "Texture coordinates", sizeof("Texture coordinates"));
			loadingBarPercent += 1.0f / totalNumTiles;
			al_rest(progressBarLoadTimes);
		}
	loadingBarPercent = 0.0f;
}

//-----------------------------------------------------------------------------
//
// Draw a single tile from the tile sheet
void inline gam_drawSingleTile(float destX, float destY, int whichTile)
//-----------------------------------------------------------------------------
{
	static float previousTile = -1;
	static cpVect textureCoords;

#ifdef TILE_LOOKUP
	if (previousTile != whichTile)
		{
			textureCoords = gam_getTileTexCoords(whichTile);
			previousTile = whichTile;
			profileTileCalcSkipped++;
		}
#else
	if (previousTile != whichTile)
		{
			textureCoords.x = (float) (whichTile % numTileAcrossInTexture) * TILE_SIZE;
			textureCoords.y = (float) (whichTile / numTilesDownInTexture) * TILE_SIZE;

			previousTile = whichTile;
			profileTileCalcSkipped++;
		}
#endif

	al_draw_bitmap_region(image[tiles].image, textureCoords.x, textureCoords.y, TILE_SIZE, TILE_SIZE, destX + 0.5f, destY + 0.5f, 0);
	profileTotalTileDrawn++;

}

//-----------------------------------------------------------------------------
//
// Copy all the tiles that are visible this screen to the array
void gam_drawAllTiles()
//-----------------------------------------------------------------------------
{
	int across, down;
	int countX, countY;
	int whichTilePtr, whichTile;

	across = winWidth / TILE_SIZE;
	down = winHeight / TILE_SIZE;

	profileTileCalcSkipped = 0; // Performance debug
	profileTotalTileDrawn = 0;

	profileTimeStart = al_get_time();

	if (true == useHoldBitmap)
		al_hold_bitmap_drawing(true);

	for (countY = 0; countY < down + 1; countY++) // +1 to allow overdraw for fine scroll
		{
			for (countX = 0; countX < across + 1; countX++)
				{
					whichTilePtr = (((countY + tilePosY) * shipLevel[currentLevel].levelDimensions.x) + countX + tilePosX);
//			whichTilePtr = (((countY) * shipLevel[currentLevel].levelDimensions.x) + countX);

					//
					// TODO: Going past the edge of the tile array - adding tilePosX and tilePosY is making whichTilePtr too large
					//
					if (whichTilePtr >= shipLevel[currentLevel].levelDimensions.x * shipLevel[currentLevel].levelDimensions.y)
						whichTilePtr = 0;

					if ( shipLevel[currentLevel].tiles.size() == 0 )
					{
						printf ("ERROR: Tile vector array is currently 0.\n");
						return;
					}

					whichTile = shipLevel[currentLevel].tiles[whichTilePtr];

					if ((whichTile < 0) || (whichTile > 64))
						whichTile = 10;

					if (whichTile != 0) // Don't draw the empty tiles
						{
							switch (whichTile)
								{
									case ALERT_GREEN_TILE:
									case ALERT_YELLOW_TILE:
									case ALERT_RED_TILE:
										switch (currentAlertLevel)
											{
												case ALERT_GREEN_TILE:
													whichTile = ALERT_GREEN_TILE;
													break;
												case ALERT_YELLOW_TILE:
													whichTile = ALERT_YELLOW_TILE;
													break;
												case ALERT_RED_TILE:
													whichTile = ALERT_RED_TILE;
													break;
											}
										break;
								}
							gam_drawSingleTile((countX * TILE_SIZE) - pixelX, (countY * TILE_SIZE) - pixelY, whichTile);
						}
				}
		}

	if (true == useHoldBitmap)
		al_hold_bitmap_drawing(false);

	profileTimeEnd = al_get_time();
	profileTimeTotal += (profileTimeEnd - profileTimeStart);
	profileTimeCount++;
}

//-----------------------------------------------------------------------------
//
// Animate the level indicator shown on deck map
void gam_animateIndicator(float thinkInterval)
//-----------------------------------------------------------------------------
{
	static float	currentCount;

	currentCount += indicatorAnimSpeed * thinkInterval;
	if (currentCount > 1.0f)
	{
		currentCount = 0;
		indicatorStateOn = !indicatorStateOn;
	}
}

//-----------------------------------------------------------------------------
//
// Draw the player location indicator
void term_drawIndicator(float screenTileSize)
//-----------------------------------------------------------------------------
{
	cpVect playerTilePos;
	cpVect drawScreenOffset;

	drawScreenOffset.x = (winWidth - (shipLevel[currentLevel].levelDimensions.x * (TILE_SIZE / 2))) / 2;
	drawScreenOffset.y = (winHeight - (shipLevel[currentLevel].levelDimensions.y * (TILE_SIZE / 2))) / 2;

	playerTilePos.x = (playerWorldMiddlePos.x / TILE_SIZE)  * screenTileSize;
	playerTilePos.y = (playerWorldMiddlePos.y / TILE_SIZE)  * screenTileSize;

	playerTilePos.x += drawScreenOffset.x;
	playerTilePos.y += drawScreenOffset.y;

	if(indicatorStateOn)
		al_draw_filled_circle(playerTilePos.x, playerTilePos.y, 8.0f, al_map_rgba_f(1.0f, 1.0f, 1.0f, 1.0f));
}

//-----------------------------------------------------------------------------
//
// Draw the current level onto the entire screen
void term_showCurrentLevel()
//-----------------------------------------------------------------------------
{
	int countX, countY, whichTile;
	cpVect worldPosDraw;
	cpVect drawScreenOffset;
	cpVect srcTexture;
	cpVect textureSize;

	textureSize.x = al_get_bitmap_width(image[tilesSmall].image) / (TILE_SIZE / 2);
	textureSize.y = al_get_bitmap_height(image[tilesSmall].image) / (TILE_SIZE / 2);

	drawScreenOffset.x = (winWidth - (shipLevel[currentLevel].levelDimensions.x * (TILE_SIZE / 2))) / 2;
	drawScreenOffset.y = (winHeight - (shipLevel[currentLevel].levelDimensions.y * (TILE_SIZE / 2))) / 2;

	for (countY = 0; countY != shipLevel[currentLevel].levelDimensions.y; countY++)
		{
			for (countX = 0; countX != shipLevel[currentLevel].levelDimensions.x; countX++)
				{
					whichTile = shipLevel[currentLevel].tiles[(countY * shipLevel[currentLevel].levelDimensions.x) + countX];
					if (0 != whichTile)
						{
							worldPosDraw.x = (countX * (TILE_SIZE / 2) + drawScreenOffset.x);
							worldPosDraw.y = (countY * (TILE_SIZE / 2) + drawScreenOffset.y);

							srcTexture.x = (whichTile % (int) textureSize.x) * (TILE_SIZE / 2);
							srcTexture.y = (whichTile / (int) textureSize.y) * (TILE_SIZE / 2);

							al_draw_bitmap_region(image[tilesSmall].image, srcTexture.x, srcTexture.y, (TILE_SIZE / 2), (TILE_SIZE / 2), worldPosDraw.x, worldPosDraw.y, 0);
						}
				}
		}

	sys_printStringExt(33.0f, winHeight - 65, "Deck View - %s", shipLevel[currentLevel].levelName);

	term_drawIndicator(TILE_SIZE / 2);
}

//-----------------------------------------------------------------------------
//
// Draw an image - pass in index and location
void gam_drawImage(int imageIndex, float posX, float posY, int flags)
//-----------------------------------------------------------------------------
{
//	ALLEGRO_COLOR keyColor;
	float drawPosX;
	float drawPosY;
	//
	// Check it's a valid index
	if ((imageIndex < 0) || (imageIndex > numImages))
		return;

	if ((flags & flagCenterX) == flagCenterX)
		{
			drawPosX = (winWidth - al_get_bitmap_width(image[imageIndex].image)) / 2;
			posX = drawPosX;
		}

	if ((flags & flagCenterY) == flagCenterY)
		{
			drawPosY = (winHeight - al_get_bitmap_height(image[imageIndex].image)) / 2;
			posY = drawPosY;
		}
	//
	// See if it gets shown full screen
	if ((flags & flagFullScreen) == flagFullScreen)
		{
			al_draw_scaled_bitmap(image[imageIndex].image, 0.0f, 0.0f,
			                      al_get_bitmap_width(image[imageIndex].image),
			                      al_get_bitmap_height(image[imageIndex].image),
			                      0.0f, 0.0f, winWidth, winHeight, 0);
			return;
		}

	if (hud == imageIndex)
		al_draw_filled_rectangle(0.0f, 0.0f, winWidth, TILE_SIZE * 2, al_map_rgb_f(0.0f, 0.0f, 0.0f));

	al_draw_tinted_bitmap(image[imageIndex].image, al_map_rgba_f(1.0f, 1.0f, 1.0f, 1.0f), posX, posY, 0);
}

//-----------------------------------------------------------------------------
//
// Modify the global fadeValue - set to run on mode change
//
// Runs in think time - not drawing time
void gam_processFadeValue()
//-----------------------------------------------------------------------------
{
	static float countDelay = 0.0f;

	if (false == processFadeValue)
		return;

	switch (currentFadeAction)
		{
			case FADE_ON:
				countDelay += fadeSpeed * thinkInterval;
				if (countDelay > 1.0f)
					{
						countDelay = 0.0f;
						fadeValue -= 0.1f;
						if (fadeValue < 0.0f)
							{
								processFadeValue = false; // finished fading on
								fadeValue = 1.0f;
							}
					}
				break;

			case FADE_OFF:
				countDelay += fadeSpeed * thinkInterval;
				if (countDelay > 1.0f)
					{
						countDelay = 0.0f;
						fadeValue += 0.1f;
						if (fadeValue > 1.0f)
							{
								currentFadeAction = FADE_ON;
								fadeValue = 1.0f;
								currentMode = modeAfterFadeOff;
							}
					}
				break;
		}
}

//-----------------------------------------------------------------------------
//
// Draw a sprite from a script
//
// Pass in image index, position, tint color
void gam_drawBitmapFromScript(int index, float posX, float posY, float red, float green, float blue, float alpha)
//-----------------------------------------------------------------------------
{
	ALLEGRO_COLOR spriteColor;

	if ((index < 0) || (index > numImages))
		{
			io_logToFile("Error: Invalid index passed to [ %s ]", __func__);
			return;
		}

	spriteColor = al_map_rgba_f(red, green, blue, alpha);

	al_draw_tinted_bitmap(image[index].image, spriteColor, posX, posY, 0);
}

//-----------------------------------------------------------------------------
//
// Draw the frame of a sprite image
//
// Position param is in Screen coords
void gam_drawSprite(int currentFrame, int imageIndex, cpVect position, float angle, ALLEGRO_COLOR color)
//-----------------------------------------------------------------------------
{
	float sourceX;
	int frameWidth, frameHeight;

	switch (imageIndex)
		{
			case bulletType1:
			case bulletType2:
			case bulletType3:
				frameWidth = al_get_bitmap_width(image[imageIndex].image) / NUM_FRAMES_IN_BULLET;
				break;

			case dbImage: // Gets loaded on the fly
				frameWidth = al_get_bitmap_width(image[imageIndex].image) / NUM_DB_FRAMES;
				break;

			default:
				frameWidth = SPRITE_SIZE;
				break;
		}

	frameHeight = al_get_bitmap_height(image[imageIndex].image);

	sourceX = currentFrame * frameWidth;

	if ((position.x == -1) || (position.y == -1))
		return;
	//
	// Why the offset
	if (angle == -1.0f)
		al_draw_tinted_bitmap_region(image[imageIndex].image, color, sourceX, 0.0f, frameWidth, frameHeight, position.x, position.y, 0);
	else
		al_draw_tinted_scaled_rotated_bitmap_region(image[imageIndex].image, sourceX, 0.0f, frameWidth, frameHeight, color, frameWidth / 2, frameHeight / 2, position.x, position.y, 1.0f, 1.0f, angle, 0);
}

//-----------------------------------------------------------------------------
//
// Draw a sprite from a script
void gam_drawSpriteFromScript(int index, float posX, float posY, float red, float green, float blue, float alpha)
//-----------------------------------------------------------------------------
{
	cpVect drawPosition;

	if ((index < 0) || (index > numImages))
		{
			io_logToFile("Error: Invalid index passed to [ %s ]", __func__);
			return;
		}

	drawPosition.x = posX;
	drawPosition.y = posY;

	gam_drawSprite(0, index, drawPosition, 0, al_map_rgba_f(red, green, blue, alpha));
}


// ----------------------------------------------------------------------------
//
// draw a 3d looking rectangle to show off the levels
void draw3dRectangle(float x1, float y1, float x2, float y2, ALLEGRO_COLOR color)
// ----------------------------------------------------------------------------
{
	_myRectangle singleRect;

	singleRect.x1 = x1;
	singleRect.y1 = y1;
	singleRect.x2 = x2;
	singleRect.y2 = y2;

	al_draw_filled_rounded_rectangle(singleRect.x1, singleRect.y1, singleRect.x2, singleRect.y2, 5.0f, 5.0f, color);

	al_draw_rounded_rectangle(singleRect.x1, singleRect.y1, singleRect.x2, singleRect.y2, 5.0f, 5.0f, al_map_rgb(255, 255, 255), 2.0f);
}

// ----------------------------------------------------------------------------
//
// Show the ship in it's sideview on the screen
void gam_drawSideView()
// ----------------------------------------------------------------------------
{
	int count;
	int lifts = 8;
	int toLifts = 0;
	int x1;
	int y1;
	ALLEGRO_COLOR tempAlert;

	count = 0;
	x1 = sideviewLevels[count].x2;
	y1 = sideviewLevels[count].y2;
	draw3dRectangle(x1, y1, sideviewLevels[count].x1, sideviewLevels[count].y1, SHIP_COLOR);
	toLifts++;

	for (count = 1; count != MAX_LEVELS - lifts; count++)
		{
			x1 = sideviewLevels[count].x1;
			y1 = sideviewLevels[count].y1;
			draw3dRectangle(x1, y1, sideviewLevels[count].x2, sideviewLevels[count].y2, SHIP_COLOR);
			toLifts++;
		}

	if (currentMode == MODE_LIFT_VIEW)
		{
			//currentLevel = tunnel[currentTunnel].current;
			// highlite current level
			if (0 == currentLevel)
				draw3dRectangle(sideviewLevels[currentLevel].x2, sideviewLevels[currentLevel].y2, sideviewLevels[currentLevel].x1, sideviewLevels[currentLevel].y1, ACTIVE_DECK_COLOR);
			else
				draw3dRectangle(sideviewLevels[currentLevel].x1, sideviewLevels[currentLevel].y1, sideviewLevels[currentLevel].x2, sideviewLevels[currentLevel].y2, ACTIVE_DECK_COLOR);
		}
	else
		{
			switch (currentAlertLevel)
				{
					case ALERT_GREEN_TILE:
						tempAlert = al_map_rgb(0x00, 0xff, 0x00);
						break;
					case ALERT_YELLOW_TILE:
						tempAlert = al_map_rgb(0xff, 0xff, 0x00);
						break;
					case ALERT_RED_TILE:
						tempAlert = al_map_rgb(0xff, 0x00, 0x00);
						break;
				}

			if (0 == currentLevel)
				draw3dRectangle(sideviewLevels[currentLevel].x2, sideviewLevels[currentLevel].y2, sideviewLevels[currentLevel].x1, sideviewLevels[currentLevel].y1, tempAlert);
			else
				draw3dRectangle(sideviewLevels[currentLevel].x1, sideviewLevels[currentLevel].y1, sideviewLevels[currentLevel].x2, sideviewLevels[currentLevel].y2, tempAlert);
		}


	if ((currentTunnel != 3) && (currentTunnel != 6))
		{
			count = 13;
			draw3dRectangle(sideviewLevels[count].x1, sideviewLevels[count].y1, sideviewLevels[count].x2, sideviewLevels[count].y2, SHIP_COLOR);
		}
	else
		{
			// using tunnel connecting to level 13
			if (currentLevel != 13)
				{
					count = 13;
					draw3dRectangle(sideviewLevels[count].x1, sideviewLevels[count].y1, sideviewLevels[count].x2, sideviewLevels[count].y2, SHIP_COLOR);
				}
		}

	// fill in engine part
	draw3dRectangle(sideviewLevels[7].x1, sideviewLevels[7].y1, sideviewLevels[7].x2, sideviewLevels[7].y2, ENGINE_COLOR);
	//
	// draw the lifts
	//
	for (count = 0; count != lifts; count++)
		{
			draw3dRectangle(sideviewLevels[count + toLifts].x1, sideviewLevels[count + toLifts].y1, sideviewLevels[count + toLifts].x2, sideviewLevels[count + toLifts].y2, LIFT_COLOR);
		}

	// highlight the current tunnel
	draw3dRectangle(sideviewLevels[21 + currentTunnel].x1, sideviewLevels[21 + currentTunnel].y1, sideviewLevels[21 + currentTunnel].x2, sideviewLevels[21 + currentTunnel].y2, ACTIVE_LIFT_COLOR);

	if (currentMode == MODE_LIFT_VIEW)
		{
			sys_setFont(FONT_LARGE);
			sys_setFontColor(0.8f, 0.8f, 0.8f, 1.0f);
			draw3dRectangle(10, winHeight - 85, 30, winHeight - 65, ACTIVE_LIFT_COLOR);
			sys_printString(33.0f, winHeight - 95, "Active lift");

			draw3dRectangle(10, winHeight - 55, 30, winHeight - 35, ACTIVE_DECK_COLOR);
			sys_printString(33.0f, winHeight - 65, "Current deck");

			draw3dRectangle(10, winHeight - 25, 30, winHeight - 5, LIFT_COLOR);
			sys_printString(33.0f, winHeight - 35, "Inactive lift");
		}
	else
		{
			sys_setFont(FONT_LARGE);
			sys_setFontColor(0.8f, 0.8f, 0.8f, 1.0f);

			draw3dRectangle(10, winHeight - 55, 30, winHeight - 35, tempAlert);
			sys_printStringExt(33.0f, winHeight - 65, "You are here - %s", shipLevel[currentLevel].levelName);
		}
}
