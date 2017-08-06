#include "data/scripts/commonDefines.h"

int			PLAYER_TRANSFER = 0;
int			DROID_TRANSFER 	= 1;

int			MOVE_UP			= 0;
int			MOVE_DOWN		= 1;

int			enemyBlockDirection = -1;
bool		circuitFound = false;
int			nextCircuitToUse = 0;
int			mistakeChance;

array<int> leftSide (NUM_CELLS);
array<int> rightSide (NUM_CELLS);

// ----------------------------------------------------------------------------
//
// work out chance of doing something stupid - higher droids have less chance
int chanceOfMistake ( int droidType )
// ----------------------------------------------------------------------------
{
	int returnValue;

	returnValue = ( 24 + 5 ) - droidType;

//	returnValue = returnValue / 2;

	if ( returnValue == 0 )
		returnValue = 1;	// There is always a chance

	return returnValue;
}

//------------------------------------------------------------
//
// Find a suitable circuit to use
//
// TODO: Also only choose ones that need to be changed color - maybe only for high droids
int findSuitableCircuitToUse ( int whichSide )
//------------------------------------------------------------
{
	int i = 0;
	int circuitType = -1;

	for ( i = 0; i != NUM_CELLS; i++ )
		{
			//
			// Get the type of circuit we are looking at for this line
			// 
			if ( LEFT_SIDE == whichSide )	// YELLOW
				{
					if ( false == as_circuitNotPowered ( i, whichSide ) )
						{
							if ( droidToTransferInto > 2 )
								{
									// see what color
									if ( POWER_PURPLE == as_getCellColor ( i, whichSide ) )
										circuitType = leftSide[i];
								}
							else
								{
									circuitType = leftSide[i];
								}
						}
				}
			else	// RIGHT_SIDE - PURPLE
				{
					if ( false == as_circuitNotPowered ( i, whichSide ) )
						{
							if ( droidToTransferInto > 2 )
								{
									if ( POWER_YELLOW == as_getCellColor ( i, whichSide ) )
										circuitType = rightSide[i];
								}
							else
								{
									circuitType = rightSide[i];
								}
						}
				}

	//
	// Now decide how to handle it
	// 
			if ( circuitType != -1 )
				{
					switch ( circuitType )
						{
							case CIRCUIT_ONE_INTO_TWO:
								circuitFound = true;
								return i;
//                            break;

							case CIRCUIT_ONE_INTO_TWO_SHORT:
								circuitFound = true;
								return i;
//                            break;

							case CIRCUIT_NORMAL:
							case CIRCUIT_NORMAL_1:
							case CIRCUIT_NORMAL_2:
								if ( LEFT_SIDE == whichSide)
								{
									if (POWER_YELLOW == as_getCellColor (i, whichSide))
									{
										mistakeChance = as_getCappedRandomNum ( 100 );
										if (mistakeChance < chanceOfMistake (droidToTransferInto))
										{
											circuitFound = true;
											return i;
										}
									}
									if (POWER_PURPLE == as_getCellColor (i, whichSide))
									{
										circuitFound = true;
										return i;
									}
								}
								
								if ( RIGHT_SIDE == whichSide)
								{
									if (POWER_PURPLE == as_getCellColor (i, whichSide))
									{
										mistakeChance = as_getCappedRandomNum ( 100 );
										if (mistakeChance < chanceOfMistake (droidToTransferInto))
										{
											circuitFound = true;
											return i;
										}
									}
									if (POWER_YELLOW == as_getCellColor (i, whichSide))
									{
										circuitFound = true;
										return i;
									}
								}
//                            break;

							case CIRCUIT_THREEQUARTERS:
							case CIRCUIT_HALF:
							case CIRCUIT_QUARTER:
								mistakeChance = as_getCappedRandomNum ( 100 );
								if ( mistakeChance < chanceOfMistake ( droidToTransferInto ) )
									{
										circuitFound = true;
										return i;
									}
								break;

							case CIRCUIT_TWO_INTO_ONE:
								if ( LEFT_SIDE == whichSide )	// YELLOW
									{
										if ( POWER_PURPLE == as_getCellColor ( i, whichSide ) )
										{
											// If the middle leg is not powered then chose the one above it
											if ( true == as_circuitNotPowered ( i - 1, whichSide ) )
											{
												circuitFound = true;
												return i - 1;	// Choose the leg above the middle one
											}
											// if the top leg is powered, then chose the bottom one
											if ( false == as_circuitNotPowered (i + 1, whichSide))
											{
												circuitFound = true;
												return i + 1;	// Choose the leg below the middle one
											}
											
										}
									}
									
								if ( RIGHT_SIDE == whichSide )	// PURPLE
									{
										if ( POWER_YELLOW == as_getCellColor ( i, whichSide ) )
										{
											// If the middle leg is not powered then chose the one above it
											if ( true == as_circuitNotPowered ( i - 1, whichSide ) )
											{
												circuitFound = true;
												return i - 1;	// Choose the leg above the middle one
											}
											// if the top leg is powered, then chose the bottom one
											if ( false == as_circuitNotPowered (i + 1, whichSide))
											{
												circuitFound = true;
												return i + 1;	// Choose the leg below the middle one
											}
											
										}
									}
								break;

							case CIRCUIT_SPLIT_HALF:
							case CIRCUIT_SPLIT_TWO_INTO_ONE:
								mistakeChance = as_getCappedRandomNum ( 100 );
								if ( mistakeChance < chanceOfMistake ( droidToTransferInto ) )
									{
										circuitFound = true;
										return i;
									}
								break;
						}
				}
		}

	return -1;      // Didn't find a circuit to use
}

