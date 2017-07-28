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

cpVect		screenPos;
float		explodeFrameDelay;
float		baseDroidAnimateValue;	// Set from script
float		maxDistanceFromPath;
float		moveBackToLineSegment;
int			collisionDamageInflicted;
int			collisionExplosionDamage;

float 		wayPointDestinationSize;
float 		healingTimer; // Used to time healing for droids

//---------------------------------------------------------------
//
// Update the droids information from physics properties
void drd_updateDroidPosition ( int whichDroid )
//---------------------------------------------------------------
{
	if (false == processedPhysics)
		return;

	cpVect		tempPosition;

	tempPosition = cpBodyGetPosition ( shipLevel[currentLevel].droid[whichDroid].body );

	if ( (tempPosition.x < 0) || (tempPosition.y < 0))
	{
		printf("ERROR: Setting invalid worldPos from body Droid [ %i ] Level [ %i ] Frame [ %d ]\n", whichDroid, currentLevel, frameCount);
		return;
	}

	shipLevel[currentLevel].droid[whichDroid].worldPos = tempPosition;
}

//---------------------------------------------------------------
//
// Do damage to a droid
//
// damageSource can be either a bullet, explosion or collision
void drd_damageToDroid ( int whichLevel, int whichDroid, int damageSource, int sourceDroid )
//---------------------------------------------------------------
{
	switch ( damageSource )
		{
			case DAMAGE_BULLET:
				if ( -1 == sourceDroid ) // Player bullet
					{
						if ( true == shipLevel[whichLevel].droid[whichDroid].isExploding )
							return;

						shipLevel[currentLevel].droid[whichDroid].targetIndex = sourceDroid;	// Set player as the target
						shipLevel[currentLevel].droid[whichDroid].beenShotByPlayer = true;
						shipLevel[currentLevel].droid[whichDroid].beenShotCountdown = droidBeenShotValue;

						//
						// Need to work out bullet damage when using non firing droid
						//
						if ( true == dataBaseEntry[playerDroidTypeDBIndex].canShoot )
							shipLevel[whichLevel].droid[whichDroid].currentHealth -= dataBaseEntry[playerDroidTypeDBIndex].bulletDamage;
						else
							shipLevel[whichLevel].droid[whichDroid].currentHealth -= dataBaseEntry[0].bulletDamage;

						if ( shipLevel[whichLevel].droid[whichDroid].currentHealth <= 0 )
							{
								gam_destroyDroid ( whichLevel, whichDroid );
							}
						else
							{
								sys_playSound ( SND_DAMAGE, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );
							}
					}
				else // hit by another droid bullet
					{
						if ( sourceDroid == whichDroid )
							return;

						shipLevel[currentLevel].droid[whichDroid].targetIndex = sourceDroid;	// Set this droid as the target
						shipLevel[currentLevel].droid[whichDroid].beenShotByPlayer = false;
						shipLevel[whichLevel].droid[whichDroid].currentHealth -= dataBaseEntry[shipLevel[whichLevel].droid[sourceDroid].droidType].bulletDamage;
						sys_playSound ( SND_DAMAGE, SND_PAN_CENTER, ALLEGRO_PLAYMODE_ONCE );

						if ( shipLevel[whichLevel].droid[whichDroid].currentHealth <= 0 )
							{
								gam_destroyDroid ( whichLevel, whichDroid );
							}
					}

				break;

			case DAMAGE_EXPLOSION:

				if ( true == shipLevel[whichLevel].droid[whichDroid].isExploding )
					return;

				if ( -1 == sourceDroid )
					{
						//
						// Player is colliding with exploding sprite
						gam_doDamageToPlayer ( DAMAGE_EXPLOSION, whichDroid );
					}
				else
					{
						//
						// Enemy Droid is colliding with another one exploding
						sys_playSound ( SND_DAMAGE, 0.0f, ALLEGRO_PLAYMODE_ONCE );
						shipLevel[whichLevel].droid[whichDroid].currentHealth -= collisionExplosionDamage;

						if ( shipLevel[whichLevel].droid[whichDroid].currentHealth <= 0 )
							{
								gam_destroyDroid ( whichLevel, whichDroid );
							}

						/*
												if ( shipLevel[whichLevel].droid[sourceDroid].currentHealth <= 0 )
													{
														gam_destroyDroid ( whichLevel, sourceDroid );
													}
													*/
					}
				break;

			case DAMAGE_COLLISION:
				if ( -1 == sourceDroid )
					{
						gam_doDamageToPlayer ( DAMAGE_COLLISION, whichDroid );

						shipLevel[whichLevel].droid[whichDroid].currentHealth -= collisionDamageInflicted;
						if ( shipLevel[whichLevel].droid[whichDroid].currentHealth <= 0 )
							{
								gam_destroyDroid ( whichLevel, whichDroid );
							}
					}
				break;
		}
}

