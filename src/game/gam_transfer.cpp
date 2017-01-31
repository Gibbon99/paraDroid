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
#include "../../hdr/game/gam_transferRender.h"

int 			droidTypeToTransferInto;
//int             droidTransfer;

int 			squareWidth;
int 			squareHeight;
_transferCells 	transferCells[NUMBER_CELLS];

//int      transferWithSprite;	// which sprite are we transferring to

int 			playerOnSide = LEFT_SIDE; // Setup default side
int 			droidOnSide = RIGHT_SIDE;

int 			numPlayerTokens;
int 			numDroidTokens;

float			transferPlayCountDown;
float			transferPlayCount = 0.0f;
int				transferPlayCountLeft = 0;
int				transferPlayCountLeftDefault;

float 			enemyTransferThinkCount = 0.0f;
float 			enemyTransferThinkDelay = 0.0f;
bool 			foundACircuit = false;
int 			nextCircuitToUse;

int 			selectSideCounter = TRANSFER_COUNTER;
float 			delayTransferIntro = TRANSFER_DELAY;

int 			playerBlockPos;
int 			droidBlockPos;

int 			transferDroidPosX;
int 			transferDroidPosY;
int 			transferTextPosX;
int 			transferTextHeight;
int 			transferTextPosY;
int 			transferTitlePosX;
int 			transferTitlePosY;

bool 			circuitFound = false;
float 			circuitTimeAlive;

int 			droidTransferedIntoIndex = -1;	// also used to bypass explosion detection after transfer

int 			numActiveLeft = 0;
int 			numActiveRight = 0;
int 			numActiveDispute = 0;

//------------------------------------------------------------
//
// Return a random float value - used for color

float trn_getRandColor()
//------------------------------------------------------------
{
	return float ( sys_genRand_int32() % 100 ) / 100.0f;
}

//------------------------------------------------------------
//
// Process active circuits

