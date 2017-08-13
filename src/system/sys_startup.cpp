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
#include "../../hdr/system/sys_sound.h"

ALLEGRO_THREAD		*loadingThread;
bool				stopLoadingThread;
bool				loadedAllBitmaps;
bool				loadingThreadHasFinished;

//-----------------------------------------------------------------------------
//
// Run all the loading etc in this function as a thread
//void *sys_runLoadThread ( ALLEGRO_THREAD *thread, void *arg )
void *sys_runLoadThread ( ALLEGRO_THREAD *thread, void *arg )
//-----------------------------------------------------------------------------
{
	al_set_physfs_file_interface();	// Allow reading files from this thread

	loadingThreadHasFinished = false;

	if ( false == io_loadAllImages() )
		{
			con_setColor ( 1.0f, 0.0f, 0.0f, 1.0f );
			con_print ( true, false, "Failed to load image file" );
		}

	loadedAllBitmaps = true;	// Set this flag to call convertBitmaps in main thread

	gam_getDBInformation();

	gam_loadAllLevels();

	sys_loadSoundSamples();

	if ( false == io_loadSideViewData ( "sideview.dat" ) )
		sys_errorFatal ( __FILE__, __LINE__, "Error: Could not load sideview data" );

	gam_calcTileTexCoords();
	//
	// Start on level
	// Done in GUI script for starting level

	gam_setupPlayerValues ( 0 );	// 0 == 001 in database

	gam_setupStarField();

	stopLoadingThread = true;

//	while ( !al_get_thread_should_stop ( thread ) )
		{
//		printf("Still in thread.\n");
//		printf("THREAD - Waiting for loading thread to finish - [ %i ]\n", loadingThreadHasFinished);
		}

	loadingThreadHasFinished = true;

	return NULL;
}

