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

//----------------------------------------------------------------------------
//
// Structure holding all the level information
//
//-----------------------------------------------------------------------------
int currentLevel = -1;
_levelStruct shipLevel[NUM_OF_LEVELS];
int currentAlertLevel;
_my2DInt drawOffset;

//-----------------------------------------------------------------------------
//
// structure to hold information for healing tiles
//
//-----------------------------------------------------------------------------
_basicHealing *healing; // hold information for animating healing tiles

//-----------------------------------------------------------------------------
//
// Score
//
//-----------------------------------------------------------------------------
char	displayScoreString[NUM_SCORE_CHARS];
int		currentScore = 0;
int		displayScore = 0;
float	scoreDelay;
float	scoreDelayValue;
int		yellowAlertLevel = 0; // From script
int		redAlertLevel = 0; // From script
int		levelBonus; // From script
bool	gameWon = false;

//---------------------------------------------------------
//
// Check to see if the ship is empty of all droids
void gam_isShipEmpty()
//---------------------------------------------------------
{
	if ( true == gameWon )
		return;

	for ( int i = 0; i != NUM_OF_LEVELS; i++ )
		{
			if ( i != 7 )
				{
					if ( shipLevel[i].numDroids > 0 )
						return;
				}
		}
	//
	// All levels are empty

	// Update score
//	currentScore = currentScore + displayScore;
	gameWon = true;
	playerCurrentHealth  = -100; // trigger end game sequence
	gam_doDamageToPlayer ( DAMAGE_EXPLOSION, -1 );
}
//---------------------------------------------------------
//
// Restart the game
void gam_restart()
//---------------------------------------------------------
{
	sys_clearAllPhysics();
	sys_setupPlayerPhysics();	// Before gam_setupPlayerValues so body is ready for mass setting
	gam_setupPlayerValues ( 0 );	// 0 == 001 in database
	gam_resetAllPaths();
	par_resetAllEmitters();
	bul_initBullets();

	//
	// Set droids to default values again
	//
	for ( int i = 0; i != NUM_OF_LEVELS; i++ )
		{
			if ( i != 7 )
				{
					shipLevel[i].droid.clear();
					gam_initDroidValues ( i );
				}
		}
	//
	// Reset Alert level
	//
	currentAlertLevel = ALERT_GREEN_TILE;
	processedPhysics = false;
	gameWon = false;
}

//---------------------------------------------------------
//
// All the droids are dead on this level - change tileset
void gam_powerDownLevel ( int whichLevel, bool playSound )
//---------------------------------------------------------
{
	if ( true == playSound )
		{
			sys_playSound ( SND_LEVEL_SHUTDOWN, 1.0f, ALLEGRO_PLAYMODE_ONCE );
			gam_addToScore ( levelBonus );
		}

	if ( 0 == shipLevel[whichLevel].numEnemiesAlive )
		{
			io_setTileSetColor ( "save" );
			io_setTileSetColor ( "dark" );
		}
	else
		io_setTileSetColor ( "restore" );

	gam_isShipEmpty();
}

//---------------------------------------------------------
//
// Check current alert level and change tiles and background sound