void trn_processCircuits ( float interpolate )
//------------------------------------------------------------
{
	numActiveLeft = 0;
	numActiveRight = 0;

	for ( int i = 0; i != NUMBER_CELLS; i++ )
		{

			if ( ( true == transferCells[i].isRepeaterLeft ) && ( true == transferCells[i].repeaterLeftActivated ) )
				{
					host_transSetCellColor ( i, POWER_YELLOW, COL_YELLOW );
				}

			if ( ( true == transferCells[i].isRepeaterRight ) && ( true == transferCells[i].repeaterRightActivated ) )
				{
					host_transSetCellColor ( i, POWER_PURPLE, COL_PURPLE );
				}

			if ( true == transferCells[i].powerOnLeft )
				{
					transferCells[i].powerOnStartLeft -= 1.0f * interpolate;
					if ( transferCells[i].powerOnStartLeft < 0.0f )
						{
							transferCells[i].powerOnLeft = false;
						}


					switch ( transferCells[i].circuitTypeLeft )
						{
							case CIRCUIT_NORMAL:
							case CIRCUIT_NORMAL_1:
							case CIRCUIT_NORMAL_2:
								host_transSetCellColor ( i, POWER_YELLOW, COL_YELLOW );
								if ( transferCells[i].isReversedLeft )
									{
										host_transSetCellColor ( i, POWER_PURPLE, COL_PURPLE );
									}

								break;

							case CIRCUIT_THREEQUARTERS:
							case CIRCUIT_HALF:
							case CIRCUIT_QUARTER:
								break;

							case CIRCUIT_SPLIT_HALF:

								//printf("CIRCUIT_SPLIT_HALF [ %i ]\n", transferCells[i].circuitTypeLeft);
								break;

							case CIRCUIT_SPLIT_TWO_INTO_ONE:

								if ( transferCells[i + 1].circuitTypeLeft == CIRCUIT_TWO_INTO_ONE ) // One below is the short circuit
									{
										if ( ( transferCells[i].powerOnLeft == true ) && ( transferCells[i + 2].powerOnLeft == true ) ) // Both legs active
											{
												host_transSetCellColor ( i + 1, POWER_YELLOW, COL_YELLOW );
											}
									}
								/*
															else if (transferCells[i - 1].circuitTypeLeft == CIRCUIT_TWO_INTO_ONE)   // One above is the short circuit
																{
																	if ((transferCells[i].powerOnLeft == true) && (transferCells[i - 2].powerOnLeft == true)) // Both legs active
																		{
																			host_transSetCellColor(i + 1, POWER_YELLOW, COL_YELLOW);
																			numActiveLeft++;
																		}
																}
								 */
								break;

							case CIRCUIT_TWO_INTO_ONE:
								break;

							case CIRCUIT_ONE_INTO_TWO:
								host_transSetCellColor ( i - 1, POWER_YELLOW, COL_YELLOW );
								host_transSetCellColor ( i + 1, POWER_YELLOW, COL_YELLOW );
								break;
						}

				}

			if ( true == transferCells[i].powerOnRight )
				{
					transferCells[i].powerOnStartRight -= 1.0f * interpolate;
					if ( transferCells[i].powerOnStartRight < 0.0f )
						{
							transferCells[i].powerOnRight = false;
						}


					switch ( transferCells[i].circuitTypeRight )
						{
							case CIRCUIT_NORMAL:
							case CIRCUIT_NORMAL_1:
							case CIRCUIT_NORMAL_2:
								host_transSetCellColor ( i, POWER_PURPLE, COL_PURPLE );
								if ( transferCells[i].isReversedRight )
									{
										host_transSetCellColor ( i, POWER_YELLOW, COL_YELLOW );
									}


								break;

							case CIRCUIT_THREEQUARTERS:
							case CIRCUIT_HALF:
							case CIRCUIT_QUARTER:
								break;

							case CIRCUIT_SPLIT_HALF:

								//                            printf("CIRCUIT_SPLIT_HALF [ %i ]\n", transferCells[i].circuitTypeLeft);
								break;

							case CIRCUIT_SPLIT_TWO_INTO_ONE:

								if ( transferCells[i + 1].circuitTypeRight == CIRCUIT_TWO_INTO_ONE ) // One below is the short circuit
									{
										if ( ( transferCells[i].powerOnRight == true ) && ( transferCells[i + 2].powerOnRight == true ) ) // Both legs active
											{
												host_transSetCellColor ( i + 1, POWER_PURPLE, COL_PURPLE );
											}
									}
								/*
															if (transferCells[i - 1].circuitTypeRight == CIRCUIT_TWO_INTO_ONE)   // One above is the short circuit
																{
																	if ((transferCells[i].powerOnRight == true) && (transferCells[i - 2].powerOnRight == true)) // Both legs active
																		{
																			host_transSetCellColor(i + 1, POWER_PURPLE, COL_PURPLE);
																			numActiveRight++;
																		}
																}
								 */
								break;

							case CIRCUIT_TWO_INTO_ONE:
								break;

							case CIRCUIT_ONE_INTO_TWO:
								host_transSetCellColor ( i - 1, POWER_PURPLE, COL_PURPLE );
								host_transSetCellColor ( i + 1, POWER_PURPLE, COL_PURPLE );
								break;
						}

				}

			//
			// Flash cell if both sides are powering the same cell
			//
			if ( ( true == transferCells[i].powerOnLeft ) && ( true == transferCells[i].powerOnRight ) )
				{
					//                    host_transSetCellColor(i, POWER_DISPUTE, trn_getRandColor(), trn_getRandColor(), trn_getRandColor(), trn_getRandColor());

					switch ( transferCells[i].circuitTypeLeft )
						{
							case CIRCUIT_THREEQUARTERS:
							case CIRCUIT_HALF:
							case CIRCUIT_QUARTER:
								break;
						}

					switch ( transferCells[i].circuitTypeRight )
						{
							case CIRCUIT_THREEQUARTERS:
							case CIRCUIT_HALF:
							case CIRCUIT_QUARTER:
								break;
						}

					switch ( transferCells[i].circuitTypeRight )
						{
							case CIRCUIT_NORMAL:
							case CIRCUIT_NORMAL_1:
							case CIRCUIT_NORMAL_2:
								switch ( transferCells[i].circuitTypeLeft )
									{
										case CIRCUIT_NORMAL:
										case CIRCUIT_NORMAL_1:
										case CIRCUIT_NORMAL_2:
											host_transSetCellColor ( i, POWER_DISPUTE, trn_getRandColor(), trn_getRandColor(), trn_getRandColor(), trn_getRandColor() );
											break;

										case CIRCUIT_SPLIT_TWO_INTO_ONE:
											if ( transferCells[i + 1].circuitTypeLeft == CIRCUIT_TWO_INTO_ONE ) // One below is the short circuit
												if ( ( transferCells[i].powerOnLeft == true ) && ( transferCells[i + 2].powerOnLeft == true ) ) // Both legs active
													host_transSetCellColor ( i, POWER_DISPUTE, trn_getRandColor(), trn_getRandColor(), trn_getRandColor(), trn_getRandColor() );
											break;

									}
								break;
						}
				}

			if ( transferCells[i].currentColor == POWER_YELLOW )
				numActiveLeft++;

			if ( transferCells[i].currentColor == POWER_PURPLE )
				numActiveRight++;
		}

	if ( numActiveLeft > numActiveRight )
		currentWinner = al_map_rgba_f ( COL_YELLOW );

	if ( numActiveRight > numActiveLeft )
		currentWinner = al_map_rgba_f ( COL_PURPLE );

	if ( numActiveLeft == numActiveRight )
		currentWinner = al_map_rgba_f ( trn_getRandColor(), trn_getRandColor(), trn_getRandColor(), trn_getRandColor() );
}