//-----------------------------------------------------------------------------
//
//Setup initial values for enemy droids
void gam_initDroidValues ( int whichLevel )
//-----------------------------------------------------------------------------
{
	_droid tempDroid;

	shipLevel[whichLevel].droid.reserve ( shipLevel[whichLevel].numDroids );

	for ( int i = 0; i != shipLevel[whichLevel].numDroids; i++ )
		{
			tempDroid.droidType = shipLevel[whichLevel].droidTypes[i];
			tempDroid.currentHealth = dataBaseEntry[tempDroid.droidType].maxHealth;
			tempDroid.isAlive = true;
			tempDroid.wayPointIndex = sys_genRand_int32() % ( shipLevel[whichLevel].numWaypoints - 1 );
			tempDroid.worldPos.x = shipLevel[whichLevel].wayPoints[tempDroid.wayPointIndex].x;
			tempDroid.worldPos.y = shipLevel[whichLevel].wayPoints[tempDroid.wayPointIndex].y;

			if (tempDroid.worldPos.x < 0)
				printf("ERROR: worlPos set to less than 0 - Level [ %i ] Droid [ %i ]\n", whichLevel, i);

			tempDroid.screenPos = sys_worldToScreen ( tempDroid.worldPos, DROID_BODY_SIZE );
			tempDroid.wayPointDirection = WAYPOINT_UP;
			tempDroid.collisionCount = 0.0f;
			tempDroid.ignoreCollisions = false;
			tempDroid.ignoreCollisionsCounter = IGNORE_COLLISION_TIME;

			tempDroid.visibleToPlayer = false;
			tempDroid.isExploding = false;
			tempDroid.isStopped = false;
			tempDroid.hasCollided = false;
			tempDroid.currentFrameDelay = 0.0f;
			tempDroid.currentFrame = 5;
			tempDroid.weaponCanFire = true;
			tempDroid.weaponDelay = 0.0f;
			tempDroid.mass = atoi ( dataBaseEntry[tempDroid.droidType].weight );

			tempDroid.ai_moveMode = AI_MODE_WAYPOINT;
			tempDroid.onHealingTile = false; // Not on healing tile to start with
			tempDroid.foundHealingTile = false;
			tempDroid.aStarInitDone = false;
			tempDroid.returnToOrigin = false;
			tempDroid.foundOriginPosition = false;
			tempDroid.originPosition = cpVect {0, 0};
			tempDroid.foundOriginPath = false;

			tempDroid.witnessShooting = false;
			tempDroid.chanceToShoot = 0.0f;
			tempDroid.beenShotByPlayer = false;
			tempDroid.beenShotCountdown = 0.0f;
			tempDroid.witnessShootingCountDown = 0.0f;

			tempDroid.isNotPatrolling = false;
			tempDroid.destSet = false;
			tempDroid.resumeDestFound = false;
			tempDroid.resumeThreadStarted = false;

			tempDroid.healingCount = 0.0f;
			tempDroid.foundWPTile = false;

			tempDroid.numberPathNodes = 0;
			tempDroid.aStarPathIndex = -1;

			//
			// Setup path line segment ends
			// Start segment
			tempDroid.previousWaypoints = tempDroid.worldPos;
			//
			// Get the destination waypoints
			tempDroid.wayPointIndex++;

			if ( tempDroid.wayPointIndex >= shipLevel[whichLevel].numWaypoints )
				tempDroid.wayPointIndex = 0;

			tempDroid.destinationCoords.x = shipLevel[whichLevel].wayPoints[tempDroid.wayPointIndex].x;
			tempDroid.destinationCoords.y = shipLevel[whichLevel].wayPoints[tempDroid.wayPointIndex].y;

			shipLevel[whichLevel].droid.push_back ( tempDroid );
		}
}