void gam_checkAlertLevel()
//---------------------------------------------------------
{
	if ( alertLevelDistance > volumeLevel )
		alertLevelDistance = volumeLevel; 	// Cap to user set level

	switch ( currentAlertLevel )
		{
		case ALERT_GREEN_TILE:
			if ( false == sys_isSoundPlaying ( SND_GREEN_ALERT ) )
				{
					sys_setVolume ( SND_GREEN_ALERT, alertLevelDistance );
					sys_playSound ( SND_GREEN_ALERT, SND_PAN_CENTER, ALLEGRO_PLAYMODE_LOOP );
				}
			else
				sys_setVolume ( SND_GREEN_ALERT, alertLevelDistance );

			if ( true == sys_isSoundPlaying ( SND_YELLOW_ALERT ) )
				sys_stopSound ( SND_YELLOW_ALERT );

			if ( true == sys_isSoundPlaying ( SND_RED_ALERT ) )
				sys_stopSound ( SND_RED_ALERT );

			break;

		case ALERT_YELLOW_TILE:
			if ( true == sys_isSoundPlaying ( SND_GREEN_ALERT ) )
				sys_stopSound ( SND_GREEN_ALERT );

			if ( false == sys_isSoundPlaying ( SND_YELLOW_ALERT ) )
				{
					sys_playSound ( SND_YELLOW_ALERT, SND_PAN_CENTER, ALLEGRO_PLAYMODE_LOOP );
					sys_setVolume ( SND_YELLOW_ALERT, alertLevelDistance );
				}
			else
				sys_setVolume ( SND_YELLOW_ALERT, alertLevelDistance );

			if ( true == sys_isSoundPlaying ( SND_RED_ALERT ) )
				sys_stopSound ( SND_RED_ALERT );

			break;

		case ALERT_RED_TILE:
			if ( true == sys_isSoundPlaying ( SND_GREEN_ALERT ) )
				sys_stopSound ( SND_GREEN_ALERT );

			if ( true == sys_isSoundPlaying ( SND_YELLOW_ALERT ) )
				sys_stopSound ( SND_YELLOW_ALERT );

			if ( false == sys_isSoundPlaying ( SND_RED_ALERT ) )
				{
					sys_playSound ( SND_RED_ALERT, SND_PAN_CENTER, ALLEGRO_PLAYMODE_LOOP );
					sys_setVolume ( SND_RED_ALERT, alertLevelDistance );
				}
			else
				sys_setVolume ( SND_RED_ALERT, alertLevelDistance );

			break;
		}
}

//---------------------------------------------------------
//
// Change to a new level from script - default to lift 0

void gam_changeToLevelFromScript ( int newLevel )
//---------------------------------------------------------
{
	gam_changeToLevel ( newLevel, 0 );
}

//---------------------------------------------------------
//
// Change to a new level

void gam_changeToLevel ( int newLevel, int whichLift )
//---------------------------------------------------------
{
	//
	// Remove any threads that might still be running from previous level
	//
	if ( currentLevel > -1 )
		{
			if ( shipLevel[currentLevel].droid.size() > 0 )
				{
					for ( int i = 0; i != shipLevel[currentLevel].numDroids; i++ )
						{
							if ( true == shipLevel[currentLevel].droid[i].isAlive )
								{
									if ( shipLevel[currentLevel].droid[i].aStarPathIndex > -1 )
										{
//											gam_AStarRemovePath ( shipLevel[currentLevel].droid[i].aStarPathIndex );
											ai_resetFleeFlags ( i );
											ai_resetHealthFlags ( i );
											ai_resetResumeFlags ( i );
										}
								}
						}
				}
		}

	currentLevel = newLevel;

//	con_print ( true, false, "Change to new level [ %i ]", currentLevel );

	//
	// Place player on lift
	lvl_getLiftPositions ( currentLevel );
	//
	// Find location of all the Alert tiles
	gam_findAllAlertTiles ( true );
	//
	// Get healing tiles for this level
	gam_findHealingTiles ( currentLevel );
	//
	// Setup door triggers
	gam_doorTriggerSetup();
	//
	// Player is not moving when starting
	sys_stopPlayerMovement();
	//
	// clear keys
	io_resetKeyboardStates();

	bul_initBullets();

	sys_createSolidWalls();

	gam_putPlayerOnLiftFromTunnel();
	//
	// See what color tiles we need
	gam_powerDownLevel ( currentLevel, false );

	gam_updateScrollingVars ( false );

	processedPhysics = false;
}

//---------------------------------------------------------
//
// Load the level into structure

