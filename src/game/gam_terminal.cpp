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

int currentTermMenu;
int currentTermMode;

float term0PosX;
float term0PosY;

float term1PosX;
float term1PosY;

float term2PosX;
float term2PosY;

float term3PosX;
float term3PosY;

int droidTypeDBIndex;
int dbTexWidth;
int dbTexHeight;
int dbCurrentFrame = 0;

float dbImagePositionX;
float dbImagePositionY;

float dbStartX;
float dbStartY;
float dbWidth;
float dbHeight;
float dbScrollspeed;
float dbBackRed;
float dbBackGreen;
float dbBackBlue;
float dbBackAlpha;
float dbFontRed;
float dbFontGreen;
float dbFontBlue;
float dbFontAlpha;
float dbScrollDelay;


_scrollBox 	databaseScrollBox;
string 		databaseScrollText;
static int 	textCounter = 0;

//-----------------------------------------------------------------------------
//
// Draw the initial menu showing the sprites

//------------------------------------------------------------
//
// Get the next droid model

bool gam_loadDroidModel(int whichDroid)
//------------------------------------------------------------
{
	char tempFileName[MAX_STRING_SIZE];

	strcpy(tempFileName, "");
	strcpy(tempFileName, dataBaseEntry[whichDroid].dbImageFileName);
	strcat(tempFileName, ".jpg");

	con_print(true, false, "Load droid model - image name : %s", tempFileName);

	return io_loadImage(dbImage, tempFileName);
}

//----------------------------------------------------------
//
// Called from endScreen to set currentFrame to 0
void gam_setFrameStart()
//----------------------------------------------------------
{
	dbCurrentFrame = 0;
}

//----------------------------------------------------------
//
// Draw the 3d droid model image
// Pass in -1 to PosX to use settings from script
//
void gam_drawDB_Droid(float posX, float posY)
//----------------------------------------------------------
{
	cpVect dbImagePosition;

	if (posX < 0)
	{
		dbImagePosition.x = dbImagePositionX;
		dbImagePosition.y = dbImagePositionY;
	}
	else
	{
		dbImagePosition.x = posX;
		dbImagePosition.y = posY;
	}
//	dbTexWidth = al_get_bitmap_width(image[dbImage].image) / NUM_DB_FRAMES;
//	dbTexHeight = al_get_bitmap_height(image[dbImage].image);
	//
	// show loaded image
	//
	gam_drawSprite(dbCurrentFrame, dbImage, dbImagePosition, -1.0f, al_map_rgba_f(1.0f, 1.0f, 1.0f, 1.0f));
}

//------------------------------------------------------------
//
// Move to the next droid in the database

void gam_getNextDroid()
//------------------------------------------------------------
{
	// TODO (dberry#1#): Remove when transfer is done

	//		if (playerDroid.droidType < currentDroidType)
	//			currentDroidType++;

	if (droidTypeDBIndex < 23)
	  {
		sys_playSound(SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE);
		droidTypeDBIndex++;
		gam_loadDroidModel(droidTypeDBIndex);
		gam_setupDBText();
	  }
	else
	  sys_playSound(SND_KEYPRESS_BAD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE);
}

//------------------------------------------------------------
//
// Move to the previous droid in the database

void gam_getPreviousDroid()
//------------------------------------------------------------
{
	if (droidTypeDBIndex > 0)
	  {
		sys_playSound(SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE);
		droidTypeDBIndex--;
		gam_loadDroidModel(droidTypeDBIndex);
		gam_setupDBText();
	  }
	else
	  sys_playSound(SND_KEYPRESS_BAD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE);
}

//----------------------------------------------------------
//
// Animate the 3d database Image

void gam_animateDB_Droid()
//----------------------------------------------------------
{
	static float delayCount = 0.0f;

	delayCount += 0.5f;
	if (delayCount > 1.0f)
	  {
		delayCount = 0.0f;
		dbCurrentFrame++;
		if (dbCurrentFrame == NUM_DB_FRAMES)
		  dbCurrentFrame = 0;
	  }
}

//------------------------------------------------------------
//
// Database routines
//
//------------------------------------------------------------

//------------------------------------------------------------
//
// Callback for more database text

void gam_moreDatabaseText()
//------------------------------------------------------------
{
	textCounter++;
	if (textCounter > 11)
	  textCounter = 0;

	databaseScrollText.clear();
	switch (textCounter) {
		case 0:
			databaseScrollText = "Description: ";
			databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].description);
			break;
		case 1:
			databaseScrollText = "ClassName: ";
			databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].className);
			break;
		case 2:
			databaseScrollText = "Height: ";
			databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].height);
			break;
		case 3:
			databaseScrollText = "Weight: ";
			databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].weight);
			break;
		case 4:
			databaseScrollText = "Drive: ";
			databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].drive);
			break;
		case 5:
			databaseScrollText = "Brain: ";
			databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].brain);
			break;
		case 6:
			databaseScrollText = "Weapon: ";
			databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].weapon);
			break;
		case 7:
			databaseScrollText = "Sensor 1: ";
			databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].sensor1);
			break;
		case 8:
			databaseScrollText = "Sensor 2: ";
			databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].sensor2);
			break;
		case 9:
			databaseScrollText = "Sensor 3: ";
			databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].sensor3);
			break;
		case 10:
			databaseScrollText = "Notes: ";
			databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].notes);
			break;
		case 11:
			databaseScrollText = " ! ! ! ! ! ! ! ! - Information repeats - ! ! ! ! ! # ";
			databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].notes);
			break;

	}
	databaseScrollText.append(" # ");
	databaseScrollBox.sourceText = databaseScrollText;
	databaseScrollBox.charPtr = -1;
}

//------------------------------------------------------------
//
// Setup the scrollbox for droid information

void gam_setupDBText()
//------------------------------------------------------------
{
	static bool databaseScrollSetup = false;
	if (false == databaseScrollSetup)
	  {
		databaseScrollSetup = true;
		databaseScrollText.clear();
		databaseScrollText = "Description: ";
		databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].description);
		databaseScrollText.append(" ! # ");
		gui_setupScrollBox(SCROLLBOX_DB, &databaseScrollBox, databaseScrollText, (ExternFunc) gam_moreDatabaseText);
	  }
	textCounter = 0;
	databaseScrollBox.linesToPrint = 0;
	databaseScrollBox.scrollY = databaseScrollBox.height; // scroll starting position
	databaseScrollBox.charPtr = 0; // start of the text

	for (int i = 0; i != MAX_LINES_IN_SCREEN; i++)
	  strcpy(databaseScrollBox.line[i].text, "");

	databaseScrollText.clear();
	databaseScrollText.append("Description: ");
	databaseScrollText.append(dataBaseEntry[droidTypeDBIndex].description);
	databaseScrollText.append(" ! # ");
	databaseScrollBox.sourceText = databaseScrollText;
}
