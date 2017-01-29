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

_bullet			bullet[MAX_NUM_BULLETS];
float		   	bulletTravelSpeed;
float      		bulletDisruptSpeed;

//-----------------------------------------------------------------------------
//
// Check if a disrupter hits an enemy droid
void bul_processDisrupter(int whichLevel, int whichBullet)
//-----------------------------------------------------------------------------
{
	//
	// Handle disrupter flash bullets
	//
	for (int j = 0; j != shipLevel[whichLevel].numDroids; j++)
		{
			if (true == shipLevel[whichLevel].droid[j].visibleToPlayer)
				{
					if (true == shipLevel[whichLevel].droid[j].isAlive)
						{
							if (false == shipLevel[whichLevel].droid[j].isExploding)
							{
								if (false == dataBaseEntry[shipLevel[whichLevel].droid[j].droidType].disrupterImmune)
									{
										drd_damageToDroid(whichLevel, j, DAMAGE_BULLET, bullet[whichBullet].sourceDroid);
									}
							}
						}
				}
		}
}

//-----------------------------------------------------------------------------
//
// Init the bullet array before using
void bul_initBullets()
//-----------------------------------------------------------------------------
{
	for (int i = 0; i != MAX_NUM_BULLETS; i++)
		{
			bullet[i].alive = false;
			bullet[i].angle = 0.0f;
			bullet[i].type = -1;
			bullet[i].currentAnimFrame = 0;
			bullet[i].animDelayCount = 0.0f;
			bullet[i].speed = bulletTravelSpeed;
		}
}

//-----------------------------------------------------------------------------
//
// Get the direction for a bullet to travel in - not the same as the current
// direction vector
cpVect bul_getPlayerBulletDirection()
//-----------------------------------------------------------------------------
{
	cpVect  bulletDirection;

	bulletDirection.x = 0;
	bulletDirection.y = 0;

	if (true == inputAction[gameLeft].currentlyDown)
		bulletDirection.x = -1;

	if (true == inputAction[gameRight].currentlyDown)
		bulletDirection.x = 1;

	if (true == inputAction[gameDown].currentlyDown)
		bulletDirection.y = 1;

	if (true == inputAction[gameUp].currentlyDown)
		bulletDirection.y = -1;

	return bulletDirection;
}

//-----------------------------------------------------------------------------
//
// Get the starting position of a newly fired bullet
// based on current velocity of the droid
cpVect bul_getStartingPosition(cpVect currentPos, int whichBullet, int bulletType)
//-----------------------------------------------------------------------------
{
	cpVect	bulletStart = {0.0f, 0.0f};
	int		bulletWidth, bulletHeight, widthDiff, heightDiff;

	bulletStart = currentPos;

	bulletStart.y -= TILE_SIZE / 2;

	return bulletStart;
}

//-----------------------------------------------------------------------------
//
// Create a new bullet
void bul_newBullet(cpVect sourcePos, cpVect destPos, int type, int sourceDroid)
//-----------------------------------------------------------------------------
{
	cpVect	bulletOffset;

	bulletOffset = {0.0f, 0.0f};

	for (int i = 0; i != MAX_NUM_BULLETS; i++)
		{
			if (false == bullet[i].alive)
				{
					if (-1 == sourceDroid)
						{
							bullet[i].travelDirection = bul_getPlayerBulletDirection();
							//
							// Needed for when firing without having any keys pressed - gliding to a stop
							//
							// Not needed - no longer can fire with no keys down - Remove?
							//
							if ((0 == bullet[i].travelDirection.x) && (0 == bullet[i].travelDirection.y))
								bullet[i].travelDirection = cpvsub(destPos, sourcePos);
						}
					else
						{
							bullet[i].travelDirection = cpvsub(destPos, sourcePos);
						}

					if (BULLET_TYPE_DISRUPTER != type)  // if bullet is not a disrupter
						{
							if (type == -1)     // Fix for when non bullet transferred droid shoots
								type = BULLET_TYPE_NORMAL; // Default to normal 001 bullet type

							bullet[i].currentAnimFrame = 0;
							bullet[i].travelDirection = cpvnormalize(bullet[i].travelDirection);
							bullet[i].worldPos = sourcePos;

							bullet[i].angle = cpvtoangle(bullet[i].travelDirection);

							bullet[i].size.x = al_get_bitmap_width(image[bul_getBulletImageByType(type)].image) / NUM_FRAMES_IN_BULLET;
							bullet[i].size.y = al_get_bitmap_height(image[bul_getBulletImageByType(type)].image);

							bullet[i].worldPos = bul_getStartingPosition(sourcePos, i, type);
							//
							// Create collision information
							//
							bullet[i].bulletPhysicsObject.body = cpSpaceAddBody(space,
								cpBodyNew(bulletMass, cpMomentForBox(bulletMass, bullet[i].size.x, bullet[i].size.y)));

							cpBodySetMass(bullet[i].bulletPhysicsObject.body, bulletMass);

							bullet[i].bulletPhysicsObject.shape = cpSpaceAddShape(space,
								cpBoxShapeNew(bullet[i].bulletPhysicsObject.body, bullet[i].size.x, bullet[i].size.y, 2));

							cpShapeSetCollisionType(bullet[i].bulletPhysicsObject.shape, PHYSIC_TYPE_BULLET);
							cpShapeSetUserData(bullet[i].bulletPhysicsObject.shape, i);	// Passed into collision routine

							sys_playSound(SND_LASER, 0.0f, ALLEGRO_PLAYMODE_ONCE);

							par_addEmitter(bullet[i].worldPos, PARTICLE_TYPE_TRAIL, i);
						}
					else
						{
							sys_playSound(SND_DISRUPTOR, 0.0f, ALLEGRO_PLAYMODE_ONCE);
							type = BULLET_TYPE_DISRUPTER;
							bullet[i].damageDone = false;
						}

					bullet[i].type = type;
					bullet[i].sourceDroid = sourceDroid;
					bullet[i].alive = true;

					return;
				}
		}
	con_print (true, false, "No more bullet slots.");
}