bool gam_loadLevel ( char *fileName )
//---------------------------------------------------------
{
	int checkVersion;
	ALLEGRO_FILE *fp;

	//
	// Open in READ-ONLY mode
	//
	fp = al_fopen ( fileName, "r" );
	if ( NULL == fp )
		{
			printf ( "Error opening file for reading [ %s ]\n", fileName );
			return false;
		}
	//
	// Check this version is ok to use
	//
	al_fread ( fp, &checkVersion, sizeof ( checkVersion ) );
	if ( checkVersion != MAP_VERSION )
		{
			printf ( "MAP_VERSION wrong. Failed to read file.\n" );
			return false;
		}
	//
	// Read number variables
	//
	al_fread ( fp, ( void * ) &shipLevel[currentLevel].numLineSegments, sizeof ( shipLevel[currentLevel].numLineSegments ) );
	al_fread ( fp, ( void * ) &shipLevel[currentLevel].numWaypoints, sizeof ( shipLevel[currentLevel].numWaypoints ) );
	al_fread ( fp, ( void * ) &shipLevel[currentLevel].numDroids, sizeof ( shipLevel[currentLevel].numDroids ) );
	al_fread ( fp, ( void * ) &shipLevel[currentLevel].numLifts, sizeof ( shipLevel[currentLevel].numLifts ) );
	al_fread ( fp, ( void * ) &shipLevel[currentLevel].levelDimensions, sizeof ( shipLevel[currentLevel].levelDimensions ) );
	//
	// Line segments for physics collisions
	//
	_lineSegment tempSegment;
	for ( int i = 0; i != shipLevel[currentLevel].numLineSegments; i++ )
		{
			al_fread ( fp, &tempSegment.start, sizeof ( tempSegment.start ) );
			al_fread ( fp, &tempSegment.finish, sizeof ( tempSegment.finish ) );

			tempSegment.start.x += ( drawOffset.x / 2 ) * TILE_SIZE;
			tempSegment.start.y += ( drawOffset.y / 2 ) * TILE_SIZE;

			tempSegment.start.y -= ( TILE_SIZE / 2 );

			tempSegment.finish.x += ( drawOffset.x / 2 ) * TILE_SIZE;
			tempSegment.finish.y += ( drawOffset.y / 2 ) * TILE_SIZE;

			tempSegment.finish.y -= ( TILE_SIZE / 2 );

			shipLevel[currentLevel].lineSegments.push_back ( tempSegment );
		}
	//
	// Waypoints for Droid patrol
	//
	cpVect tempWaypoint;
	for ( int i = 0; i != shipLevel[currentLevel].numWaypoints; i++ )
		{
			al_fread ( fp, &tempWaypoint, sizeof ( tempWaypoint ) );
			//
			// Why is this needed?  Pixel offset from screen res?
			//
			tempWaypoint.x += ( drawOffset.x / 2 ) * TILE_SIZE;
			tempWaypoint.y += ( drawOffset.y / 2 ) * TILE_SIZE;

			tempWaypoint.y -= ( TILE_SIZE / 2 );

			shipLevel[currentLevel].wayPoints.push_back ( tempWaypoint );
		}
	//
	// Load each droid type on the current level
	//
	int tempDroidType;
	for ( int i = 0; i != shipLevel[currentLevel].numDroids; i++ )
		{
			al_fread ( fp, &tempDroidType, sizeof ( tempDroidType ) );
			shipLevel[currentLevel].droidTypes.push_back ( tempDroidType );
		}
	//
	// Array holding tile types
	//
	int tempTile;
	for ( int i = 0; i != shipLevel[currentLevel].levelDimensions.x * shipLevel[currentLevel].levelDimensions.y; i++ )
		{
			al_fread ( fp, &tempTile, sizeof ( tempTile ) );
			shipLevel[currentLevel].tiles.push_back ( tempTile );
		}
	al_fread ( fp, &shipLevel[currentLevel].levelName, sizeof ( shipLevel[currentLevel].levelName ) );

	//
	// Finished - close the file
	//
	al_fclose ( fp );

	return true;
}

//---------------------------------------------------------
//
// Convert current tile information into padded array

