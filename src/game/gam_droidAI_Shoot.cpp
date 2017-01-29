#include "../../hdr/sys_globals.h"

#define DEBUG_SHOOT 1

// ----------- Values set from script ------------
float		droidBeenShotValue;
float		witnessShootValue;
float		ai_beenShot;
float		ai_healthAmount;
float		ai_playerVisible;
float		ai_witnessShoot;
float		ai_greenFactor;
float		ai_yellowFactor;
float		ai_redFactor;


//-----------------------------------------------------------------------------
//
// Should the Droid shoot or not
int ai_notShoot ( int whichDroid )
//-----------------------------------------------------------------------------
{
	if ( false == dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].canShoot )
		return AI_RESULT_SUCCESS;

	if ( shipLevel[currentLevel].droid[whichDroid].chanceToShoot < 10.0f )
		return AI_RESULT_SUCCESS;

#ifdef DEBUG_SHOOT
	printf ( "Droid [ %i ] can shoot\n", whichDroid );
#endif

	if ( false == shipLevel[currentLevel].droid[whichDroid].weaponCanFire )
		return AI_RESULT_SUCCESS;

#ifdef DEBUG_SHOOT
	printf ( "Droid [ %i ] weapon is ready\n", whichDroid );
#endif

	return AI_RESULT_FAILED;

}
//-----------------------------------------------------------------------------
//
// Locate the player position - see if the droid can take the shot or not
int ai_findBulletDest ( int whichDroid )
//-----------------------------------------------------------------------------
{
	if ( true == shipLevel[currentLevel].droid[whichDroid].visibleToPlayer )
		{
#ifdef DEBUG_SHOOT
			printf ( "Droid has a destination to shoot at [ %i ]\n", whichDroid );
#endif
			return AI_RESULT_FAILED;
		}
	else
		{
			return AI_RESULT_SUCCESS;
		}
}

//-----------------------------------------------------------------------------
//
// Take the shot
int ai_shootBullet ( int whichDroid )
//-----------------------------------------------------------------------------
{
	cpVect		bulletStartPos;
	cpVect		bulletDestPos;

#ifdef DEBUG_SHOOT
	printf ( "Fire bullet [ %i ]\n", whichDroid );
#endif

	bulletStartPos = shipLevel[currentLevel].droid[whichDroid].worldPos;
	bulletStartPos.y += TILE_SIZE / 2;

	if (shipLevel[currentLevel].droid[whichDroid].targetIndex == -1)	// Player is the target
	{
		bulletDestPos = playerWorldPos;
	}
	else
	{
		bulletDestPos = shipLevel[currentLevel].droid[shipLevel[currentLevel].droid[whichDroid].targetIndex].worldPos;
	}

	bulletDestPos.y += TILE_SIZE / 2;

	bul_newBullet ( bulletStartPos, bulletDestPos,
	                dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].bulletType, whichDroid );

	shipLevel[currentLevel].droid[whichDroid].chanceToShoot = 0.0f;
	shipLevel[currentLevel].droid[whichDroid].weaponCanFire = false;

	return AI_RESULT_SUCCESS;
}

//------------------------------------------------------------
//
// Work out the chance to shoot for this round
void gam_findChanceToShoot ( int whichDroid )
//------------------------------------------------------------
{
	//
	// Reset to 0 at the start for each?
	//

	if ( false == shipLevel[currentLevel].droid[whichDroid].isAlive )
		return;

	if ( false == dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].canShoot )
		return;

	//
	// Process how long droid remembers being shot for
	//
	if ( true == shipLevel[currentLevel].droid[whichDroid].beenShotByPlayer )
		{
			shipLevel[currentLevel].droid[whichDroid].beenShotCountdown -= 1.0f * thinkInterval;
			if ( shipLevel[currentLevel].droid[whichDroid].beenShotCountdown < 0.0f )
				shipLevel[currentLevel].droid[whichDroid].beenShotByPlayer = false;
		}

	if ( true == shipLevel[currentLevel].droid[whichDroid].beenShotByPlayer )
		shipLevel[currentLevel].droid[whichDroid].chanceToShoot += ai_beenShot;
	else
		shipLevel[currentLevel].droid[whichDroid].chanceToShoot -= ai_beenShot;

	//
	// Process how long droid remembers witnessing a shooting by the player
	//
	if ( true == shipLevel[currentLevel].droid[whichDroid].witnessShooting )
		{
			shipLevel[currentLevel].droid[whichDroid].witnessShootingCountDown -= 1.0f * thinkInterval;
			if ( shipLevel[currentLevel].droid[whichDroid].witnessShootingCountDown < 0.0f )
				shipLevel[currentLevel].droid[whichDroid].witnessShooting = false;
		}

	if ( true == shipLevel[currentLevel].droid[whichDroid].witnessShooting )
		shipLevel[currentLevel].droid[whichDroid].chanceToShoot += ai_witnessShoot;
	else
		shipLevel[currentLevel].droid[whichDroid].chanceToShoot -= ai_witnessShoot;

	//
	// Is Droid healthy enought to engage in combat
	//
	if ( shipLevel[currentLevel].droid[whichDroid].currentHealth > dataBaseEntry[shipLevel[currentLevel].droid[whichDroid].droidType].maxHealth / 2 )
		shipLevel[currentLevel].droid[whichDroid].chanceToShoot += ai_healthAmount;
	else
		shipLevel[currentLevel].droid[whichDroid].chanceToShoot -= ai_healthAmount;

	//
	// Add the effect of the current alert level
	//
	switch ( currentAlertLevel )
		{
			case ALERT_GREEN_TILE:
				shipLevel[currentLevel].droid[whichDroid].chanceToShoot -= ai_greenFactor;
				break;

			case ALERT_YELLOW_TILE:
				shipLevel[currentLevel].droid[whichDroid].chanceToShoot += ai_yellowFactor;
				break;

			case ALERT_RED_TILE:
				shipLevel[currentLevel].droid[whichDroid].chanceToShoot += ai_redFactor;
				break;
		}

	//
	// See if the player is visible or not
	//
	if ( true == shipLevel[currentLevel].droid[whichDroid].visibleToPlayer )
		shipLevel[currentLevel].droid[whichDroid].chanceToShoot += ai_playerVisible;
	else
		shipLevel[currentLevel].droid[whichDroid].chanceToShoot -= ai_playerVisible;

	//
	// Cap the chance to always above zero
	//
	if ( shipLevel[currentLevel].droid[whichDroid].chanceToShoot < 0.0f )
		shipLevel[currentLevel].droid[whichDroid].chanceToShoot = 0.0f;
}

//------------------------------------------------------------
//
// Flag if a droid witnesses the player shooting another droid
//
// Called when player shoots a bullet
void gam_processWitnessShooting()
//------------------------------------------------------------
{
	for ( int i = 0; i != shipLevel[currentLevel].numDroids; i++ )
		{
			if ( true == shipLevel[currentLevel].droid[i].isAlive )
				{
					if ( true == shipLevel[currentLevel].droid[i].visibleToPlayer )
						{
							shipLevel[currentLevel].droid[i].witnessShooting = true;
							shipLevel[currentLevel].droid[i].witnessShootingCountDown = witnessShootValue;
						}
				}
		}
}
