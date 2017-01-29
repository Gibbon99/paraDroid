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

extern cpVect	playerVelocity;
extern cpVect	playerWorldPos;
extern cpVect	playerWorldMiddlePos;
extern cpVect	playerWorldNextPos;		// move to this before collision detection
extern cpVect	playerScreenPos;
extern int		playerCurrentFrame;

extern float 	playerAcceleration;
extern float	playerMaxSpeed;
extern float	playerMaxVelocity;
extern float	playerCurrentHealth;
extern float	playerHealthWarning;
extern float	playerHealthWarningFactor;		// Set from startup script
extern int		playerDroidTypeDBIndex;
extern float	playerTransferTimeRemaining;
extern float	playerTransferTimeSet;
extern float	playerTransferTimeAmount;
extern float	playerTransferWarning;
extern float	playerFlashValue;
extern float	playerFlashAmount;
extern bool		playerIsExploding;

extern bool	    playerWeaponReadyToFire;
extern float	playerWeaponRechargeTime;

extern int		playerDroidTypeImage;

extern float 	gravity;


extern bool	    tileCollisionOn;
extern bool	    doClampPlayerVelocity;
extern bool	    inTransferMode;

extern int 		playerOverTile;

extern float    healingRate;

// Process player movement - do scroll and tile movements
void gam_processPlayerMovement(float thinkTime);

// Copy the values from an enemy to player sprite
// Init for 001 on first run
void gam_setupPlayerValues(int copyFrom);

void gam_animatePlayerSprite(float delayInterval);

// Recharge player weapon
void gam_weaponRechargePlayer();

// Play transfer sound if in transfer mode
void gam_checkTransferSound();

// Check to see if the player over a healing tile
void gam_checkPlayerHealingTile(float thinkTime);

// Do damage to player health
// Can either be from DAMAGE_BULLET, DAMAGE_COLLISION or DAMAGE_EXPLOSION
void gam_doDamageToPlayer(int damageSource, int sourceDroid);

// Player has lost transfer or transfer time is up
// Drop player back to 001, play explosion and set health
void gam_dropPlayerTo001();

// If the player has transferred into a droid - check the time remaining
void gam_playerTransferTime();