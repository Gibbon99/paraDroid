#pragma once

//-----------------------------------------------------------------------------
//
// sideview of ship stuff
//
//-----------------------------------------------------------------------------

#define MAX_LEVELS		  29
#define NUM_OF_TUNNELS		8

extern int	currentTunnel;

//-----------------------------------------------------------------------------
//
// structure for sideview of ship
//
//-----------------------------------------------------------------------------

struct _basicTunnel
{
	int		top;
	int		bottom;
	int		current;
	int		current_deck;
	int		decks[6];
};

extern _basicTunnel tunnel[NUM_OF_TUNNELS];

struct _sideviewBasicLevel
{
	int		x1;
	int		y1;
	int		x2;
	int		y2;
	int		up;
	int		down;
};

struct _myRectangle
{
	float	x1;
	float	y1;
	float	x2;
	float	y2;
};

extern _sideviewBasicLevel sideviewLevels[MAX_LEVELS];

// load the sideview data from the external file
bool io_loadSideViewData(const char *sideviewFileName);

// Draw the sideview
void gam_drawSideViewLevels();

// setup the way the tunnels are linked to the levels
void gam_setupTunnels();

// Get the tunnel being used by the lift the player sprite is over
void gam_getTunnelToUse();

// Put the player onto lift on new deck
void gam_putPlayerOnLiftFromTunnel();
