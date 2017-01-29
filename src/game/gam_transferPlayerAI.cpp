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

//----------------------------------------------------------------
//
//Next player token - move ready to use and decrement count
void trn_nextPlayerToken()
//----------------------------------------------------------------
{
	if (numPlayerTokens == 0)
	{
		numPlayerTokens--;
		return;
	}


    playerBlockPos = TOKEN_POS_LAUNCH;
    numPlayerTokens--;
//    if (numPlayerTokens == 0)
//        playerBlockPos = TOKEN_POS_HIDE;
}

//----------------------------------------------------------------
//
// Activate a circuit
void trn_activateCircuit(int whichCell, int whichSide)
//----------------------------------------------------------------
{
    if (numPlayerTokens < 0)
        return;

    switch (whichSide)
        {
        case LEFT_SIDE:
            transferCells[whichCell].powerOnLeft = true;
            transferCells[whichCell].powerOnStartLeft = circuitTimeAlive;
            sys_playSound(SND_KEYPRESS_GOOD, SND_PAN_LEFT, ALLEGRO_PLAYMODE_ONCE);

            if (true == transferCells[whichCell].isRepeaterLeft)
				transferCells[whichCell].repeaterLeftActivated = true;

			if (true == transferCells[whichCell].isReversedLeft)
				transferCells[whichCell].reversedLeftActivated = true;
            break;

        case RIGHT_SIDE:
            transferCells[whichCell].powerOnRight = true;
            transferCells[whichCell].powerOnStartRight = circuitTimeAlive;
            sys_playSound(SND_KEYPRESS_GOOD, SND_PAN_RIGHT, ALLEGRO_PLAYMODE_ONCE);

            if (true == transferCells[whichCell].isRepeaterRight)
				transferCells[whichCell].repeaterRightActivated = true;

			if (true == transferCells[whichCell].isReversedRight)
				transferCells[whichCell].reversedRightActivated = true;
            break;
        }
}

//----------------------------------------------------------------
//
// Place the token on the currently selected circuit
void trn_placeToken(int whichCell, int whichSide, int playerOrDroid)
//----------------------------------------------------------------
{
    int currentCircuitType;

    if (numPlayerTokens < 0)
        return;

    switch (whichSide)
        {
        case LEFT_SIDE:
            currentCircuitType = transferCells[whichCell].circuitTypeLeft;
            break;

        case RIGHT_SIDE:
            currentCircuitType = transferCells[whichCell].circuitTypeRight;
            break;
        }

    debugCurrentCircuitType = currentCircuitType;
//
// Can't power an already powered circuit
    if (true == host_circuitPowered(whichCell, whichSide))
        {
            sys_playSound(SND_KEYPRESS_BAD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE);
            return;
        }
//
// Only activate a valid circuit type
    switch (currentCircuitType)
        {
        case CIRCUIT_NORMAL:
        case CIRCUIT_NORMAL_1:
        case CIRCUIT_NORMAL_2:
        case CIRCUIT_SPLIT_TWO_INTO_ONE:
        case CIRCUIT_ONE_INTO_TWO:
        case CIRCUIT_THREEQUARTERS:
        case CIRCUIT_HALF:
        case CIRCUIT_QUARTER:
            trn_activateCircuit(whichCell, whichSide);
            if (PLAYER_TRANSFER == playerOrDroid)
                trn_nextPlayerToken();
            else
				trn_nextDroidToken();
			sys_playSound(SND_TRANSFER_SELECT, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE);
            break;

        case CIRCUIT_TWO_INTO_ONE:
        case CIRCUIT_SPLIT_HALF:
            sys_playSound(SND_KEYPRESS_BAD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE);
            break;
        }
}

// Only check for blank circuit type????

//----------------------------------------------------------------
//
// Handle moving the player controlled token
// Put a player token onto a circuit
void trn_moveToken(int whichDirection)
//----------------------------------------------------------------
{
    switch (whichDirection)
        {
        case MOVE_DOWN:
            playerBlockPos++;
            if (playerBlockPos > NUMBER_CELLS - 1)
                playerBlockPos = 0;
            break;

        case MOVE_UP:
            playerBlockPos--;
            if (playerBlockPos < 0)
                playerBlockPos = NUMBER_CELLS - 1;
            break;
        }
    sys_playSound(SND_KEYPRESS_GOOD, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE);
}
