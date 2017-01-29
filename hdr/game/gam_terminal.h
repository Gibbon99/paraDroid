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

#define TERM_MODE_MENU		0
#define TERM_MODE_DB		1
#define TERM_MODE_LEVEL		2
#define TERM_MODE_SIDE		3

extern int		currentTermMenu;
extern int		currentTermMode;

extern float	term0PosX;
extern float	term0PosY;

extern float	term1PosX;
extern float 	term1PosY;

extern float	term2PosX;
extern float	term2PosY;

extern float	term3PosX;
extern float	term3PosY;

extern float	dbImagePositionX;
extern float	dbImagePositionY;

extern float 	dbStartX;
extern float 	dbStartY;
extern float 	dbWidth;
extern float 	dbHeight;
extern float 	dbScrollspeed;
extern float 	dbBackRed;
extern float 	dbBackGreen;
extern float 	dbBackBlue;
extern float 	dbBackAlpha;
extern float 	dbFontRed;
extern float 	dbFontGreen;
extern float 	dbFontBlue;
extern float 	dbFontAlpha;
extern float 	dbScrollDelay;

extern int		droidTypeDBIndex;

// Load the 3d droid model image
void gam_drawDB_Droid(float posX, float posY);

// Animate the 3d database Image
void gam_animateDB_Droid();

// Setup the scrollbox for droid information
void gam_setupDBText();

// Move to the next droid in the database
void gam_getNextDroid();

// Move to the previous droid in the database
void gam_getPreviousDroid();

// Get the next droid model
bool gam_loadDroidModel(int whichDroid);

// Called from endScreen to set currentFrame to 0
void gam_setFrameStart();