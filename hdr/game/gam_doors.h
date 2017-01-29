#pragma once

//-----------------------------------------------------------------------------
//
// defines used for controlling doors - tile number
//-----------------------------------------------------------------------------
#define DOOR_ACROSS				14
#define DOOR_ACROSS_OPEN_1		15
#define DOOR_ACROSS_OPEN_2		16
#define DOOR_ACROSS_OPENED		NO_PASS_TILE + 10
#define DOOR_ACROSS_CLOSING_1	22
#define DOOR_ACROSS_CLOSING_2	21
#define DOOR_ACROSS_CLOSED		14

#define DOOR_UP					11
#define DOOR_UP_OPEN_1			12
#define DOOR_UP_OPEN_2			13
#define DOOR_UP_OPENED			NO_PASS_TILE + 9
#define DOOR_UP_CLOSING_1		19
#define DOOR_UP_CLOSING_2		18
#define DOOR_UP_CLOSED			11

#define MAX_NUM_OF_DOORS		21

//-----------------------------------------------------------------------------
//
// structure used for doors
//
//-----------------------------------------------------------------------------

typedef struct
{
	int				mapPos;
	int				mapX;
	int				mapY;
	cpVect			topLeft;
	cpVect			topRight;
	cpVect			botLeft;
	cpVect			botRight;
	int				currentFrame;			// which frame are we on
	float			frameDelay;				// speed to animate them at
	float			nextFrame;				// counter for incrementing to next frame
	bool			inUse;
	_physicObject	physics;				// Used for bullet collision detection
} _doorTrigger;

extern _doorTrigger			doorTrigger[MAX_NUM_OF_DOORS];		// pointer to memory to hold door trigger information
extern int					numDoorsOnLevel;					// how many doors are on this level
extern float				doorFrameDelay;

// Setup each door doorTrigger for this level
void gam_doorTriggerSetup();

// Show door trigger area
void gam_doorDebugTrigger();

// Process all the doors that are currently inUse
void gam_doorProcessActions();

// show trigger area on the screen using lines
void gam_doorCheckTriggerAreas();

