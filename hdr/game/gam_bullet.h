#pragma once

struct _bullet
{
	bool			alive;
	float 			angle;
	cpVect 			worldPos;
	cpVect			travelDirection;
	cpVect			size;
	int				type;
	int				currentAnimFrame;
	float			animDelayCount;
	float			speed;
	int				sourceDroid;
	bool			damageDone;	// Use for disrupter to only damage once
	int				particleIndex;	// Used to remove a particle emitter

	_physicObject	bulletPhysicsObject;
};

extern _bullet	    bullet[];

extern float		bulletTravelSpeed;
extern float       	bulletDisruptSpeed;

// Process all the alive bullets
void bul_processBullets();

// Move all the alive bullets
void bul_newBullet(cpVect sourcePos, cpVect destPos, int type, int sourceDroid);

// Draw all the valid bullets
void bul_drawBullets();

void bul_initBullets();

// Check if a disrupter hits an enemy droid
void bul_processDisrupter(int whichLevel, int whichBullet);

// Pass in bullet type and get index into image array
int bul_getBulletImageByType(int whichType);