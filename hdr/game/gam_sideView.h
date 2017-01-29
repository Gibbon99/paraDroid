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
