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

static int			staticImageIndex = static_1;
static float		staticScreenCounter;
static float		staticImageCounter;
static float		endScreenCounter;
float				staticScreenShowTime;		// Set from script
float				staticImageCounterTime;		// Set from script
float				endScreenCounterTime;		// Set from script

//-----------------------------------------------------------------------------
//
// Draw the end game screen
void gam_drawEndScreen()
//-----------------------------------------------------------------------------
{
	float 		textPosX, textPosY;
	float		dbTexWidth, dbTexHeight;

	string		messageText;

	float		imagePosX, imagePosY;

	dbTexWidth = al_get_bitmap_width ( image[dbImage].image ) / NUM_DB_FRAMES;
	dbTexHeight = al_get_bitmap_height ( image[dbImage].image );

	imagePosX = ( winWidth  / 2 ) - ( dbTexWidth / 2 ) ;
	imagePosY = ( winHeight - dbTexHeight ) / 2 ;

	messageText = gui_getString ( "TransTerm" );
	textPosX = ( winWidth - al_get_text_width ( font[currentFont].ttfFont, messageText.c_str() ) ) / 2;
	textPosY = imagePosY + ( dbTexHeight / 2 ) + ( al_get_font_line_height ( font[currentFont].ttfFont ) * 3 );

	gam_setFrameStart ();

	switch ( currentMode )
		{
			case MODE_END_STATIC:
			case MODE_END_STATIC_PRE:
				gam_drawImage ( staticImageIndex, 0.0f, 0.0f, flagFullScreen );
				gam_drawHud();
				break;

			case MODE_END_TEXT:
				gam_drawHud();
				sys_printString ( textPosX, textPosY, messageText );
				gam_drawDB_Droid ( imagePosX, imagePosY );
				break;
		}
}

//-----------------------------------------------------------------------------
//
// Process the end game screen
void gam_processEndScreen()
//-----------------------------------------------------------------------------
{

	switch ( currentMode )
		{
			case MODE_END_STATIC_PRE:	// Try to work around sound playing not starting right away
				staticScreenCounter = staticScreenShowTime;
				staticImageCounter = staticImageCounterTime;
				currentMode = MODE_END_STATIC;
				break;

			case MODE_END_STATIC:
				if ( true == playSounds )
					{
						if ( false == sys_isSoundPlaying ( SND_END_TRANSMISSION_1 ) )
							{
								sys_playSound ( SND_END_TRANSMISSION_1, SND_PAN_CENTER, ALLEGRO_PLAYMODE_LOOP );
							}
					}

				staticScreenCounter -= 1.0f * thinkInterval;

				if ( staticScreenCounter < 0.0f )
					{
						staticScreenCounter = staticScreenShowTime;
						if ( true == sys_isSoundPlaying ( SND_END_TRANSMISSION_1 ) )
						{
							sys_stopSound ( SND_END_TRANSMISSION_1 );
						}

						gam_loadDroidModel ( sprite_999 - 2 );

						sys_stopAllSounds();
						if (true == playSounds)
							sys_playSound ( SND_END_TRANSMISSION_2, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );

						endScreenCounter = endScreenCounterTime;
						sys_changeMode ( MODE_END_TEXT, false );
						return;
					}

				staticImageCounter -= 1.0f * thinkInterval;
				if ( staticImageCounter < 0.0f )
					{
						staticImageCounter = staticImageCounterTime;
						staticImageIndex++;
						if ( staticImageIndex > static_9 )
							staticImageIndex = static_1;
					}
				break;

			case MODE_END_TEXT:
				if ( false == playSounds )
					{
						endScreenCounter -= 1.0f * thinkInterval;
						if ( endScreenCounter < 0.0f )
							{
								sys_changeMode ( MODE_END_TEXT_FINISH, false );
							}
					}
				else
					{
						if ( false == sys_isSoundPlaying ( SND_END_TRANSMISSION_2 ) )
							{
								sys_changeMode ( MODE_END_TEXT_FINISH, false );
								return;
							}
					}
				break;

			case MODE_END_TEXT_FINISH:
				if (-1 == gam_addNewScore())	// Didn't get a high score - just show the table
				{
					gui_populateScoreTableGUI();
					currentGUIScreen = gui_findIndex ( GUI_OBJECT_SCREEN, "scrDisplayScoreTable" );
					gui_setObjectFocus ("buttonDisplayYes");
				}
				else
				{
					// Goto score edit screen
					currentGUIScreen = gui_findIndex ( GUI_OBJECT_SCREEN, "scrEditScoreTable" );
					gui_setObjectFocus("scoreSliderOne");
				}

				gam_restart();
				sys_changeMode ( MODE_GUI, false );
				break;
		}
}