void lvl_addPaddingToLevel()
//---------------------------------------------------------
{
	vector<int> tempLevel;
	_my2DInt tempDimensions;
	int countY, countX, whichTile;
	int destX, destY;

	countY = 0;
	countX = 0;

	destX = drawOffset.x / 2;
	destY = drawOffset.y / 2;

	tempDimensions.x = shipLevel[currentLevel].levelDimensions.x;
	tempDimensions.y = shipLevel[currentLevel].levelDimensions.y;

	tempLevel.reserve ( ( shipLevel[currentLevel].levelDimensions.x + drawOffset.x ) * ( shipLevel[currentLevel].levelDimensions.y + drawOffset.y ) );

	for ( int i = 0; i != ( shipLevel[currentLevel].levelDimensions.x + drawOffset.x ) * ( shipLevel[currentLevel].levelDimensions.y + drawOffset.y ); i++ )
		tempLevel[i] = 0; // Blank tile


	for ( countY = 0; countY != shipLevel[currentLevel].levelDimensions.y; countY++ )
		{
			destX = ( drawOffset.x / 2 );
			for ( countX = 0; countX != shipLevel[currentLevel].levelDimensions.x; countX++ )
				{
					whichTile = shipLevel[currentLevel].tiles[ ( countY * shipLevel[currentLevel].levelDimensions.x ) + countX];
					tempLevel[ ( destY * ( tempDimensions.x + drawOffset.x ) ) + destX] = whichTile;
					destX++;
				}
			destY++;
		}

	tempDimensions.x += drawOffset.x;
	tempDimensions.y += drawOffset.y;

	shipLevel[currentLevel].levelDimensions.x = tempDimensions.x;
	shipLevel[currentLevel].levelDimensions.y = tempDimensions.y;

	shipLevel[currentLevel].tiles.clear();

	shipLevel[currentLevel].tiles.reserve ( tempDimensions.x * tempDimensions.y );

	for ( int i = 0; i != tempDimensions.x * tempDimensions.y; i++ )
		{
			shipLevel[currentLevel].tiles[i] = tempLevel[i];
		}
	tempLevel.clear();
}

//---------------------------------------------------------
//
// Store the lift linking information into vector array
//
// Call a second time to add to vector on same level

void lvl_setupLiftsStore ( int whichLevel, int whichTunnel )
//---------------------------------------------------------
{
	_liftBasic tempLift;

	tempLift.tunnel = whichTunnel;
	shipLevel[whichLevel].lifts.push_back ( tempLift );

//	con_print ( true, false, "[ %i ] - Lift size [ %i ]", whichLevel, shipLevel[whichLevel].lifts.size() );
}

//---------------------------------------------------------
//
// Setup lifts

void lvl_setupLifts()
//---------------------------------------------------------
{
	lvl_setupLiftsStore ( 0, 1 );
	lvl_setupLiftsStore ( 1, 4 );
	lvl_setupLiftsStore ( 2, 5 );
	lvl_setupLiftsStore ( 3, 0 );
	lvl_setupLiftsStore ( 4, 0 );
	lvl_setupLiftsStore ( 5, 1 );
	lvl_setupLiftsStore ( 5, 4 );
	lvl_setupLiftsStore ( 6, 1 );
	lvl_setupLiftsStore ( 6, 4 );
	lvl_setupLiftsStore ( 8, 5 );
	lvl_setupLiftsStore ( 9, 5 );
	lvl_setupLiftsStore ( 9, 6 );
	lvl_setupLiftsStore ( 10, 6 );
	lvl_setupLiftsStore ( 11, 6 );
	lvl_setupLiftsStore ( 12, 6 );
	lvl_setupLiftsStore ( 12, 3 );
	lvl_setupLiftsStore ( 12, 4 );
	lvl_setupLiftsStore ( 13, 6 );
	lvl_setupLiftsStore ( 13, 3 );
	lvl_setupLiftsStore ( 14, 6 );
	lvl_setupLiftsStore ( 14, 3 );
	lvl_setupLiftsStore ( 14, 7 );
	lvl_setupLiftsStore ( 15, 0 );
	lvl_setupLiftsStore ( 15, 2 );
	lvl_setupLiftsStore ( 16, 0 );
	lvl_setupLiftsStore ( 17, 2 );
	lvl_setupLiftsStore ( 17, 3 );
	lvl_setupLiftsStore ( 18, 7 );
	lvl_setupLiftsStore ( 19, 0 );
	lvl_setupLiftsStore ( 20, 3 );
}

