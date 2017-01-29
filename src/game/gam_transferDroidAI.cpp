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
