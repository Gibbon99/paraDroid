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
#include "../../hdr/game/gam_transfer.h"
#include "../../hdr/system/sys_sound.h"
#include "../../hdr/game/gam_terminal.h"

int 		currentMode;
double 		profileRenderFrameStart, profileRenderFrameEnd;

float 		loadingBarPercent;
char 		loadingStatus[MAX_STRING_SIZE];

bool 		gamePaused = false;

static float splashCounter = 5.0f;

//-----------------------------------------------------------------------------
//
// Display loading screen
void sys_displayLoadingScreen()
//-----------------------------------------------------------------------------
{
	float rectWidth, rectHeight;
	cpVect rectTopLeft, rectBotRight;

	float barWidth, barHeight, barFill;
	cpVect barTopLeft, barBotRight;
	cpVect fillTopLeft, fillBotRight;

	rectWidth = winWidth * 0.75;
	rectHeight = winHeight * 0.25;

	rectTopLeft.x = ( winWidth - rectWidth ) / 2;
	rectTopLeft.y = ( winHeight - rectHeight ) / 2;

	rectBotRight.x = rectTopLeft.x + rectWidth;
	rectBotRight.y = rectTopLeft.y + rectHeight;

	barWidth = rectWidth * 0.75;
	barHeight = rectHeight * 0.25;

	barTopLeft.x = ( ( rectWidth - barWidth ) / 2 ) + rectTopLeft.x;
	barTopLeft.y = ( ( rectHeight - barHeight ) / 2 ) + rectTopLeft.y;

	barBotRight.x = barTopLeft.x + barWidth;
	barBotRight.y = barTopLeft.y + barHeight;
	//
	// How far along are we
	barFill = loadingBarPercent * barWidth;

	fillTopLeft = barTopLeft;
	fillBotRight.x = barTopLeft.x + barFill;
	fillBotRight.y = barTopLeft.y + barHeight;

	al_draw_filled_rounded_rectangle ( rectTopLeft.x, rectTopLeft.y, rectBotRight.x, rectBotRight.y, 5.0f, 5.0f, al_map_rgb_f ( 0.2, 0.2, 0.2 ) );
	al_draw_rounded_rectangle ( barTopLeft.x, barTopLeft.y, barBotRight.x, barBotRight.y, 5.0f, 5.0f, al_map_rgb_f ( 1.0, 1.0, 1.0 ), 2.0f );
	al_draw_filled_rounded_rectangle ( fillTopLeft.x, fillTopLeft.y, fillBotRight.x, fillBotRight.y, 5.0f, 5.0f, al_map_rgb_f ( 1.0, 0.2, 0.2 ) );

	al_draw_textf ( builtInFont, al_map_rgb_f ( 1.0f, 1.0f, 1.0f ), rectTopLeft.x + 2.0f, rectTopLeft.y + 2.0f, ALLEGRO_ALIGN_LEFT, "Loading : [ %s ]", loadingStatus );
}

