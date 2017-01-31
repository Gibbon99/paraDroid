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

ALLEGRO_KEYBOARD_STATE	keyboard;

//-----------------------------------------------------------------------------
//
// Read a key for keycode capture
void io_readKeyCodeInput ( ALLEGRO_EVENT event )
//-----------------------------------------------------------------------------
{
	if ( event.type == ALLEGRO_EVENT_KEY_DOWN )
		{
			if ( event.keyboard.keycode == ALLEGRO_KEY_ESCAPE )	// Back without setting a key
				{
					currentGUIScreen = gui_findIndex ( GUI_OBJECT_SCREEN, "scrGameControls" );
					currentMode = MODE_GUI;
					return;
				}
			guiKeyCodes[gui_findIndex ( GUI_OBJECT_KEYCODE, "keyCodeControl" )].element[guiKeyCodes[gui_findIndex ( GUI_OBJECT_KEYCODE, "keyCodeControl" )].elementFocus].keyCode = event.keyboard.keycode;

			printf ( "[ %s ]\n", al_keycode_to_name ( event.keyboard.keycode ) );
			currentGUIScreen = gui_findIndex ( GUI_OBJECT_SCREEN, "scrGameControls" );
			currentMode = MODE_GUI;
		}
}

//-----------------------------------------------------------------------------
//
// Handle the player action key
void io_processActionKey()
//-----------------------------------------------------------------------------
{
	if ( true == inputAction[gameFire].currentlyDown )
		{
			//
			// Use a lift
			if ( ( playerOverTile == LIFT_TILE ) && ( playerVelocity.x == 0.0f ) && ( playerVelocity.y == 0.0f ) )
				{
					sys_destroyPhysicObjects(currentLevel);
					gam_getTunnelToUse();
					sys_changeMode ( MODE_LIFT_VIEW, true );
					inputAction[gameFire].currentlyDown = false;
					sys_stopAllSounds();
					sys_pauseSoundSystem ( true );
					gam_setHUDState ( HUD_STATE_LIFT );
					return;
				}
			//
			// Use a terminal
			if ( ( playerOverTile == TERMINAL_TOP ) || ( playerOverTile == TERMINAL_BOTTOM ) || ( playerOverTile == TERMINAL_LEFT ) || ( playerOverTile == TERMINAL_RIGHT ) )
				if ( ( playerVelocity.x == 0.0f ) && ( playerVelocity.y == 0.0f ) )
					{
						currentGUIScreen = gui_findIndex ( GUI_OBJECT_SCREEN, "scrTerminal" );
						sys_changeMode ( MODE_GUI, true );
						sys_stopAllSounds();
						sys_pauseSoundSystem ( true );
						return;
					}
			//
			// Go into transfer mode
			if ( ( playerVelocity.x == 0.0 ) && ( playerVelocity.y == 0.0 ) && ( inputAction[gameLeft].currentlyDown == false ) &&
			        ( false == inputAction[gameRight].currentlyDown ) && ( false == inputAction[gameDown].currentlyDown ) &&
			        ( false == inputAction[gameUp].currentlyDown ) )
				{
					inTransferMode = true;
					return;
				}
			//
			// Shoot a bullet - no shooting in transfer mode
			if ( false == inTransferMode )
				{
					if ( ( playerVelocity.x != 0.0f ) || ( playerVelocity.y != 0.0f ) )
						if ( ( inputAction[gameLeft].currentlyDown == true ) ||
						        ( inputAction[gameRight].currentlyDown == true ) ||
						        ( inputAction[gameUp].currentlyDown == true ) ||
						        ( inputAction[gameDown].currentlyDown == true ) )
							{
								if ( true == playerWeaponReadyToFire )
									{
										bul_newBullet ( playerWorldMiddlePos, cpvadd ( playerWorldPos, cpvmult ( playerVelocity, 1000 ) ),
										                dataBaseEntry[playerDroidTypeImage - 2].bulletType, -1 );
										playerWeaponReadyToFire = false;
										gam_processWitnessShooting();
									}
							}
				}
		}
	else
		{
			inTransferMode = false;
		}
}

//-----------------------------------------------------------------------------
//
// Keyboard while showing intro mode
void io_readIntroInput ( ALLEGRO_EVENT event )
//-----------------------------------------------------------------------------
{
	switch ( event.type )
		{
			case ALLEGRO_EVENT_KEY_DOWN:
				if ( ( event.keyboard.keycode == ALLEGRO_KEY_SPACE ) ||
				        ( event.keyboard.keycode == inputAction[gameFire].keyValue ) ||
				        ( event.keyboard.keycode == inputAction[menuAction].keyValue ) )
					{
						sys_changeMode ( MODE_GUI, true );
						if ( sys_isSoundPlaying ( SND_SCROLLBEEPS ) )
							sys_stopSound ( SND_SCROLLBEEPS );
					}
				break;
		}
}

