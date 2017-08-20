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
#include <math.h>

#define DEBUG_PHYSICS 1

vector<_physicObject>		solidWalls;
_physicObject				playerPhysicsObject;

cpSpace			*space = NULL;
cpVect			shipGravity;
float			shipDamping;		// Set from startup script
float			collisionSlop;		// Set from startup script
float			wallFriction;		// Set from startup script
float			wallRadius;			// Set from startup script

float			playerMass;			// Set from startup script
float			playerRadius;		// Set from startup script
float			playerFriction;		// Set from startup script
float			playerElastic;		// Set from startup script
float			bulletMass;			// Set from startup script

bool			debugDrawPhysics = false;
bool			processedPhysics = false;

//-------------------------------------------------------------------
//
// Set the player physics position in the world
void sys_setPlayerPhysicsPosition ( cpVect newPosition )
//-------------------------------------------------------------------
{
	if ( playerPhysicsObject.body == NULL )
		return;

	cpBodySetPosition ( playerPhysicsObject.body, newPosition );
}

//-------------------------------------------------------------------
//
// Destroy player physics
void sys_destroyPlayerPhysics()
//-------------------------------------------------------------------
{
#ifdef DEBUG_PHYSICS
	con_print ( true, false, "Removing player physics shape and body." );
#endif

	if (cpTrue == cpSpaceContainsShape ( space, playerPhysicsObject.shape ) )
		{
			cpSpaceRemoveShape ( space, playerPhysicsObject.shape );
			cpShapeFree ( playerPhysicsObject.shape );
			playerPhysicsObject.shape = NULL;
		}

	if (cpTrue == cpSpaceContainsBody ( space, playerPhysicsObject.body ) )
		{
			cpSpaceRemoveBody ( space, playerPhysicsObject.body );
			cpBodyFree ( playerPhysicsObject.body );
			playerPhysicsObject.body = NULL;
		}
}

//-------------------------------------------------------------------
//
// Set the player physics mass after transferring to a new droid
void sys_setPlayerMass ( float newMass )
//-------------------------------------------------------------------
{
	if ( newMass == playerMass )
		return;

	playerMass = newMass;

	cpBodySetMass ( playerPhysicsObject.body, playerMass );
}

//-------------------------------------------------------------------
//
// Setup player droid physics information
void sys_setupPlayerPhysics()
//-------------------------------------------------------------------
{
	cpVect	playerOffset;

	playerOffset = {0.0f, 0.0f};

	if ( playerPhysicsObject.body != NULL )
		return;

	playerPhysicsObject.body = cpSpaceAddBody ( space, cpBodyNew ( playerMass,  cpMomentForCircle ( playerMass, 0.0f, playerRadius, playerOffset ) ) );
	cpBodySetMass ( playerPhysicsObject.body, playerMass );

	playerPhysicsObject.shape = cpSpaceAddShape ( space, cpCircleShapeNew ( playerPhysicsObject.body, playerRadius, playerOffset ) );
	cpShapeSetFriction ( playerPhysicsObject.shape, playerFriction );
	cpShapeSetElasticity ( playerPhysicsObject.shape, playerElastic );
	cpShapeSetCollisionType ( playerPhysicsObject.shape, PHYSIC_TYPE_PLAYER );
	cpShapeSetUserData ( playerPhysicsObject.shape, (cpDataPointer)-1 );	// Passed into collision routine
}

//-------------------------------------------------------------------
//
// Setup Physics engine - run once
bool sys_setupPhysicsEngine()
//--------------------------------------------------------------------
{
	shipGravity = cpv ( 0, 0 );
	space = cpSpaceNew();
	cpSpaceSetGravity ( space, shipGravity );
	cpSpaceSetDamping ( space, shipDamping );
	// Amount of overlap between shapes that is allowed.
	cpSpaceSetCollisionSlop ( space, collisionSlop );

	sys_setupPlayerPhysics();

	sys_setupCollisionHandlers();

	return true;
}

//-------------------------------------------------------------------
//
// Cleanup Physics engine - run once
void sys_freePhysicsEngine()
//--------------------------------------------------------------------
{
	if ( NULL == space )
		return;

	cpSpaceFree ( space );
}

//-----------------------------------------------------------------------------------
//
// Remove the physics information associated to an enemy droid
void sys_removeDroidCPInfo ( int whichDroid )
//-----------------------------------------------------------------------------------
{
#ifdef DEBUG_PHYSICS
	con_print ( true, false, "Remove physics objects for droid [ %i ]", whichDroid );
#endif

	if (cpTrue == cpSpaceContainsShape ( space, shipLevel[currentLevel].droid[whichDroid].shape ) )
		{
			cpSpaceRemoveShape ( space, shipLevel[currentLevel].droid[whichDroid].shape );
			cpShapeFree ( shipLevel[currentLevel].droid[whichDroid].shape );
		}

	if (cpTrue == cpSpaceContainsBody ( space, shipLevel[currentLevel].droid[whichDroid].body ) )
		{
			cpSpaceRemoveBody ( space, shipLevel[currentLevel].droid[whichDroid].body );
			cpBodyFree ( shipLevel[currentLevel].droid[whichDroid].body );
		}
}