//------------------------------------------------------------
//
// Transfer into new droid and copy attributes across
void trn_copyDroidAttributes()
//------------------------------------------------------------
{
	gam_destroyDroid ( currentLevel, droidTransferedIntoIndex );
	gam_setupPlayerValues ( droidTypeToTransferInto );
	//
	// TODO : Need to copy physics properties across as well or recreate with new values
}

//------------------------------------------------------------
//
// Setup the squares with default values
void trn_setupSquares()
//------------------------------------------------------------
{
	if ( false == sys_executeScriptFunction ( "as_setupTransferCellValues", "" ) )
		{
			con_print ( true, true, "Error: as_setupTransferCellValues: Script failed to init cells." );
			return;
		}
}

//------------------------------------------------------------
//
// Setup number of tokens

void trn_setupTokens()
//------------------------------------------------------------
{
	numDroidTokens = dataBaseEntry[droidTypeToTransferInto].tokenCount;
	numPlayerTokens = dataBaseEntry[playerDroidTypeImage].tokenCount;
	playerBlockPos = TOKEN_POS_HIDE;
	droidBlockPos = TOKEN_POS_HIDE;
}

// ----------------------------------------------------------------------------
//
// Enter into transfer mode - transferToDroid is index into current level
//
// Run script
// Stop any sound effects
// start transfer sound

void trn_startTransferMode ( int transferToDroid )
// ----------------------------------------------------------------------------
{
	gam_setHUDState ( HUD_STATE_TRANSFER );
	sys_stopPlayerMovement();
	//
	// Set timer for transfer screens
	delayTransferIntro = TRANSFER_DELAY;
	//
	// No longer in transfer mode when finished
	inTransferMode = false;
	//    playerDroid.inCollisionWithDroid = false;

	droidTypeToTransferInto = shipLevel[currentLevel].droid[transferToDroid].droidType;
	droidTransferedIntoIndex = transferToDroid;

	trn_setupSquares();
	trn_setupTokens();
}

