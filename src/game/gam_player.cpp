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

float 		playerAcceleration;
float 		playerMaxSpeed;
float 		playerMaxVelocity;
float		playerCurrentHealth;
float		playerHealthWarning;
float		playerHealthWarningFactor;
cpVect		playerScreenPos;
int			playerCurrentFrame;
int			playerDroidTypeDBIndex;
float		playerTransferTimeRemaining;
float		playerTransferTimeSet;		// Set from startup script
float		playerTransferTimeAmount;	// Set from startup script
float		playerTransferWarning;		// Set from startup script
float		playerFlashValue;
float		playerFlashAmount;
bool		playerIsExploding;

float 		gravity;	// Set as global from startup script

cpVect		playerVelocity;
cpVect		playerWorldPos;
cpVect 	    playerWorldMiddlePos;	// Middle of the player sprite in world coordinates
cpVect		playerWorldNextPos;		// move to this before collision detection

bool		tileCollisionOn = true;
bool		doClampPlayerVelocity;
bool		inTransferMode = false;

int 		playerOverTile;

bool		playerWeaponReadyToFire = true;
float		playerWeaponRechargeTime = 0.0f;
int			playerDroidTypeImage = sprite_001;

float       healingRate;

//-----------------------------------------------------------------------------
//
// Player has lost transfer or transfer time is up
// Drop player back to 001, play explosion and set health
void gam_dropPlayerTo001()
//-----------------------------------------------------------------------------
{
	par_addEmitter ( playerWorldPos, PARTICLE_TYPE_EXPLOSION, -1 );
	gam_setupPlayerValues ( 0 );
	playerCurrentHealth = dataBaseEntry[0].maxHealth / 3;
}

//-----------------------------------------------------------------------------
//
// If the player has transferred into a droid - check the time remaining
void gam_playerTransferTime()
//-----------------------------------------------------------------------------
{
	if ( playerDroidTypeDBIndex == 0 )	// 0 == 001 - not in transferred droid
		return;

	playerTransferTimeRemaining -= playerTransferTimeAmount * thinkInterval;
	if ( playerTransferTimeRemaining < 0.0f )
		{
			gam_dropPlayerTo001();
		}
}

//-----------------------------------------------------------------------------
//
// Play transfer sound if in transfer mode
void gam_checkTransferSound()
//-----------------------------------------------------------------------------
{
	if ( ( true == inTransferMode ) && ( false == sys_isSoundPlaying ( SND_TRANSFER_1 ) ) )
		sys_playSound ( SND_TRANSFER_1, SND_PAN_CENTER, ALLEGRO_PLAYMODE_LOOP );

	if ( false == inTransferMode )
		sys_stopSound ( SND_TRANSFER_1 );
}

//-----------------------------------------------------------------------------
//
// Recharge player weapon
void gam_weaponRechargePlayer()
//-----------------------------------------------------------------------------
{
	if ( false == playerWeaponReadyToFire )
		{
			if ( true == dataBaseEntry[playerDroidTypeDBIndex].canShoot )
				playerWeaponRechargeTime += dataBaseEntry[playerDroidTypeDBIndex].rechargeTime * thinkInterval;
			else
				playerWeaponRechargeTime += dataBaseEntry[0].rechargeTime * thinkInterval;

			if ( playerWeaponRechargeTime > 1.0f )
				{
					playerWeaponRechargeTime = 0.0f;
					playerWeaponReadyToFire = true;
				}
		}
}

//-----------------------------------------------------------------------------
//
// Animate player sprite
void gam_animatePlayerSprite ( float delayInterval )
//-----------------------------------------------------------------------------
{
	static float	countDelay = 0.0f;
	float			animspeed = 0.0f;

	if (true == playerIsExploding)
		animspeed = 20.0f;
	else
		animspeed = 10.0f; // TODO: work out health

	countDelay += animspeed * delayInterval;
	if ( countDelay > 1.0f )
		{
			countDelay = 0.0f;
			playerCurrentFrame++;

			if ( false == playerIsExploding )
				{
					if ( playerCurrentFrame > NUM_FRAMES_IN_DROID )
						playerCurrentFrame = 0;
				}
			else
				{
					if ( playerCurrentFrame > NUM_FRAMES_IN_EXPLOSION )
						{
							playerCurrentFrame = 0;
							sys_changeMode ( MODE_END_STATIC_PRE, true );
						}
				}
		}

	playerFlashValue -= playerFlashAmount;
	if ( playerFlashValue < 0.0f )
		playerFlashValue = 1.0f;
}

