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

typedef struct
{
	cpBody		*body;
	cpShape		*shape;
} _physicObject;

extern cpSpace			*space;
extern float			shipDamping;
extern float			collisionSlop;
extern float			wallFriction;		// Set from startup script
extern float			wallRadius;			// Set from startup script
extern float			playerMass;
extern float			playerRadius;
extern float			playerFriction;
extern float			playerElastic;
extern float			bulletMass;

extern bool				debugDrawPhysics;
extern bool				processedPhysics;

//-------------------------------------------------------
//
// Routines for physics engine
//
// ------------------------------------------------------

// Setup Physics engine - run once
bool sys_setupPhysicsEngine();

// Cleanup Physics engine - run once
void sys_freePhysicsEngine();

// Create all the bodies and shapes for this level
void sys_destroyPhysicObjects(int whichLevel);

// Setup player droid physics information
void sys_setupPlayerPhysics();

// Destory player physics
void sys_destroyPlayerPhysics();

// Set the player physics position in the world
void sys_setPlayerPhysicsPosition(cpVect newPosition);

// Create the solid walls for this level
void sys_createSolidWalls();

// Remove the physics information associated to an enemy droid
void sys_removeDroidCPInfo(int whichDroid);

// Sensor used to detect bullet collision
void sys_createDoorSensor(int whichDoor, cpFloat width, cpFloat height, cpVect center);

// Show the line segments for solid walls
void sys_debugLineSegments();

// Destroy all the physics objects ready to restart the game
void sys_clearAllPhysics();

// Set scrolling variables from playerWorldPos
void gam_updateScrollingVars(bool getPlayerPos);

// Show bounding boxes
void sys_debugBoundingBoxes();

// Stop player movement
void sys_stopPlayerMovement();

// Return true if this point is over a solid tile
bool sys_isOverSolidTile(cpVect checkPoint, int objectType);

// Check tiles around droid, if solid check line segment
bool sys_checkDroidTileCollision(int whichLevel, int whichDroid, int whichDirection);

// Set the player physics mass after transferring to a new droid
void sys_setPlayerMass(float newMass);
