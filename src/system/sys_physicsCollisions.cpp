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

cpCollisionHandler 		*handlerWallBullet;
cpCollisionHandler		*handlerEnemyBullet;
cpCollisionHandler		*handlerEnemyPlayer;
cpCollisionHandler		*handlerEnemyEnemy;
cpCollisionHandler		*handlerPlayerBullet;
cpCollisionHandler		*handlerWallPlayer;
cpCollisionHandler		*handlerDoorBullet;
cpCollisionHandler		*handlerIgnoreParticle;
cpCollisionHandler		*handlerIgnoreParticleEnemy;
cpCollisionHandler		*handlerBulletBullet;

//-------------------------------------------------------------------
//
// Routines to handle collision between different shapes
//
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//
// Safely remove the bullet body and shape from the space
//
// Pass in -1 to add a spark at the removed location
// Otherwise pass in index of second bullet to remove from bullet to bullet
static void postStepRemoveBullet ( cpSpace *space, cpShape *shape, int *unused )
//-------------------------------------------------------------------
{
	cpDataPointer		bulletIndexDataPointer;
	intptr_t			bulletIndex;
	int					controlParam;

	//
	// deference pointer to data
	controlParam = *(static_cast<int*>(unused));
	//
	// Remove the variable assigned in the function setting this callback
	delete[] unused;

	bulletIndexDataPointer = cpShapeGetUserData ( shape );
	//
	// Cast cpDataPointer to an INT type
	bulletIndex = (intptr_t)bulletIndexDataPointer;

	
//	con_print ( true, false, "Remove bullet index [ %i ]", bulletIndex );

	if (cpTrue == cpSpaceContainsShape(space, bullet[bulletIndex].bulletPhysicsObject.shape))
	{
		cpSpaceRemoveShape ( space, bullet[bulletIndex].bulletPhysicsObject.shape );
		cpShapeFree ( bullet[bulletIndex].bulletPhysicsObject.shape );
	}
	else
		printf("ERROR: Attempted to remove non existant shape - bullet [ %Ii ]\n", bulletIndex);

	if (cpTrue == cpSpaceContainsBody(space, bullet[bulletIndex].bulletPhysicsObject.body))
	{
		cpSpaceRemoveBody ( space, bullet[bulletIndex].bulletPhysicsObject.body );
		cpBodyFree ( bullet[bulletIndex].bulletPhysicsObject.body );
	}
	else
		printf("ERROR: Attempted to remove non existant body - bullet [ %Ii ]\n", bulletIndex);

	bullet[bulletIndex].alive = false;

	par_removeEmitter ( bulletIndex );

	if ( -1 == controlParam )
		{
			par_addEmitter ( bullet[bulletIndex].worldPos, PARTICLE_TYPE_SPARK, -1 );
			return;
		}

	if ( controlParam > -1 )
		{
			// This is the index of the second bullet to remove
			par_addEmitter ( bullet[controlParam].worldPos, PARTICLE_TYPE_SPARK, -1 );

			if (cpTrue == cpSpaceContainsShape(space, bullet[controlParam].bulletPhysicsObject.shape))
			{
				cpSpaceRemoveShape ( space, bullet[controlParam].bulletPhysicsObject.shape );
				cpSpaceRemoveBody ( space, bullet[controlParam].bulletPhysicsObject.body );

				cpShapeFree ( bullet[controlParam].bulletPhysicsObject.shape );
				cpBodyFree ( bullet[controlParam].bulletPhysicsObject.body );
			}

			bullet[controlParam].alive = false;
			bullet[controlParam].particleIndex = -1;

			par_removeEmitter ( controlParam );
		}
}

//-------------------------------------------------------------------
//
// Handle collision between PHYSIC_TYPE_WALL and PHYSIC_TYPE_PLAYER_BULLET
cpBool handleCollisionWallPlayerBullet ( cpArbiter *arb, cpSpace *space, int *unused )
//-------------------------------------------------------------------
{
	// Get the cpShapes involved in the collision
	//
	// The order is A = WALL and B = BULLET
	//
	cpShape *a, *b;
	cpArbiterGetShapes ( arb, &a, &b );

	cpContactPointSet set = cpArbiterGetContactPointSet ( arb );
	cpVect p;

	for ( int i=0; i < set.count; i++ )
		{
			p = set.points[i].pointA;
		}

	// Alternatively you can use the CP_ARBITER_GET_SHAPES() macro
	// It defines and sets the variables for you.
	//CP_ARBITER_GET_SHAPES(arb, a, b);

	// Add a post step callback to safely remove the body and shape from the space.
	// Calling cpSpaceRemove*() directly from a collision handler callback can cause crashes.

	int *passValue = new int();
	*passValue= -1;

	cpSpaceAddPostStepCallback ( space, ( cpPostStepFunc ) postStepRemoveBullet, b, passValue );

	// The object is dead, don’t process the collision further
	return cpFalse;
}

