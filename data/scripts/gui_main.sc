//
// Script to setup the GUI elements
#include "data/scripts/commonDefines.h"

bool playSoundsTemp;
bool videoUseHoldingBMPTemp;
bool videoUseBackingBMPTemp;
bool fullScreenTemp;

void as_guiHandleMouseOver ( string &in )
{

	return;
}
//------------------------------------------------------------
//
// Change to a new GUI screen
void as_changeGUIScreen ( int newScreen )
//------------------------------------------------------------
{
	currentGUIScreen = newScreen;
}

//------------------------------------------------------------
//
// This function is called when a object is clicked
void as_guiHandleButtonPress ( string &in objectID )
//------------------------------------------------------------
{
	int randomStartLevel;

	//
	// Main option screen
	//
	if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) )
		{
			if ( objectID == "buttonStartGame" )
				{
					currentTileColorStr = as_guiGetSliderValue ( "sliderTileColor" );
					currentTileTypeStr = as_guiGetSliderValue ( "sliderTileType" );

					io_setTileSetColor ( currentTileColorStr );
					io_setTileSetType ( currentTileTypeStr );

					io_setTileSetColor ( "save" );

					volumeLevelStr = as_guiGetSliderValue ( "sliderAudioVol" );

					currentMode = MODE_BEAM_ON;
					sys_playSoundFromScript ( SND_START_ALL, SND_PAN_CENTER, false );

					randomStartLevel = as_getCappedRandomNum ( 21 );
					if ( 7 == randomStartLevel )
						randomStartLevel = 6;
						
					gam_changeToLevelFromScript ( randomStartLevel, true ); // Get random start level
					gam_resetScore();
					return;
				}

			if ( objectID == "buttonStartTutorial" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutorial" ) );
					as_guiSetObjectFocus ( "buttonTutMoveNext" );
					return;
				}

			if ( objectID == "buttonOptions" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrOptions" ) );
					as_guiSetObjectFocus ( "buttonOptionVideo" );
					return;
				}

			if ( objectID == "buttonHelp" )
				{
					currentMode = MODE_INTRO;
					return;
				}

			if ( objectID == "buttonQuit" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrExitQuestion" ) );
					as_guiSetObjectFocus ( "buttonExitCancel" );
					return;
				}
		}
	//
	// Options screen
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrOptions" ) )
		{
			if ( objectID == "buttonOptionVideo" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrVideoOptions" ) );
					currentObjectSelected = 0;
					return;
				}

			if ( objectID == "buttonOptionAudio" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrAudioOptions" ) );
					currentObjectSelected = 0;
					return;
				}

			if ( objectID == "buttonOptionGraphics" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrGraphics" ) );
					currentObjectSelected = 0;
					return;
				}

			if ( objectID == "buttonOptionGame" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrGameOptions" ) );
					as_guiSetObjectFocus ( "sliderLanguage" );
					return;
				}

			if ( objectID == "buttonOptionControls" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrGameControls" ) );
					as_guiSetObjectFocus ( "buttonControlApply" );
					return;
				}

			if ( objectID == "buttonOptionReturn" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}
		}
	//
	// Show video options
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrVideoOptions" ) )

		{
			if ( objectID == "checkBoxFullScreen" )
				{
					fullScreenTemp = !fullScreenTemp;
					as_guiSetCheckboxTick ( "checkBoxFullScreen", -1, fullScreenTemp );
					return;
				}

			if ( objectID == "videoUseHoldingBMP" )
				{
					videoUseHoldingBMPTemp = !videoUseHoldingBMPTemp;
					as_guiSetCheckboxTick ( "videoUseHoldingBMP", -1, videoUseHoldingBMPTemp );
					return;
				}

			if ( objectID == "videoUseBackingBMP" )
				{
					//
					// Message box to do a restart
					// Don't change variable but write to config file
					//
					as_guiSetCheckboxTick ( "videoUseBackingBMP", -1, videoUseBackingBMPTemp );
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrRestartQuestion" ) );
					return;
				}

			if ( objectID == "buttonVideoOptionsApply" )
				{
					swapMethod = gui_StringToInt ( as_guiGetSliderValue ( "sliderSwapType" ) );
					vsyncOption = gui_StringToInt ( as_guiGetSliderValue ( "sliderVSyncType" ) );

					if ( fullScreenTemp == true )
						fullScreenValue = 1;
					else
						fullScreenValue = 0;

//					useBackingBitmap = videoUseBackingBMPTemp;
					useHoldBitmap = videoUseHoldingBMPTemp;
					as_guiSetCheckboxTick ( "checkBoxFullScreen", -1, fullScreenValue == 1 ? true : false );
					as_guiSetCheckboxTick ( "videoUseBackingBMP", -1, useBackingBitmap );
					as_guiSetCheckboxTick ( "videoUseHoldingBMP", -1, useHoldBitmap );
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}

			if ( objectID == "buttonVideoOptionsCancel" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrOptions" ) );
					as_guiSetObjectFocus ( "buttonOptionVideo" );
					return;
				}
		}
	//
	// Show Audio options
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrAudioOptions" ) )
		{
			if ( objectID == "buttonAudioOptionsApply" )
				{
					volumeLevelStr = as_guiGetSliderValue ( "sliderAudioVol" );

					playSounds = playSoundsTemp;
					as_guiSetCheckboxTick ( "audioCheckBox", -1, playSounds );
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}

			if ( objectID == "buttonAudioOptionsCancel" )
				{
					as_guiSetCheckboxTick ( "audioCheckBox", -1, playSounds );
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrOptions" ) );
					as_guiSetObjectFocus ( "buttonOptionVideo" );
					return;
				}

			if ( objectID == "audioCheckBox" )
				{
					playSoundsTemp = !playSoundsTemp;
					as_guiSetCheckboxTick ( "audioCheckBox", -1, playSoundsTemp );
					return;
				}
		}
	//
	// Show Graphics options
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrGraphics" ) )
		{
			if ( objectID == "buttonGraphicsApply" )
				{
					currentTileColorStr = as_guiGetSliderValue ( "sliderTileColor" );
					currentTileTypeStr = as_guiGetSliderValue ( "sliderTileType" );
					particleRenderType = gui_StringToInt ( as_guiGetSliderValue ( "sliderParticleType" ) );

					io_setTileSetColor ( currentTileColorStr );
					io_setTileSetType ( currentTileTypeStr );

					io_setTileSetColor ( "save" );

					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}

			if ( objectID == "buttonGraphicsCancel" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrOptions" ) );
					as_guiSetObjectFocus ( "buttonOptionVideo" );
					return;
				}
		}

	//
	// Show Game Options
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrGameOptions" ) )
		{
			if ( objectID == "buttonGameOptionsApply" )
				{
					currentLanguage = gui_StringToInt ( as_guiGetSliderValue ( "sliderLanguage" ) );
					as_setLanguageStrings();

					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}

			if ( objectID == "buttonGameOptionsCancel" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrOptions" ) );
					as_guiSetObjectFocus ( "buttonOptionVideo" );
					return;
				}
		}
	//
	// Show keycode setting screen
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrGameControls" ) )
		{
			if ( objectID == "buttonControlApply" )
				{
					if ( numJoysticks > 0 )
						inputMethod = gui_StringToInt ( as_guiGetSliderValue ( "controlType" ) );
					else
						inputMethod = INPUT_KEYBOARD;

					as_guiCopyGUIIntoKeys ( "keyCodeControl" );
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrOptions" ) );
					as_guiSetObjectFocus ( "buttonOptionVideo" );
					return;
				}

			if ( objectID == "buttonControlCancel" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrOptions" ) );
					as_guiSetObjectFocus ( "buttonOptionVideo" );
					return;
				}

			if ( objectID == "keyCodeControl" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrKeyCodeCapture" ) );
					currentMode = MODE_KEYCODE;
					return;
				}
		}
	//
	// Ask if we should exit game
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrExitQuestion" ) )
		{
			if ( objectID == "buttonExitYes" )
				{
					gameIsRunning = false;
					return;
				}

			if ( objectID == "buttonExitCancel" )
				{
					if ( false == confirmExit )	// Called from menu
						{
							as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
							as_guiSetObjectFocus ( "buttonStartGame" );
						}
					else	// Called while playing
						{
							currentMode = MODE_SHOWLEVEL;
						}
					return;
				}
		}
	//
	// Game is over - player won
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrEndGame" ) )
		{
			if ( objectID == "buttonEndGameYes" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}
		}
	//
	// Inform user a restart is required
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrRestartQuestion" ) )
		{
			if ( objectID == "buttonRestartYes" )
				{
					videoUseBackingBMPTemp = useBackingBitmap;
					videoUseBackingBMPTemp = !videoUseBackingBMPTemp;
					io_saveConfigValueInt ( "useBackingBitmap", videoUseBackingBMPTemp == true ? 1 : 0 );
					io_saveConfigFile ( "config.ini" );
					io_closeConfigFile();
					gameIsRunning = false;
					return;
				}

			if ( objectID == "buttonRestartCancel" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrVideoOptions" ) );
					return;
				}
		}
	//
	// In Game terminal options
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTerminal" ) )
		{
			if ( objectID == "buttonTermLogOff" )
				{
					as_changeGameMode ( MODE_SHOWLEVEL, true );
					return;
				}

			if ( objectID == "buttonTermInfo" )
				{
					droidTypeDBIndex = playerDroidTypeDBIndex;	// Start database at current droid player type
					gam_loadDroidModel ( droidTypeDBIndex );

					as_changeGameMode ( MODE_TERMINAL_DB, true );
					as_setdbValues ();
					as_guiSetupDBText ();
					return;
				}

			if ( objectID == "buttonTermDeck" )
				{
					as_changeGameMode ( MODE_TERMINAL_LEVEL, true );
					return;
				}

			if ( objectID == "buttonTermShip" )
				{
					as_changeGameMode ( MODE_TERMINAL_SIDEVIEW, true );
					return;
				}
		}
	//
	// Start of the tutorial screens
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutorial" ) )
		{
			if ( objectID == "buttonTutorialCancel" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}

			if ( objectID == "buttonTutMoveNext" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutTransfer" ) );
					as_guiSetObjectFocus ( "buttonTutTransNext" );
					return;
				}
		}
	//
	// Tutorial on doing a transfer
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutTransfer" ) )
		{
			if ( objectID == "buttonTutTransCancel" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}

			if ( objectID == "buttonTutTransNext" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutTransGame" ) );
					as_guiSetObjectFocus ( "buttonTutTransGameNext" );
					return;
				}
		}
	//
	// Tutorial on transfer game
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutTransGame" ) )
		{
			if ( objectID == "buttonTutTransGameCancel" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}

			if ( objectID == "buttonTutTransGameNext" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutLifts" ) );
					as_guiSetObjectFocus ( "buttonTutLiftNext" );
					return;
				}
		}
	//
	// How to use the lifts
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutLifts" ) )
		{
			if ( objectID == "buttonTutLiftCancel" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}

			if ( objectID == "buttonTutLiftNext" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutTerminals" ) );
					as_guiSetObjectFocus ( "buttonTutTerminalNext" );
					return;
				}
		}
	//
	// How to use the terminals
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutTerminals" ) )
		{
			if ( objectID == "buttonTutTerminalCancel" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}

			if ( objectID == "buttonTutTerminalNext" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutHealing" ) );
					as_guiSetObjectFocus ( "buttonTutHealinglNext" );
					return;
				}
		}

	//
	// How to use the healing tiles
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutHealing" ) )
		{
			if ( objectID == "buttonTutHealingCancel" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}

			if ( objectID == "buttonTutHealinglNext" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutTips" ) );
					as_guiSetObjectFocus ( "buttonTutTipsCancel" );
					return;
				}
		}

	//
	// Gameplay tips
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrTutTips" ) )
		{
			if ( objectID == "buttonTutTipsCancel" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}
		}
	//
	// Display high score table
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrDisplayScoreTable" ) )
		{
			if ( objectID == "buttonDisplayYes" )
				{
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrMainMenu" ) );
					as_guiSetObjectFocus ( "buttonStartGame" );
					return;
				}
		}
	//
	// Edit initials for new high score - then change to display score table
	//
	else if ( currentGUIScreen == as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrEditScoreTable" ) )
		{
			if ( objectID == "buttonEditYes" )
				{
					newInitials = as_guiGetSliderValue ( "scoreSliderOne" );
					newInitials = newInitials + as_guiGetSliderValue ( "scoreSliderTwo" );
					newInitials = newInitials + as_guiGetSliderValue ( "scoreSliderThree" );
					gam_addNewInitials();
					gui_populateScoreTableGUI();
					as_changeGUIScreen ( as_guiFindIndex ( GUI_OBJECT_SCREEN, "scrDisplayScoreTable" ) );
					as_guiSetObjectFocus ( "buttonDisplayYes" );
					return;
				}
		}
}