//-----------------------------------------------------------------------------
//
// Keyboard while showing transfer mode
void io_readTransferKeyboard ( ALLEGRO_EVENT event )
//-----------------------------------------------------------------------------
{
	//
	// TODO: Remove for Prod
	//
	if ( ( event.type == ALLEGRO_EVENT_KEY_DOWN ) && ( event.keyboard.keycode == inputAction[gameEscape].keyValue ) )
		{
			sys_changeMode ( MODE_SHOWLEVEL, true );
			return;
		}

	switch ( currentMode )
		{
			case MODE_TRANSFER_INTRO:
				if ( ( event.type == ALLEGRO_EVENT_KEY_DOWN ) && ( event.keyboard.keycode == inputAction[gameFire].keyValue ) )
					{
						sys_changeMode ( MODE_TRANSFER_INTRO_1, true );
						inputAction[gameFire].currentlyDown = false;
						al_flush_event_queue ( eventQueue );
						return;
					}
				break;

			case MODE_TRANSFER_INTRO_1:
				if ( ( event.type == ALLEGRO_EVENT_KEY_DOWN ) && ( event.keyboard.keycode == inputAction[gameFire].keyValue ) )
					{
						sys_changeMode ( MODE_TRANSFER_INTRO_2, true );
						inputAction[gameFire].currentlyDown = false;
						al_flush_event_queue ( eventQueue );
						return;
					}
				break;

			case MODE_TRANSFER_INTRO_2:
				if ( ( event.type == ALLEGRO_EVENT_KEY_DOWN ) && ( event.keyboard.keycode == inputAction[gameFire].keyValue ) )
					{
						sys_changeMode ( MODE_TRANSFER_SELECT_SIDE, true );
						inputAction[gameFire].currentlyDown = false;
						al_flush_event_queue ( eventQueue );
						return;
					}
				break;


			case MODE_TRANSFER_SELECT_SIDE:
				if ( ( event.type == ALLEGRO_EVENT_KEY_DOWN ) && ( event.keyboard.keycode == inputAction[gameFire].keyValue ) )
					{
						sys_changeMode ( MODE_TRANSFER_SELECT, false );
						inputAction[gameFire].currentlyDown = false;
						al_flush_event_queue ( eventQueue );
						selectSideCounter = TRANSFER_COUNTER;
						delayTransferIntro = TRANSFER_DELAY;
						trn_nextPlayerToken();
						sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
					}

				if ( ( event.type == ALLEGRO_EVENT_KEY_DOWN ) && ( event.keyboard.keycode == inputAction[gameRight].keyValue ) )
					{
						playerOnSide = RIGHT_SIDE;
						droidOnSide = LEFT_SIDE;
						sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
					}

				if ( ( event.type == ALLEGRO_EVENT_KEY_DOWN ) && ( event.keyboard.keycode == inputAction[gameLeft].keyValue ) )
					{
						playerOnSide = LEFT_SIDE;
						droidOnSide = RIGHT_SIDE;
						sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
					}
				break;

			case MODE_TRANSFER_SELECT:

				if ( numPlayerTokens < 0 )
					return;

				if ( ( event.type == ALLEGRO_EVENT_KEY_DOWN ) && ( event.keyboard.keycode == inputAction[gameFire].keyValue ) )
					{
						trn_placeToken ( playerBlockPos, playerOnSide, PLAYER_TRANSFER );
					}

				if ( ( event.type == ALLEGRO_EVENT_KEY_DOWN ) && ( event.keyboard.keycode == inputAction[gameDown].keyValue ) )
// TODO (roam#1#): Make this move when the key is down, not on every press
					{
						inputAction[gameDown].currentlyDown = true;
						trn_moveToken ( MOVE_DOWN );
						return;
					}

				if ( ( event.type == ALLEGRO_EVENT_KEY_DOWN ) && ( event.keyboard.keycode == inputAction[gameUp].keyValue ) )
					{
						inputAction[gameUp].currentlyDown = true;
						trn_moveToken ( MOVE_UP );
						return;
					}
				break;
		}
}

