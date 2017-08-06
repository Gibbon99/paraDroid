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

#define PROGRAM_NAME                "Droid3"

#define MAX_STRING_SIZE             128

#define MAX_PATH       				128

#define SPRITE_SIZE					32
#define SPRITE_SIZE_GAME			28
#define DROID_BODY_SIZE			    20
#define NUM_FRAMES_IN_DROID         8
#define NUM_FRAMES_IN_BULLET	    8
#define NUM_FRAMES_IN_EXPLOSION     25

#define NO_PASS_TILE	   			40  // Solid tile

#define COLLIDE_TILE				1
#define COLLIDE_PLAYER				2
#define COLLIDE_DROID				3

#define MAX_NUM_SPRITES			20	// Number of enemy sprites on each level

#define MAX_NUM_BULLETS		    12	// How many on screen at once

#define NUM_SPRITE_TYPES   	    24	// Used for database - each individual type

#define WAYPOINT_UP				0
#define WAYPOINT_DOWN		    1

#define MOVE_LEFT				0
#define MOVE_RIGHT			    1
#define MOVE_UP					2
#define MOVE_DOWN			    3

#define FADE_OFF				0
#define FADE_ON				    1

#define TILE_CHECK_BULLET	    0
#define TILE_CHECK_PLAYER	    1
#define TILE_CHECK_ASTAR	    2
#define TILE_CHECK_LOS			3

#define DAMAGE_BULLET			0
#define DAMAGE_EXPLOSION 	    1
#define DAMAGE_COLLISION 	    2

#define	LIFT_COLOR			    al_map_rgb(18, 76, 88)
#define	ACTIVE_LIFT_COLOR	    al_map_rgb(38, 221, 188)
#define	SHIP_COLOR			    al_map_rgb(142, 182, 12)
#define	ACTIVE_DECK_COLOR	    al_map_rgb(71, 199, 232)
#define	ENGINE_COLOR		    al_map_rgb(34, 34, 34)

#define MAX_LINES_IN_SCREEN		64
#define MAX_NUM_CHARS_IN_LINE	128

// Terminal defines
#define NUM_DB_FRAMES 		    32	// Number of frames in 3d view file

#define NUM_STATIC_FRAMES		9

#define SCROLLBOX_INTRO		    0
#define SCROLLBOX_DB		    1

#define SELECT_COUNTER          12
#define TRANSFER_COUNTER        10

#define NUM_SCORE_CHARS         7

#define COL_PURPLE	            0.54f,0.27f,0.68f,1.0f
#define COL_YELLOW	            1.0f,1.0f,0.0f,1.0f
#define COL_BLACK	            0.0f,0.0f,0.0f,1.0f
#define COL_WHITE				1.0f,1.0f,1.0f,1.0f

#define HUD_STATE_HEALING		1
#define HUD_STATE_TRANSFER		2
#define HUD_STATE_TERMINAL		3
#define HUD_STATE_MOVE			4
#define HUD_STATE_PAUSED		5
#define HUD_STATE_LIFT			6
#define HUD_STATE_SHIP_VIEW		7
#define HUD_STATE_DATABASE		8
#define HUD_STATE_DECK_VIEW		9
#define HUD_STATE_CAPTURED		10
#define HUD_STATE_SELECT_TIME	11
#define HUD_STATE_TIME_LEFT		12
#define HUD_STATE_CHARGING		13
#define HUD_STATE_HEALED		14
#define HUD_STATE_DEADLOCK		15
#define HUD_STATE_LOST			16
#define HUD_STATE_BEAM_ON		17

#define BULLET_TYPE_NORMAL		0
#define BULLET_TYPE_SINGLE		1
#define BULLET_TYPE_DOUBLE		2
#define BULLET_TYPE_DISRUPTER	3

#define IGNORE_COLLISION_TIME	2.0f

#define MAX_JOYSTICKS_NUM		2
#define MAX_JOYSTICK_AXES		3
#define MAX_JOYSTICK_BUTTONS	16
#define JOYSTICK_AXIS_X			0
#define JOYSTICK_AXIS_Y			1