//---------------------------------------------------------
//
// Load all the levels into memory

bool gam_loadAllLevels()
//---------------------------------------------------------
{
	char levelName[40];

	currentLevel = 0;

	drawOffset.x = ( winWidth / TILE_SIZE ); // Padding to make tilePosX always positive
	drawOffset.y = ( winHeight / TILE_SIZE ); // Padding to make tilePosY always positive

	for ( int i = 0; i < NUM_OF_LEVELS; i++ )
		{
			if ( 7 == i )
				currentLevel++;
			else
				{
					sprintf ( levelName, "newDeck%i.dat", i );

					if ( false == gam_loadLevel ( levelName ) )
						return false;

					lvl_addPaddingToLevel();

					con_setColor ( 0.0f, 1.0f, 1.0f, 1.0f );
					con_print ( true, false, "Loaded ship level [ %s ]", shipLevel[currentLevel].levelName );

					loadingBarPercent += 1.0f / NUM_OF_LEVELS;
					strlcpy ( loadingStatus, shipLevel[currentLevel].levelName, sizeof ( shipLevel[currentLevel].levelName ) );

					al_rest ( progressBarLoadTimes );

					gam_initDroidValues ( currentLevel );
					shipLevel[currentLevel].lifts.reserve ( shipLevel[currentLevel].numLifts );

					shipLevel[currentLevel].numEnemiesAlive = shipLevel[currentLevel].numDroids;
					currentLevel++;
				}
		}

	loadingBarPercent = 0.0f;

	lvl_setupLifts();

	sys_createSolidWalls();
	sys_setupPlayerPhysics();

	return true;
}

// ----------------------------------------------------------------------------
//
// Set pixel offsets based on screen resolution
//
// Sets global variables pixelX and pixelY

void gam_getPixelOffsets()
// ----------------------------------------------------------------------------
{
	//
	// Set global scroll offsets based on resolution
	//
	if ( fmod ( ( float ) winHeight / ( float ) TILE_SIZE, 2.0f ) != 0 )
		{
			// Number is odd
			pixelY = 16.0f;
			if ( winHeight == 600 )
				pixelY = 4.0f;
		}
	else
		pixelY = 16.0f;

	if ( fmod ( ( float ) winWidth / ( float ) TILE_SIZE, 2.0f ) != 0 )
		pixelX = 16.0f; // Number is odd
	else
		pixelX = 0.0f; // Number is even
}

// ----------------------------------------------------------------------------
//
// Position the player on the requested lift on the new level

void gam_putPlayerOnLift ( int whichLift )
// ----------------------------------------------------------------------------
{
	int whichTile, countY, countX, liftCounter;

	liftCounter = 0;

	for ( countY = 0; countY != shipLevel[currentLevel].levelDimensions.y; countY++ )
		{
			for ( countX = 0; countX != shipLevel[currentLevel].levelDimensions.x; countX++ )
				{
					whichTile = shipLevel[currentLevel].tiles[ ( countY * shipLevel[currentLevel].levelDimensions.x ) + countX];

					if ( LIFT_TILE == whichTile )
						{
							if ( liftCounter == whichLift )
								{
									tilePosX = countX;
									tilePosY = countY;

									//					tilePosX += 1;
									//					tilePosY += 1;

									gam_getPixelOffsets();

									pixelX = 16;
									pixelY = 0;
									playerWorldPos.x = ( tilePosX * TILE_SIZE ) + pixelX;
									playerWorldPos.y = ( tilePosY * TILE_SIZE ) + pixelY;

									sys_setPlayerPhysicsPosition ( playerWorldPos );

									return;
								}
							else
								{
									liftCounter++;
								}
						}
				}
		}

	io_logToFile ( "STOP: Couldn't find lift [ %i ] on level [ %i ]", whichLift, currentLevel );
	gam_putPlayerOnLift ( 0 ); // Infinite loop possible ??
}