//-----------------------------------------------------------------------------
//
// Routine to display stuff onto the screen
//
// Pass in interpolation to draw screen
void sys_displayFrame ( float interpolation )
//-----------------------------------------------------------------------------
{
	float introBlockOut = 60.0f;

	frameCount++;

	profileRenderFrameStart = al_get_time();

	if ( true == useBackingBitmap )
		{
			al_set_target_bitmap ( bitmapFBO );
		}

	al_clear_to_color ( al_map_rgb ( 0, 0, 0 ) );

	switch ( currentMode )
		{
		case MODE_LOADING:
			con_drawScreen ( false, interpolation );
			sys_displayLoadingScreen();

			if ( true == stopLoadingThread )
				{
					al_set_thread_should_stop ( loadingThread );
				}

			//
			// Convert bitmaps loaded in another thread to hardware ones in display thread
			if ( true == loadedAllBitmaps )
				{
					io_convertImages();
					loadedAllBitmaps = false;
					io_loadNewTileSet();
				}

			if ( ( true == loadingThreadHasFinished ) && ( false == processFadeValue ) )
				{
//						printf ( "Waiting for loading thread to finish - [ %i ]\n", loadingThreadHasFinished );
					sys_changeMode ( MODE_SHOW_SPLASH, true );
					io_loadNewTileSet();
				}

			break;

		case MODE_INTRO:
			gam_displayIntroText();
			gam_drawHud();
			gui_drawRasterLines();
			al_draw_filled_rectangle ( 0.0f, winHeight - introBlockOut, winWidth, winHeight, al_map_rgba_f ( 0.0f, 0.0f, 0.0f, 1.0f ) );
			break;

		case MODE_GUI:
			gam_drawHud();
			gui_drawGUI();
			break;

		case MODE_CONSOLE:
			con_drawScreen ( false, interpolation );
			break;

		case MODE_KEYCODE:
			gam_drawHud();
			gui_drawGUI();
			break;

		case MODE_SHOW_SPLASH:
			gam_drawImage ( splashImg, 0.0f, 0.0f, flagFullScreen );
			break;

		case MODE_PAUSED:
			gam_drawAllTiles();
			par_drawParticles();
			bul_drawBullets();
			gam_drawAllDroids ( currentLevel );
			gam_drawSpotLights();
			gam_drawHud();
			break;

		case MODE_BEAM_ON:
			gam_drawAllTiles();
			gam_drawSpotLights();
			gam_drawHud();
			gam_drawBeamOn();
			break;
			
		case MODE_SHOWLEVEL:
		case MODE_PLAYER_EXPLODE:
			gam_drawAllTiles();
			par_drawParticles();
			bul_drawBullets();
			gam_drawAllDroids ( currentLevel );
			gam_drawSpotLights();
			gam_drawHud();
			
			// Draw Paused window
			if ( true == debugShowStats )
			{
				sys_displayDebug();
				gam_AStarDebugWayPoints(debugAStarIndex);
			}
				
			//sys_displayScreenMiddle();

			//gam_debugShowDestination();
			//gam_debugShowWaypoints();

			//sys_debugLineSegments();
			//sysDoorDebugTrigger();
			//lvl_debugLOS();
			//
			break;

		case MODE_LIFT_VIEW:
			statusText = gui_getString ( "lift" );
			gam_drawStarfield();
			gam_drawHud();
			gam_drawSideView();
			break;

		case MODE_TERMINAL_SIDEVIEW:
			gam_setHUDState ( HUD_STATE_SHIP_VIEW );
			gam_drawStarfield();
			gam_drawHud();
			gam_drawSideView();
			gui_drawRasterLines();
			break;

		case MODE_TERMINAL_DB:
			gam_setHUDState ( HUD_STATE_DATABASE );
			gam_drawDB_Droid ( -1, -1 );
			gui_drawScrollBox ( &databaseScrollBox );
			gui_drawRasterLines();
			gam_drawHud();
			al_draw_filled_rectangle ( 0, winHeight - 48.0f, winWidth, winHeight, al_map_rgba_f ( 0.0, 0.0, 0.0, 1 ) );
			break;

		case MODE_TERMINAL_LEVEL:
			gam_setHUDState ( HUD_STATE_DECK_VIEW );
			gam_drawStarfield();
			gam_drawHud();
			term_showCurrentLevel();
			gui_drawRasterLines();
			break;

		case MODE_TRANSFER_INTRO:
		case MODE_TRANSFER_INTRO_1:
		case MODE_TRANSFER_INTRO_2:
		case MODE_TRANSFER_FINISH:
		case MODE_TRANSFER_START:
		case MODE_TRANSFER_COPY:
		case MODE_TRANSFER_DEADLOCK:
		case MODE_TRANSFER_SELECT:
		case MODE_TRANSFER_SELECT_SIDE:
		case MODE_TRANSFER_LOST:
			gam_drawHud();
			trn_drawTransferScreen();

//			sys_displayDebug();
			break;

		case MODE_END_STATIC_PRE:
		case MODE_END_STATIC:
		case MODE_END_TEXT:
		case MODE_END_TEXT_FINISH:
			gam_drawEndScreen();
			break;
		}

	//
	// Is there an active hardware FBO in use
	if ( true == useBackingBitmap )
		{
			al_set_target_backbuffer ( display );
			al_draw_bitmap ( bitmapFBO, 0.0f, 0.0f, 0 );

			//
			// Fade screen
			if ( true == processFadeValue )
				al_draw_filled_rectangle ( 0.0f, 0.0f, winWidth, winHeight, al_map_rgba_f ( 0.0f, 0.0f, 0.0f, fadeValue ) );
		}

	// else ?? otherwise just flip
	al_flip_display();

	profileRenderFrameEnd = al_get_time();
}

