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

using namespace std;
#include <vector>
#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>

//#include "pthread.h"

//#include "system/mmgr.h"


#include "../hdr/script/angelscript.h"
#include "../hdr/script/scriptstdstring.h"
#include "../hdr/script/scriptarray.h"

//
//-----------------------------------------------------------------------------
// Generic callback function pointer
// needs the (...) to allow for any parameter passing without hardcoding
// different types for different functions
//-----------------------------------------------------------------------------
typedef int	(*ExternFunc)(...);


#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

//
// Shared with script files
//
#include "../data/scripts/commonDefines.h"

#include "system/sys_defines.h"

#include "../hdr/chipmunk/chipmunk.h"

#include "../hdr/physfs/physfs.h"

#include "gui/gui_main.h"
#include "gui/gui_render.h"
#include "gui/gui_scrollBox.h"
#include "gui/gui_text.h"

#include "io/io_logFile.h"
#include "io/io_configFile.h"
#include "io/io_keyboard.h"
#include "io/io_images.h"
#include "io/io_keyBind.h"
#include "io/io_packFile.h"
#include "io/io_joystick.h"

#include "system/sys_random.h"
#include "system/sys_script.h"
#include "system/sys_console.h"
#include "system/sys_conCommands.h"
#include "system/sys_defines.h"
#include "system/sys_shutdown.h"
#include "system/sys_startup.h"
#include "system/sys_timing.h"
#include "system/sys_frame.h"
#include "system/sys_utils.h"
#include "system/sys_physicsCollisions.h"
#include "system/sys_physics.h"
#include "system/sys_font.h"
#include "system/sys_sound.h"
#include "system/sys_maths.h"
#include "system/sys_debug.h"

#include "game/gam_scoreTable.h"
#include "game/gam_transfer.h"
#include "game/gam_droid.h"
#include "game/gam_levels.h"
#include "game/gam_starfield.h"
#include "game/gam_render.h"
#include "game/gam_player.h"
#include "game/gam_droidAI.h"
#include "game/gam_doors.h"
#include "game/gam_sideView.h"
#include "game/gam_LOS.h"
#include "game/gam_database.h"
#include "game/gam_bullet.h"
#include "game/gam_terminal.h"
#include "game/gam_intro.h"
#include "game/gam_spotLight.h"
#include "game/gam_particles.h"
#include "game/gam_pathFind.h"
#include "game/gam_endScreen.h"

//#include "astar/astar.h"

#define RET_FALSE(n, v) {io_logToFile(n, v); return false;}

extern ALLEGRO_DISPLAY     	        *display;
extern ALLEGRO_EVENT_QUEUE  	    *eventQueue;
extern ALLEGRO_FONT        	        *builtInFont;
extern ALLEGRO_EVENT       	        event;
extern ALLEGRO_BITMAP			    *bitmapFBO;

extern int					fpsPrint;
extern int					thinkFpsPrint;
extern double				renderTick1;
extern double				thinkTick1;
extern long					frameCount;

extern float				baseGameSpeed;

extern float				progressBarLoadTimes;

extern bool					useHoldBitmap;
extern bool                	gameIsRunning;
extern bool					useBackingBitmap;

extern string				gameVersion;

extern int					winWidth;
extern int					winHeight;
extern int					fullScreenValue;

extern int                 	currentMode;

extern int					inputMethod;

extern bool					useCollisionDetection;
extern bool					debugShowStats;

extern int					maxNumSpritesForPhysics;

extern bool					stopLoadingThread;
extern ALLEGRO_THREAD		*loadingThread;
extern bool					loadedAllBitmaps;
extern bool					loadingThreadHasFinished;
extern bool					confirmExit;	// Used as flag in scrExitQuestion GUI

//-----------------------------------------------------------------
//
// Values used for performance testing
extern int					profileTileCalcSkipped;
extern int					profileTotalTileDrawn;

extern double 				profileTimeStart;
extern double				profileTimeEnd;
extern float				profileTimeAverage;
extern float				profileTimeCount;
extern float				profileTimeTotal;

extern double				profileParticleRenderStart;
extern double 				profileParticleRenderEnd;
extern double				profileParticleThinkStart;
extern double				profileParticleThinkEnd;
extern bool					profileParticles;

extern int 					tileCollision;

//
//-----------------------------------------------------------------