// ----------------------------------------------------------------------------
//
// Find out where on the level the healing tiles are
// Remember for this level for animation

void gam_findHealingTiles ( int whichLevel )
// ----------------------------------------------------------------------------
{
	int i;
	int healCounter = 0;
	int currentTile = 0;
	int countX = 0;
	int countY = 0;

	for ( i = 0; i != shipLevel[currentLevel].levelDimensions.x * shipLevel[currentLevel].levelDimensions.y; i++ )
		{
			currentTile = shipLevel[whichLevel].tiles[i];

			switch ( currentTile )
				{
				case HEALING_TILE:
				case HEALING_TILE + 1:
				case HEALING_TILE + 2:
				case HEALING_TILE + 3:
					healCounter++;
					break;
				}
		}

	if ( healing )
		al_free ( healing );

	// no tiles on this level so just return but we still need to allocate enough
	// space to hold one record
	if ( 0 == healCounter )
		{
			healCounter++;
			healing = ( _basicHealing * ) al_malloc ( healCounter * sizeof ( _basicHealing ) );

			if ( NULL == healing )
				sys_errorFatal ( __FILE__, __LINE__, "Fatal error: Could not allocate [ %li ] bytes for healing array.", healCounter * sizeof ( _basicHealing ) );

			healing[0].numOnLevel = 0;
			return;
		}

	//
	// more than 0 tiles found so continue on with positions etc
	//
	healing = ( _basicHealing * ) malloc ( healCounter * sizeof ( _basicHealing ) );

	if ( NULL == healing )
		sys_errorFatal ( __FILE__, __LINE__, "Fatal error: Could not allocate [ %li ] bytes for healing array.", healCounter * sizeof ( _basicHealing ) );

	//
	// now assign positions to healing tiles
	//
	healCounter = 0;

	for ( i = 0; i != shipLevel[currentLevel].levelDimensions.x * shipLevel[currentLevel].levelDimensions.y; i++ )
		{
			currentTile = shipLevel[whichLevel].tiles[i];

			switch ( currentTile )
				{
				case HEALING_TILE:
				case HEALING_TILE + 1:
				case HEALING_TILE + 2:
				case HEALING_TILE + 3:
					healing[healCounter].pos = i;
					healing[healCounter].numFrames = 4;
					healing[healCounter].currentFrame = HEALING_TILE;
					healing[healCounter].frameDelay = 0.3f;
					healing[healCounter].nextFrame = 0.0f;
					healing[healCounter].worldPosition.x = ( countX * TILE_SIZE ) + ( TILE_SIZE / 2 );
					healing[healCounter].worldPosition.y = ( countY * TILE_SIZE ) + ( TILE_SIZE / 2 );
					healCounter++;
					break;
				}

			countX++;

			if ( countX == shipLevel[currentLevel].levelDimensions.x )
				{
					countX = 0;
					countY++;
				}
		}

	healing[0].numOnLevel = healCounter;
}

// ---------------------------------------------------------------------------
//
// draw the frames of the healing tile animation

void gam_animateHealTiles ( int whichLevel )
// ---------------------------------------------------------------------------
{
	int i;

	if ( 0 == healing[0].numOnLevel )
		return;

	for ( i = 0; i != healing[0].numOnLevel; i++ )
		{
			healing[i].nextFrame += healing[i].frameDelay;

			if ( healing[i].nextFrame > 1.0f )
				{
					healing[i].nextFrame = 0.0f;
					healing[i].currentFrame++;

					if ( healing[i].currentFrame > HEALING_TILE + 3 )
						healing[i].currentFrame = HEALING_TILE;

					//
					// update map with new tile
					shipLevel[whichLevel].tiles[healing[i].pos] = ( int ) healing[i].currentFrame;
				}
		}
}


