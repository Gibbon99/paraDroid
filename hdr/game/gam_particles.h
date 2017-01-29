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
	ALLEGRO_COLOR 	color;
	cpVect 			direction;
	cpVect 			worldPos;
	cpBody 			*body;
	cpShape 		*shape;
} _particle;

typedef struct
{
	cpVect			worldPos;
	unsigned int 	numParticlesAlive;
	unsigned int 	type;
	int				bulletLink;
	int				image;
	bool			usePhysics;
	bool			useDirections;
	list<_particle> particle;
} _emitter;

extern list<_emitter> 					emitter;
extern list<_particle>::iterator 		itr;
extern int								particleRenderType;

//
//------------ These are all set from the particle script
//
extern int particleVelocityExplosion;
extern int particleVelocityExplosionAdd;
extern int particleVelocitySpark;
extern int particleVelocitySparkAdd;
extern int particleNumberExplosion;
extern int particleNumberExplosionAdd;
extern int particleNumberTrail;
extern int particleNumberTrailAdd;
extern int particleTrailAlphaReset;
extern int particleTrailAlphaResetAdd;
extern int particleNumberSpark;
extern int particleNumberSparkAdd;
extern float particleExplosionReduce;
extern float particleSparkReduce;
extern float particleTrailReduce;
extern int particleTrailLimit;

extern float particleMass; //   0.05f
extern float particleFriction; //  0.2f
extern float particleElastic; //  0.9f
extern float particleSize; //  2.0f

extern int particleBlendSrc;
extern int particleBlendDst;

extern double profileParticleRenderStart;
extern double profileParticleRenderEnd;
extern double profileParticleThinkStart;
extern double profileParticleThinkEnd;
extern bool profileParticles; // Set from startup script

extern ALLEGRO_COLOR particleSparkColor;
extern ALLEGRO_COLOR particleExplosionColor;
extern ALLEGRO_COLOR particleTrailColor;

// Start an emitter at the passed in location
int par_initEmitter ( cpVect worldPos, int type, int whichBullet );

// Draw all emitters and particles
void par_drawParticles();

// Move the particles
void par_animateParticles();

// Remove an emitter attached to a bullet
void par_removeEmitter ( int whichEmitter );

// Debug the particle emitters
void par_debugParticles();

// Set the particle colors from the script
void par_hostSetParticleColor ( int whichParticle, int red, int green, int blue, int alpha );

// Process emitter queue - done outside of physics stepWorld
void par_processEmitterQueue();

// Add an emitter to the queue for processing
void par_addEmitter(cpVect worldPos, int type, int whichBullet);

// Reset all emitters ready for new game to start
void par_resetAllEmitters();
