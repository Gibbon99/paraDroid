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

extern float		useKeysDelay;	// From scripts
extern float		consoleKeysDelay;
extern bool			repeatKeys;
extern float		repeatKeysDelay;	// Set from script

// Process keyboard events based on current mode
void io_readKeyboard();

// Read a key for keycode capture
void io_readKeyCodeInput(ALLEGRO_EVENT event);

// Get the state of the Down key press dependant on repeatKeys setting
inline bool io_getKeyStateDown ( int keyName );

// Set the repeat state for the keyboard
void io_setRepeatKeysState(bool newState);

// Process the keys already set from the event based system
void io_processKeyboard(double frameTime);