//-------------------------------------------------------------------
//
// Destroy all the bodies and shapes for this level
void sys_destroyPhysicObjects ( int whichLevel )
//-------------------------------------------------------------------
{
	for ( int i = 0; i != shipLevel[whichLevel].numLineSegments; i++ )
		{
			if (cpTrue == cpSpaceContainsShape ( space, solidWalls[i].shape ) )
				{
					cpSpaceRemoveShape ( space, solidWalls[i].shape );
					cpShapeFree ( solidWalls[i].shape );
				}

			if (cpTrue == cpSpaceContainsBody ( space, solidWalls[i].body ) )
				{
					cpSpaceRemoveBody ( space, solidWalls[i].body );
					cpBodyFree ( solidWalls[i].body );
				}
		}
	solidWalls.clear();

	for ( int i = 0; i != shipLevel[whichLevel].numDroids; i++ )
		{
			if ( true == shipLevel[whichLevel].droid[i].isAlive )
				{
					sys_removeDroidCPInfo ( i );
				}
		}

	for ( int i = 0; i != numDoorsOnLevel; i++ )
		{
			if (cpTrue == cpSpaceContainsShape ( space, doorTrigger[i].physics.shape ) )
				{
					cpSpaceRemoveShape ( space, doorTrigger[i].physics.shape );
					cpShapeFree ( doorTrigger[i].physics.shape );
				}

			if ( cpTrue == cpSpaceContainsBody ( space, doorTrigger[i].physics.body ) )
				{
					cpSpaceRemoveBody ( space, doorTrigger[i].physics.body );
					cpBodyFree ( doorTrigger[i].physics.body );
				}
		}
}

//-------------------------------------------------------------------
//
// Destroy all the physics objects ready to restart the game
void sys_clearAllPhysics()
//-------------------------------------------------------------------
{
	//
	// Clear door sensors, droid shapes and wall segments
	//
	sys_destroyPhysicObjects ( currentLevel );
	//
	// Clear player
	//
	sys_destroyPlayerPhysics();
}

//-------------------------------------------------------------------
//
// Create the physics bodies and shapes for the enemy droids
void sys_createEnemyPhysics()
//-------------------------------------------------------------------
{
	int weight;
	float massWeight;

	for ( intptr_t i = 0; i != shipLevel[currentLevel].numDroids; i++ )
		{
			if ( true == shipLevel[currentLevel].droid[i].isAlive )
				{
					weight = atoi ( dataBaseEntry[shipLevel[currentLevel].droid[i].droidType].weight );
					massWeight = weight / 100.0f;
					massWeight += playerMass;

					shipLevel[currentLevel].droid[i].body = cpSpaceAddBody ( space, cpBodyNew ( massWeight,  cpMomentForCircle ( massWeight, 0.0f, playerRadius, cpvzero ) ) );

					cpBodySetMass ( shipLevel[currentLevel].droid[i].body, massWeight );

					shipLevel[currentLevel].droid[i].shape = cpSpaceAddShape ( space, cpCircleShapeNew ( shipLevel[currentLevel].droid[i].body, playerRadius, cpvzero ) );
					cpShapeSetFriction ( shipLevel[currentLevel].droid[i].shape, playerFriction );
					cpShapeSetElasticity ( shipLevel[currentLevel].droid[i].shape, playerElastic );

					cpShapeSetCollisionType ( shipLevel[currentLevel].droid[i].shape, PHYSIC_TYPE_ENEMY );

					cpBodySetPosition ( shipLevel[currentLevel].droid[i].body, shipLevel[currentLevel].droid[i].worldPos );

					printf ("worldpos [ %Ii ] [ %3.3f %3.3f ]\n", i, shipLevel[currentLevel].droid[i].worldPos.x, shipLevel[currentLevel].droid[i].worldPos.y);

					cpShapeSetUserData ( shipLevel[currentLevel].droid[i].shape, (void *)i );	// Passed into collision routine
				}
		}
}

//-------------------------------------------------------------------
//
// Show the line segments for solid walls
void sys_debugLineSegments()
//-------------------------------------------------------------------
{
	cpVect 			wallStart, wallFinish;
	ALLEGRO_COLOR	lineColor;

	lineColor = al_map_rgb ( 255, 0, 0 );

	for ( int i = 0; i != shipLevel[currentLevel].numLineSegments; i++ )
		{
			wallStart = shipLevel[currentLevel].lineSegments[i].start;
			wallFinish = shipLevel[currentLevel].lineSegments[i].finish;

			wallStart = sys_worldToScreen ( wallStart, 200 );
			wallFinish = sys_worldToScreen ( wallFinish, 200 );

			al_draw_line ( wallStart.x, wallStart.y, wallFinish.x, wallFinish.y, lineColor, 2 );
		}
}