//-----------------------------------------------------------------------------
//
// Main routine to read input for GUI
void io_readGUIInput ( ALLEGRO_EVENT event )
//-----------------------------------------------------------------------------
{
	switch ( event.type )
		{
			case ALLEGRO_EVENT_KEY_DOWN:
				if ( event.keyboard.keycode == inputAction[menuAction].keyValue )
					{
						gui_handleFocusMove ( -1, true );
						sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
					}

				if ( event.keyboard.keycode == inputAction[menuUp].keyValue )
					{
						gui_handleFocusMove ( FOCUS_PREVIOUS, false );
						sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
					}

				if ( event.keyboard.keycode == inputAction[menuDown].keyValue )
					{
						gui_handleFocusMove ( FOCUS_NEXT, false );
						sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
					}

				if ( event.keyboard.keycode == inputAction[gameLeft].keyValue )
					{
						gui_handleFocusMove ( FOCUS_LEFT, false );
						sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
					}

				if ( event.keyboard.keycode == inputAction[gameRight].keyValue )
					{
						gui_handleFocusMove ( FOCUS_RIGHT, false );
						sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
					}

		}
}

//-----------------------------------------------------------------------------
//
// Handle keys for moving around terminal menus
void io_readTerminalKeyboard ( ALLEGRO_EVENT event )
//-----------------------------------------------------------------------------
{
	switch ( currentMode )
		{
			case MODE_TERMINAL_DB:
				switch ( event.type )
					{
						case ALLEGRO_EVENT_KEY_DOWN:
							if ( event.keyboard.keycode == inputAction[gameFire].keyValue )
								{
									sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
									currentGUIScreen = gui_findIndex ( GUI_OBJECT_SCREEN, "scrTerminal" );
									sys_changeMode ( MODE_GUI, true );
									sys_pauseSoundSystem ( false );
								}

							if ( event.keyboard.keycode == inputAction[gameRight].keyValue )
								{
									gam_getNextDroid();
								}

							if ( event.keyboard.keycode == inputAction[gameLeft].keyValue )
								{
									gam_getPreviousDroid();
								}
							break;
					}
				break;


			case MODE_TERMINAL_LEVEL:
				if ( ( event.keyboard.keycode == inputAction[gameFire].keyValue ) && ( event.type == ALLEGRO_EVENT_KEY_DOWN ) )
					{
						sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
						currentGUIScreen = gui_findIndex ( GUI_OBJECT_SCREEN, "scrTerminal" );
						sys_changeMode ( MODE_GUI, true );

					}
				break;

			case MODE_TERMINAL_SIDEVIEW:
				if ( ( event.keyboard.keycode == inputAction[gameFire].keyValue ) && ( event.type == ALLEGRO_EVENT_KEY_DOWN ) )
					{
						currentGUIScreen = gui_findIndex ( GUI_OBJECT_SCREEN, "scrTerminal" );
						sys_changeMode ( MODE_GUI, true );
						sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
					}
				break;
		}
}

