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

//-----------------------------------------------------------------------------
//
// Enum different types for the keyboard actions keys
//
//-----------------------------------------------------------------------------

extern int *keystate;         // holds all the keystates

struct _inputAction
{
	int             keyValue;
	bool            currentlyDown;
	bool			beenUp;
	float			repeatDelayCount;
	string          stringValue;
};

extern  _inputAction            inputAction[NUMBER_ACTIONS];

// assign the keyvalues used for actions
void io_mapActionKeys();

// Save the current key bindings for the keyboard
bool io_saveKeyBindings(string fileName);

// Read in the keybindings used from the file
bool io_readKeyBindings(string fileName);

// Perform the keybind functions - if keybind file does not exist then populate
// the inputActions array and save it, then reload it
bool io_doKeyBindings(string fileName);

// Set all keys to false state
void io_resetKeyboardStates();