//-----------------------------------------------------------------------------
//
// Start up one time routines
bool sys_startOnce()
//-----------------------------------------------------------------------------
{
	io_startLogFile ( "logfile.log" );

	con_initConsole();

	ai_setupAITree();

	sys_initRandomNum();

	if ( !al_init() )
		{
			al_show_native_message_box ( display, "Error", "Error", "Failed to initialize allegro!", NULL, ALLEGRO_MESSAGEBOX_ERROR );
			return false;
		}

	if ( false == io_startFileSystem ( false ) )
		sys_errorFatal ( __FILE__, __LINE__, "Could not start packfile" );

	al_set_physfs_file_interface();

	if ( false == sys_initScriptEngine() )
		sys_errorFatal ( "sys_initScrptEngine", __LINE__, "Could not start script engine" );

//
// Run scripts to start things up
//
	if ( false == sys_executeScriptFunction ( "as_initGameVars", "" ) )
		{
			io_logToFile ( "Error: Couldn't execute as_initGameVars" );
			return false;
		}

	if ( false == sys_executeScriptFunction ( "as_setLanguageStrings", "" ) )
		{
			io_logToFile ( "Error: Couldn't execute as_setLanguageStrings" );
			return false;
		}

	if ( false == sys_executeScriptFunction ( "as_initParticleValues", "" ) )
		{
			io_logToFile ( "Error: Couldn't execute as_initParticleValues" );
			return false;
		}

	if ( false == sys_executeScriptFunction ( "as_addAllScriptCommands", "" ) )
		{
			io_logToFile ( "Error: Couldn't execute as_addAllScriptCommands" );
			return false;
		}

	as_testObject();

	uint32_t version = al_get_allegro_version();
	con_setColor ( 1.0f, 1.0f, 0.0f, 1.0f );
	con_print ( true, false, "Allegro Version: %i.%i.%i Release %i", version >> 24, ( version >> 16 ) & 255, ( version >> 8 ) & 255, version & 255 );

	if ( !al_install_keyboard() )
		{
			al_show_native_message_box ( display, "Error", "Error", "Failed to initialize al_install_keyboard!", NULL, ALLEGRO_MESSAGEBOX_ERROR );
			return false;
		}

	io_logToFile ( "INFO: Keyboard installed." );
//
// Display flags and settings are done in the startup script
//
	display = al_create_display ( winWidth, winHeight );

	if ( !display )
		{
			al_show_native_message_box ( display, "Error", "Error", "Failed to initialize display!", NULL, ALLEGRO_MESSAGEBOX_ERROR );
			return false;
		}

	al_init_font_addon();
	builtInFont = al_create_builtin_font();
	if ( NULL == builtInFont )
		{
			al_show_native_message_box ( display, "Error", "Error", "Failed to initialize built in font", NULL, ALLEGRO_MESSAGEBOX_ERROR );
			return false;
		}
	version = al_get_allegro_font_version();
	con_setColor ( 1.0f, 1.0f, 0.0f, 1.0f );
	con_print ( true, false, "INFO: Allegro Font Version: %i.%i.%i Release %i", version >> 24, ( version >> 16 ) & 255, ( version >> 8 ) & 255, version & 255 );



	if ( !al_init_image_addon() )
		{
			al_show_native_message_box ( display, "Error", "Error", "Failed to initialize al_init_image_addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR );
			return false;
		}
	io_logToFile ( "INFO: Image addon installed." );

	if ( !al_init_primitives_addon() )
		{
			al_show_native_message_box ( display, "Error", "Error", "Failed to initialize al_init_primitives_addon", NULL, ALLEGRO_MESSAGEBOX_ERROR );
			return false;
		}
	io_logToFile ( "INFO: Primitives addon installed." );

	if ( !al_install_joystick() )
		{
			al_show_native_message_box ( display, "Error", "Error", "Failed to initialize al_install_joystick", NULL, ALLEGRO_MESSAGEBOX_ERROR );
			return false;
		}
	io_logToFile ( "INFO: Joysticks installed." );
	io_setupJoystickValues ();

	eventQueue = al_create_event_queue();

	if ( !eventQueue )
		{
			al_show_native_message_box ( display, "Error", "Error", "Failed to start event queue", NULL, ALLEGRO_MESSAGEBOX_ERROR );
			return false;
		}

	al_register_event_source ( eventQueue, al_get_keyboard_event_source() );
	al_register_event_source ( eventQueue, al_get_joystick_event_source() );
	al_register_event_source ( eventQueue, al_get_display_event_source ( display ) );
	//
	// Needed for TTF fonts
	if ( !al_init_ttf_addon() )
		{
			al_show_native_message_box ( display, "Error", "Error", "Failed to start TTF Addon", NULL, ALLEGRO_MESSAGEBOX_ERROR );
			return false;
		}

	version = al_get_allegro_ttf_version();
	con_setColor ( 1.0f, 1.0f, 0.0f, 1.0f );
	con_print ( true, false, "INFO: Allegro TTF Version: %i.%i.%i Release %i", version >> 24, ( version >> 16 ) & 255, ( version >> 8 ) & 255, version & 255 );
	sys_setFontColor ( 1.0f, 1.0f, 1.0f, 1.0f );

	soundSystemStarted = sys_startSound();

	//
	// Setup backing bitmap
	if ( true == useBackingBitmap )
		{
			if ( false == gam_setupFBO ( ALLEGRO_VIDEO_BITMAP ) )
				{
					useBackingBitmap = false;
				}
			else
				{
					//
					// Check we have a hardware bitmap
					int bitmapFlags = al_get_bitmap_flags ( bitmapFBO );

					if ( ( bitmapFlags & ALLEGRO_VIDEO_BITMAP ) == ALLEGRO_VIDEO_BITMAP )
						{
							con_print ( true, true, "INFO: Got hardware accelerated backing bitmap" );
						}
					else
						{
							con_print ( true, true, "INFO: Didn't get a hardware accelerated backing bitmap. Use flip" );
							useBackingBitmap = false;
						}
				}
		}

	sys_initTTFSystem();

	sys_setFontColor ( 1.0f, 1.0f, 1.0f, 1.0f );

	sys_executeScriptFunction ( "as_loadFonts", "" );

	sys_setFont ( FONT_GUI );

	if ( false == gui_initGUI() )
		{
			al_show_native_message_box ( display, "Error", "Error", "Failed to start GUI", NULL, ALLEGRO_MESSAGEBOX_ERROR );
			return false;
		}

	sys_setupPhysicsEngine();

	sys_setupTables();

	sys_changeMode ( MODE_LOADING, true );

	stopLoadingThread = false;
	loadedAllBitmaps = false;
	loadingBarPercent = 0;	// Loading bar starts at 0 percent

//sys_runLoadThread();

	loadingThread = al_create_thread ( sys_runLoadThread, NULL );
	al_start_thread ( loadingThread );

	currentAlertLevel = ALERT_GREEN_TILE;

	gam_readScoreTable ( ( char * ) SCORE_TABLE_FILENAME );

	io_setRepeatKeysState(false);

	return true;
}