// ----------------------------------------------------------------------------
//
// Get the current deck we are on and are moving around on

void gam_getCurrentDeck()
// ----------------------------------------------------------------------------
{
	int i;
	int topDeck = tunnel[currentTunnel].top;

	//
	// Look for the matching deck and current level
	//
	for ( i = 0; i != 10; i++ )
		{
			if ( currentLevel == tunnel[currentTunnel].decks[i] )
				{
					//
					// Found a match on this tunnel
					//
					tunnel[currentTunnel].current_deck = i;
					return;
				}

			if ( topDeck == tunnel[currentTunnel].decks[i] )
				{
					//
					// we have a problem - didn't find a matching level for this tunnel
					con_print ( true, true, "Error: Couldn't find a matching level for tunnel. Game in unstable state" );
				}
		}
}

// ----------------------------------------------------------------------------
//
// Get the tunnel being used by the lift the player sprite is over

void gam_getTunnelToUse()
// ----------------------------------------------------------------------------
{
	switch ( shipLevel[currentLevel].numLifts )
		{
		case 1: // only one lift on these levels
			currentTunnel = shipLevel[currentLevel].lifts[0].tunnel;
			break;

		case 2: // two lifts on these levels
			if ( ( ( playerWorldMiddlePos.x ) > shipLevel[currentLevel].lifts[0].posX - TILE_SIZE ) &&
			        ( ( playerWorldMiddlePos.x ) < shipLevel[currentLevel].lifts[0].posX + TILE_SIZE ) )
				{
					currentTunnel = shipLevel[currentLevel].lifts[0].tunnel;
				}
			else
				{
					currentTunnel = shipLevel[currentLevel].lifts[1].tunnel;
				}

			break;

		case 3: // only two levels with three lifts on them
			if ( currentLevel == 12 )
				{
					if ( ( playerWorldMiddlePos.x ) > shipLevel[currentLevel].lifts[0].posX - TILE_SIZE )
						{
							currentTunnel = shipLevel[currentLevel].lifts[0].tunnel;
							gam_getCurrentDeck();
							return;
						}

					if ( ( ( playerWorldMiddlePos.y ) > shipLevel[currentLevel].lifts[1].posY - TILE_SIZE ) &&
					        ( ( playerWorldMiddlePos.y ) < shipLevel[currentLevel].lifts[1].posY + TILE_SIZE ) )
						{
							currentTunnel = shipLevel[currentLevel].lifts[1].tunnel;
							gam_getCurrentDeck();
							return;
						}

					if ( ( playerWorldMiddlePos.y ) > shipLevel[currentLevel].lifts[2].posY - TILE_SIZE )
						{
							currentTunnel = shipLevel[currentLevel].lifts[2].tunnel;
							gam_getCurrentDeck();
							return;
						}
				} // end of if level is 12 test
			else
				{
					if ( currentLevel == 14 )
						{
							if ( ( ( playerWorldMiddlePos.x ) > shipLevel[currentLevel].lifts[2].posX - TILE_SIZE ) &&
							        ( ( playerWorldMiddlePos.x ) < shipLevel[currentLevel].lifts[2].posX + TILE_SIZE ) )
								{
									currentTunnel = shipLevel[currentLevel].lifts[2].tunnel;
									gam_getCurrentDeck();
									return;
								}

							if ( ( ( playerWorldMiddlePos.x ) > shipLevel[currentLevel].lifts[1].posX - TILE_SIZE ) &&
							        ( ( playerWorldMiddlePos.x ) < shipLevel[currentLevel].lifts[1].posX + TILE_SIZE ) )
								{
									currentTunnel = shipLevel[currentLevel].lifts[1].tunnel;
									gam_getCurrentDeck();
									return;
								}

							if ( ( ( playerWorldMiddlePos.x ) > shipLevel[currentLevel].lifts[0].posX - TILE_SIZE ) &&
							        ( ( playerWorldMiddlePos.x ) < shipLevel[currentLevel].lifts[0].posX + TILE_SIZE ) )
								{
									currentTunnel = shipLevel[currentLevel].lifts[0].tunnel;
									gam_getCurrentDeck();
									return;
								}
						} // end of is level 14 test
				} // end of else statement
		} // end of switch statement

	con_print ( true, false, "Tunnel in use [ %i ]", currentTunnel );

	//
	// Get the current deck pointer
	gam_getCurrentDeck();
} // end of function

