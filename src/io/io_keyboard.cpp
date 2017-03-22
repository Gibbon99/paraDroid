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

bool					repeatKeys;
float					repeatKeysDelay;	// Set from script

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
					sys_destroyPhysicObjects ( currentLevel );
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
void io_readIntroInput ( )
//-----------------------------------------------------------------------------
{
	if ( true == io_getKeyStateDown ( gameFire ) )
		{
			sys_changeMode ( MODE_GUI, true );

			if ( sys_isSoundPlaying ( SND_SCROLLBEEPS ) )
				sys_stopSound ( SND_SCROLLBEEPS );

			io_setRepeatKeysState ( true );
		}
}

//-----------------------------------------------------------------------------
//
// Keyboard while showing transfer mode
void io_readTransferKeyboard ( )
//-----------------------------------------------------------------------------
{
	switch ( currentMode )
		{
		case MODE_TRANSFER_INTRO:
			if ( true == io_getKeyStateDown ( gameFire ) )
				{
					sys_changeMode ( MODE_TRANSFER_INTRO_1, true );
					inputAction[gameFire].currentlyDown = false;
					al_flush_event_queue ( eventQueue );
					return;
				}

			break;

		case MODE_TRANSFER_INTRO_1:
			if ( true == io_getKeyStateDown ( gameFire ) )
				{
					sys_changeMode ( MODE_TRANSFER_INTRO_2, true );
					inputAction[gameFire].currentlyDown = false;
					al_flush_event_queue ( eventQueue );
					return;
				}

			break;

		case MODE_TRANSFER_INTRO_2:
			if ( true == io_getKeyStateDown ( gameFire ) )
				{
					sys_changeMode ( MODE_TRANSFER_SELECT_SIDE, true );
					inputAction[gameFire].currentlyDown = false;
					al_flush_event_queue ( eventQueue );
					return;
				}

			break;


		case MODE_TRANSFER_SELECT_SIDE:
			if ( true == io_getKeyStateDown ( gameFire ) )
				{
					sys_changeMode ( MODE_TRANSFER_SELECT, false );
					inputAction[gameFire].currentlyDown = false;
					al_flush_event_queue ( eventQueue );
					selectSideCounter = TRANSFER_COUNTER;
					delayTransferIntro = TRANSFER_DELAY;
					trn_nextPlayerToken();
					sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
				}

			if ( true == io_getKeyStateDown ( gameRight ) )
				{
					playerOnSide = RIGHT_SIDE;
					droidOnSide = LEFT_SIDE;
					sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
				}

			if ( true == io_getKeyStateDown ( gameLeft ) )
				{
					playerOnSide = LEFT_SIDE;
					droidOnSide = RIGHT_SIDE;
					sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
				}

			break;

		case MODE_TRANSFER_SELECT:

			if ( numPlayerTokens < 0 )
				return;

			if ( true == io_getKeyStateDown ( gameFire ) )
				{
					trn_placeToken ( playerBlockPos, playerOnSide, PLAYER_TRANSFER );
				}

			if ( true == io_getKeyStateDown ( gameDown ) )
				{
					inputAction[gameDown].currentlyDown = true;
					trn_moveToken ( MOVE_DOWN );
					return;
				}

			if ( true == io_getKeyStateDown ( gameUp ) )
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
void io_readGUIInput ( )
//-----------------------------------------------------------------------------
{
	if ( true == io_getKeyStateDown ( gameFire ) )
		{
			gui_handleFocusMove ( -1, true );
			sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
		}

	if ( true == io_getKeyStateDown ( gameUp ) )
		{
			gui_handleFocusMove ( FOCUS_PREVIOUS, false );
			sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
		}

	if ( true == io_getKeyStateDown ( gameDown ) )
		{
			gui_handleFocusMove ( FOCUS_NEXT, false );
			sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
		}

	if ( true == io_getKeyStateDown ( gameLeft ) )
		{
			gui_handleFocusMove ( FOCUS_LEFT, false );
			sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
		}

	if ( true == io_getKeyStateDown ( gameRight ) )
		{
			gui_handleFocusMove ( FOCUS_RIGHT, false );
			sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
		}
}

//-----------------------------------------------------------------------------
//
// Handle keys for moving around terminal menus
void io_readTerminalKeyboard ( )
//-----------------------------------------------------------------------------
{
	switch ( currentMode )
		{
		case MODE_TERMINAL_DB:
		{
			if ( true == io_getKeyStateDown ( gameFire ) )
				{
					sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
					currentGUIScreen = gui_findIndex ( GUI_OBJECT_SCREEN, "scrTerminal" );
					sys_changeMode ( MODE_GUI, true );
					sys_pauseSoundSystem ( false );
				}

			if ( true == io_getKeyStateDown ( gameRight ) )
				{
					gam_getNextDroid();
				}

			if ( true == io_getKeyStateDown ( gameLeft ) )
				{
					gam_getPreviousDroid();
				}
		}
		break;


		case MODE_TERMINAL_LEVEL:
			if ( true == io_getKeyStateDown ( gameFire ) )
				{
					sys_playSound ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
					currentGUIScreen = gui_findIndex ( GUI_OBJECT_SCREEN, "scrTerminal" );
					sys_changeMode ( MODE_GUI, true );

				}

			break;

		case MODE_TERMINAL_SIDEVIEW:
			if ( true == io_getKeyStateDown ( gameFire ) )
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
void io_readLiftKeyboard ( )
//-----------------------------------------------------------------------------
{
	if ( true == io_getKeyStateDown ( gameFire ) )
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

	if ( true == io_getKeyStateDown ( gameUp ) )
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
		}

	if ( true == io_getKeyStateDown ( gameDown ) )
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
		}
}

//-----------------------------------------------------------------------------
//
// Set the repeat state for the keyboard
void io_setRepeatKeysState ( bool newState )
//-----------------------------------------------------------------------------
{
	repeatKeys = newState;
}

//-----------------------------------------------------------------------------
//
// Get the state of the Down key press dependant on repeatKeys setting
inline bool io_getKeyStateDown ( int keyName )
//-----------------------------------------------------------------------------
{
	if ( false == repeatKeys )
		{
			if ( true == inputAction[keyName].beenUp )
				{
					if ( true == inputAction[keyName].currentlyDown )
						{
							inputAction[keyName].beenUp = false;
							return true;
						}
					else
						{
							return false;
						}
				}
			else
				{
					return false;
				}
		}
	else	// repeatKeys is on
		{
			if ( true == inputAction[keyName].currentlyDown )
				{
					if ( inputAction[keyName].repeatDelayCount < 0.0f )
						{
							inputAction[keyName].repeatDelayCount = repeatKeysDelay;
							return true;
						}
					else
						return false;
				}
			else
				return false;
		}
}

//-----------------------------------------------------------------------------
//
// Set beenUp back to true and return false to set currentlyDown state
inline bool io_getKeyStateUp ( int keyName )
//-----------------------------------------------------------------------------
{
	inputAction[keyName].beenUp = true;
	inputAction[keyName].repeatDelayCount = -1.0f;
	return false;
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
			if ( inputMethod == INPUT_KEYBOARD )
				{
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
				}

			if ( event.keyboard.keycode == inputAction[actionScreenShot].keyValue )
				inputAction[actionScreenShot].currentlyDown = true;

			if ( event.keyboard.keycode == inputAction[gameEscape].keyValue )
				inputAction[gameEscape].currentlyDown = true;

			if ( event.keyboard.keycode == inputAction[gamePause].keyValue )
				inputAction[gamePause].currentlyDown = true;

			break;

		case ALLEGRO_EVENT_KEY_UP:
			if ( inputMethod == INPUT_KEYBOARD )
				{
					if ( event.keyboard.keycode == inputAction[gameLeft].keyValue )
						inputAction[gameLeft].currentlyDown = io_getKeyStateUp ( gameLeft );

					if ( event.keyboard.keycode == inputAction[gameRight].keyValue )
						inputAction[gameRight].currentlyDown = io_getKeyStateUp ( gameRight );

					if ( event.keyboard.keycode == inputAction[gameUp].keyValue )
						inputAction[gameUp].currentlyDown = io_getKeyStateUp ( gameUp );

					if ( event.keyboard.keycode == inputAction[gameDown].keyValue )
						inputAction[gameDown].currentlyDown = io_getKeyStateUp ( gameDown );

					if ( event.keyboard.keycode == inputAction[gameFire].keyValue )
						inputAction[gameFire].currentlyDown = io_getKeyStateUp ( gameFire );

					if ( event.keyboard.keycode == inputAction[consoleAction].keyValue )
						inputAction[consoleAction].currentlyDown = io_getKeyStateUp ( consoleAction );
				}

			if ( event.keyboard.keycode == inputAction[actionScreenShot].keyValue )
				inputAction[actionScreenShot].currentlyDown = io_getKeyStateUp ( actionScreenShot );

			if ( event.keyboard.keycode == inputAction[gameEscape].keyValue )
				inputAction[gameEscape].currentlyDown = io_getKeyStateUp ( gameEscape );

			if ( event.keyboard.keycode == inputAction[gamePause].keyValue )
				inputAction[gamePause].currentlyDown = io_getKeyStateUp ( gamePause );

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
	return (char *)tempChar[0];
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
}

//-----------------------------------------------------------------------------
//
// Process the keys already set from the event based system
// Also decrement the counter for delay between key actions being processed
// All these functions do not use the event based system
void io_processKeyboard ( double frameTime )
//-----------------------------------------------------------------------------
{
	//
	// Decrement counter for repeat delay
	//
	for ( int i = 0; i != NUMBER_ACTIONS; i++ )
		{
			inputAction[i].repeatDelayCount -= 1.0f * frameTime;
		}

	switch ( currentMode )
		{
		case MODE_INTRO:
			io_readIntroInput ( );
			break;

		case MODE_GUI:
			io_readGUIInput ( );
			break;

		case MODE_TERMINAL_MENU:
		case MODE_TERMINAL_SIDEVIEW:
		case MODE_TERMINAL_DB:
		case MODE_TERMINAL_LEVEL:
			io_readTerminalKeyboard ( );
			break;

		case MODE_LIFT_VIEW:
			io_readLiftKeyboard ( );
			break;

		case MODE_TRANSFER_INTRO:
		case MODE_TRANSFER_INTRO_1:
		case MODE_TRANSFER_INTRO_2:
		case MODE_TRANSFER_FINISH:
		case MODE_TRANSFER_START:
		case MODE_TRANSFER_COPY:

//			case MODE_TRANSFER_DRAW:
		case MODE_TRANSFER_SELECT:
		case MODE_TRANSFER_SELECT_SIDE:
			io_readTransferKeyboard ( );
			break;

		case MODE_PAUSED:
			if ( true == io_getKeyStateDown ( gamePause ) )
				{
					sys_changeMode ( MODE_SHOWLEVEL, false );
					gamePaused = false;
				}

			break;

		}
}

//-----------------------------------------------------------------------------
//
// Read the keyboard
//
// This reads keyboard events and sets the key state in our own array
// It also runs functions that need the 'event' system
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
			gameIsRunning = false;
			return;
			break;

		case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING:
// TODO (dberry#1#): call al_acknowledge_drawing_halt immediately.
			break;

		case ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING:
// TODO (dberry#1#): call al_acknowledge_drawing_resume immediately.
			break;

		case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:
// TODO - Reset up joystick information
			break;
		}

	//
	//  Assign true/false to currentlyDown for our keys
	io_getKeyboardState ( event );

	switch ( currentMode )
		{

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

			if ( true == inputAction[gameEscape].currentlyDown )
				{
					if (currentMode == MODE_SHOWLEVEL)
						confirmExit = true;

					inputAction[gameEscape].currentlyDown = false;
					sys_changeMode ( MODE_GUI, false );
					gui_changeToGUIScreen ( gui_findIndex ( GUI_OBJECT_SCREEN, "scrExitQuestion" ) );
				}

			break;

		case MODE_KEYCODE:
			io_readKeyCodeInput ( event );
			break;
		}

	//
	// Exit from game - move to menus when ready
	if ( true == inputAction[gameEscape].currentlyDown )
		{
//			gameIsRunning = false;
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
