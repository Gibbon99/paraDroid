
//--------------------------------------------------------
//
// Helper function - return true if circuit is a split type
int isCircuitSplit (int whichType)
//--------------------------------------------------------
{
    if ( (whichType == CIRCUIT_TWO_INTO_ONE) ||
            (whichType == CIRCUIT_ONE_INTO_TWO) ||
            (whichType == CIRCUIT_SPLIT_HALF) ||
            (whichType == CIRCUIT_SPLIT_TWO_INTO_ONE) )
        {
            return TRUE;
        }
    else
        return FALSE;
}

//------------------------------------------------------------
//
// Called to setup the transfer cell values
void setupTransferCellValues()
//------------------------------------------------------------
{
    int i = 0;
    int randNum = 0;
    int randEffect = 0;     // Will the circuit be reversed or repeater
    bool validCircuit;

    for (i = 0; i != NUM_CELLS; i++)
        {
            as_transSetDefaultValues (i);
//
// Left side
//
            randNum = as_getCappedRandomNum (NUM_CIRCUITS);
            randNum = randNum + 1;

            if ( (i == 0) || (i == NUM_CELLS - 1) )	// Can't have split on first or last slot
                {
                    if (TRUE == isCircuitSplit (randNum) )
                        leftSide[i] = CIRCUIT_NORMAL;
                    else
                        leftSide[i] = randNum;
                }
            else if (CIRCUIT_ONE_INTO_TWO == randNum)
                {
                    if (TRUE == isCircuitSplit (leftSide[i - 1]) )
                        leftSide[i] = CIRCUIT_NORMAL;
                    else
                        {
                            leftSide[i] = randNum;
                            leftSide[i - 1] = CIRCUIT_SPLIT_HALF;
                            i++;
                            leftSide[i] = CIRCUIT_SPLIT_HALF;
                        }
                }
            else if (CIRCUIT_TWO_INTO_ONE == randNum)
                {
                    if (TRUE == isCircuitSplit (leftSide[i - 1]) )
                        leftSide[i] = CIRCUIT_NORMAL;
                    else
                        {
                            leftSide[i] = randNum;
                            leftSide[i - 1] = CIRCUIT_SPLIT_TWO_INTO_ONE;
                            i++;
                            leftSide[i] = CIRCUIT_SPLIT_TWO_INTO_ONE;
                        }
                }
            else // Ok to use non split one here
                {
                    leftSide[i] = randNum;
                }
        }
//
// Right side
//

    for (i = 0; i != NUM_CELLS; i++)
        {
            as_transSetDefaultValues (i);

            randNum = as_getCappedRandomNum (NUM_CIRCUITS);
            randNum = randNum + 1;

            if ( (i == 0) || (i == NUM_CELLS - 1) )	// Can't have split on first or last slot
                {
                    if (TRUE == isCircuitSplit (randNum) )
                        rightSide[i] = CIRCUIT_NORMAL;
                    else
                        rightSide[i] = randNum;
                }
            else if (CIRCUIT_ONE_INTO_TWO == randNum)
                {
                    if (TRUE == isCircuitSplit (rightSide[i - 1]) )
                        rightSide[i] = CIRCUIT_NORMAL;
                    else
                        {
                            rightSide[i] = randNum;
                            rightSide[i - 1] = CIRCUIT_SPLIT_HALF;
                            i++;
                            rightSide[i] = CIRCUIT_SPLIT_HALF;
                        }
                }
            else if (CIRCUIT_TWO_INTO_ONE == randNum)
                {
                    if (TRUE == isCircuitSplit (rightSide[ i - 1]) )
                        rightSide[i] = CIRCUIT_NORMAL;
                    else
                        {
                            rightSide[i] = randNum;
                            rightSide[i - 1] = CIRCUIT_SPLIT_TWO_INTO_ONE;
                            i++;
                            rightSide[i] = CIRCUIT_SPLIT_TWO_INTO_ONE;
                        }
                }
            else // Ok to use non split one here
                {
                    rightSide[i] = randNum;
                }
        }
//
// Now copy the values to the host application
    for (i = 0; i != NUM_CELLS; i++)
        {
            randEffect = as_getCappedRandomNum (MAX_EFFECT);
            randEffect += 1;

            if (leftSide[i] == CIRCUIT_NORMAL)
                {
                    if (randEffect == CIRCUIT_REVERSED)
                        as_tranSetEffectType (i, LEFT_SIDE, CIRCUIT_REVERSED);
                    if (randEffect == CIRCUIT_REPEATER)
                        as_tranSetEffectType (i, LEFT_SIDE, CIRCUIT_REPEATER);
                }

            randEffect = as_getCappedRandomNum (MAX_EFFECT);
            randEffect += 1;

            if (rightSide[i] == CIRCUIT_NORMAL)
                {
                    if (randEffect == CIRCUIT_REVERSED)
                        as_tranSetEffectType (i, RIGHT_SIDE, CIRCUIT_REVERSED);
                    if (randEffect == CIRCUIT_REPEATER)
                        as_tranSetEffectType (i, RIGHT_SIDE, CIRCUIT_REPEATER);
                }

            as_tranSetCircuitType (i, LEFT_SIDE, leftSide[i]);
            as_tranSetCircuitType (i, RIGHT_SIDE, rightSide[i]);
        }
}

//------------------------------------------------------------
//
// Setup the default values for the middle squares
void setupDefaultCellValues()
//------------------------------------------------------------
{
    int i = 0;
    int startX = 320;
    int startY = 10;

    circuitTimeAlive = 5.0f;

    squareHeight = (winHeight * 0.025);
    squareWidth = (winWidth * 0.666);

    squareHeight = 16.0f;
    squareWidth = 32.0f;

    startX = (winWidth / 2) - (squareWidth / 2);
    startY = (winHeight - (NUM_CELLS * squareHeight) ) - 70;

    for (i = 0; i != NUM_CELLS; i++)
        {
            as_transSetDefaultValues (i);
            as_transSetCellPos (i, startX, startY + (i * squareHeight) );
            as_transSetCellColor (i, POWER_PURPLE, 0.54f, 0.27f, 0.68f, 1.0f);
        }

    for (i = 0; i < NUM_CELLS; i++)
        {
            as_transSetCellColor (i, POWER_YELLOW, 1.0f, 1.0f, 0.0f, 1.0f);
            i++;
        }
    setupTransferCellValues();
}
