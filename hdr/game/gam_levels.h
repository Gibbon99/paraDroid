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

#define MAP_VERSION     115

#define MAX_NUM_SPRITES    20   // maximum number of sprites per level
#define MAX_NUM_OF_WAYPOINTS  100
#define NUM_OF_LEVELS    21
#define MAX_NUM_OF_LIFTS   3

#define TILE_SIZE      32

#define NO_PASS_TILE    40
#define LIFT_TILE     NO_PASS_TILE + 1
#define HEALING_TILE    LIFT_TILE + 4

#define ALERT_GREEN_TILE   4
#define ALERT_YELLOW_TILE   5
#define ALERT_RED_TILE    6

#define TERMINAL_TOP    51
#define TERMINAL_BOTTOM    52
#define TERMINAL_RIGHT    53
#define TERMINAL_LEFT    54

#define HEALING_TILE    LIFT_TILE + 4

extern int currentAlertLevel;

struct _my2DInt {
    int x;
    int y;
};

extern _my2DInt drawOffset;

//-----------------------------------------------------------------------------
//
// Loaded from disk per level
//
//-----------------------------------------------------------------------------

typedef struct {
    cpVect start;
    cpVect finish;
} _lineSegment;

//-----------------------------------------------------------------------------
//
// information needed for one lift
//
//-----------------------------------------------------------------------------

struct _liftBasic {
    int tunnel;
    int posX;
    int posY;
};

struct _levelStruct {
    int mapVersion;
    int numLineSegments;
    int numWaypoints;
    int numDroids;
    int numLifts;
    cpVect levelDimensions;
    vector<_lineSegment> lineSegments;
    vector<cpVect> wayPoints;
    vector<int> tiles;
    vector<int> droidTypes;
    char levelName[20];
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    // Above here is loaded from file
    //
    int numEnemiesAlive;
    vector<_liftBasic> lifts;
    vector<_droid> droid;
};

extern _levelStruct shipLevel[];

extern int currentLevel;
extern bool	gameWon;

//-----------------------------------------------------------------------------
//
// structure to hold information for healing tiles
//
//-----------------------------------------------------------------------------

struct _basicHealing {
    int numOnLevel; // first record holds number of tiles in level
    int pos; // position in array
    int numFrames; // how many frames make up the animationdownload
    int currentFrame; // which frame are we on
    float frameDelay; // speed to animate them at
    float nextFrame; // counter for incrementing to next frame
    cpVect worldPosition; // Used for aStar finding
};

extern _basicHealing *healing; // hold information for animating healing tiles

extern char displayScoreString[NUM_SCORE_CHARS];
extern int currentScore;
extern int displayScore;
extern float scoreDelay;
extern float scoreDelayValue;
extern int yellowAlertLevel; // From script
extern int redAlertLevel; // From script
extern int levelBonus; // From script

//-----------------------------------------------------------------------------
//
// Function declarations
//
//-----------------------------------------------------------------------------

// Load all the levels into memory
bool gam_loadAllLevels();

// Position the player on the requested lift on the new level
void gam_putPlayerOnLift(int whichLift);

// Change to a new level
void gam_changeToLevel(int newlevel, int whichLift);

// Change to a new level from script - default to lift 0
void gam_changeToLevelFromScript(int newLevel);

// Find out where on the level the healing tiles are
// Remember for this level for animation
void gam_findHealingTiles(int whichLevel);

// draw the frames of the healing tile animation
void gam_animateHealTiles(int whichLevel);

// Get the positions of lifts
void lvl_getLiftPositions(int whichLevel);

// Check current alert level and change tiles and background sound
void gam_checkAlertLevel();

// All the droids are dead on this level - change tileset
void gam_powerDownLevel(int whichLevel, bool playSound);

// Restart the game
void gam_restart();

// Check to see if the ship is empty of all droids
void gam_isShipEmpty();