//-----------------------------------------------------------------------------
//
// Copy the values from an enemy to player sprite
// Init for 001 on first run
void gam_setupPlayerValues ( int copyFrom )
//-----------------------------------------------------------------------------
{
	int weight;
	float massWeight;

	playerMaxSpeed = 				dataBaseEntry[copyFrom].maxSpeed;
	playerAcceleration =			dataBaseEntry[copyFrom].accelerate;
	playerCurrentHealth = 			dataBaseEntry[copyFrom].maxHealth;
	playerHealthWarning = 			playerCurrentHealth * playerHealthWarningFactor;
	playerCurrentFrame = 			0;
	playerDroidTypeImage = 			copyFrom + 2; // Base is 0 - sprite_001 is two higher than 0
	playerDroidTypeDBIndex =		copyFrom;
	playerWeaponReadyToFire = 		true;
	playerWeaponRechargeTime = 		0.0f;
	playerFlashValue = 				1.0f;
	playerIsExploding = 			false;

	weight = atoi ( dataBaseEntry[copyFrom].weight );
	massWeight = weight / 100.0f;

	sys_setPlayerMass ( massWeight );

	if ( copyFrom != 0 )	// Done a transfer into droid
		{
			playerTransferTimeRemaining = playerTransferTimeSet;
		}
}

//-----------------------------------------------------------------------------
//
// Process player movement - do scroll and tile movements
void gam_processPlayerMovement ( float thinkTime )
//-----------------------------------------------------------------------------
{
	if ( true == inputAction[gameLeft].currentlyDown )
		{
			playerVelocity.x -= playerAcceleration * thinkTime;
			if ( playerVelocity.x < -playerMaxSpeed )
				playerVelocity.x = -playerMaxSpeed;
		}

	else if ( true == inputAction[gameRight].currentlyDown )
		{
			playerVelocity.x += playerAcceleration * thinkTime;
			if ( playerVelocity.x > playerMaxSpeed )
				playerVelocity.x = playerMaxSpeed;
		}

	if ( true == inputAction[gameUp].currentlyDown )
		{
			playerVelocity.y -= playerAcceleration * thinkTime;
			if ( playerVelocity.y < -playerMaxSpeed )
				playerVelocity.y = -playerMaxSpeed;
		}

	else if ( true == inputAction[gameDown].currentlyDown )
		{
			playerVelocity.y += playerAcceleration * thinkTime;
			if ( playerVelocity.y > playerMaxSpeed )
				playerVelocity.y = playerMaxSpeed;
		}

//
// Do gravity slowdown when no key is pressed
	if ( false == inputAction[gameLeft].currentlyDown )
		{
			if ( playerVelocity.x < 0.0f )
				{
					playerVelocity.x += gravity * thinkTime;
					if ( playerVelocity.x > 0.0f )
						playerVelocity.x = 0.0f;
				}
		}

	if ( false == inputAction[gameRight].currentlyDown )
		{
			if ( playerVelocity.x > 0.0f )
				{
					playerVelocity.x -= gravity * thinkTime;
					if ( playerVelocity.x < 0.0f )
						playerVelocity.x = 0.0f;
				}
		}

	if ( false == inputAction[gameUp].currentlyDown )
		{
			if ( playerVelocity.y < 0.0f )
				{
					playerVelocity.y += gravity * thinkTime;
					if ( playerVelocity.y > 0.0f )
						playerVelocity.y = 0.0f;
				}
		}

	if ( false == inputAction[gameDown].currentlyDown )
		{
			if ( playerVelocity.y > 0.0f )
				{
					playerVelocity.y -= gravity * thinkTime;
					if ( playerVelocity.y < 0.0f )
						playerVelocity.y = 0.0f;
				}
		}
//
// Option set in startup script - is this needed? Limited to max speed ??
//
	if ( true == doClampPlayerVelocity )
		cpvclamp ( playerVelocity, playerMaxVelocity );

	gam_setHUDState ( HUD_STATE_MOVE );

	if ( true == inTransferMode )
		gam_setHUDState ( HUD_STATE_TRANSFER );

	if ( false == playerWeaponReadyToFire )
		gam_setHUDState ( HUD_STATE_CHARGING );
}