//-------------------------------------------------------------------
//
// Post-Step callback for hitting droid - needed in post-step so when its
// destroyed the shape and body are removed from the world
static void handleDamageDroidBullet ( cpSpace *space, cpShape *shape, int *unused )
//-------------------------------------------------------------------
{
	int 				bulletIndex;
	cpDataPointer		droidIndexDataPointer;
	intptr_t			droidIndex;
	//
	// deference pointer to data
	bulletIndex = *(static_cast<int*>(unused));

	droidIndexDataPointer = cpShapeGetUserData ( shape );
	//
	// Cast cpDataPointer to an INT type
	droidIndex = (intptr_t)droidIndexDataPointer;

	if ( false == shipLevel[currentLevel].droid[droidIndex].isExploding )
		drd_damageToDroid ( currentLevel, droidIndex, DAMAGE_BULLET, bullet[bulletIndex].sourceDroid );

	delete[] unused;
}

//-------------------------------------------------------------------
//
// Handle bullet hitting enemy droid - could be another enemy or player bullet
bool handleCollisionEnemyBullet ( cpArbiter *arb, cpSpace *space, int *unused )
//-------------------------------------------------------------------
{
	// Get the cpShapes involved in the collision
	//
	// The order is A = ENEMY and B = BULLET
	//
	cpShape 			*a, *b;
	cpDataPointer		aDataPointer, bDataPointer;
	intptr_t			whichDroid;
	intptr_t 			bulletIndex;

	cpArbiterGetShapes ( arb, &a, &b );

	aDataPointer = cpShapeGetUserData ( a );
	bDataPointer = cpShapeGetUserData ( b );
	//
	// Cast cpDataPointer to an type large enough to hold a 64 bit pointer
	// 
	whichDroid = (intptr_t)aDataPointer;
	bulletIndex = (intptr_t)bDataPointer;

	//
	// Is this bullet belongs to this droid stop collision
	//
	if ( whichDroid == bullet[bulletIndex].sourceDroid )
		{
			return cpFalse;
		}

	int *passValue = new int();
	*passValue= bulletIndex;

	int *removeBullet = new int();
	*removeBullet = -2;

	cpSpaceAddPostStepCallback ( space, ( cpPostStepFunc ) handleDamageDroidBullet, a, passValue );
	//
	// Remove the bullet after the collision
	cpSpaceAddPostStepCallback ( space, ( cpPostStepFunc ) postStepRemoveBullet, b, removeBullet );
	return 0;	// should this be true or false
}

//-------------------------------------------------------------------
//
// Post-Step callback for hitting droid - needed in post-step so when its
// destroyed the shape and body are removed from the world
static void handleDamageDroidCollision ( cpSpace *space, cpShape *shape, int *unused )
//-------------------------------------------------------------------
{
	intptr_t			damageSource;
	intptr_t			whichDroid;
	cpDataPointer		whichDroidDataPointer;

	//
	// deference pointer to data
	damageSource = (intptr_t)unused;

	whichDroidDataPointer = cpShapeGetUserData(shape);
	whichDroid = (intptr_t)whichDroidDataPointer;

	drd_damageToDroid ( currentLevel, whichDroid, damageSource, -1 );

	delete[] unused;
}

//-------------------------------------------------------------------
//
// Collision between PLAYER and DROID - check transfer status
// Continue to post solve if TRANSFER == FALSE
bool handleCollisionTransferCheck ( cpArbiter *arb, cpSpace *space, int *unused )
//-------------------------------------------------------------------
{
	// Get the cpShapes involved in the collision
	//
	// The order is A = ENEMY and B = PLAYER if userData == -1
	//
	//  Or A = ENEMY and B = ENEMY
	cpShape 			*a, *b;
	cpDataPointer		aDataPointer, bDataPointer;
	intptr_t 			whichDroid_A;
	intptr_t 			whichDroid_B;
	cpVect				stopVelocity;

	cpArbiterGetShapes ( arb, &a, &b );
	aDataPointer = cpShapeGetUserData ( a );
	bDataPointer = cpShapeGetUserData ( b );
	
	whichDroid_A = (intptr_t)aDataPointer;
	whichDroid_B = (intptr_t)bDataPointer;

	if ( -1 == whichDroid_B )	// Is B the player
		{
			if ( false == inTransferMode )
				return cpTrue;	// Continue processing collision
		}
	//
	// Ignore collision and start transfer process
	// Transfer into Droid
	//
	stopVelocity.x = 0.0f;
	stopVelocity.y = 0.0f;
	cpBodySetVelocity ( shipLevel[currentLevel].droid[whichDroid_A].body, stopVelocity );
	trn_startTransferMode ( whichDroid_A );
	sys_changeMode ( MODE_TRANSFER_INTRO, true );

	return cpFalse;	// Don't continue processing collision
}