//-----------------------------------------------------------------------------
//
// Which tile is the droid over
void drd_getOverWhichTile ( int whichDroid )
//-----------------------------------------------------------------------------
{
	int tempPosX, tempPosY;
	int	tileIndex = 0;

	if (false == processedPhysics)
		return;

	//
	// Now get which tile we are over
	tempPosX = ( int ) shipLevel[currentLevel].droid[whichDroid].worldPos.x / TILE_SIZE;
	tempPosY = ( int ) shipLevel[currentLevel].droid[whichDroid].worldPos.y / TILE_SIZE;

	if ( tempPosX < 0 )
		{
			printf ( "Error: X [ %i ] Y [ %i ]\n", tempPosX, tempPosY );
			printf ( "World [ %3.2f : %3.2f ]\n", shipLevel[currentLevel].droid[whichDroid].worldPos.x, shipLevel[currentLevel].droid[whichDroid].worldPos.y );
			return;
		}
	else
		{
		tileIndex = (tempPosY * (shipLevel[currentLevel].levelDimensions.x) + tempPosX);
		if (tileIndex < (int)shipLevel[currentLevel].tiles.size ())
			shipLevel[currentLevel].droid[whichDroid].overTileMiddle = shipLevel[currentLevel].tiles[tileIndex];
		else
			printf ("Error: drd_getOverWhichTile index larger than vector size.\n");
		}
}

//-----------------------------------------------------------------------------
//
// Draw all the enemy droids on the level
void gam_drawAllDroids ( int whichLevel )
//-----------------------------------------------------------------------------
{
	for ( int i = 0; i != shipLevel[whichLevel].numDroids; i++ )
		{
			screenPos = sys_worldToScreen ( shipLevel[whichLevel].droid[i].worldPos, DROID_BODY_SIZE );
			screenPos.x -= SPRITE_SIZE / 2;		// Offset because worldPos is the middle of the sprite
			screenPos.y -= SPRITE_SIZE / 2;		// Draw is from top left

			if ( ( true == shipLevel[whichLevel].droid[i].visibleToPlayer ) && ( true == shipLevel[whichLevel].droid[i].isAlive ) )
				{
					if ( false == shipLevel[whichLevel].droid[i].isExploding )
						{
							gam_drawSprite ( shipLevel[whichLevel].droid[i].currentFrame, dataBaseEntry[shipLevel[whichLevel].droid[i].droidType].imageIndex, screenPos, -1.0f, al_map_rgb_f ( 0.0f, 0.0f, 0.0f ) );

							if ( true == debugDrawPhysics )
								{
									cpVect currentPos = cpBodyGetPosition ( shipLevel[whichLevel].droid[i].body );
									cpVect scaled = cpvmult ( shipLevel[whichLevel].droid[i].velocity, 35 );
									scaled = cpvnormalize ( scaled );
									scaled = cpvmult ( scaled, 55 );
									cpVect direction = cpvadd ( currentPos, scaled );

									al_draw_circle ( screenPos.x + ( SPRITE_SIZE / 2 ), screenPos.y + ( SPRITE_SIZE / 2 ), playerRadius, al_map_rgb_f ( 1.0f, 1.0f, 1.0f ), 2.0 );

									currentPos = sys_worldToScreen ( currentPos, 35 );
									direction = sys_worldToScreen ( direction, 35 );

									al_draw_line ( currentPos.x, currentPos.y, direction.x, direction.y, al_map_rgb_f ( 1.0f, 1.0f, 1.0f ), 2 );
								}
						}
					else
						{
							gam_drawSprite ( shipLevel[whichLevel].droid[i].currentFrame, explosion, screenPos, -1.0f, al_map_rgb_f ( 1.0f, 1.0f, 1.0f ) );
						}
				}
		}

	if ( playerIsExploding == true )
		{
			gam_drawSprite ( playerCurrentFrame, explosion, playerScreenPos, -1.0f, al_map_rgb_f ( 1.0f, 1.0f, 1.0f ) );
			return;
		}
	//
	// Flash player sprite depending on conditions below ( set from startup script )
	//
	if ( inTransferMode == true )
		{
			gam_drawSprite ( playerCurrentFrame, playerDroidTypeImage, playerScreenPos, -1.0f, al_map_rgba_f ( 0.0f, 0.0f, playerFlashValue, 1.0f ) );
			return;
		}

	if ( ( playerTransferTimeRemaining < playerTransferWarning ) && ( playerDroidTypeDBIndex != 0 ) )
		{
			gam_drawSprite ( playerCurrentFrame, playerDroidTypeImage, playerScreenPos, -1.0f, al_map_rgba_f ( playerFlashValue, playerFlashValue, playerFlashValue, 1.0f ) );
			return;
		}

	if ( playerCurrentHealth < playerHealthWarning )
		{
			gam_drawSprite ( playerCurrentFrame, playerDroidTypeImage, playerScreenPos, -1.0f, al_map_rgba_f ( playerFlashValue, 0.0f, 0.0f, 1.0f ) );

			if ( false == sys_isSoundPlaying ( SND_LOW_ENERGY ) )
				sys_playSound ( SND_LOW_ENERGY, SND_PAN_CENTER, ALLEGRO_PLAYMODE_LOOP );

			return;
		}
	else
		{
			sys_stopSound ( SND_LOW_ENERGY );
		}
	//
	// No conditions meet - draw normally
	//
	gam_drawSprite ( playerCurrentFrame, playerDroidTypeImage, playerScreenPos, -1.0f, al_map_rgba_f ( 1.0f, 1.0f, 1.0f, 1.0f ) );
}




