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
