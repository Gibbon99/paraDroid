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

extern bool		    processFadeValue;
extern int			currentFadeAction;
extern float		fadeValue;
extern float		fadeSpeed;			// Setup from script
extern int			modeAfterFadeOff;
extern float		indicatorAnimSpeed;
extern float		staticScreenShowTime;
extern float		staticImageCounterTime;

extern int 		    tilePosX;
extern int 		    tilePosY;
extern float 		pixelX;
extern float 		pixelY;
extern string	    statusText;
extern float        statusTextX, statusTextY, scoreTextX;


// Copy all the tiles that are visible this screen to the array
void gam_drawAllTiles();

// Draw an image - pass in index and location
void gam_drawImage(int imageIndex, float posX, float posY, int flags);

// Setup a bitmap to hold graphics before blitting to screen
bool gam_setupFBO(int flags);

// Modify the global fadeValue - set to run on mode change
void gam_processFadeValue();

// Draw the frame of a sprite image
void gam_drawSprite(int currentFrame, int imageIndex, cpVect position, float angle, ALLEGRO_COLOR color);

// Pass in image index, position, tint color
void gam_drawBitmapFromScript(int index, float posX, float posY, float red, float green, float blue, float alpha);

// Draw a sprite from a script
void gam_drawSpriteFromScript(int index, float posX, float posY, float red, float green, float blue, float alpha);

// Free memory for tileTexCoords
void gam_freeTileTexMemory();

// Setup up precalculated coords for the tiles
void gam_calcTileTexCoords();

// Return texture coords for passed in tile
cpVect gam_getTileTexCoords(int whichTile);

// Show the ship in it's sideview on the screen
void gam_drawSideView();

// Draw the starfield
void gam_drawStarfield();

// Draw the current level onto the entire screen
void term_showCurrentLevel();

// Draw the HUD and status text and current score
void gam_drawHud();

// Change state to display status text in HUD
void gam_setHUDState(int newState);

// Animate the level indicator shown on deck map
void gam_animateIndicator(float thinkInterval);