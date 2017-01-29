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

_basicTunnel        tunnel[NUM_OF_TUNNELS];

_sideviewBasicLevel sideviewLevels[MAX_LEVELS];

//-----------------------------------------------------------------------------
//
// setup the way the tunnels are linked to the levels
void gam_setupTunnels()
//-----------------------------------------------------------------------------
    {
    tunnel[0].top = 19;
    tunnel[0].bottom = 3;
    tunnel[0].decks[0] = 3;
    tunnel[0].decks[1] = 4;
    tunnel[0].decks[2] = 15;
    tunnel[0].decks[3] = 16;
    tunnel[0].decks[4] = 19;
    tunnel[0].current_deck = 0;
    tunnel[0].current = tunnel[0].decks[tunnel[0].current_deck];

    tunnel[1].top = 6;
    tunnel[1].bottom = 0;
    tunnel[1].decks[0] = 0;
    tunnel[1].decks[1] = 5;
    tunnel[1].decks[2] = 6;
    tunnel[1].current_deck = 0;
    tunnel[1].current = tunnel[1].decks[tunnel[1].current_deck];

    tunnel[2].top = 17;
    tunnel[2].bottom = 15;
    tunnel[2].decks[0] = 15;
    tunnel[2].decks[1] = 17;
    tunnel[2].current_deck = 0;
    tunnel[2].current = tunnel[2].decks[tunnel[2].current_deck];

    tunnel[3].top = 20;
    tunnel[3].bottom = 12;
    tunnel[3].decks[0] = 12;
    tunnel[3].decks[1] = 13;
    tunnel[3].decks[2] = 14;
    tunnel[3].decks[3] = 17;
    tunnel[3].decks[4] = 20;
    tunnel[3].current_deck = 0;
    tunnel[3].current = tunnel[3].decks[tunnel[3].current_deck];

    tunnel[4].top = 12;
    tunnel[4].bottom = 1;
    tunnel[4].decks[0] = 1;
    tunnel[4].decks[1] = 5;
    tunnel[4].decks[2] = 6;
    tunnel[4].decks[3] = 12;
    tunnel[4].current_deck = 0;
    tunnel[4].current = tunnel[4].decks[tunnel[4].current_deck];

    tunnel[5].top = 9;
    tunnel[5].bottom = 2;
    tunnel[5].decks[0] = 2;
    tunnel[5].decks[1] = 8;
    tunnel[5].decks[2] = 9;
    tunnel[5].current_deck = 0;
    tunnel[5].current = tunnel[5].decks[tunnel[5].current_deck];

    tunnel[6].top = 14;
    tunnel[6].bottom = 9;
    tunnel[6].decks[0] = 9;
    tunnel[6].decks[1] = 10;
    tunnel[6].decks[2] = 11;
    tunnel[6].decks[3] = 12;
    tunnel[6].decks[4] = 13;
    tunnel[6].decks[5] = 14;
    tunnel[6].current_deck = 0;
    tunnel[6].current = tunnel[6].decks[tunnel[6].current_deck];

    tunnel[7].top = 18;
    tunnel[7].bottom = 14;
    tunnel[7].decks[0] = 14;
    tunnel[7].decks[1] = 18;
    tunnel[7].current_deck = 0;
    tunnel[7].current = tunnel[7].decks[tunnel[7].current_deck];
    }

