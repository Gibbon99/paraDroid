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

#include "hdr/sys_globals.h"

ALLEGRO_DISPLAY      		*display = NULL;
ALLEGRO_FONT         		*builtInFont = NULL;

ALLEGRO_KEYBOARD_STATE  	*keyboardState = NULL;
ALLEGRO_EVENT_QUEUE  		*eventQueue = NULL;
ALLEGRO_EVENT        		event;
ALLEGRO_BITMAP				*bitmapFBO;

string				gameVersion;
float				baseGameSpeed;

float				progressBarLoadTimes;
bool 				gameIsRunning = true;
bool				useHoldBitmap = true;
bool				useBackingBitmap = true;

bool				useCollisionDetection = false;

double				next_game_tick;
int					loops;
float				interpolation;
long				frameCount;

int					winWidth;
int					winHeight;
int					fullScreenValue;

int 				tileCollision = false;

int					inputMethod;	// Set from GUI value

//-----------------------------------------------------------------------------
//
// Start
int main ( int argc, char **argv )
//-----------------------------------------------------------------------------
{
	double singleTimeValue;

	gameVersion = "v0.8";

	if ( false == sys_startOnce() )
		{
			exit ( -1 );
		}

	renderTick1 = 0.0f;
	thinkTick1 = 0.0f;
	frameCount = 0;

	next_game_tick = al_get_time();

	while ( true == gameIsRunning )
		{
			loops = 0;
			singleTimeValue = al_get_time();

			while ( singleTimeValue > next_game_tick && loops < MAX_FRAMESKIP )
				{
					sys_updateFrame();
					next_game_tick += SKIP_TICKS;
					loops++;
					sys_CalculateThinkFrameRate ( singleTimeValue );
				}

			if ( false == processFadeValue )
				{
					io_readKeyboard();	// Best in 30fps loop - or as fast as possible??
					//
					// Are we using a joystick for input
					//
					if (inputMethod == INPUT_JOYSTICK)
						io_readJoystick();
				}

			interpolation = float ( singleTimeValue + SKIP_TICKS - next_game_tick ) / float ( SKIP_TICKS );

			sys_displayFrame ( interpolation );

			sys_CalculateFrameRate ( singleTimeValue );
		}

	sys_shutDown();

	return 0;
}