//-----------------------------------------------------------------------------
//
// Move all the alive bullets
void bul_moveBullet(int whichBullet)
//-----------------------------------------------------------------------------
{
	cpVect	tempVelocity;

	if (BULLET_TYPE_DISRUPTER == bullet[whichBullet].type)
		return;

	tempVelocity = cpvmult(bullet[whichBullet].travelDirection, bullet[whichBullet].speed);
	bullet[whichBullet].worldPos = cpvadd(bullet[whichBullet].worldPos, tempVelocity);

//
// TODO: use physics to move the bullet
//
	cpBodySetPosition(bullet[whichBullet].bulletPhysicsObject.body, bullet[whichBullet].worldPos);
	cpSpaceReindexShapesForBody(space, bullet[whichBullet].bulletPhysicsObject.body);
}

//-----------------------------------------------------------------------------
//
// Animate a bullet
void bul_animateBullet(int whichBullet)
//-----------------------------------------------------------------------------
{
	float bulletAnimSpeed = 5.0f;

	if (bullet[whichBullet].type == BULLET_TYPE_DISRUPTER)
		{
			bullet[whichBullet].animDelayCount += bulletDisruptSpeed * thinkInterval;
			if (bullet[whichBullet].animDelayCount > 1.0f)
				{
					bullet[whichBullet].animDelayCount = 0.0f;
					bullet[whichBullet].alive = false;
				}
		}
	else
		{
			bullet[whichBullet].animDelayCount += bulletAnimSpeed * thinkInterval;
			if (bullet[whichBullet].animDelayCount > 1.0f)
				{
					bullet[whichBullet].animDelayCount = 0.0f;
					bullet[whichBullet].currentAnimFrame++;
					if (bullet[whichBullet].currentAnimFrame > NUM_FRAMES_IN_BULLET)
						bullet[whichBullet].currentAnimFrame = 0;
				}
		}
}

//-----------------------------------------------------------------------------
//
// Pass in bullet type and get index into image array
int bul_getBulletImageByType(int whichType)
//-----------------------------------------------------------------------------
{
	switch (whichType)
		{
			case BULLET_TYPE_NORMAL:
				return bulletType1;
				break;

			case BULLET_TYPE_SINGLE:
				return bulletType2;
				break;

			case BULLET_TYPE_DOUBLE:
				return bulletType3;
				break;
		}
	return -1;
}

//-----------------------------------------------------------------------------
//
// Draw a valid bullet
void bul_drawBullet(int whichBullet)
//-----------------------------------------------------------------------------
{
	int		bulletImage = 0;

	if (BULLET_TYPE_DISRUPTER == bullet[whichBullet].type)
		{
			al_draw_filled_rectangle(0.0f, 0.0f, winWidth, winHeight, al_map_rgba_f(1.0f, 1.0f, 1.0f, 0.3f));
		}
	else
		{
			bulletImage = bul_getBulletImageByType(bullet[whichBullet].type);
			//
			// Draw the bullet sprite
			gam_drawSprite(bullet[whichBullet].currentAnimFrame, bulletImage, sys_worldToScreen(bullet[whichBullet].worldPos, DROID_BODY_SIZE), bullet[whichBullet].angle, al_map_rgb_f(1.0f, 1.0f, 1.0f));
		}
}

//-----------------------------------------------------------------------------
//
// Process all the alive bullets
void bul_processBullets()
//-----------------------------------------------------------------------------
{
	for (int i = 0; i != MAX_NUM_BULLETS; i++)
		{
			if (true == bullet[i].alive)
				{
					if (BULLET_TYPE_DISRUPTER == bullet[i].type)    // disrupter flash
						{
							if (false == bullet[i].damageDone)
								{
									bul_processDisrupter(currentLevel, i);
									bullet[i].damageDone = true;
								}
						}
					else
						bul_moveBullet(i);	// Disrupter bullets don't move

					bul_animateBullet(i);
				}
		}
}

//-----------------------------------------------------------------------------
//
// Draw all the valid bullets
void bul_drawBullets()
//-----------------------------------------------------------------------------
{
	for (int i = 0; i != MAX_NUM_BULLETS; i++)
		{
			if (true == bullet[i].alive)
				{
					bul_drawBullet(i);
				}
		}
}