// ----------------------------------------------------------------------------
//
//Process timings for transfer screen
void trn_processTransferScreen ( float interpolate )
// ----------------------------------------------------------------------------
{
	switch ( currentMode )
		{
			case MODE_TRANSFER_INTRO:
				gam_setHUDState ( HUD_STATE_CAPTURED );
				delayTransferIntro -= TRANSFER_SPEED * interpolate;
				if ( delayTransferIntro < 0.0 )
					{
						delayTransferIntro = TRANSFER_DELAY;
						sys_changeMode ( MODE_TRANSFER_INTRO_1, true );
						sys_playSound ( SND_TRANSFER_STAGE_1, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
						gam_loadDroidModel ( playerDroidTypeImage - 2 );
					}
				break;

			case MODE_TRANSFER_INTRO_1:
				gam_setHUDState ( HUD_STATE_CAPTURED );
				delayTransferIntro -= TRANSFER_SPEED * interpolate;
				//
				// Play sound, move to next stage when finished
				if ( ( false == sys_isSoundPlaying ( SND_TRANSFER_STAGE_1 ) ) && ( true == playSounds ) )
					delayTransferIntro = -1;

				if ( delayTransferIntro < 0.0 )
					{
						gam_loadDroidModel ( droidTypeToTransferInto );
						delayTransferIntro = TRANSFER_DELAY;
						sys_changeMode ( MODE_TRANSFER_INTRO_2, false );
						sys_stopSound ( SND_TRANSFER_STAGE_1 );
						sys_playSound ( SND_TRANSFER_STAGE_2, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
					}
				break;

			case MODE_TRANSFER_INTRO_2:
				gam_setHUDState ( HUD_STATE_CAPTURED );
				delayTransferIntro -= TRANSFER_SPEED * interpolate;
				//
				// Play sound, move to next stage when finished
				if ( ( false == sys_isSoundPlaying ( SND_TRANSFER_STAGE_2 ) ) && ( true == playSounds ) )
					delayTransferIntro = -1;

				if ( delayTransferIntro < 0.0 )
					{
						sys_stopSound ( SND_TRANSFER_STAGE_2 );
						delayTransferIntro = TRANSFER_DELAY;
						sys_changeMode ( MODE_TRANSFER_START, false );
					}
				break;

			case MODE_TRANSFER_START:
				gam_setHUDState ( HUD_STATE_CAPTURED );
				sys_changeMode ( MODE_TRANSFER_SELECT_SIDE, false );

				playerOnSide = LEFT_SIDE;
				enemyTransferThinkCount = 0.0f;
				enemyTransferThinkDelay = 0.0f;
				foundACircuit = false;
				delayTransferIntro = TRANSFER_DELAY;
				selectSideCounter = TRANSFER_COUNTER;
				sys_playSound ( SND_TRANSFER_START, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
				break;

			case MODE_TRANSFER_SELECT_SIDE: // Countdown to choose side

				gam_setHUDState ( HUD_STATE_SELECT_TIME );
				transferPlayCount = transferPlayCountDown;
				transferPlayCountLeft = transferPlayCountLeftDefault;
				delayTransferIntro -= TRANSFER_SPEED * interpolate;
				if ( delayTransferIntro < 0.0 )
					{
						selectSideCounter--;
						delayTransferIntro = TRANSFER_DELAY;
						if ( selectSideCounter < 0.0 )
							{
								sys_changeMode ( MODE_TRANSFER_SELECT, false );
								selectSideCounter = TRANSFER_COUNTER;
								delayTransferIntro = TRANSFER_DELAY;
								transferPlayCount = transferPlayCountDown;
								transferPlayCountLeft = transferPlayCountLeftDefault;
								trn_nextPlayerToken();
								trn_nextDroidToken();
							}
					}
				break;

			case MODE_TRANSFER_SELECT: // Play the transfer game

				gam_doTransferThink ( interpolate );
				trn_processCircuits ( interpolate );
				gam_setHUDState ( HUD_STATE_TIME_LEFT );
				transferPlayCount -= transferPlayCountDown * interpolate;
				if ( transferPlayCount < 0.0 )
					{
						transferPlayCountLeft--;
						transferPlayCount = transferPlayCountDown;
						if ( transferPlayCountLeft < 0 )
							{
								//
								// Got a deadlock - start again
								//
								if ( numActiveLeft == numActiveRight )
									{
										currentMode = MODE_TRANSFER_DEADLOCK;
										sys_playSound ( SND_TRANSFER_DEADLOCK, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
										delayTransferIntro = TRANSFER_DELAY * 3;
										return;
									}

								if ( playerOnSide == LEFT_SIDE )
									{
										if ( numActiveLeft > numActiveRight )
											{
												// Player has more active lines
												currentMode = MODE_TRANSFER_COPY;
												return;
											}
										else
											{
												// enemy droid has won
												if ( playerDroidTypeDBIndex == 0 )	// Currently a 001
													{
														playerCurrentHealth = -1;	// Kill player
														gam_doDamageToPlayer ( DAMAGE_EXPLOSION, -1 );
														currentMode = MODE_SHOWLEVEL;
														return;
													}
												else
													{
														// Lost - drop to 001
														gam_dropPlayerTo001();
														currentMode = MODE_SHOWLEVEL;
														return;
													}
											}
									}
								else	// Player is on right hand side
									{
										if ( numActiveRight > numActiveLeft )
											{
												currentMode = MODE_TRANSFER_COPY;
												return;
											}
										else
											{
												// enemy droid has won
												if ( playerDroidTypeDBIndex == 0 )	// Currently a 001
													{
														playerCurrentHealth = -1;	// Kill player
														gam_doDamageToPlayer ( DAMAGE_EXPLOSION, -1 );
														currentMode = MODE_SHOWLEVEL;
														return;
													}
												else
													{
														// Lost - drop to 001
														gam_dropPlayerTo001();
														currentMode = MODE_SHOWLEVEL;
														return;
													}
											}
									}
							}
					}
				break;

			case MODE_TRANSFER_COPY:
				gam_setHUDState ( HUD_STATE_CAPTURED );
				delayTransferIntro -= TRANSFER_SPEED * interpolate;
				if ( delayTransferIntro < 0.0 )
					{
						delayTransferIntro = TRANSFER_DELAY;
						trn_copyDroidAttributes();
						currentMode = MODE_TRANSFER_FINISH;
					}
				break;

			case MODE_TRANSFER_DEADLOCK:
				gam_setHUDState(HUD_STATE_DEADLOCK);

				delayTransferIntro -= TRANSFER_SPEED * interpolate;
				//
				// Play sound, move to next stage when finished
				if ( ( false == sys_isSoundPlaying ( SND_TRANSFER_DEADLOCK ) ) && ( true == playSounds ) )
					delayTransferIntro = -1;

				if ( delayTransferIntro < 0.0 )
					{
						delayTransferIntro = TRANSFER_DELAY;
						sys_changeMode ( MODE_TRANSFER_START, false );
						trn_setupSquares();
						trn_setupTokens();
					}
				break;

			case MODE_TRANSFER_FINISH:
				currentMode = MODE_SHOWLEVEL;
				//			sys_changeMode(MODE_SHOWLEVEL, true);
				//            trn_exitTransfermode();
				break;
		}

	if ( true == sys_isSoundPlaying ( SND_GREEN_ALERT ) )
		sys_stopSound ( SND_GREEN_ALERT );

	if ( true == sys_isSoundPlaying ( SND_YELLOW_ALERT ) )
		sys_stopSound ( SND_YELLOW_ALERT );

	if ( true == sys_isSoundPlaying ( SND_RED_ALERT ) )
		sys_stopSound ( SND_RED_ALERT );
}

// ----------------------------------------------------------------------------
//
// show some text during the transfer screens

void trn_showTransferInfo ( int whichDroid )
// ----------------------------------------------------------------------------
{
	sys_printStringExt ( transferTextPosX, transferTextPosY + ( transferTextHeight * 0 ), "%s", dataBaseEntry[whichDroid].description.c_str() );
	sys_printStringExt ( transferTextPosX, transferTextPosY + ( transferTextHeight * 1 ), "Type : %s", dataBaseEntry[whichDroid].className.c_str() );
	sys_printStringExt ( transferTextPosX, transferTextPosY + ( transferTextHeight * 2 ), "Drive : %s", dataBaseEntry[whichDroid].drive.c_str() );
	sys_printStringExt ( transferTextPosX, transferTextPosY + ( transferTextHeight * 3 ), "Brain : %s", dataBaseEntry[whichDroid].brain.c_str() );
	sys_printStringExt ( transferTextPosX, transferTextPosY + ( transferTextHeight * 4 ), "Weapon : %s", dataBaseEntry[whichDroid].weapon.c_str() );
}

// ----------------------------------------------------------------------------
//
// Draw the transfer screen

void trn_drawTransferScreen()
// ----------------------------------------------------------------------------
{
	cpVect screenPos;

	screenPos.x = transferDroidPosX;
	screenPos.y = transferDroidPosY;

	switch ( currentMode )
		{
			case MODE_TRANSFER_INTRO:
				sys_printString ( 70.0f, 110.0f, gui_getString ( "transferOne" ) );
				break;

			case MODE_TRANSFER_INTRO_1:
				tran_drawTransferCells ( true );
				sys_printString ( 70.0f, 110.0f, gui_getString ( "transferTwo" ) );

				gam_drawSprite ( 0, dbImage, screenPos, -1.0f, al_map_rgba_f ( 1.0f, 1.0f, 1.0f, 1.0f ) );

				trn_showTransferInfo ( playerDroidTypeImage - 2 );
				break;

			case MODE_TRANSFER_INTRO_2:
				tran_drawTransferCells ( true );
				sys_printString ( 70.0f, 110.0f, gui_getString ( "transferThree" ) );

				gam_drawSprite ( 0, dbImage, screenPos, -1.0f, al_map_rgba_f ( 1.0f, 1.0f, 1.0f, 1.0f ) );

				trn_showTransferInfo ( droidTypeToTransferInto );
				break;

			case MODE_TRANSFER_START:
				currentWinner = al_map_rgba_f ( COL_BLACK );
				break;

			case MODE_TRANSFER_SELECT_SIDE: // Countdown to choose side
				tran_drawTransferCells ( false );
				break;

			case MODE_TRANSFER_SELECT:
				tran_drawTransferCells ( false );
				break;

			case MODE_TRANSFER_COPY:
				sys_printString ( 70.0f, 110.0f, gui_getString ( "transferFour" ) );
				break;

			case MODE_TRANSFER_DEADLOCK:
				sys_printString ( 70.0f, 110.0f, gui_getString ( "transferFive" ) );
				break;

			case MODE_TRANSFER_FINISH:
				break;
		}
}


//----------------------------------------------------------
//
// These are the wrapper functions passed to the script
//
//----------------------------------------------------------

//----------------------------------------------------------
//
// Helper function to check index passed in is ok

bool trans_checkCellIndex ( int value )
//----------------------------------------------------------
{
	if ( ( value < 0 ) || ( value > NUMBER_CELLS ) )
		return false;
	else
		return true;
}

//----------------------------------------------------------
//
// Set the type of effect on this cell

void host_transSetEffectType ( int whichCell, int whichSide, int whichEffect )
//----------------------------------------------------------
{
	if ( false == trans_checkCellIndex ( whichCell ) )
		return;

	if ( LEFT_SIDE == whichSide )
		{
			if ( whichEffect == CIRCUIT_REVERSED )
				transferCells[whichCell].isReversedLeft = true;

			if ( whichEffect == CIRCUIT_REPEATER )
				transferCells[whichCell].isRepeaterLeft = true;
		}
	if ( RIGHT_SIDE == whichSide )
		{
			if ( whichEffect == CIRCUIT_REVERSED )
				transferCells[whichCell].isReversedRight = true;

			if ( whichEffect == CIRCUIT_REPEATER )
				transferCells[whichCell].isRepeaterRight = true;
		}
}

//----------------------------------------------------------
//
// Set the type of circuit in this cell

void host_tranSetCircuitType ( int whichCell, int whichSide, int typeSet )
//----------------------------------------------------------
{
	if ( false == trans_checkCellIndex ( whichCell ) )
		return;

	if ( LEFT_SIDE == whichSide )
		transferCells[whichCell].circuitTypeLeft = typeSet;

	if ( RIGHT_SIDE == whichSide )
		transferCells[whichCell].circuitTypeRight = typeSet;
}

//----------------------------------------------------------
//
// Setup default states for the cells

void host_transSetDefaultValues ( int whichCell )
//----------------------------------------------------------
{
	if ( false == trans_checkCellIndex ( whichCell ) )
		return;

	transferCells[whichCell].powerOnLeft = false;
	transferCells[whichCell].powerOnStartLeft = 0.0f;
	transferCells[whichCell].circuitTypeLeft = -1;
	transferCells[whichCell].isRepeaterLeft = false;
	transferCells[whichCell].isReversedLeft = false;
	transferCells[whichCell].reversedLeftActivated = false;
	transferCells[whichCell].repeaterLeftActivated = false;

	transferCells[whichCell].powerOnRight = false;
	transferCells[whichCell].powerOnStartRight = 0.0f;
	transferCells[whichCell].circuitTypeRight = -1;
	transferCells[whichCell].isRepeaterRight = false;
	transferCells[whichCell].isReversedRight = false;
	transferCells[whichCell].reversedRightActivated = false;
	transferCells[whichCell].repeaterRightActivated = false;
}

//----------------------------------------------------------
//
// Get the color of the passed in cell from passed in side
int host_getCellColor ( int whichCell, int whichSide )
//----------------------------------------------------------
{
	if ( false == trans_checkCellIndex ( whichCell ) )
		return -1;

	return transferCells[whichCell].currentColor;
}

//----------------------------------------------------------
//
// Pass current power status to script
bool host_circuitPowered ( int whichCell, int whichSide )
//----------------------------------------------------------
{
	if ( false == trans_checkCellIndex ( whichCell ) )
		return false;

	if ( LEFT_SIDE == whichSide )
		{
			return transferCells[whichCell].powerOnLeft;
		}
	else
		{
			return transferCells[whichCell].powerOnRight;
		}
}

//----------------------------------------------------------
//
// Get a random number capped at parameter

int host_getCappedRandomNum ( int cap )
//----------------------------------------------------------
{
	int returnValue;

	returnValue = sys_genRand_int32() % cap;
	if ( returnValue == 19 )
		returnValue = sys_genRand_int32() % cap;

//	printf("Random value [ %i ] Cap [ %i ]\n", returnValue, cap);
	return returnValue;
}

//----------------------------------------------------------
//
// Set current color

void host_transSetCellColor ( int whichCell, int colType, float red, float green, float blue, float alpha )
//----------------------------------------------------------
{
	if ( false == trans_checkCellIndex ( whichCell ) )
		{
			con_print ( true, true, "Error: host_transSetCellColor: Invalid cell index." );
			return;
		}

	transferCells[whichCell].color = al_map_rgba_f ( red, green, blue, alpha );
	transferCells[whichCell].currentColor = colType;
}

//----------------------------------------------------------
//
// Set the start position of a cell

void host_transSetCellPos ( int whichCell, int posX, int posY )
//----------------------------------------------------------
{
	if ( false == trans_checkCellIndex ( whichCell ) )
		{
			con_print ( true, true, "Error: host_transSetCellPos: Invalid cell index." );
			return;
		}

	transferCells[whichCell].startX = posX;
	transferCells[whichCell].startY = posY;

	//    con_print(true, "Cell [ %i ] PosY [ %i ] Height [ %i ]", whichCell, posY, squareHeight);
}

//----------------------------------------------------------
//
// Setup the transfer screen - GUI bits

bool trans_setupTransferGUI()
//----------------------------------------------------------
{
	// execute script to setup struct

	return true;
}

/*
// ----------------------------------------------------------------------------
// Time the enemy movement so it's not too fast
void trn_processEnemyMove()
// ----------------------------------------------------------------------------
{
	if (numDroidTokens < 0)
		return;

	sys_executeScriptFunction("as_processTransferAI", "");
}

 */