//-----------------------------------------------------------------------------
//
// Handle keys for moving around lifts
void io_readLiftKeyboard ( ALLEGRO_EVENT event )
//-----------------------------------------------------------------------------
{
	switch ( event.type )
		{
			case ALLEGRO_EVENT_KEY_DOWN:
				if ( event.keyboard.keycode == inputAction[gameFire].keyValue )
					{
						//
						// Choose tunnel to use
						gam_changeToLevel ( currentLevel, 0 );
						gam_putPlayerOnLiftFromTunnel();
						inputAction[gameFire].currentlyDown = false;
						sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
						// Change back to normal view
						sys_changeMode ( MODE_SHOWLEVEL, true );
					}

				if ( event.keyboard.keycode == inputAction[gameUp].keyValue )
					{
						if ( currentLevel != tunnel[currentTunnel].top )
							{
								tunnel[currentTunnel].current_deck++;
								tunnel[currentTunnel].current = tunnel[currentTunnel].decks[tunnel[currentTunnel].current_deck];
								currentLevel = tunnel[currentTunnel].current;
								sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
							}
						else
							{
								sys_playSound ( SND_KEYPRESS_BAD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
							}

						con_print ( true, false, "UP: CurrentLevel [ %i ] tunnel.current [ %i ]", currentLevel, tunnel[currentTunnel].current );
					}

				if ( event.keyboard.keycode == inputAction[gameDown].keyValue )
					{
						if ( currentLevel != tunnel[currentTunnel].bottom )
							{
								tunnel[currentTunnel].current_deck--;
								tunnel[currentTunnel].current = tunnel[currentTunnel].decks[tunnel[currentTunnel].current_deck];
								currentLevel = tunnel[currentTunnel].current;
								sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
							}
						else
							{
								sys_playSound ( SND_KEYPRESS_BAD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
							}

						con_print ( true, false, "DOWN: CurrentLevel [ %i ] currentTunnel [ %i ] currentDesk [ %i ]",
						            currentLevel, currentTunnel, tunnel[currentTunnel].current_deck );
					}
		}
}

//-----------------------------------------------------------------------------
//
// Read and process the game keyboard
void io_getKeyboardState ( ALLEGRO_EVENT event )
//-----------------------------------------------------------------------------
{
	switch ( event.type )
		{
			case ALLEGRO_EVENT_KEY_DOWN:
				if ( event.keyboard.keycode == inputAction[gameLeft].keyValue )
					inputAction[gameLeft].currentlyDown = true;

				if ( event.keyboard.keycode == inputAction[gameRight].keyValue )
					inputAction[gameRight].currentlyDown = true;

				if ( event.keyboard.keycode == inputAction[gameUp].keyValue )
					inputAction[gameUp].currentlyDown = true;

				if ( event.keyboard.keycode == inputAction[gameDown].keyValue )
					inputAction[gameDown].currentlyDown = true;

				if ( event.keyboard.keycode == inputAction[gameFire].keyValue )
					inputAction[gameFire].currentlyDown = true;

				if ( event.keyboard.keycode == inputAction[consoleAction].keyValue )
					inputAction[consoleAction].currentlyDown = true;

				if ( event.keyboard.keycode == inputAction[actionScreenShot].keyValue )
					inputAction[actionScreenShot].currentlyDown = true;

				if ( event.keyboard.keycode == inputAction[gameEscape].keyValue )
					inputAction[gameEscape].currentlyDown = true;

				if ( event.keyboard.keycode == inputAction[gamePause].keyValue )
					inputAction[gamePause].currentlyDown = true;

				break;

			case ALLEGRO_EVENT_KEY_UP:
				if ( event.keyboard.keycode == inputAction[gameLeft].keyValue )
					inputAction[gameLeft].currentlyDown = false;

				if ( event.keyboard.keycode == inputAction[gameRight].keyValue )
					inputAction[gameRight].currentlyDown = false;

				if ( event.keyboard.keycode == inputAction[gameUp].keyValue )
					inputAction[gameUp].currentlyDown = false;

				if ( event.keyboard.keycode == inputAction[gameDown].keyValue )
					inputAction[gameDown].currentlyDown = false;

				if ( event.keyboard.keycode == inputAction[gameFire].keyValue )
					inputAction[gameFire].currentlyDown = false;

				if ( event.keyboard.keycode == inputAction[consoleAction].keyValue )
					inputAction[consoleAction].currentlyDown = false;

				if ( event.keyboard.keycode == inputAction[actionScreenShot].keyValue )
					inputAction[actionScreenShot].currentlyDown = false;

				if ( event.keyboard.keycode == inputAction[gameEscape].keyValue )
					inputAction[gameEscape].currentlyDown = false;

				if ( event.keyboard.keycode == inputAction[gamePause].keyValue )
					inputAction[gamePause].currentlyDown = false;

				break;
		}
}

//-----------------------------------------------------------------------------
//
// Get the character from a keycode
char *io_getAsciiFromKeycode ( int keyCode )
//-----------------------------------------------------------------------------
{
	char tempChar[1];

	sprintf ( tempChar, "%s", al_keycode_to_name ( keyCode ) );
	return tempChar[0];
}

//-----------------------------------------------------------------------------
//
// Add a character to the current console line
void io_addCharToLine ( char keyCode )
//-----------------------------------------------------------------------------
{
	switch ( keyCode )
		{
			case ALLEGRO_KEY_SPACE:
				if ( conCurrentCharCount < MAX_STRING_SIZE - 1 )
					conCurrentLine.conLine += " ";

				conCurrentCharCount++;
				break;

			default:
				if ( conCurrentCharCount < MAX_STRING_SIZE - 1 )
					conCurrentLine.conLine += keyCode;

				conCurrentCharCount++;
				break;
		}
}

//-----------------------------------------------------------------------------
//
// Read and process the console keyboard
void io_readConsoleKeyboard ( ALLEGRO_EVENT event )
//-----------------------------------------------------------------------------
{
	int             unichar;

	if ( ALLEGRO_EVENT_KEY_CHAR == event.type )
		{
			//
			// Get non ASCII keyvalues
			unichar = event.keyboard.keycode;

			switch ( unichar )
				{
					case ALLEGRO_KEY_BACKSPACE:
						conBackSpaceDown = 1;
						break;

					case ALLEGRO_KEY_TAB:
						con_completeCommand ( conCurrentLine.conLine );
						break;

					case ALLEGRO_KEY_ENTER:
						con_processCommand ( conCurrentLine.conLine );
						break;

					case ALLEGRO_KEY_TILDE:
						inputAction[consoleAction].currentlyDown = false;
						sys_changeMode ( -1, true );
						break;

					case ALLEGRO_KEY_DOWN:
						conHistoryPtr++;
						if ( conHistoryPtr > conNumInHistory )
							conHistoryPtr = conNumInHistory;
						con_popHistoryCommand();
						break;

					case ALLEGRO_KEY_UP:
						conHistoryPtr--;
						if ( conHistoryPtr < 0 )
							conHistoryPtr = 0;
						con_popHistoryCommand();
						break;

					default:
						unichar = event.keyboard.unichar;
						if ( ( unichar >= 32 ) && ( unichar < ALLEGRO_KEY_MAX ) )
							io_addCharToLine ( unichar );
						conBackSpaceDelay = 0.0f;
						break;
				}
		}
	//
	// Can't be delayed by running inside the delay loop
	if ( false == al_key_down ( &keyboard, ALLEGRO_KEY_BACKSPACE ) )
		{
			conBackSpaceDown = 0;
		}
}

//-----------------------------------------------------------------------------
//
// Read the keyboard
void io_readKeyboard()
//-----------------------------------------------------------------------------
{
	ALLEGRO_EVENT   event;

	if ( false == al_get_next_event ( eventQueue, &event ) )
		return; // No events in the queue
	//
	// Windows was switched away
	switch ( event.type )
		{
			case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
				gamePaused = true;
				break;

			case ALLEGRO_EVENT_DISPLAY_CLOSE:
// TODO (dberry#1#): Change to use MODE_FINISH
				sys_shutDown();
				break;

			case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING:
// TODO (dberry#1#): call al_acknowledge_drawing_halt immediately.
				break;

			case ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING:
// TODO (dberry#1#): call al_acknowledge_drawing_resume immediately.
				break;
		}
	//
	// Read the state of the keyboard
	al_get_keyboard_state ( &keyboard );
	//
	//  and assign true/false to state of our keys
	io_getKeyboardState ( event );

	switch ( currentMode )
		{
			case MODE_PAUSED:
				if ( true == inputAction[gamePause].currentlyDown )
					{
						inputAction[gamePause].currentlyDown = false;
						sys_changeMode ( MODE_SHOWLEVEL, false );
						gamePaused = false;
					}
				break;

			case MODE_CONSOLE:
				io_readConsoleKeyboard ( event );
				break;

			case MODE_SHOWLEVEL:
				//
				// Handle action key
				io_processActionKey();
				//
				// Activate the console from any mode
				if ( true == inputAction[consoleAction].currentlyDown )
					{
						inputAction[consoleAction].currentlyDown = false;
						al_flush_event_queue ( eventQueue );
						sys_changeMode ( MODE_CONSOLE, true );
						sys_stopAllSounds();
					}

				if ( true == inputAction[gamePause].currentlyDown )
					{
						sys_changeMode ( MODE_PAUSED, false );
						inputAction[gamePause].currentlyDown = false;
						gamePaused = true;
						sys_stopAllSounds();
						gam_setHUDState ( HUD_STATE_PAUSED );
					}
				break;

			case MODE_LIFT_VIEW:
				io_readLiftKeyboard ( event );
				break;

			case MODE_TERMINAL_MENU:
			case MODE_TERMINAL_SIDEVIEW:
			case MODE_TERMINAL_DB:
			case MODE_TERMINAL_LEVEL:
				io_readTerminalKeyboard ( event );
				break;

			case MODE_GUI:
				io_readGUIInput ( event );
				break;

			case MODE_KEYCODE:
				io_readKeyCodeInput ( event );
				break;

			case MODE_INTRO:
				io_readIntroInput ( event );
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
				io_readTransferKeyboard ( event );
				break;
		}
	//
	// Exit from game - move to menus when ready
	if ( true == inputAction[gameEscape].currentlyDown )
		{
			gameIsRunning = false;
			return;
		}
	//
	// Take a screenshot
	if ( true == inputAction[actionScreenShot].currentlyDown )
		{
			inputAction[actionScreenShot].currentlyDown = false;
			if ( io_screenShot ( "", "paradroid" ) == false )
				{
					con_print ( true, true, "Error: Could not save screenshot. PHYSFS_close is failing, although the file is written." );
				}
		}
}