//-----------------------------------------------------------------------------
//
// Routine to handle all non graphics routines
void sys_updateFrame()
//-----------------------------------------------------------------------------
{
	if ( false == processFadeValue )	// No keyboard while fade is happening
		{
			io_processKeyboard ( thinkInterval );
			io_processJoystick();
		}

	switch ( currentMode )
		{
		case MODE_LOADING:
			break;

		case MODE_CONSOLE:
			// Process the cursor
			con_processCursor ( thinkInterval );
			con_processBackspaceKey ( thinkInterval );
			break;

		case MODE_SHOW_SPLASH:
			splashCounter -= 1.0f * thinkInterval;

			if ( splashCounter < 0.0f )
				{
					splashCounter = 1.0f;
					sys_changeMode ( MODE_INTRO, true );
					sys_playSound ( SND_SCROLLBEEPS, SND_PAN_CENTER, ALLEGRO_PLAYMODE_LOOP );
				}

			break;

		case MODE_INTRO:
			gui_animateRasterLine();
			gui_scrollScrollBox ( &introScrollBox );
			break;

		case MODE_PLAYER_EXPLODE:
			gam_animatePlayerSprite ( thinkInterval );
			break;

		case MODE_BEAM_ON:
			if (false == playSounds)
			{
				sys_changeMode ( MODE_SHOWLEVEL, false );
				gam_resetBeamOn();
				return;
			}
			
			if (false == sys_isSoundPlaying ( SND_START_ALL ))
			{
				sys_changeMode ( MODE_SHOWLEVEL, false );
				gam_resetBeamOn();
				return;
			}
			
			gam_setHUDState (HUD_STATE_BEAM_ON);
			gam_processBeamOn (thinkInterval);
			gam_animateSpotLights();
			gam_animateHealTiles ( currentLevel );
			break;
			
		case MODE_SHOWLEVEL:
			if ( false == gamePaused )
				{
					gam_processPlayerMovement ( thinkInterval );
					//
					// Work around physics and worldPos timings
					// Results in droid with invalid worldPos values
					//
					if ( true == processedPhysics )
						{
							if ( false == processFadeValue )
								drd_processDroidAI ( currentLevel, thinkInterval );
						}

					gam_updateScrollingVars ( false );
					gam_doorCheckTriggerAreas();
					gam_doorProcessActions();
					bul_processBullets();
					gam_weaponRechargePlayer();
					gam_checkTransferSound();
					gam_checkAlertLevel();
					gam_checkPlayerHealingTile ( thinkInterval );
					par_processEmitterQueue();
					par_animateParticles();
					gam_playerTransferTime();
				}

			gam_animateSpotLights();
			gam_animateHealTiles ( currentLevel );
			lvl_LOS();
			gam_animatePlayerSprite ( thinkInterval );
			gam_processScore ( thinkInterval );
			drd_clearHadCollidedFlag();
			gam_AStarProcessPaths();
			cpSpaceStep ( space, SKIP_TICKS );
			processedPhysics = true;
			break;

		case MODE_LIFT_VIEW:
			gam_animateStarField ( thinkInterval );
			sys_pauseSoundSystem ( false );
			break;

		case MODE_TERMINAL_SIDEVIEW:
		case MODE_TERMINAL_LEVEL:
			gam_animateStarField ( thinkInterval );
			gui_animateRasterLine();
			gam_animateIndicator ( thinkInterval );
			break;

		case MODE_TERMINAL_DB:
			gam_animateDB_Droid();
			gui_scrollScrollBox ( &databaseScrollBox );
			gui_animateRasterLine();
			break;

		case MODE_GUI:
			gui_animateRasterLine();
			sys_pauseSoundSystem ( false );
			break;

		case MODE_TRANSFER_INTRO:
		case MODE_TRANSFER_INTRO_1:
		case MODE_TRANSFER_INTRO_2:
		case MODE_TRANSFER_FINISH:
		case MODE_TRANSFER_START:
		case MODE_TRANSFER_COPY:
		case MODE_TRANSFER_DEADLOCK:
		case MODE_TRANSFER_SELECT:
		case MODE_TRANSFER_SELECT_SIDE:
		case MODE_TRANSFER_LOST:
			trn_processTransferScreen ( thinkInterval );
			break;

		case MODE_END_STATIC_PRE:
		case MODE_END_STATIC:
		case MODE_END_TEXT:
		case MODE_END_TEXT_FINISH:
			gam_processEndScreen();
			break;
		}

	//
	// See if the fade value needs to be processed
	gam_processFadeValue();
}
