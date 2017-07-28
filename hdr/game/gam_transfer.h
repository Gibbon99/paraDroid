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

#define PLAYER_TRANSFER					0
#define DROID_TRANSFER					1

#define CIRCUIT_REVERSED                1
#define CIRCUIT_REPEATER                2

#define CIRCUIT_NORMAL				    1
#define CIRCUIT_THREEQUARTERS		    2
#define CIRCUIT_HALF				    3
#define CIRCUIT_QUARTER				    4
#define CIRCUIT_TWO_INTO_ONE			5
#define CIRCUIT_ONE_INTO_TWO			6
#define CIRCUIT_NORMAL_1    			7
#define CIRCUIT_NORMAL_2              	8

#define CIRCUIT_SPLIT_HALF		        16
#define CIRCUIT_ONE_INTO_TWO_SHORT      17
#define CIRCUIT_SPLIT_TWO_INTO_ONE      18

#define NUMBER_CELLS		16	// Need to change in script as well - What about dynamic circuits based on level of droid ??
#define POWER_ON_TIME		20

#define LEFT_SIDE       	1
#define RIGHT_SIDE      	2

#define TRANSFER_DELAY      2.0f
#define TRANSFER_SPEED      1.0f

#define TOKEN_POS_LAUNCH	-1
#define TOKEN_POS_HIDE		-2

#define	MAX_NUM_BLOBS		12
#define BLOB_DISTANCE		20

extern int      	playerOnSide;
extern int			droidOnSide;
extern int      	squareWidth;
extern int      	squareHeight;

//extern int transferWithSprite;	// which sprite are we transferring to
extern int      	droidTypeToTransferInto;
extern int      	numPlayerTokens;
extern int      	numDroidTokens;
extern int      	nextCircuitToUse;
extern int      	playerBlockPos;
extern int      	droidBlockPos;
extern int      	selectSideCounter;
extern float    	delayTransferIntro;
extern int 			droidTransferedIntoIndex;

extern int			transferDroidPosX;
extern int			transferDroidPosY;
extern int			transferTextPosX;
extern int			transferTextHeight;
extern int			transferTextPosY;
extern int			transferTitlePosX;
extern int 			transferTitlePosY;

extern bool 		circuitFound;
extern float		circuitTimeAlive;

extern ALLEGRO_COLOR    currentWinner;

extern int          numActiveLeft;
extern int          numActiveRight;
extern int          numActiveDispute;

extern float	transferPlayCountDown;
extern int		transferPlayCountLeft;
extern int		transferPlayCountLeftDefault;

struct _blob
{
	int position;
	bool active;
};

struct _transferCells
	{
	int		startX;
	int		startY;
	int		currentColor;
	ALLEGRO_COLOR	color;

	bool	powerOnLeft;
	float	powerOnStartLeft;
	int		circuitTypeLeft;

	bool	powerOnRight;
	float	powerOnStartRight;
	int		circuitTypeRight;

	bool	repeaterLeftActivated;
	bool	repeaterRightActivated;

	bool    reversedLeftActivated;
	bool    reversedRightActivated;

    bool    isRepeaterLeft;
    bool    isRepeaterRight;

	bool    isReversedLeft;
	bool    isReversedRight;
	
	int		numberBlobsLeft;
	int		numberBlobsRight;
	
	int		lengthLeft;
	int		lengthRight;
	
	_blob		blobLeft[MAX_NUM_BLOBS];
	_blob		blobRight[MAX_NUM_BLOBS];
	};

extern _transferCells	transferCells[];

//-------------------------------------------------------------
//
// Host based functions called from the script
//
//-------------------------------------------------------------

// Set the type of effect on this cell
void host_transSetEffectType(int whichCell, int whichSide, int whichEffect);

// Setup default states for the cells
void host_transSetDefaultValues(int whichCell);

// Set the type of circuit in this cell
void host_tranSetCircuitType(int whichCell, int whichSide, int typeSet);

// Set current color
void host_transSetCellColor(int whichCell, int colType, float red, float green, float blue, float alpha);

// Set the start position of a cell
void host_transSetCellPos(int whichCell, int posX, int posY);

// Get a random number capped at parameter
int host_getCappedRandomNum(int cap);

// Pass current power status to script
bool host_circuitPowered(int whichCell, int whichSide);

// Activate a circuit
void trn_activateCircuit(int whichCell, int whichSide);

// Setup number of tokens
void trn_setupTokens();


// Place the token on the currently selected circuit
void trn_placeToken(int whichCell, int whichSide, int playerOrDroid);

// Put a player token onto a circuit
void trn_moveToken(int whichDirection);

//Process timings for transfer screen
void trn_processTransferScreen();

// Process the transfer circuits
void trn_processCircuits(float interpolate);

// Time the enemy movement so it's not too fast
void trn_processEnemyMove();

//Next player token - move ready to use and decrement count
void trn_nextPlayerToken();

// Get the length of the circuit
int tran_getCircuitLength (int whichSide, int whichCell, int type );

//-------------------------------------------------------------
//
// Functions in sys_transferDroidAI.cpp
//
//-------------------------------------------------------------

// Do think cycle for droid doing transfer
void gam_doTransferThink(float thinkInterval);

//Next droid token - move ready to use and decrement count
void trn_nextDroidToken();

//-------------------------------------------------------------
//
// Functions in sys_transferDraw.cpp
//
//-------------------------------------------------------------

// Draw the transfer graphics
void tran_drawTransferCells(bool justBackground);

// Debug the circuits
void tran_debugCircuits();

// Draw a circuit block
// posX is the location along the whichCell circuit
void tran_drawCircuitBlock(int blockDirection, int whichCell, int posX, float red, float green, float blue, float alpha);

// Draw the tokens for each side
void tran_drawTokens();


// Enter into transfer mode
void trn_startTransferMode(int transferToDroid);

// Exit transfer mode
void trn_exitTransfermode();

// Draw the transfer screen
void trn_processTransferScreen(float interpolate);

// move to render

// Draw the transfer screen
void trn_drawTransferScreen();

// Get the color of the passed in cell from passed in side
int host_getCellColor(int whichCell, int whichSide);