//-------------------------------------------------------------------
//
// Collision between player and wall
// Used to ignore collisions for debugging
//
bool handleCollisionPlayerWall ( cpArbiter *arb, cpSpace *space, int *unused )
//-------------------------------------------------------------------
{
	if ( false == useCollisionDetection )
		return cpFalse;
	else
		return cpTrue;
}

//-------------------------------------------------------------------
//
// Handle PLAYER hitting ENEMY droid - could be another enemy or player bullet
void handleCollisionDroidToDroid ( cpArbiter *arb, cpSpace *space, int *unused )
//-------------------------------------------------------------------
{
	// Get the cpShapes involved in the collision
	//
	// The order is A = ENEMY and B = PLAYER if userData == -1
	//
	//  Or A = ENEMY and B = ENEMY
	cpShape 			*a, *b;
	cpDataPointer		aDataPointer, bDataPointer;	
	intptr_t			whichDroid_A;
	intptr_t			whichDroid_B;

	cpArbiterGetShapes ( arb, &a, &b );
	
	aDataPointer = cpShapeGetUserData ( a );
	bDataPointer = cpShapeGetUserData ( b );
	
	whichDroid_A = (intptr_t)aDataPointer;
	whichDroid_B = (intptr_t)bDataPointer;

	int *passValue = new int();

	if ( -1 == whichDroid_B )	// Is B the player
		{
			//
			// Process player vs enemy collision
			if ( false == shipLevel[currentLevel].droid[whichDroid_A].isExploding )
				{
					*passValue= DAMAGE_COLLISION;
					cpSpaceAddPostStepCallback ( space, ( cpPostStepFunc ) handleDamageDroidCollision, a, passValue );
					sys_playSound ( SND_COLLIDE_1, 0.0f, ALLEGRO_PLAYMODE_ONCE );
				}
			else	// Player collided with exploding sprite - take damage
				{
					if ( whichDroid_A != droidTransferedIntoIndex )
						// Ignore explosion if colliding with recently transferred droid
						{
							*passValue= DAMAGE_EXPLOSION;
							cpSpaceAddPostStepCallback ( space, ( cpPostStepFunc ) handleDamageDroidCollision, a, passValue );
							sys_playSound ( SND_DAMAGE, 0.0f, ALLEGRO_PLAYMODE_ONCE );
						}
				}
		}
	else
		{
			//
			// Droid collided with another droid
			//
			shipLevel[currentLevel].droid[whichDroid_A].collisionCount++;
			shipLevel[currentLevel].droid[whichDroid_B].collisionCount++;

			shipLevel[currentLevel].droid[whichDroid_A].hasCollided = true;
			shipLevel[currentLevel].droid[whichDroid_B].hasCollided = true;

			shipLevel[currentLevel].droid[whichDroid_A].collidedWith = whichDroid_B;
			shipLevel[currentLevel].droid[whichDroid_B].collidedWith = whichDroid_A;

			if ( true == shipLevel[currentLevel].droid[whichDroid_A].isExploding )
				{
					drd_damageToDroid ( currentLevel, whichDroid_B, DAMAGE_EXPLOSION, whichDroid_A );
					sys_playSound ( SND_DAMAGE, 0.0f, ALLEGRO_PLAYMODE_ONCE );
				}
			else
				{
					drd_damageToDroid ( currentLevel, whichDroid_B, DAMAGE_COLLISION, whichDroid_A );
					sys_playSound ( SND_COLLIDE_1, 0.0f, ALLEGRO_PLAYMODE_ONCE );
				}

			if ( true == shipLevel[currentLevel].droid[whichDroid_B].isExploding )
				{
					drd_damageToDroid ( currentLevel, whichDroid_A, DAMAGE_EXPLOSION, whichDroid_B );
					sys_playSound ( SND_DAMAGE, 0.0f, ALLEGRO_PLAYMODE_ONCE );
				}
			else
				{
					drd_damageToDroid ( currentLevel, whichDroid_A, DAMAGE_COLLISION, whichDroid_B );
					sys_playSound ( SND_COLLIDE_1, 0.0f, ALLEGRO_PLAYMODE_ONCE );
				}
		}
}