//-------------------------------------------------------------------
//
// Create the solid walls for this level
void sys_createSolidWalls()
//-------------------------------------------------------------------
{
	cpVect 			wallStart, wallFinish;
	_physicObject	tempWall;

	if ( 0 == shipLevel[currentLevel].numLineSegments )
		return;

	solidWalls.reserve ( shipLevel[currentLevel].numLineSegments );

	for ( intptr_t i = 0; i != shipLevel[currentLevel].numLineSegments; i++ )
		{
			wallStart = shipLevel[currentLevel].lineSegments[i].start;
			wallFinish = shipLevel[currentLevel].lineSegments[i].finish;

			tempWall.body = cpBodyNewStatic();
			tempWall.shape = cpSegmentShapeNew ( tempWall.body, wallStart, wallFinish, wallRadius );
			solidWalls.push_back ( tempWall );
			cpSpaceAddShape ( space, solidWalls[i].shape );
			cpShapeSetFriction ( solidWalls[i].shape, wallFriction );

			cpShapeSetCollisionType ( solidWalls[i].shape, PHYSIC_TYPE_WALL );

			cpShapeSetUserData ( solidWalls[i].shape, (cpDataPointer)i );
		}

	sys_createEnemyPhysics();

	cpSpaceReindexStatic ( space );
}

//-----------------------------------------------------------------------------
//
// Set scrolling variables from playerWorldPos
void gam_updateScrollingVars ( bool getPlayerPos )
//-----------------------------------------------------------------------------
{
	int posX, posY;

	cpBodySetForce ( playerPhysicsObject.body, playerVelocity );

	playerWorldPos = cpBodyGetPosition ( playerPhysicsObject.body );
	//
	// Coords used to display the player sprite
	//
	playerScreenPos.x = ( winWidth / 2 ) - ( SPRITE_SIZE / 2 );
	playerScreenPos.y = ( winHeight / 2 ) - ( SPRITE_SIZE / 2 );
	//
	// Scroll the screen according to the player position
	//
	tilePosX = abs ( ( int ) playerWorldPos.x / TILE_SIZE ) - ( ( winWidth / TILE_SIZE ) / 2 );
	tilePosY = abs ( ( int ) playerWorldPos.y / TILE_SIZE ) - ( ( winHeight / TILE_SIZE ) / 2 );

	pixelX = fmod ( playerWorldPos.x, ( float ) TILE_SIZE );
	pixelY = fmod ( playerWorldPos.y, ( float ) TILE_SIZE );

	//
	// Get the tile the player is over
	//
	playerWorldMiddlePos.x = ( playerWorldPos.x - ( TILE_SIZE / 2 ) ) + ( SPRITE_SIZE / 2 );
	playerWorldMiddlePos.y = playerWorldPos.y + ( SPRITE_SIZE / 2 );

	posX = playerWorldMiddlePos.x / TILE_SIZE;
	posY = playerWorldMiddlePos.y / TILE_SIZE;

	playerOverTile = shipLevel[currentLevel].tiles[ ( posY * shipLevel[currentLevel].levelDimensions.x ) + posX];
	//
	// Used for translating from world to screen space
	viewableScreenCoord.x = playerWorldPos.x - ( winWidth / 2 );
	viewableScreenCoord.y = playerWorldPos.y - ( winHeight / 2 );
}

//-----------------------------------------------------------------------------
//
// Stop player movement
void sys_stopPlayerMovement()
//-----------------------------------------------------------------------------
{
	playerVelocity.x = 0.0f;
	playerVelocity.y = 0.0f;
	if ( playerPhysicsObject.body != NULL )
		cpBodySetVelocity ( playerPhysicsObject.body, playerVelocity );
}

//-----------------------------------------------------------------------------
//
// Create physics sensor shape for the passed in door index
//
// Sensor used to detect bullet collision
void sys_createDoorSensor ( int whichDoor, cpFloat width, cpFloat height, cpVect center )
//-----------------------------------------------------------------------------
{
	doorTrigger[whichDoor].physics.body = cpBodyNewStatic();
	cpBodySetCenterOfGravity ( doorTrigger[whichDoor].physics.body, center );
	cpBodySetPosition ( doorTrigger[whichDoor].physics.body, center );

	doorTrigger[whichDoor].physics.shape = cpBoxShapeNew ( doorTrigger[whichDoor].physics.body, width, height, 0 );
	cpSpaceAddShape ( space, doorTrigger[whichDoor].physics.shape );
	cpShapeSetCollisionType ( doorTrigger[whichDoor].physics.shape, PHYSIC_TYPE_DOOR );

	cpShapeSetUserData ( doorTrigger[whichDoor].physics.shape, (cpDataPointer)(intptr_t)whichDoor );

	cpShapeSetSensor ( doorTrigger[whichDoor].physics.shape, true );
}
