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

//--------------------------------------------------------
//
// Do think cycle for droid doing transfer
void gam_doTransferThink(float thinkInterval)
//--------------------------------------------------------
{
static float 		enemyTransferThinkCount = 0.0f;
		float		enemyTransferThink = numDroidTokens * 2;

	enemyTransferThinkCount += enemyTransferThink * thinkInterval;
	if (enemyTransferThinkCount > 1.0f)
	{
		enemyTransferThinkCount = 0.0f;
		sys_executeScriptFunction("as_processTransferAI", "");
	}
}



//----------------------------------------------------------------
//
//Next droid token - move ready to use and decrement count
void trn_nextDroidToken()
//----------------------------------------------------------------
{
    droidBlockPos = 0; //TOKEN_POS_LAUNCH;
    numDroidTokens--;
    if (numDroidTokens == -1)
        droidBlockPos = TOKEN_POS_HIDE;
}