//-------------------------------------------------------------------
//
// Check before collision needs to be handled if it should continue
//
bool handleCollisionDroidCheck ( cpArbiter *arb, cpSpace *space, int *unused )
//-------------------------------------------------------------------
{
	// Get the cpShapes involved in the collision
	//
	// The order is A = ENEMY and B = PLAYER if userData == -1
	//
	//  Or A = ENEMY and B = ENEMY
	cpShape 			*a, *b;
	cpDataPointer		aDataPointer, bDataPointer;
	intptr_t 			whichDroid_A;
	intptr_t 			whichDroid_B;

	cpArbiterGetShapes ( arb, &a, &b );
	
	aDataPointer = cpShapeGetUserData ( a );
	bDataPointer = cpShapeGetUserData ( b );
	
	whichDroid_A = (intptr_t)aDataPointer;
	whichDroid_B = (intptr_t)bDataPointer;
	
	if ( ( true == shipLevel[currentLevel].droid[whichDroid_A].ignoreCollisions ) ||
	        ( true == shipLevel[currentLevel].droid[whichDroid_B].ignoreCollisions ) )
		return cpFalse;

	return cpTrue;
}

//-------------------------------------------------------------------
//
// Collision when a bullet hits a bullet - remove both
bool handleBulletBullet ( cpArbiter *arb, cpSpace *space, int *unused )
//-------------------------------------------------------------------
{
	// Get the shapes involved in the collision
	cpShape 			*a, *b;
	cpDataPointer		bulletIndexDataPointer;
	intptr_t 			bulletIndex2;

	cpArbiterGetShapes ( arb, &a, &b );
	bulletIndexDataPointer = cpShapeGetUserData ( b );
	
	bulletIndex2 = (intptr_t)bulletIndexDataPointer;

	int *passValue = new int();
	*passValue = bulletIndex2;

	// Add a post step callback to safely remove the body and shape from the space.
	// Calling cpSpaceRemove*() directly from a collision handler callback can cause crashes.
	cpSpaceAddPostStepCallback ( space, ( cpPostStepFunc ) postStepRemoveBullet, a, passValue );

	return cpTrue;
}

//-------------------------------------------------------------------
//
// Collision handler to ignore player hitting own bullets
bool handlePlayerBullet ( cpArbiter *arb, cpSpace *space, int *unused )
//-------------------------------------------------------------------
{
	// Get the cpShapes involved in the collision
	//
	// The order is A = ENEMY and B = BULLET
	//
	cpShape 			*a, *b;
	cpDataPointer		bulletIndexDataPointer;
	intptr_t			bulletIndex;

	cpArbiterGetShapes ( arb, &a, &b );
	bulletIndexDataPointer = cpShapeGetUserData ( b );
	
	bulletIndex = (intptr_t)bulletIndexDataPointer;

	//
	// Does this bullet belong to the player - if so ignore the collision
	//
	if ( -1 == bullet[bulletIndex].sourceDroid )
		{
			return cpFalse;
		}
	//
	// Otherwise it's an enemy bullet that hit the player
	//

	// Add a post step callback to safely remove the body and shape from the space.
	// Calling cpSpaceRemove*() directly from a collision handler callback can cause crashes.
	int *passValue = new int();
	*passValue= -2;

	cpSpaceAddPostStepCallback ( space, ( cpPostStepFunc ) postStepRemoveBullet, b, passValue );
	//
	// Damage to player
	//
	gam_doDamageToPlayer ( DAMAGE_BULLET, bullet[bulletIndex].sourceDroid );

	return cpTrue;
}

//-------------------------------------------------------------------
//
// Collision handler to ignore particles hitting player and enemies
bool handleIgnoreParticles ( cpArbiter *arb, cpSpace *space, int *unused )
//-------------------------------------------------------------------
{
	return cpFalse;
}