// ----------------------------------------------------------------------------
//
// Put the player onto lift on new deck

void gam_putPlayerOnLiftFromTunnel()
// ----------------------------------------------------------------------------
{
	int temp_lift = 0;

	switch ( shipLevel[currentLevel].numLifts )
		{
		case 1:
			currentTunnel = shipLevel[currentLevel].lifts[0].tunnel;
			gam_putPlayerOnLift ( 0 );
			break;

		case 2:
			while ( currentTunnel != shipLevel[currentLevel].lifts[temp_lift].tunnel )
				{
					temp_lift++;
				}

			if ( currentLevel == 13 )
				{
					if ( currentTunnel == 3 )
						temp_lift = 1;
					else
						temp_lift = 0;
				}
			else
				currentTunnel = shipLevel[currentLevel].lifts[temp_lift].tunnel;

			gam_putPlayerOnLift ( temp_lift );
			break;

		case 3:
			if ( currentLevel == 14 )
				{
					switch ( currentTunnel )
						{
						case 7:
							gam_putPlayerOnLift ( 2 );
							break;

						case 6:
							gam_putPlayerOnLift ( 0 );
							break;

						case 3:
							gam_putPlayerOnLift ( 1 );
							break;

						default:
							gam_putPlayerOnLift ( 0 );
							break;
						}

					return;
				}
			else if ( currentLevel == 12 )
				{
					switch ( currentTunnel )
						{
						case 3:
							gam_putPlayerOnLift ( 1 );
							break;

						case 4:
							gam_putPlayerOnLift ( 2 );
							break;

						case 6:
							gam_putPlayerOnLift ( 0 );
							break;

						default:
							gam_putPlayerOnLift ( 0 );
							break;
						}
				}
		}
} // end of function


//---------------------------------------------------------
//
// Get the positions of lifts

void lvl_getLiftPositions ( int whichLevel )
//---------------------------------------------------------
{
	int countX = 0;
	int countY = 0;
	int i;
	int currentTile = 0;
	int countLift = 0;

	countLift = 0;
	countX = 0;
	countY = 0;

	for ( i = 0; i != shipLevel[whichLevel].levelDimensions.x * shipLevel[whichLevel].levelDimensions.y; i++ )
		{
			currentTile = shipLevel[whichLevel].tiles[ ( ( countY * ( shipLevel[whichLevel].levelDimensions.x ) ) + countX )];

			if ( LIFT_TILE == currentTile )
				{
					shipLevel[whichLevel].lifts[countLift].posX = countX * TILE_SIZE;
					shipLevel[whichLevel].lifts[countLift].posY = countY * TILE_SIZE;
					countLift++;
				}

			countX++;

			if ( countX == shipLevel[whichLevel].levelDimensions.x )
				{
					countX = 0;
					countY++;
				}
		}

	for ( int i = 0; i != shipLevel[whichLevel].numLifts; i++ )
		{
			con_print ( true, true, "Lift [ %i ] PosX [ %i ] PosY [ %i ]", i, shipLevel[whichLevel].lifts[i].posX, shipLevel[whichLevel].lifts[i].posY );
		}
}