//-----------------------------------------------------------------------------
//
// Process weapon timings

void gam_droidWeaponCharge ( int whichLevel, int whichDroid )
//-----------------------------------------------------------------------------
{
	if ( true == shipLevel[whichLevel].droid[whichDroid].weaponCanFire )
		return;

	shipLevel[whichLevel].droid[whichDroid].weaponDelay += dataBaseEntry[shipLevel[whichLevel].droid[whichDroid].droidType].rechargeTime * thinkInterval;

	if ( shipLevel[whichLevel].droid[whichDroid].weaponDelay > 1.0f )
		{
			shipLevel[whichLevel].droid[whichDroid].weaponDelay = 0.0f;
			shipLevel[whichLevel].droid[whichDroid].weaponCanFire = true;
		}
}

// ----------------------------------------------------------------------------
//
// Animate enemy droid

void gam_animateDroid ( int whichLevel, int whichDroid, float delayInterval )
// ----------------------------------------------------------------------------
{
	float animSpeed = 0.0f;

	if ( false == shipLevel[whichLevel].droid[whichDroid].isAlive )
		return;

	if ( true == shipLevel[whichLevel].droid[whichDroid].isExploding )
		animSpeed = explodeFrameDelay;
	else
		animSpeed = baseDroidAnimateValue * ( ( float ) shipLevel[whichLevel].droid[whichDroid].currentHealth / ( float ) dataBaseEntry[shipLevel[whichLevel].droid[whichDroid].droidType].maxHealth );

	shipLevel[whichLevel].droid[whichDroid].currentFrameDelay += animSpeed * delayInterval;

	if ( shipLevel[whichLevel].droid[whichDroid].currentFrameDelay > 1.0f )
		{
			shipLevel[whichLevel].droid[whichDroid].currentFrameDelay = 0.0f;
			shipLevel[whichLevel].droid[whichDroid].currentFrame++;
		}

	if ( false == shipLevel[whichLevel].droid[whichDroid].isExploding )
		{
			if ( shipLevel[whichLevel].droid[whichDroid].currentFrame > NUM_FRAMES_IN_DROID )
				shipLevel[whichLevel].droid[whichDroid].currentFrame = 0;
		}
	else
		{
			if ( shipLevel[whichLevel].droid[whichDroid].currentFrame > NUM_FRAMES_IN_EXPLOSION )
				{
					shipLevel[whichLevel].droid[whichDroid].currentFrame = 0;
					shipLevel[whichLevel].droid[whichDroid].isAlive = false;
					sys_removeDroidCPInfo ( whichDroid );
				}
		}
}

//-----------------------------------------------------------------------------
//
// Stop droid moving and movement

void gam_stopDroidMovement ( int whichLevel, int whichDroid )
//-----------------------------------------------------------------------------
{
	shipLevel[whichLevel].droid[whichDroid].velocity.x = 0.0f;
	shipLevel[whichLevel].droid[whichDroid].velocity.y = 0.0f;

	shipLevel[whichLevel].droid[whichDroid].currentSpeed = 0.0f;
}

