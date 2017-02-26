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

extern int		numJoysticks;
extern int		numJoystickButtons;
extern int		numJoystickAxes[MAX_JOYSTICKS_NUM];

extern float	joysticks[MAX_JOYSTICKS_NUM][MAX_JOYSTICK_AXES];
extern bool		joystickButtons[MAX_JOYSTICK_BUTTONS];

extern ALLEGRO_JOYSTICK_STATE	joystickState;

// Get information about any connected joysticks
void io_setupJoystickValues();

// Get the current state of the joystick
void io_readJoystick();

// Process joystick states
void io_processJoystick();