// ----------------------------------------------------------------------------
//
// Move the enemy token based on passed on direction
void moveEnemyToken ( int direction )
// ----------------------------------------------------------------------------
{
	if ( MOVE_UP == direction )
		{
			droidBlockPos--;
			if ( droidBlockPos < 0 )
				droidBlockPos = NUM_CELLS - 1;	// wrap around to bottom
		}

	if ( MOVE_DOWN == direction )
		{
			droidBlockPos++;
			if ( droidBlockPos > NUM_CELLS - 1 )
				droidBlockPos = 0;					// wrap back to top
		}
	sys_playSoundFromScript ( SND_KEYPRESS_GOOD, SND_PAN_CENTER, false );
}

// ----------------------------------------------------------------------------
//
// Move the circuit we have picked
void moveToCircuit()
// ----------------------------------------------------------------------------
{
	//
	// Didn't find one to use - so just wander
	if ( -1 == nextCircuitToUse )
		{
			moveEnemyToken ( MOVE_UP );
			return;
		}

	if ( droidBlockPos == nextCircuitToUse )
		{
			trn_placeToken ( droidBlockPos, droidOnSide, DROID_TRANSFER );
			circuitFound = false;
			droidBlockPos = 0;
		}
	else
		{
			if ( enemyBlockDirection == MOVE_UP )
				{
					moveEnemyToken ( MOVE_UP );
				}
			else
				{
					moveEnemyToken ( MOVE_DOWN );
				}
		}
}

//-------------------------------------------------------------
//
// Process enemy transfer tokens
void as_processTransferAI()
//-------------------------------------------------------------
{
	if ( numDroidTokens < 0 )
		return;

	if ( false == circuitFound )
		{
			nextCircuitToUse = findSuitableCircuitToUse ( droidOnSide );
			if ( nextCircuitToUse != -1 )
				{
					circuitFound = true;
					if ( nextCircuitToUse > 7 )
						enemyBlockDirection = MOVE_UP;
					else
						enemyBlockDirection = MOVE_DOWN;
				}
		}
	else
		{
			moveToCircuit();
		}
}