//-----------------------------------------------------------------------------
//
// Kill an enemy droid
void gam_destroyDroid ( int whichLevel, int whichDroid )
//-----------------------------------------------------------------------------
{
	if ( true == shipLevel[whichLevel].droid[whichDroid].isExploding )
		return;

	if ( false == shipLevel[whichLevel].droid[whichDroid].isAlive )
		return;

	if ( 0 == shipLevel[whichLevel].numEnemiesAlive )
		{
			con_print ( true, false, "No enemies left on level. [ %i ]", whichLevel );
			return;
		}

	if ( shipLevel[whichLevel].droid[whichDroid].droidType < 6 )
		sys_playSound ( SND_EXPLODE_1, 0.0f, ALLEGRO_PLAYMODE_ONCE );
	else
		sys_playSound ( SND_EXPLODE_2, 0.0f, ALLEGRO_PLAYMODE_ONCE );

	shipLevel[whichLevel].droid[whichDroid].isExploding = true;
	shipLevel[whichLevel].droid[whichDroid].currentFrame = 0;
	shipLevel[whichLevel].droid[whichDroid].currentFrameDelay = 0.0f;
	shipLevel[whichLevel].droid[whichDroid].isStopped = true;
	gam_addToScore ( dataBaseEntry[shipLevel[whichLevel].droid[whichDroid].droidType].score );

	if ( shipLevel[currentLevel].droid[whichDroid].aStarPathIndex > -1 )
		gam_AStarRemovePath ( shipLevel[currentLevel].droid[whichDroid].aStarPathIndex, false );

	par_addEmitter ( shipLevel[whichLevel].droid[whichDroid].worldPos, PARTICLE_TYPE_EXPLOSION, -1 );

//
// TODO: Need extra checks here
//
	shipLevel[whichLevel].numEnemiesAlive--;

	if ( 0 == shipLevel[whichLevel].numEnemiesAlive )
		{
			gam_powerDownLevel ( whichLevel, true );
		}
}

//-----------------------------------------------------------------------------
//
// Increment the collision count - DELETE THIS
// this is used to see if the droid should start ignoring collisions after having too many

void gam_incrementCollisionCount ( int whichLevel, int droid1, int droid2 )
//-----------------------------------------------------------------------------
{
	static float localCounter = 0.0f;

	localCounter += 1.0f * thinkInterval; // space it out a bit

	if ( localCounter > 10.0f )
		{
			localCounter = 0.0f;
			shipLevel[whichLevel].droid[droid1].collisionCount++;
			shipLevel[whichLevel].droid[droid2].collisionCount++;

			if ( shipLevel[whichLevel].droid[droid1].collisionCount > 10.0f )
				{
					shipLevel[whichLevel].droid[droid1].ignoreCollisions = true;
					shipLevel[whichLevel].droid[droid1].ignoreCollisionsCounter = 1.0f;
				}

			if ( shipLevel[whichLevel].droid[droid2].collisionCount > 10.0f )
				{
					shipLevel[whichLevel].droid[droid2].ignoreCollisions = true;
					shipLevel[whichLevel].droid[droid2].ignoreCollisionsCounter = 1.0f;
				}
		}
}

//-----------------------------------------------------------------------------
//
// process ignore collosions

void gam_processIgnoreCollisions ( int whichLevel, int whichDroid )
//-----------------------------------------------------------------------------
{
	//	int collideIndex;

	if ( shipLevel[currentLevel].droid[whichDroid].collisionCount < int ( rand() % 5 ) + 3 )
		{
			return;
		}

	shipLevel[whichLevel].droid[whichDroid].ignoreCollisions = true;

	shipLevel[whichLevel].droid[whichDroid].ignoreCollisionsCounter -= 1.0f * thinkInterval;

	if ( shipLevel[whichLevel].droid[whichDroid].ignoreCollisionsCounter < 0.0f )
		{
			shipLevel[whichLevel].droid[whichDroid].ignoreCollisionsCounter = IGNORE_COLLISION_TIME;
			shipLevel[whichLevel].droid[whichDroid].ignoreCollisions = false;
			shipLevel[whichLevel].droid[whichDroid].collisionCount = 0;
			shipLevel[whichLevel].droid[whichDroid].hasCollided = false;
		}
}