//-------------------------------------------------------------------
//
// Handle bullet hitting door sensor
bool handleDoorBullet ( cpArbiter	*arb, cpSpace *space, int *unused )
//-------------------------------------------------------------------
{
	// Get the cpShapes involved in the collision
	//
	// The order is A = DOOR and B = BULLET
	//
	cpShape 			*a, *b;
	cpDataPointer		whichDoorDataPointer;
	intptr_t			whichDoor;

	cpArbiterGetShapes ( arb, &a, &b );	
	
	whichDoorDataPointer = cpShapeGetUserData ( a );
	whichDoor = (intptr_t)whichDoorDataPointer;

	int *passValue = new int();
	switch ( doorTrigger[whichDoor].currentFrame )
		{
			case DOOR_ACROSS_OPENED:
			case DOOR_UP_OPENED:
				return cpTrue;
				break;

			default:
				*passValue = -1;
				cpSpaceAddPostStepCallback ( space, ( cpPostStepFunc ) postStepRemoveBullet, b, passValue );
				return cpFalse;
				break;
		}
}

//-------------------------------------------------------------------
//
// Setup all the collision handlers
void sys_setupCollisionHandlers()
//-------------------------------------------------------------------
{
	//
	// Handle PLAYER hitting a WALL
	//
	handlerWallPlayer = cpSpaceAddCollisionHandler ( space, PHYSIC_TYPE_PLAYER, PHYSIC_TYPE_WALL );
	handlerWallPlayer->beginFunc = (cpCollisionBeginFunc)handleCollisionPlayerWall;
	//
	// Handle BULLET hitting a WALL
	//
	handlerWallBullet = cpSpaceAddCollisionHandler ( space, PHYSIC_TYPE_WALL, PHYSIC_TYPE_BULLET );
	handlerWallBullet->postSolveFunc = (cpCollisionPostSolveFunc)handleCollisionWallPlayerBullet;

	//
	// Handle BULLET hitting an ENEMY
	//
	handlerEnemyBullet = cpSpaceAddCollisionHandler ( space, PHYSIC_TYPE_ENEMY, PHYSIC_TYPE_BULLET );
	handlerEnemyBullet->postSolveFunc = (cpCollisionPostSolveFunc)handleCollisionEnemyBullet;

	//
	// Handle collision between PLAYER and ENEMY
	//
	handlerEnemyPlayer = cpSpaceAddCollisionHandler ( space, PHYSIC_TYPE_ENEMY, PHYSIC_TYPE_PLAYER );
	handlerEnemyPlayer->beginFunc = (cpCollisionBeginFunc)handleCollisionTransferCheck;
	handlerEnemyPlayer->postSolveFunc = (cpCollisionPostSolveFunc)handleCollisionDroidToDroid;

	//
	// Handle collision between ENEMY and ENEMY
	//
	handlerEnemyEnemy = cpSpaceAddCollisionHandler ( space, PHYSIC_TYPE_ENEMY, PHYSIC_TYPE_ENEMY );
	handlerEnemyEnemy->beginFunc = (cpCollisionBeginFunc)handleCollisionDroidCheck;
	handlerEnemyEnemy->postSolveFunc = (cpCollisionPostSolveFunc)handleCollisionDroidToDroid;

	//
	// Stop player physics being impacted by bullet shape
	//
	handlerPlayerBullet = cpSpaceAddCollisionHandler ( space, PHYSIC_TYPE_PLAYER, PHYSIC_TYPE_BULLET );
	handlerPlayerBullet->beginFunc = (cpCollisionBeginFunc)handlePlayerBullet;

	//
	// Stop bullets going through closed doors
	//
	handlerDoorBullet = cpSpaceAddCollisionHandler ( space, PHYSIC_TYPE_DOOR, PHYSIC_TYPE_BULLET );
	handlerDoorBullet->beginFunc = (cpCollisionBeginFunc)handleDoorBullet;
	//
	// Handle when a bullet hits a bullet
	//
	handlerBulletBullet = cpSpaceAddCollisionHandler ( space, PHYSIC_TYPE_BULLET, PHYSIC_TYPE_BULLET );
	handlerBulletBullet->beginFunc = (cpCollisionBeginFunc)handleBulletBullet;
	//
	// Ignore particles hitting players and enemies
	//
	handlerIgnoreParticle = cpSpaceAddCollisionHandler ( space, PHYSIC_TYPE_PLAYER, PHYSIC_TYPE_PARTICLE );
	handlerIgnoreParticleEnemy = cpSpaceAddCollisionHandler ( space, PHYSIC_TYPE_ENEMY,  PHYSIC_TYPE_PARTICLE );
	handlerIgnoreParticle->beginFunc = (cpCollisionBeginFunc)handleIgnoreParticles;
	handlerIgnoreParticleEnemy->beginFunc = (cpCollisionBeginFunc)handleIgnoreParticles;
}
