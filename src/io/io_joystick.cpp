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

int		numJoysticks;

typedef struct
{
	int						numAxes;
	vector<float>			axesState;
} _stick;


typedef struct
{
	ALLEGRO_JOYSTICK				*joystick;
	ALLEGRO_JOYSTICK_STATE			joystickState;
	int								numSticks;
	int								numButtons;
	vector<bool>					buttonState;
	vector<_stick>					stick;
	string							name;
} _joyStick;

vector<_joyStick>					joySticks;

#define deadSpot 0.005

//-------------------------------------------------------------
//
// Process joystick states
void io_processJoystick()
//-------------------------------------------------------------
{
	if ( 0 == numJoysticks )
		return;

	//
	// Move LEFT
	if ( joySticks[0].joystickState.stick[0].axis[JOYSTICK_AXIS_X] < 0.0f )
		inputAction[gameLeft].currentlyDown = true;
	else
		inputAction[gameLeft].currentlyDown = false;
	//
	// Move RIGHT
	if ( joySticks[0].joystickState.stick[0].axis[JOYSTICK_AXIS_X] > deadSpot )
		inputAction[gameRight].currentlyDown = true;
	else
		inputAction[gameRight].currentlyDown = false;
	//
	// Move UP
	if ( joySticks[0].joystickState.stick[0].axis[JOYSTICK_AXIS_Y] < 0.0f )
		inputAction[gameUp].currentlyDown = true;
	else
		inputAction[gameUp].currentlyDown = false;
	//
	// Move DOWN
	if ( joySticks[0].joystickState.stick[0].axis[JOYSTICK_AXIS_Y] > deadSpot )
		inputAction[gameDown].currentlyDown = true;
	else
		inputAction[gameDown].currentlyDown = false;

// TODO: Need to check state
	if ( joySticks[0].joystickState.button[0] )
		inputAction[gameFire].currentlyDown = true;
	else
		inputAction[gameFire].currentlyDown = false; //io_getKeyStateUp ( gameFire );
}

//-------------------------------------------------------------
//
// Get the current state of the joystick
void io_readJoystick()
//-------------------------------------------------------------
{
	for ( int i = 0; i != numJoysticks; i++ )
		{
			al_get_joystick_state ( joySticks[i].joystick, &joySticks[i].joystickState );
		}
}

//-------------------------------------------------------------
//
// Get information about any connected joysticks
void io_setupJoystickValues ()
//-------------------------------------------------------------
{
	_joyStick		tempJoyStick;
	_stick			tempStick;

	//
	// Clear out
	joySticks.clear();
	//
	// How many are there
	numJoysticks = al_get_num_joysticks();

	if ( 0 == numJoysticks )
		{
			inputMethod = INPUT_KEYBOARD;
			return;
		}
	else
		{
			inputMethod = io_readConfigValuesInt ( "inputMethod" );
		}

	//
	// Clear out any previous state
	if ( joySticks.size() > 0 )
		joySticks.clear();
	//
	// Get enough space for each joystick
	joySticks.reserve ( numJoysticks );

	for ( int i = 0; i != numJoysticks; i++ )
		{
			tempJoyStick.joystick = al_get_joystick ( i );
			tempJoyStick.name = al_get_joystick_name ( tempJoyStick.joystick );
			//
			// Number of sticks on this joystick
			tempJoyStick.numSticks = al_get_joystick_num_sticks ( tempJoyStick.joystick );
			tempJoyStick.stick.reserve ( tempJoyStick.numSticks );
			//
			// Number of buttons on this joystick
			tempJoyStick.numButtons = al_get_joystick_num_buttons ( tempJoyStick.joystick );
			tempJoyStick.buttonState.reserve ( tempJoyStick.numButtons );

			tempJoyStick.stick.reserve ( tempJoyStick.numSticks );
			for ( int j = 0; j != tempJoyStick.numSticks; j++ )
				{
					tempStick.numAxes = al_get_joystick_num_axes ( tempJoyStick.joystick, j );
					tempStick.axesState.reserve ( tempStick.numAxes );
					tempJoyStick.stick.push_back ( tempStick );
				}

			joySticks.push_back ( tempJoyStick );
		}
}

//
// Define action button
//