//-----------------------------------------------------------------------------
//
// Check to see if the player over a healing tile
void gam_checkPlayerHealingTile ( float thinkTime )
//-----------------------------------------------------------------------------
{
	static float   healingDelay = 1.5f;
	static float   healingDelayCount = 0.0f;

	if ( ( playerOverTile >= HEALING_TILE ) &&
	        ( playerOverTile <= HEALING_TILE + 3 ) )
		{
			if ( playerCurrentHealth < dataBaseEntry[playerDroidTypeImage - 2].maxHealth )
				gam_setHUDState ( HUD_STATE_HEALING );
			else
				gam_setHUDState ( HUD_STATE_HEALED );

			if ( ( playerVelocity.x == 0.0f ) && ( playerVelocity.y == 0.0f ) )
				{
					healingDelayCount += healingDelay * thinkTime;
					if ( healingDelayCount > 1.0f )
						{
							healingDelayCount = 0.0f;
							if ( playerCurrentHealth < dataBaseEntry[playerDroidTypeImage - 2].maxHealth )
								{
									if ( false == sys_isSoundPlaying ( SND_ENERGY_HEAL ) )
										sys_playSound ( SND_ENERGY_HEAL, 0.0f, ALLEGRO_PLAYMODE_ONCE );

									currentScore -= healingRate * 2;
									if ( currentScore < 0 )
										currentScore = 0;
									playerCurrentHealth += healingRate;
									if ( playerCurrentHealth > dataBaseEntry[playerDroidTypeImage - 2].maxHealth )
										playerCurrentHealth = dataBaseEntry[playerDroidTypeImage - 2].maxHealth;
								}
						}
				}
		}
}

//-----------------------------------------------------------------------------
//
// Do damage to player health
// Can either be from DAMAGE_BULLET, DAMAGE_COLLISION or DAMAGE_EXPLOSION
void gam_doDamageToPlayer ( int damageSource, int sourceDroid )
//-----------------------------------------------------------------------------
{
	switch ( damageSource )
		{
			case DAMAGE_BULLET:
				playerCurrentHealth -= dataBaseEntry[shipLevel[currentLevel].droid[sourceDroid].droidType].bulletDamage;
//				printf("Player health now [ %i ]\n", playerCurrentHealth);
				break;

			case DAMAGE_COLLISION:
				playerCurrentHealth -= collisionDamageInflicted;
				break;

			case DAMAGE_EXPLOSION:
				playerCurrentHealth -= collisionExplosionDamage;
				break;
		}

	sys_playSound ( SND_DAMAGE, 0.0f, ALLEGRO_PLAYMODE_ONCE );

	if ( playerCurrentHealth < 0 )
		{
			// End Game
//			con_print ( true, true, "PLAYER DEAD [ %i ]", playerCurrentHealth );
			playerIsExploding = true;	// Draw player exploding
			playerCurrentFrame = 0;		// Start from beginning of explosion animation
			sys_stopAllSounds();
			sys_playSound(SND_EXPLODE_2, SND_PAN_LEFT, ALLEGRO_PLAYMODE_ONCE);
			sys_playSound(SND_EXPLODE_2, SND_PAN_RIGHT, ALLEGRO_PLAYMODE_ONCE);
			sys_changeMode ( MODE_PLAYER_EXPLODE, false );
		}
}
