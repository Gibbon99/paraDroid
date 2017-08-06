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

_scoreTable			scoreTable[SCORE_NUM_ENTRIES];
int					scoreTableVersion;
char				*workingBuffer;
string				newInitials;

//-----------------------------------------------------------------------------
//
// Populate scoreTable with initial data
void gam_setupScoreTable()
//-----------------------------------------------------------------------------
{
	for ( int i = 0; i != SCORE_NUM_ENTRIES; i++ )
		{
			strcpy ( scoreTable[i].initials, ( char * ) "DAB" );
			scoreTable[i].score = 20 - ( 2 * ( i + 1 ) );

			printf ( "Score [ %i ] - [ %i ]\n", i, scoreTable[i].score );
		}
}

//-----------------------------------------------------------------------------
//
// Check for error - if found print message, close the file pointer and return false value
bool gam_scoreTableError ( ALLEGRO_FILE *fp, size_t returnValue, size_t checkValue, string message )
//-----------------------------------------------------------------------------
{
	if ( returnValue != checkValue )
		{
			con_print ( true, false, "ERROR: %s", message );
			al_fclose ( fp );
			return false;
		}
	return true;
}
//-----------------------------------------------------------------------------
//
// Write the scoretable out to a binary file
bool gam_saveScoreTable ( char *fileName )
//-----------------------------------------------------------------------------
{
	ALLEGRO_FILE		*fp;
	size_t				returnValue;

//	gam_setupScoreTable();

	scoreTableVersion = SCORE_TABLE_VERSION;

	//
	// Open in WRITE mode
	//
	fp = al_fopen ( fileName, "wb" );
	if ( NULL == fp )
		{
			con_print ( true, false, "ERROR: Unable to open score table for writing." );
			gam_setupScoreTable();
			return false;
		}
	//
	// Store the file version
	//
	returnValue = al_fwrite ( fp, &scoreTableVersion, sizeof ( scoreTableVersion ) );
	if ( false == gam_scoreTableError ( fp, returnValue, sizeof ( scoreTableVersion ), "Could not write scoreTableVersion" ) )
		return false;
	//
	// Write each row
	//
	for ( int i = 0; i != SCORE_NUM_ENTRIES; i++ )
		{
			returnValue = al_fwrite ( fp, &scoreTable[i].initials, sizeof ( scoreTable[i].initials ) );
			if ( false == gam_scoreTableError ( fp, returnValue, sizeof ( scoreTable[i].initials ), "Could not write score initials" ) )
				return false;

			returnValue = al_fwrite ( fp, &scoreTable[i].score, sizeof ( scoreTable[i].score ) );
			if ( false == gam_scoreTableError ( fp, returnValue, sizeof ( scoreTable[i].score ), "Could not write score score" ) )
				return false;
		}

	al_fclose ( fp );
	return true;
}

//-----------------------------------------------------------------------------
// Read the scoretable into the structure
bool gam_readScoreTable ( char *fileName )
//-----------------------------------------------------------------------------
{
	ALLEGRO_FILE 	*fp;
	size_t			returnValue;

	//
	// Open in READ-ONLY mode
	//
	fp = al_fopen ( fileName, "r" );
	if ( NULL == fp )
		{
			printf ( "Error opening file for reading [ %s ]\n", fileName );
			gam_setupScoreTable();
			return false;
		}
	//
	// Check this version is ok to use
	//
	al_fread ( fp, &scoreTableVersion, sizeof ( scoreTableVersion ) );
	if ( scoreTableVersion != SCORE_TABLE_VERSION )
		{
			printf ( "SCORE_TABLE_VERSION wrong. Failed to read file.\n" );
			return false;
		}
	//
	// Read number variables
	//
	for ( int i = 0; i != SCORE_NUM_ENTRIES; i++ )
		{
			returnValue = al_fread ( fp, ( void * ) &scoreTable[i].initials, sizeof ( scoreTable[i].initials ) );
			if ( false == gam_scoreTableError ( fp, returnValue, sizeof ( scoreTable[i].initials ), "Could not read score initials" ) )
				return false;

			returnValue = al_fread ( fp, ( void * ) &scoreTable[i].score, sizeof ( scoreTable[i].score ) );
			if ( false == gam_scoreTableError ( fp, returnValue, sizeof ( scoreTable[i].score ), "Could not write score score" ) )
				return false;
		}

	al_fclose ( fp );

	return true;
}

