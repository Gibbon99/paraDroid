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

#define SCORE_TABLE_VERSION		0x1
#define SCORE_TABLE_FILENAME	"scoreTable.par"
#define SCORE_NUM_ENTRIES		5
#define SCORE_INITIALS			0x01
#define SCORE_SCORE				0x02

extern 	int			scoreTableVersion;
extern	string		newInitials;

typedef struct
{
	char			initials[5];
	int				score;
} _scoreTable;

// Write the scoretable out to a binary file
bool gam_saveScoreTable(char *fileName);

// Read the scoretable into the structure
bool gam_readScoreTable(char *fileName);

// Return a string containing value from the score table
string gam_getScoreElement(int whichElement, int tableIndex);

// Pass in new score and return index of the table row that needs to be updated
int gam_addNewScore();

// Process score and set current Alert level
void gam_processScore(float interpolate);

// Add passed in value to currentScore
void gam_addToScore(int scoreValue);

// Copy the new initials into the new slot
// Called from script after pressing confirm button
void gam_addNewInitials();

// Reset the score values
void gam_resetScore();