//------------------------------------------------------------
//
// Setup the elements for the terminal screen

void as_setupGUITerminal ()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrTerminal" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTermLogOff" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTermLogOff", GUI_COORD_TYPE_PERCENT, 25, 20, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTermLogOff", GUI_LABEL_CENTER, gui_getString ( "logoffTerminal" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTermLogOff", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTermLogOff", "scrTerminal" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTermInfo" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTermInfo", GUI_COORD_TYPE_PERCENT, 25, 40, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTermInfo", GUI_LABEL_CENTER, gui_getString ( "droidInfo" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTermInfo", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTermInfo", "scrTerminal" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTermDeck" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTermDeck", GUI_COORD_TYPE_PERCENT, 25, 60, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTermDeck", GUI_LABEL_CENTER, gui_getString ( "deckMap" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTermDeck", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTermDeck", "scrTerminal" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTermShip" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTermShip", GUI_COORD_TYPE_PERCENT, 25, 80, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTermShip", GUI_LABEL_CENTER, gui_getString ( "shipView" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTermShip", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTermShip", "scrTerminal" );

}
//------------------------------------------------------------
//
// Setup all the buttons for the GUI Screens

void as_setupGUIMainMenu ()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrMainMenu" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonStartGame" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonStartGame", GUI_COORD_TYPE_PERCENT, 25, 20, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonStartGame", GUI_LABEL_CENTER, gui_getString ( "startGame" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonStartGame", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonStartGame", "scrMainMenu" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonStartTutorial" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonStartTutorial", GUI_COORD_TYPE_PERCENT, 25, 35, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonStartTutorial", GUI_LABEL_CENTER, gui_getString ( "tutorial" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonStartTutorial", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonStartTutorial", "scrMainMenu" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonOptions" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonOptions", GUI_COORD_TYPE_PERCENT, 25, 50, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonOptions", GUI_LABEL_CENTER, gui_getString ( "options" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonOptions", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonOptions", "scrMainMenu" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonHelp" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonHelp", GUI_COORD_TYPE_PERCENT, 25, 65, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonHelp", GUI_LABEL_CENTER, gui_getString ( "help" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonHelp", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonHelp", "scrMainMenu" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonQuit" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonQuit", GUI_COORD_TYPE_PERCENT, 25, 80, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonQuit", GUI_LABEL_CENTER, gui_getString ( "exit" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonQuit", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonQuit", "scrMainMenu" );
}


//------------------------------------------------------------
//
// Setup all the buttons for the GUI Screens

void as_setupGUIScreenOptions ()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrOptions" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonOptionVideo" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonOptionVideo", GUI_COORD_TYPE_PERCENT, 25, 20, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonOptionVideo", GUI_LABEL_CENTER, gui_getString ( "optionVideo" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonOptionVideo", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonOptionVideo", "scrOptions" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonOptionAudio" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonOptionAudio", GUI_COORD_TYPE_PERCENT, 25, 30, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonOptionAudio", GUI_LABEL_CENTER, gui_getString ( "optionAudio" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonOptionAudio", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonOptionAudio", "scrOptions" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonOptionGraphics" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonOptionGraphics", GUI_COORD_TYPE_PERCENT, 25, 40, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonOptionGraphics", GUI_LABEL_CENTER, gui_getString ( "optionGraphics" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonOptionGraphics", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonOptionGraphics", "scrOptions" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonOptionGame" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonOptionGame", GUI_COORD_TYPE_PERCENT, 25, 50, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonOptionGame", GUI_LABEL_CENTER, gui_getString ( "optionGame" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonOptionGame", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonOptionGame", "scrOptions" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonOptionControls" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonOptionControls", GUI_COORD_TYPE_PERCENT, 25, 60, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonOptionControls", GUI_LABEL_CENTER, gui_getString ( "optionControls" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonOptionControls", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonOptionControls", "scrOptions" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonOptionReturn" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonOptionReturn", GUI_COORD_TYPE_PERCENT, 25, 80, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonOptionReturn", GUI_LABEL_CENTER, gui_getString ( "optionMainMenu" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonOptionReturn", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonOptionReturn", "scrOptions" );
}

//------------------------------------------------------------
//
// Setup Game options screen
void as_guiGUISetupGameOptions()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrGameOptions" );

	as_guiCreateObject ( GUI_OBJECT_SLIDER, "sliderLanguage" );
	as_guiSetObjectPosition ( GUI_OBJECT_SLIDER, "sliderLanguage", GUI_COORD_TYPE_PERCENT, 5, 35, 40, 2 );
	as_guiSetObjectLabel ( GUI_OBJECT_SLIDER, "sliderLanguage", GUI_LABEL_CENTER, gui_getString ( "languageType" ) );
	as_guiAddSliderElement ( "sliderLanguage", GUI_SLIDER_STRING, gui_IntToString ( LANG_ENGLISH ), gui_getString ( "langEnglish" ) );
	as_guiAddSliderElement ( "sliderLanguage", GUI_SLIDER_STRING, gui_IntToString ( LANG_ITALIAN ), gui_getString ( "langItalian" ) );
	as_guiSetSliderValue ( "sliderLanguage", gui_IntToString ( currentLanguage ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_SLIDER, "sliderLanguage", "scrGameOptions" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonGameOptionsApply" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonGameOptionsApply", GUI_COORD_TYPE_PERCENT, 25, 70, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonGameOptionsApply", GUI_LABEL_CENTER, gui_getString ( "buttonApply" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonGameOptionsApply", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonGameOptionsApply", "scrGameOptions" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonGameOptionsCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonGameOptionsCancel", GUI_COORD_TYPE_PERCENT, 25, 80, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonGameOptionsCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonGameOptionsCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonGameOptionsCancel", "scrGameOptions" );
}

//------------------------------------------------------------
//
// Setup the screen to capture a keycode press
void as_guiGUISetupKeyCodeCapture()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrKeyCodeCapture" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "captureHelp" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "captureHelp", GUI_COORD_TYPE_PERCENT, 50, 20, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "captureHelp", GUI_LABEL_CENTER, gui_getString ( "captureHelp" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "captureHelp", "scrKeyCodeCapture" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "captureHelpCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "captureHelpCancel", GUI_COORD_TYPE_PERCENT, 50, 50, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "captureHelpCancel", GUI_LABEL_CENTER, gui_getString ( "captureHelpCancel" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "captureHelpCancel", "scrKeyCodeCapture" );
}

//------------------------------------------------------------
//
// Setup key controls screen
void as_guiGUISetupControls()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrGameControls" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "controlsHelp" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "controlsHelp", GUI_COORD_TYPE_PERCENT, 50, 16, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "controlsHelp", GUI_LABEL_CENTER, gui_getString ( "controlsHelp" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "controlsHelp", "scrGameControls" );

	as_guiCreateObject ( GUI_OBJECT_KEYCODE, "keyCodeControl" );

	for ( int i = 0; i != NUMBER_ACTIONS; i++ )
		{
			as_guiAddKeyCodeElement ( "keyCodeControl", "Blank", 0 );
		}
	as_guiCopyKeysInto ( "keyCodeControl" );
	as_guiSetObjectPosition ( GUI_OBJECT_KEYCODE, "keyCodeControl", GUI_COORD_TYPE_PERCENT, 2, 25, 40, 10 );
	as_guiSetObjectFunctions ( GUI_OBJECT_KEYCODE, "keyCodeControl", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_KEYCODE, "keyCodeControl", "scrGameControls" );

	if ( numJoysticks > 0 )
		{
			as_guiCreateObject ( GUI_OBJECT_SLIDER, "controlType" );
			as_guiSetObjectPosition ( GUI_OBJECT_SLIDER, "controlType", GUI_COORD_TYPE_PERCENT, 30, 70, 40, 2 );
			as_guiSetObjectLabel ( GUI_OBJECT_SLIDER, "controlType", GUI_LABEL_CENTER, gui_getString ( "controlType" ) );
			as_guiAddSliderElement ( "controlType", GUI_SLIDER_STRING, gui_getString ( "inputKeyboardValue" ), gui_getString ( "inputKeyboardLabel" ) );
			as_guiAddSliderElement ( "controlType", GUI_SLIDER_STRING, gui_getString ( "inputJoystickValue" ), gui_getString ( "inputJoystickLabel" ) );
			as_guiSetSliderValue ( "controlType", gui_IntToString ( inputMethod ) );
			as_guiAddObjectToScreen ( GUI_OBJECT_SLIDER, "controlType", "scrGameControls" );
		}

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonControlApply" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonControlApply", GUI_COORD_TYPE_PERCENT, 10, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonControlApply", GUI_LABEL_CENTER, gui_getString ( "buttonApply" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonControlApply", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonControlApply", "scrGameControls" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonControlCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonControlCancel", GUI_COORD_TYPE_PERCENT, 65, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonControlCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonControlCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonControlCancel", "scrGameControls" );
}

//------------------------------------------------------------
//
// Setup Video screen

void as_guiGUISetupVideo ()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrVideoOptions" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "textLabelResolution" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "textLabelResolution", GUI_COORD_TYPE_PERCENT, 25, 20, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "textLabelResolution", GUI_LABEL_CENTER, gui_getString ( "videoOptions" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "textLabelResolution", "scrVideoOptions" );

	as_guiCreateObject ( GUI_OBJECT_CHECKBOX, "videoUseBackingBMP" );
	as_guiSetObjectFunctions ( GUI_OBJECT_CHECKBOX, "videoUseBackingBMP", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiSetObjectPosition ( GUI_OBJECT_CHECKBOX, "videoUseBackingBMP", GUI_COORD_TYPE_PERCENT, 5, 30, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_CHECKBOX, "videoUseBackingBMP", GUI_LABEL_LEFT, gui_getString ( "videoUseBackingBMP" ) );
	as_guiSetCheckboxGroup ( "videoUseBackingBMP", -1 );
	as_guiSetCheckboxTick ( "videoUseBackingBMP", -1, useBackingBitmap );
	as_guiAddObjectToScreen ( GUI_OBJECT_CHECKBOX, "videoUseBackingBMP", "scrVideoOptions" );

	as_guiCreateObject ( GUI_OBJECT_CHECKBOX, "videoUseHoldingBMP" );
	as_guiSetObjectFunctions ( GUI_OBJECT_CHECKBOX, "videoUseHoldingBMP", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiSetObjectPosition ( GUI_OBJECT_CHECKBOX, "videoUseHoldingBMP", GUI_COORD_TYPE_PERCENT, 55, 30, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_CHECKBOX, "videoUseHoldingBMP", GUI_LABEL_LEFT, gui_getString ( "videoUseHoldingBMP" ) );
	as_guiSetCheckboxGroup ( "videoUseHoldingBMP", -1 );
	as_guiSetCheckboxTick ( "videoUseHoldingBMP", -1, useHoldBitmap );
	as_guiAddObjectToScreen ( GUI_OBJECT_CHECKBOX, "videoUseHoldingBMP", "scrVideoOptions" );

	as_guiCreateObject ( GUI_OBJECT_CHECKBOX, "checkBoxFullScreen" );
	as_guiSetObjectFunctions ( GUI_OBJECT_CHECKBOX, "checkBoxFullScreen", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiSetObjectPosition ( GUI_OBJECT_CHECKBOX, "checkBoxFullScreen", GUI_COORD_TYPE_PERCENT, 5, 40, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_CHECKBOX, "checkBoxFullScreen", GUI_LABEL_CENTER, gui_getString ( "fullScreen" ) );
	as_guiSetCheckboxGroup ( "checkBoxFullScreen", -1 );
	as_guiSetCheckboxTick ( "checkBoxFullScreen", -1, fullScreenValue == 1 ? true : false );
	as_guiAddObjectToScreen ( GUI_OBJECT_CHECKBOX, "checkBoxFullScreen", "scrVideoOptions" );

	as_guiCreateObject ( GUI_OBJECT_SLIDER, "sliderSwapType" );
	as_guiSetObjectPosition ( GUI_OBJECT_SLIDER, "sliderSwapType", GUI_COORD_TYPE_PERCENT, 5, 55, 40, 2 );
	as_guiSetObjectLabel ( GUI_OBJECT_SLIDER, "sliderSwapType", GUI_LABEL_CENTER, gui_getString ( "swapType" ) );
	as_guiAddSliderElement ( "sliderSwapType", GUI_SLIDER_STRING, gui_getString ( "swapCopyValue" ), gui_getString ( "swapCopyLabel" ) );
	as_guiAddSliderElement ( "sliderSwapType", GUI_SLIDER_STRING, gui_getString ( "swapFlipValue" ), gui_getString ( "swapFlipLabel" ) );
	as_guiSetSliderValue ( "sliderSwapType", gui_IntToString ( swapMethod ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_SLIDER, "sliderSwapType", "scrVideoOptions" );

	as_guiCreateObject ( GUI_OBJECT_SLIDER, "sliderVSyncType" );
	as_guiSetObjectPosition ( GUI_OBJECT_SLIDER, "sliderVSyncType", GUI_COORD_TYPE_PERCENT, 55, 55, 40, 2 );
	as_guiSetObjectLabel ( GUI_OBJECT_SLIDER, "sliderVSyncType", GUI_LABEL_CENTER, gui_getString ( "vsyncType" ) );
	as_guiAddSliderElement ( "sliderVSyncType", GUI_SLIDER_STRING, gui_getString ( "vsyncSystemValue" ), gui_getString ( "vsyncSystemLabel" ) );
	as_guiAddSliderElement ( "sliderVSyncType", GUI_SLIDER_STRING, gui_getString ( "vsyncWaitValue" ), gui_getString ( "vsyncWaitLabel" ) );
	as_guiAddSliderElement ( "sliderVSyncType", GUI_SLIDER_STRING, gui_getString ( "vsyncOffValue" ), gui_getString ( "vsyncOffLabel" ) );
	as_guiSetSliderValue ( "sliderVSyncType", gui_IntToString ( vsyncOption ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_SLIDER, "sliderVSyncType", "scrVideoOptions" );


	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonVideoOptionsApply" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonVideoOptionsApply", GUI_COORD_TYPE_PERCENT, 25, 70, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonVideoOptionsApply", GUI_LABEL_CENTER, gui_getString ( "buttonApply" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonVideoOptionsApply", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonVideoOptionsApply", "scrVideoOptions" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonVideoOptionsCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonVideoOptionsCancel", GUI_COORD_TYPE_PERCENT, 25, 80, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonVideoOptionsCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonVideoOptionsCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonVideoOptionsCancel", "scrVideoOptions" );
}

//------------------------------------------------------------
//
// Setup Audio screen
void as_guiGUISetupAudio ()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrAudioOptions" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "textLabelSoundOptions" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "textLabelSoundOptions", GUI_COORD_TYPE_PERCENT, 25, 20, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "textLabelSoundOptions", GUI_LABEL_CENTER, "Sound Options" );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "textLabelSoundOptions", "scrAudioOptions" );

	if ( false == as_useSound )
		{
			as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "textLabelNoSound" );
			as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "textLabelNoSound", GUI_COORD_TYPE_PERCENT, 30, 35, 25, 25 );
			as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "textLabelNoSound", GUI_LABEL_CENTER, gui_getString ( "audioSndDisabled" ) );
			as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "textLabelNoSound", "scrAudioOptions" );
		}
	else
		{
			as_guiCreateObject ( GUI_OBJECT_CHECKBOX, "audioCheckBox" );
			as_guiSetObjectFunctions ( GUI_OBJECT_CHECKBOX, "audioCheckBox", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
			as_guiSetObjectPosition ( GUI_OBJECT_CHECKBOX, "audioCheckBox", GUI_COORD_TYPE_PERCENT, 25, 30, 25, 25 );
			as_guiSetObjectLabel ( GUI_OBJECT_CHECKBOX, "audioCheckBox", GUI_LABEL_LEFT, gui_getString ( "audioPlaySnd" ) );
			as_guiSetCheckboxGroup ( "audioCheckBox", -1 );
			as_guiSetCheckboxTick ( "audioCheckBox", -1, playSounds );
			as_guiAddObjectToScreen ( GUI_OBJECT_CHECKBOX, "audioCheckBox", "scrAudioOptions" );


			as_guiCreateObject ( GUI_OBJECT_SLIDER, "sliderAudioVol" );
			as_guiSetObjectPosition ( GUI_OBJECT_SLIDER, "sliderAudioVol", GUI_COORD_TYPE_PERCENT, 25, 45, 40, 2 );
			as_guiSetObjectLabel ( GUI_OBJECT_SLIDER, "sliderAudioVol", GUI_LABEL_CENTER, "Volume" );

			string tempNum;

			for ( int i = 0; i != 10; i++ )
				{
					tempNum = gui_IntToString ( i + 1 );
					as_guiAddSliderElement ( "sliderAudioVol", GUI_SLIDER_STRING, tempNum, tempNum );
				}

			as_guiSetSliderValue ( "sliderAudioVol", volumeLevelStr );
			as_guiAddObjectToScreen ( GUI_OBJECT_SLIDER, "sliderAudioVol", "scrAudioOptions" );
		}

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonAudioOptionsApply" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonAudioOptionsApply", GUI_COORD_TYPE_PERCENT, 25, 70, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonAudioOptionsApply", GUI_LABEL_CENTER, gui_getString ( "buttonApply" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonAudioOptionsApply", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonAudioOptionsApply", "scrAudioOptions" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonAudioOptionsCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonAudioOptionsCancel", GUI_COORD_TYPE_PERCENT, 25, 80, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonAudioOptionsCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonAudioOptionsCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonAudioOptionsCancel", "scrAudioOptions" );
}


//------------------------------------------------------------
//
// Setup Graphics options screen
void as_guiGUISetupGraphics ()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrGraphics" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "textLabelGraphics" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "textLabelGraphics", GUI_COORD_TYPE_PERCENT, 25, 20, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "textLabelGraphics", GUI_LABEL_CENTER, "Graphics Options" );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "textLabelGraphics", "scrGraphics" );

	as_guiCreateObject ( GUI_OBJECT_SLIDER, "sliderTileColor" );
	as_guiSetObjectPosition ( GUI_OBJECT_SLIDER, "sliderTileColor", GUI_COORD_TYPE_PERCENT, 5, 35, 40, 2 );
	as_guiSetObjectLabel ( GUI_OBJECT_SLIDER, "sliderTileColor", GUI_LABEL_CENTER, gui_getString ( "tileColor" ) );
	as_guiAddSliderElement ( "sliderTileColor", GUI_SLIDER_STRING, gui_getString ( "redValue" ), gui_getString ( "redLabel" ) );
	as_guiAddSliderElement ( "sliderTileColor", GUI_SLIDER_STRING, gui_getString ( "greenValue" ), gui_getString ( "greenLabel" ) );
	as_guiAddSliderElement ( "sliderTileColor", GUI_SLIDER_STRING, gui_getString ( "blueValue" ), gui_getString ( "blueLabel" ) );
	as_guiAddSliderElement ( "sliderTileColor", GUI_SLIDER_STRING, gui_getString ( "yellowValue" ), gui_getString ( "yellowLabel" ) );
	as_guiAddSliderElement ( "sliderTileColor", GUI_SLIDER_STRING, gui_getString ( "grayValue" ), gui_getString ( "grayLabel" ) );
	as_guiAddSliderElement ( "sliderTileColor", GUI_SLIDER_STRING, gui_getString ( "turquoiseValue" ), gui_getString ( "turquoiseLabel" ) );
	as_guiSetSliderValue ( "sliderTileColor", currentTileColorStr );
	as_guiAddObjectToScreen ( GUI_OBJECT_SLIDER, "sliderTileColor", "scrGraphics" );

	as_guiCreateObject ( GUI_OBJECT_SLIDER, "sliderTileType" );
	as_guiSetObjectPosition ( GUI_OBJECT_SLIDER, "sliderTileType", GUI_COORD_TYPE_PERCENT, 55, 35, 40, 2 );
	as_guiSetObjectLabel ( GUI_OBJECT_SLIDER, "sliderTileType", GUI_LABEL_CENTER, gui_getString ( "tileType" ) );
	as_guiAddSliderElement ( "sliderTileType", GUI_SLIDER_STRING, gui_getString ( "retroValue" ), gui_getString ( "retroLabel" ) );
	as_guiAddSliderElement ( "sliderTileType", GUI_SLIDER_STRING, gui_getString ( "futureValue" ), gui_getString ( "futureLabel" ) );
	as_guiAddSliderElement ( "sliderTileType", GUI_SLIDER_STRING, gui_getString ( "classicValue" ), gui_getString ( "classicLabel" ) );
	as_guiSetSliderValue ( "sliderTileType", currentTileTypeStr );
	as_guiAddObjectToScreen ( GUI_OBJECT_SLIDER, "sliderTileType", "scrGraphics" );

	as_guiCreateObject ( GUI_OBJECT_SLIDER, "sliderParticleType" );
	as_guiSetObjectPosition ( GUI_OBJECT_SLIDER, "sliderParticleType", GUI_COORD_TYPE_PERCENT, 5, 55, 40, 2 );
	as_guiSetObjectLabel ( GUI_OBJECT_SLIDER, "sliderParticleType", GUI_LABEL_CENTER, gui_getString ( "particleType" ) );
	as_guiAddSliderElement ( "sliderParticleType", GUI_SLIDER_STRING, gui_getString ( "particleOffValue" ), gui_getString ( "particleOffLabel" ) );
	as_guiAddSliderElement ( "sliderParticleType", GUI_SLIDER_STRING, gui_getString ( "particlePrimValue" ), gui_getString ( "particlePrimLabel" ) );
	as_guiAddSliderElement ( "sliderParticleType", GUI_SLIDER_STRING, gui_getString ( "particleBitmapValue" ), gui_getString ( "particleBitmapLabel" ) );
	as_guiSetSliderValue ( "sliderParticleType", gui_IntToString ( particleRenderType ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_SLIDER, "sliderParticleType", "scrGraphics" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonGraphicsApply" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonGraphicsApply", GUI_COORD_TYPE_PERCENT, 25, 70, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonGraphicsApply", GUI_LABEL_CENTER, gui_getString ( "buttonApply" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonGraphicsApply", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonGraphicsApply", "scrGraphics" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonGraphicsCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonGraphicsCancel", GUI_COORD_TYPE_PERCENT, 25, 80, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonGraphicsCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonGraphicsCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonGraphicsCancel", "scrGraphics" );
}

//------------------------------------------------------------
//
// Setup tutorial screen
void as_guiSetupTutorial ()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrTutorial" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "textLabelTutMove" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "textLabelTutMove", GUI_COORD_TYPE_PERCENT, 50, 17, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "textLabelTutMove", GUI_LABEL_CENTER, gui_getString ( "tutMoveLabel" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "textLabelTutMove", "scrTutorial" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "tutTextBox" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "tutTextBox", GUI_COORD_TYPE_PERCENT, 5, 25, 90, 90 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "tutTextBox", GUI_OBJECT_TEXT_LABEL_COLOR, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "tutTextBox", GUI_LABEL_LEFT, gui_getString ( "tutMoveText" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "tutTextBox", "scrTutorial" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTutMoveNext" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTutMoveNext", GUI_COORD_TYPE_PERCENT, 10, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTutMoveNext", GUI_LABEL_CENTER, gui_getString ( "buttonNext" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTutMoveNext", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTutMoveNext", "scrTutorial" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTutorialCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTutorialCancel", GUI_COORD_TYPE_PERCENT, 65, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTutorialCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTutorialCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTutorialCancel", "scrTutorial" );

	//
	// Screen how to start transfer mode
	//
	as_guiCreateNewScreen ( "scrTutTransfer" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTransMove" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTransMove", GUI_COORD_TYPE_PERCENT, 50, 17, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTransMove", GUI_LABEL_CENTER, gui_getString ( "tutTransferLabel" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTransMove", "scrTutTransfer" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "tutTransTextBox" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "tutTransTextBox", GUI_COORD_TYPE_PERCENT, 5, 25, 90, 90 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "tutTransTextBox", GUI_OBJECT_TEXT_LABEL_COLOR, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "tutTransTextBox", GUI_LABEL_LEFT, gui_getString ( "tutTransText" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "tutTransTextBox", "scrTutTransfer" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTutTransNext" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTutTransNext", GUI_COORD_TYPE_PERCENT, 10, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTutTransNext", GUI_LABEL_CENTER, gui_getString ( "buttonNext" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTutTransNext", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTutTransNext", "scrTutTransfer" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTutTransCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTutTransCancel", GUI_COORD_TYPE_PERCENT, 65, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTutTransCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTutTransCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTutTransCancel", "scrTutTransfer" );

	//
	// Screen explaining transfer game
	//
	as_guiCreateNewScreen ( "scrTutTransGame" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTransGame" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTransGame", GUI_COORD_TYPE_PERCENT, 50, 17, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTransGame", GUI_LABEL_CENTER, gui_getString ( "tutTransGameLabel" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTransGame", "scrTutTransGame" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "tutTransGameTextBox" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "tutTransGameTextBox", GUI_COORD_TYPE_PERCENT, 5, 25, 90, 90 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "tutTransGameTextBox", GUI_OBJECT_TEXT_LABEL_COLOR, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "tutTransGameTextBox", GUI_LABEL_LEFT, gui_getString ( "tutTransGameText" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "tutTransGameTextBox", "scrTutTransGame" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTutTransGameNext" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTutTransGameNext", GUI_COORD_TYPE_PERCENT, 10, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTutTransGameNext", GUI_LABEL_CENTER, gui_getString ( "buttonNext" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTutTransGameNext", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTutTransGameNext", "scrTutTransGame" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTutTransGameCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", GUI_COORD_TYPE_PERCENT, 65, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", "scrTutTransGame" );

	//
	// Screen showing how to use lifts
	//
	as_guiCreateNewScreen ( "scrTutLifts" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "textLabelTutLifts" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "textLabelTutLifts", GUI_COORD_TYPE_PERCENT, 50, 17, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "textLabelTutLifts", GUI_LABEL_CENTER, gui_getString ( "tutLiftLabel" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "textLabelTutLifts", "scrTutLifts" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "tutLiftTextBox" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "tutLiftTextBox", GUI_COORD_TYPE_PERCENT, 5, 25, 90, 90 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "tutLiftTextBox", GUI_OBJECT_TEXT_LABEL_COLOR, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "tutLiftTextBox", GUI_LABEL_LEFT, gui_getString ( "tutLiftText" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "tutLiftTextBox", "scrTutLifts" );

	as_guiCreateObject ( GUI_OBJECT_IMAGE, "tutImageLift" );
	as_guiSetObjectPosition ( GUI_OBJECT_IMAGE, "tutImageLift", GUI_COORD_TYPE_PERCENT, 50, 45, 50, 10 );
	as_guiSetImageFilename ( "tutImageLift", "tut_lift.png" );
	as_guiSetObjectColor ( GUI_OBJECT_IMAGE, "tutImageLift", -1, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiAddObjectToScreen ( GUI_OBJECT_IMAGE, "tutImageLift", "scrTutLifts" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTutLiftNext" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTutLiftNext", GUI_COORD_TYPE_PERCENT, 10, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTutLiftNext", GUI_LABEL_CENTER, gui_getString ( "buttonNext" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTutLiftNext", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTutLiftNext", "scrTutLifts" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTutLiftCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTutLiftCancel", GUI_COORD_TYPE_PERCENT, 65, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTutLiftCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTutLiftCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTutLiftCancel", "scrTutLifts" );

	//
	// Screen showing terminal use
	//
	as_guiCreateNewScreen ( "scrTutTerminals" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTerminals" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTerminals", GUI_COORD_TYPE_PERCENT, 50, 17, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTerminals", GUI_LABEL_CENTER, gui_getString ( "tutTerminalLabel" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTerminals", "scrTutTerminals" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "tutTerminalTextBox" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "tutTerminalTextBox", GUI_COORD_TYPE_PERCENT, 5, 25, 90, 90 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "tutTerminalTextBox", GUI_OBJECT_TEXT_LABEL_COLOR, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "tutTerminalTextBox", GUI_LABEL_LEFT, gui_getString ( "tutTerminalText" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "tutTerminalTextBox", "scrTutTerminals" );

	as_guiCreateObject ( GUI_OBJECT_IMAGE, "tutImageTerminal" );
	as_guiSetObjectPosition ( GUI_OBJECT_IMAGE, "tutImageTerminal", GUI_COORD_TYPE_PERCENT, 50, 45, 50, 10 );
	as_guiSetImageFilename ( "tutImageTerminal", "tut_terminal.png" );
	as_guiSetObjectColor ( GUI_OBJECT_IMAGE, "tutImageTerminal", -1, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiAddObjectToScreen ( GUI_OBJECT_IMAGE, "tutImageTerminal", "scrTutTerminals" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTutTerminalNext" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTutTerminalNext", GUI_COORD_TYPE_PERCENT, 10, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTutTerminalNext", GUI_LABEL_CENTER, gui_getString ( "buttonNext" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTutTerminalNext", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTutTerminalNext", "scrTutTerminals" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTutTerminalCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", GUI_COORD_TYPE_PERCENT, 65, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", "scrTutTerminals" );

	//
	// Screen showing healing use
	//
	as_guiCreateNewScreen ( "scrTutHealing" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "textLabelTutHealing" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "textLabelTutHealing", GUI_COORD_TYPE_PERCENT, 50, 17, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "textLabelTutHealing", GUI_LABEL_CENTER, gui_getString ( "tutHealingLabel" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "textLabelTutHealing", "scrTutHealing" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "tutHealingTextBox" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "tutHealingTextBox", GUI_COORD_TYPE_PERCENT, 5, 25, 90, 90 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "tutHealingTextBox", GUI_OBJECT_TEXT_LABEL_COLOR, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "tutHealingTextBox", GUI_LABEL_LEFT, gui_getString ( "tutHealingText" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "tutHealingTextBox", "scrTutHealing" );

	as_guiCreateObject ( GUI_OBJECT_IMAGE, "tutImageHealing" );
	as_guiSetObjectPosition ( GUI_OBJECT_IMAGE, "tutImageHealing", GUI_COORD_TYPE_PERCENT, 50, 50, 50, 10 );
	as_guiSetImageFilename ( "tutImageHealing", "tut_healing.png" );
	as_guiSetObjectColor ( GUI_OBJECT_IMAGE, "tutImageHealing", -1, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiAddObjectToScreen ( GUI_OBJECT_IMAGE, "tutImageHealing", "scrTutHealing" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTutHealinglNext" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTutHealinglNext", GUI_COORD_TYPE_PERCENT, 10, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTutHealinglNext", GUI_LABEL_CENTER, gui_getString ( "buttonNext" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTutHealinglNext", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTutHealinglNext", "scrTutHealing" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTutHealingCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTutHealingCancel", GUI_COORD_TYPE_PERCENT, 65, 80, 30, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTutHealingCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTutHealingCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTutHealingCancel", "scrTutHealing" );

	//
	// Screen showing some gameplay tips
	//
	as_guiCreateNewScreen ( "scrTutTips" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTips" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTips", GUI_COORD_TYPE_PERCENT, 50, 17, 25, 25 );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTips", GUI_LABEL_CENTER, gui_getString ( "tutTipsLabel" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_LABEL, "textLabelTutTips", "scrTutTips" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "tutTipsTextBox" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "tutTipsTextBox", GUI_COORD_TYPE_PERCENT, 5, 25, 90, 90 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "tutTipsTextBox", GUI_OBJECT_TEXT_LABEL_COLOR, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "tutTipsTextBox", GUI_LABEL_LEFT, gui_getString ( "tutTipsText" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "tutTipsTextBox", "scrTutTips" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonTutTipsCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonTutTipsCancel", GUI_COORD_TYPE_PERCENT, 25, 80, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonTutTipsCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonTutTipsCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonTutTipsCancel", "scrTutTips" );
}

//------------------------------------------------------------
//
// Show the endGame screen
void as_guiSetupEndGameScreen()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrEndGame" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "endGameTextBox" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "endGameTextBox", GUI_COORD_TYPE_PERCENT, 5, 25, 90, 90 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "endGameTextBox", GUI_OBJECT_TEXT_LABEL_COLOR, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "endGameTextBox", GUI_LABEL_LEFT, gui_getString ( "endGameTextBox" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "endGameTextBox", "scrEndGame" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonEndGameYes" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonEndGameYes", GUI_COORD_TYPE_PERCENT, 15, 70, 70, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonEndGameYes", GUI_LABEL_CENTER, gui_getString ( "buttonEndGameYes" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonEndGameYes", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonEndGameYes", "scrEndGame" );
}
//------------------------------------------------------------
//
// Setup Are you sure you want to exit screen
void as_guiSetupExitQuestion ()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrExitQuestion" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "exitTextBox" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "exitTextBox", GUI_COORD_TYPE_PERCENT, 5, 25, 90, 90 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "exitTextBox", GUI_OBJECT_TEXT_LABEL_COLOR, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "exitTextBox", GUI_LABEL_LEFT, gui_getString ( "exitTextBox" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "exitTextBox", "scrExitQuestion" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonExitYes" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonExitYes", GUI_COORD_TYPE_PERCENT, 25, 70, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonExitYes", GUI_LABEL_CENTER, gui_getString ( "buttonConfirm" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonExitYes", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonExitYes", "scrExitQuestion" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonExitCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonExitCancel", GUI_COORD_TYPE_PERCENT, 25, 80, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonExitCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonExitCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonExitCancel", "scrExitQuestion" );
}

//------------------------------------------------------------
//
// Setup Are you sure you want to exit screen
void as_guiSetupRestartQuestion ()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrRestartQuestion" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "restartTextBox" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "restartTextBox", GUI_COORD_TYPE_PERCENT, 5, 25, 90, 90 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "restartTextBox", GUI_OBJECT_TEXT_LABEL_COLOR, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "restartTextBox", GUI_LABEL_LEFT, gui_getString ( "restartTextBox" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "restartTextBox", "scrRestartQuestion" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonRestartYes" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonRestartYes", GUI_COORD_TYPE_PERCENT, 25, 70, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonRestartYes", GUI_LABEL_CENTER, gui_getString ( "buttonConfirm" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonRestartYes", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonRestartYes", "scrRestartQuestion" );

	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonRestartCancel" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonRestartCancel", GUI_COORD_TYPE_PERCENT, 25, 80, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonRestartCancel", GUI_LABEL_CENTER, gui_getString ( "buttonCancel" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonRestartCancel", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonRestartCancel", "scrRestartQuestion" );
}

//------------------------------------------------------------
//
// Setup the screen to display the high score table
void as_guiSetupDisplayScoreTable()
//------------------------------------------------------------
{
	int col_1_x = 25;
	int col_2_x = 40;
	int col_3_x = 60;
	int col_y_start = 35;
	int col_y_space = 7;

	as_guiCreateNewScreen ( "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "displayScoreTextBox" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "displayScoreTextBox", GUI_COORD_TYPE_PERCENT, 40, 25, 40, 40 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "displayScoreTextBox", GUI_OBJECT_TEXT_LABEL_COLOR, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "displayScoreTextBox", GUI_LABEL_CENTER, gui_getString ( "displayScoreTextBox" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "displayScoreTextBox", "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "displayScoreOne" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "displayScoreOne", GUI_COORD_TYPE_PERCENT, col_1_x, col_y_start, 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "displayScoreOne", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "displayScoreOne", GUI_LABEL_CENTER, gui_getString ( "1" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "displayScoreOne", "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "displayScoreTwo" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "displayScoreTwo", GUI_COORD_TYPE_PERCENT, col_1_x, col_y_start + ( col_y_space * 1 ), 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "displayScoreTwo", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "displayScoreTwo", GUI_LABEL_CENTER, gui_getString ( "2" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "displayScoreTwo", "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "displayScoreThree" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "displayScoreThree", GUI_COORD_TYPE_PERCENT, col_1_x, col_y_start + ( col_y_space * 2 ), 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "displayScoreThree", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "displayScoreThree", GUI_LABEL_CENTER, gui_getString ( "3" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "displayScoreThree", "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "displayScoreFour" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "displayScoreFour", GUI_COORD_TYPE_PERCENT, col_1_x, col_y_start + ( col_y_space * 3 ), 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "displayScoreFour", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "displayScoreFour", GUI_LABEL_CENTER, gui_getString ( "4" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "displayScoreFour", "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "displayScoreFive" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "displayScoreFive", GUI_COORD_TYPE_PERCENT, col_1_x, col_y_start + ( col_y_space * 4 ), 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "displayScoreFive", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "displayScoreFive", GUI_LABEL_CENTER, gui_getString ( "5" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "displayScoreFive", "scrDisplayScoreTable" );
//
// Setup text boxes for initials
//
	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "scoreInitialsOne" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "scoreInitialsOne", GUI_COORD_TYPE_PERCENT, col_2_x, col_y_start, 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "scoreInitialsOne", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreInitialsOne", GUI_LABEL_CENTER, "AAA" );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "scoreInitialsOne", "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "scoreInitialsTwo" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "scoreInitialsTwo", GUI_COORD_TYPE_PERCENT, col_2_x, col_y_start + ( col_y_space * 1 ), 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "scoreInitialsTwo", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreInitialsTwo", GUI_LABEL_CENTER, "BBB" );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "scoreInitialsTwo", "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "scoreInitialsThree" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "scoreInitialsThree", GUI_COORD_TYPE_PERCENT, col_2_x, col_y_start + ( col_y_space * 2 ), 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "scoreInitialsThree", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreInitialsThree", GUI_LABEL_CENTER, "CCC" );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "scoreInitialsThree", "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "scoreInitialsFour" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "scoreInitialsFour", GUI_COORD_TYPE_PERCENT, col_2_x, col_y_start + ( col_y_space * 3 ), 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "scoreInitialsFour", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreInitialsFour", GUI_LABEL_CENTER, "DDD" );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "scoreInitialsFour", "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "scoreInitialsFive" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "scoreInitialsFive", GUI_COORD_TYPE_PERCENT, col_2_x, col_y_start + ( col_y_space * 4 ), 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "scoreInitialsFive", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreInitialsFive", GUI_LABEL_CENTER, "EEE" );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "scoreInitialsFive", "scrDisplayScoreTable" );
//
// Setup text boxes for score
//
	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "scoreScoreOne" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "scoreScoreOne", GUI_COORD_TYPE_PERCENT, col_3_x, col_y_start, 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "scoreScoreOne", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreScoreOne", GUI_LABEL_CENTER, "10000" );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "scoreScoreOne", "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "scoreScoreTwo" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "scoreScoreTwo", GUI_COORD_TYPE_PERCENT, col_3_x, col_y_start + ( col_y_space * 1 ), 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "scoreScoreTwo", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreScoreTwo", GUI_LABEL_CENTER, "8000" );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "scoreScoreTwo", "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "scoreScoreThree" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "scoreScoreThree", GUI_COORD_TYPE_PERCENT, col_3_x, col_y_start + ( col_y_space * 2 ), 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "scoreScoreThree", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreScoreThree", GUI_LABEL_CENTER, "6000" );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "scoreScoreThree", "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "scoreScoreFour" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "scoreScoreFour", GUI_COORD_TYPE_PERCENT, col_3_x, col_y_start + ( col_y_space * 3 ), 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "scoreScoreFour", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreScoreFour", GUI_LABEL_CENTER, "4000" );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "scoreScoreFour", "scrDisplayScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "scoreScoreFive" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "scoreScoreFive", GUI_COORD_TYPE_PERCENT, col_3_x, col_y_start + ( col_y_space * 4 ), 20, 20 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "scoreScoreFive", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreScoreFive", GUI_LABEL_CENTER, "2000" );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "scoreScoreFive", "scrDisplayScoreTable" );


	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonDisplayYes" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonDisplayYes", GUI_COORD_TYPE_PERCENT, 25, 80, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonDisplayYes", GUI_LABEL_CENTER, gui_getString ( "buttonConfirm" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonDisplayYes", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonDisplayYes", "scrDisplayScoreTable" );
}

//-----------------------------------------------------------------------------
//
// GUI Screen to edit initial for high score table
void as_guiSetupEditScoreScreen()
//-----------------------------------------------------------------------------
{
	as_guiCreateNewScreen ( "scrEditScoreTable" );

	as_guiCreateObject ( GUI_OBJECT_TEXT_BOX, "editTextBox" );
	as_guiSetObjectPosition ( GUI_OBJECT_TEXT_BOX, "editTextBox", GUI_COORD_TYPE_PERCENT, 25, 25, 90, 90 );
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_BOX, "editTextBox", GUI_OBJECT_TEXT_LABEL_COLOR, 1.0f, 1.0f, 1.0f, 0.5f );
	as_guiSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "editTextBox", GUI_LABEL_LEFT, gui_getString ( "editTextBox" ) );
	as_guiAddObjectToScreen ( GUI_OBJECT_TEXT_BOX, "editTextBox", "scrEditScoreTable" );

//
// First Initial
//
	as_guiCreateObject ( GUI_OBJECT_SLIDER, "scoreSliderOne" );
	as_guiSetObjectPosition ( GUI_OBJECT_SLIDER, "scoreSliderOne", GUI_COORD_TYPE_PERCENT, 44, 45, 3, 2 );
	as_guiSetObjectLabel ( GUI_OBJECT_SLIDER, "scoreSliderOne", GUI_LABEL_CENTER, "" );

	for ( int i = 0; i != 26; i++ )
		{
			as_guiAddSliderElement ( "scoreSliderOne", GUI_SLIDER_STRING, gui_IntToChar ( i ), gui_IntToChar ( i ) );
		}
	as_guiSetSliderValue ( "scoreSliderOne", "A" );
	as_guiAddObjectToScreen ( GUI_OBJECT_SLIDER, "scoreSliderOne", "scrEditScoreTable" );
//
// Second Initial
//
	as_guiCreateObject ( GUI_OBJECT_SLIDER, "scoreSliderTwo" );
	as_guiSetObjectPosition ( GUI_OBJECT_SLIDER, "scoreSliderTwo", GUI_COORD_TYPE_PERCENT, 48, 45, 3, 2 );
	as_guiSetObjectLabel ( GUI_OBJECT_SLIDER, "scoreSliderTwo", GUI_LABEL_CENTER, "" );

	for ( int i = 0; i != 26; i++ )
		{
			as_guiAddSliderElement ( "scoreSliderTwo", GUI_SLIDER_STRING, gui_IntToChar ( i ), gui_IntToChar ( i ) );
		}
	as_guiSetSliderValue ( "scoreSliderTwo", "B" );
	as_guiAddObjectToScreen ( GUI_OBJECT_SLIDER, "scoreSliderTwo", "scrEditScoreTable" );
//
// Third Initial
//
	as_guiCreateObject ( GUI_OBJECT_SLIDER, "scoreSliderThree" );
	as_guiSetObjectPosition ( GUI_OBJECT_SLIDER, "scoreSliderThree", GUI_COORD_TYPE_PERCENT, 52, 45, 3, 2 );
	as_guiSetObjectLabel ( GUI_OBJECT_SLIDER, "scoreSliderThree", GUI_LABEL_CENTER, "" );

	for ( int i = 0; i != 26; i++ )
		{
			as_guiAddSliderElement ( "scoreSliderThree", GUI_SLIDER_STRING, gui_IntToChar ( i ), gui_IntToChar ( i ) );
		}
	as_guiSetSliderValue ( "scoreSliderThree", "C" );
	as_guiAddObjectToScreen ( GUI_OBJECT_SLIDER, "scoreSliderThree", "scrEditScoreTable" );


	as_guiCreateObject ( GUI_OBJECT_BUTTON, "buttonEditYes" );
	as_guiSetObjectPosition ( GUI_OBJECT_BUTTON, "buttonEditYes", GUI_COORD_TYPE_PERCENT, 25, 80, 50, 7 );
	as_guiSetObjectLabel ( GUI_OBJECT_BUTTON, "buttonEditYes", GUI_LABEL_CENTER, gui_getString ( "buttonConfirm" ) );
	as_guiSetObjectFunctions ( GUI_OBJECT_BUTTON, "buttonEditYes", "scr_guiHandleButtonPress", "scr_guiHandleMouseOver" );
	as_guiAddObjectToScreen ( GUI_OBJECT_BUTTON, "buttonEditYes", "scrEditScoreTable" );
}


//-----------------------------------------------------------------------------
//
// Setup colors and variables for db scrollbox

void as_setdbValues ()
//-----------------------------------------------------------------------------
{
	// Position of the droid rotating graphic
	dbImagePositionX = ( winWidth - 290.0f );
	dbImagePositionY = 165.0f;

	// Position of the scrollbox in the droid database
	dbStartX = 70;
	dbStartY = 50;
	dbWidth = ( winWidth - dbStartX ) - 40.0f;
	dbHeight = winHeight - dbStartY;
	dbScrollspeed = 0.85;
	dbBackRed = 0.0;
	dbBackGreen = 0.0;
	dbBackBlue = 1.0;
	dbBackAlpha = 1.0;
	dbFontRed = 0.4;
	dbFontGreen = 0.7;
	dbFontBlue = 0.7;
	dbFontAlpha = 0.8;
	dbScrollDelay = 0.35f;
	scrollPauseDelayValue = 50.0f;
}

//------------------------------------------------------------
//
// Setup objects onto screens

void as_guiSetupGUI ()
//------------------------------------------------------------
{
	sliderCurveX = 2.0f;
	sliderCurveY = 2.0f;
	sliderSelectorRadius = 10.0f;
	buttonCurveX = 10.0f;
	buttonCurveY = 10.0f;
	checkBoxTickSize = 4;
	keyCodeGap = 5;
	indicatorAnimSpeed = 2.0f;

	playSoundsTemp = playSounds;
	videoUseHoldingBMPTemp = useHoldBitmap;
	videoUseBackingBMPTemp = useBackingBitmap;
	fullScreenTemp = ( fullScreenValue == 1 ? true : false );

	io_doKeyBindings ( "keybind.dat" );	// Call before as_guiGUISetupControls();

	as_setupGUIMainMenu ();
	as_setupGUIScreenOptions ();
	as_guiGUISetupVideo ();
	as_guiGUISetupAudio ();
	as_guiGUISetupGraphics ();
	as_guiGUISetupGameOptions ();
	as_guiSetupExitQuestion ();
	as_setupGUITerminal ();
	as_guiSetupTutorial ();
	as_guiGUISetupControls ();
	as_guiGUISetupKeyCodeCapture ();
	as_guiSetupRestartQuestion ();
	as_guiSetupDisplayScoreTable ();
	as_guiSetupEditScoreScreen ();
	as_guiSetupEndGameScreen();

	//
	// --------------- SET COLORS FOR CHECKBOXES -------------------------
	as_guiSetObjectColor ( GUI_OBJECT_CHECKBOX, "ALL", GUI_CHECKBOX_TICKED_COL, 0.9f, 0.9f, 0.9f, 1.0f );
	as_guiSetObjectColor ( GUI_OBJECT_CHECKBOX, "ALL", GUI_CHECKBOX_NOT_TICKED_COL, 0.6f, 0.6f, 0.6f, 1.0f );

	//
	// --------------- SET COLORS FOR BUTTONS -------------------------
	as_guiSetObjectColor ( GUI_OBJECT_BUTTON, "ALL", GUI_BUTTON_ACTIVE_COL, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectColor ( GUI_OBJECT_BUTTON, "ALL", GUI_BUTTON_INACTIVE_COL, 0.0f, 0.3f, 0.3f, 0.0f );
	as_guiSetObjectColor ( GUI_OBJECT_BUTTON, "ALL", GUI_BUTTON_ACTIVE_EDGE_COL, 0.6f, 0.9f, 0.9f, 0.8f );
	as_guiSetObjectColor ( GUI_OBJECT_BUTTON, "ALL", GUI_BUTTON_INACTIVE_EDGE_COL, 0.2f, 0.5f, 0.5f, 0.0f );
	as_guiSetObjectColor ( GUI_OBJECT_BUTTON, "ALL", GUI_BUTTON_ACTIVE_LABEL_COL, 0.0f, 0.2f, 0.2f, 0.8f );
	as_guiSetObjectColor ( GUI_OBJECT_BUTTON, "ALL", GUI_BUTTON_INACTIVE_LABEL_COL, 0.2f, 0.5f, 0.5f, 0.0f );

	//
	//------------------ SET COLORS FOR TEXT LABELS ---------------------------
	as_guiSetObjectColor ( GUI_OBJECT_TEXT_LABEL, "ALL", GUI_OBJECT_TEXT_LABEL_COLOR, 0.4f, 0.7f, 0.7f, 0.8f );

	//
	//-----------------------SET COLORS FOR SLIDERS ----------------------------
	as_guiSetObjectColor ( GUI_OBJECT_SLIDER, "ALL", GUI_SLIDER_BAR_COL_ACTIVE, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectColor ( GUI_OBJECT_SLIDER, "ALL", GUI_SLIDER_BAR_COL_NOT_ACTIVE, 0.0f, 0.3f, 0.3f, 0.0f );

	//
	//----------------------SET COLORS FOR KEYCODES ---------------------------
	as_guiSetObjectColor ( GUI_OBJECT_KEYCODE, "ALL", GUI_BUTTON_ACTIVE_COL, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectColor ( GUI_OBJECT_KEYCODE, "ALL", GUI_BUTTON_INACTIVE_COL, 0.0f, 0.3f, 0.3f, 0.0f );

	//
	// ---------------------SET COLORS FOR CHECKBOXES --------------------------
	as_guiSetObjectColor ( GUI_OBJECT_CHECKBOX, "ALL", GUI_CHECKBOX_TICKED_COL, 0.4f, 0.7f, 0.7f, 0.8f );
	as_guiSetObjectColor ( GUI_OBJECT_CHECKBOX, "ALL", GUI_CHECKBOX_NOT_TICKED_COL, 0.0f, 0.3f, 0.3f, 0.0f );
}