//-----------------------------------------------------------------------------
//
// Return a string containing value from the score table
string gam_getScoreElement ( int whichElement, int tableIndex )
//-----------------------------------------------------------------------------
{
	if ( ( tableIndex < 0 ) || ( tableIndex >= SCORE_NUM_ENTRIES ) )
		return "Error";

	switch ( whichElement )
		{
			case SCORE_INITIALS:
				return scoreTable[tableIndex].initials;
				break;

			case SCORE_SCORE:
				return gui_IntToString ( scoreTable[tableIndex].score );
				break;
		}
	return "Error";
}


//---------------------------------------------------------
//
// Process score and set current Alert level
void gam_processScore ( float interpolate )
//---------------------------------------------------------
{
	static float scoreDelayCount = 0.0f;

	switch ( currentAlertLevel )
		{
			case ALERT_GREEN_TILE:
				scoreDelayCount -= scoreDelay * interpolate;
				break;

			case ALERT_YELLOW_TILE:
				scoreDelayCount -= ( scoreDelay * 2 ) * interpolate;
				break;

			case ALERT_RED_TILE:
				scoreDelayCount -= ( scoreDelay * 3 ) * interpolate;
				break;
		}

	if ( scoreDelayCount < 0 )
		{
			scoreDelayCount = scoreDelayValue;

			if ( currentScore > 0 )
				{
					currentScore--;
					displayScore++;
				}
		}

	sprintf ( displayScoreString, "%7i", displayScore );

	currentAlertLevel = ALERT_GREEN_TILE;

	if ( ( currentScore - displayScore ) > yellowAlertLevel )
		currentAlertLevel = ALERT_YELLOW_TILE;

	if ( ( currentScore - displayScore ) > redAlertLevel )
		currentAlertLevel = ALERT_RED_TILE;
}

//---------------------------------------------------------
//
// Add passed in value to currentScore
void gam_addToScore ( int scoreValue )
//---------------------------------------------------------
{
	currentScore += scoreValue;
}

//-----------------------------------------------------------------------------
//
// Reset the score values
void gam_resetScore()
//-----------------------------------------------------------------------------
{
	//
	// Reset score
	//
	currentScore = 0;
	displayScore = 0;
}

//-----------------------------------------------------------------------------
//
// Pass in new score and return index of the table row that needs to be updated
int gam_addNewScore()
//-----------------------------------------------------------------------------
{
	int newScore;

	newScore = currentScore + displayScore;	// Add on current bonuses

	con_print ( true, false, "Checking score [ %i ]", newScore );

	for ( int i = 0; i != SCORE_NUM_ENTRIES; i++ )
		{
			con_print ( true, false, "[ %i ] : [ %i ] is higher than [ %i ]", i, newScore, scoreTable[i].score );
			if ( newScore >= scoreTable[i].score )
				return i;
		}
	return -1;	// Score lower than lowest value
}

//-----------------------------------------------------------------------------
//
// Copy the new initials into the new slot
// Called from script after pressing confirm button
void gam_addNewInitials()
//-----------------------------------------------------------------------------
{
	int insertInto = gam_addNewScore();
	int currentRow = SCORE_NUM_ENTRIES - 1;

	while (currentRow > -1)
	{
		strcpy(scoreTable[currentRow].initials, scoreTable[currentRow - 1].initials);
		scoreTable[currentRow].score = scoreTable[currentRow - 1].score;
		currentRow--;

		if (insertInto == currentRow)
		{
			strcpy(scoreTable[currentRow].initials, newInitials.c_str());
			scoreTable[currentRow].score = currentScore + displayScore;
			return;
		}
	}
}
