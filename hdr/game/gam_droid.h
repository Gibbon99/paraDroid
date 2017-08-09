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

struct _droid
{
	int			droidType;
	int			currentHealth;
	int			wayPointIndex;
	int			wayPointDirection;
	int			overTileMiddle;

	float		currentSpeed;
	cpVect		worldPos;
	cpVect		acceleration;
	cpVect		velocity;
	cpVect		screenPos;
	cpVect		previousWaypoints;		// This is the line segment start point
	cpVect		destinationCoords;		// This is the line segment end point
	cpVect		destDirection;			// Which way is the droid heading
	float		mass;					// Used for collision response
//
// Weapon
	bool		weaponCanFire;
	float		weaponDelay;
//
// States
	bool		isStopped;
	bool		hasCollided;
	bool		isAlive;
	bool		visibleToPlayer;
	bool		isExploding;
// Animation
	float		currentFrameDelay;
	int			currentFrame;

// Pathfinding
	int			aStarPathIndex;			// Index into which path to use
	int			ai_moveMode;
	int         numberPathNodes;
	int         currentAStarIndex;         // Index into aStarWaypoints array
	bool		aStarDestinationFound;
	bool		aStarInitDone;

// AI variables
	int         ai_currentState;
	int         ai_nextState;
	float       ai_noActionCounter;
	float       ai_noActionCount;
	int         collidedWith;	// Who did the droid hit
	bool        ignoreCollisions;
	float       ignoreCollisionsCounter;
	int			collisionCount;		// how many collision have occured to ignore them
	bool		witnessShooting;
	bool		witnessTransfer;
	float		chanceToShoot;
	bool		beenShotByPlayer;
	float		beenShotCountdown;
	float		witnessShootingCountDown;
	float		witnessTransferCountDown;
	int			targetIndex;		// Which droid shot this droid

	cpVect      originPosition;     // Remember this to return to
	bool        foundOriginPath;
	bool        returnToOrigin;
	bool        foundOriginPosition;
	bool        onHealingTile;
	bool        foundHealingTile;
	float       healingCount;
	bool        foundWPTile;		// Go here after healing

	bool		onFleeTile;
	bool		foundFleeTile;

	bool			isNotPatrolling;		// Used to enter resume branch
	bool			onResumeDestTile;
	bool			destSet;				// Does the droid have a destination
	cpVect			destinationTile;		// Where is the droid heading
	bool			resumeDestFound;		// Set from running thread - aStar found destination
	bool			resumeThreadStarted;	// Is the thread started and running
	ALLEGRO_THREAD 	*threadPathFind;		// Handle to thread running aStar function

	cpBody		*body;				// Used for physics and collisions
	cpShape		*shape;
};

extern _droid       droid[];
extern float        explodeFrameDelay;
extern float        baseDroidAnimateValue;
extern float        maxDistanceFromPath;
extern float        moveBackToLineSegment;
extern int          collisionDamageInflicted;
extern int			collisionExplosionDamage;
extern float        healingTimer;		// Used to time healing for droids

// Draw all the enemy droids on the level
void gam_drawAllDroids(int whichLevel);

//Setup initial values for enemy droids
void gam_initDroidValues(int whichLevel);

// Process movements for all droids
int ai_moveDroidToWaypoint(int whichDroid);

int gam_isColliding(int whichDroid);

// Setup Physics for droids for current level
void gam_initDroidPhysics(int whichlevel, int previousLevel);

// Which tile is the droid over
void drd_getOverWhichTile(int whichDroid);

// See if this droid collides with another droids
bool gam_checkDroidToDroidCollision(int whichLevel, int whichDroid);

// Handle a droid which has collided
void gam_handleDroidCollision(int whichLevel, int whichDroid);

// Animate enemy droid
void gam_animateDroid(int whichLevel, int whichDroid, float delayInterval);

// Kill an enemy droid
void gam_destroyDroid(int whichLevel, int whichDroid);

// damageSource can be either a bullet, explosion or collision
void drd_damageToDroid(int whichLevel, int whichDroid, int damageSource, int sourceDroid);

// Process weapon timings
void gam_droidWeaponCharge(int whichLevel, int whichDroid);

// process ignore collosions
void gam_processIgnoreCollisions(int whichLevel, int whichDroid);

// Stop droid moving and movement
void gam_stopDroidMovement(int whichLevel, int whichDroid);

// Increment the collision count
// this is used to see if the droid should start ignoring collisions after having too many
void gam_incrementCollisionCount(int whichLevel, int droid1, int droid2);

// Update the droids information from physics properties
void drd_updateDroidPosition(int whichDroid);
