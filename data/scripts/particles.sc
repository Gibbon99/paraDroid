//
// Script to setup the GUI elements
#include "data/scripts/commonDefines.h"

//-----------------------------------------------------------------------------
//
// Setup the values for the particles
void as_initParticleValues()
//-----------------------------------------------------------------------------
{
	particleVelocityExplosion = 35;
	particleVelocityExplosionAdd = 25;

	particleVelocitySpark = 20;
	particleVelocitySparkAdd = 25;

	particleNumberExplosion = 40;
	particleNumberExplosionAdd = 20;

	particleNumberTrail = 30;
	particleNumberTrailAdd = 10;
	particleTrailAlphaReset = 200;
	particleTrailAlphaResetAdd = 50;

	particleNumberSpark = 3;
	particleNumberSparkAdd = 5;

	particleExplosionReduce = 1.03f;
	particleSparkReduce = 1.88f;
	particleTrailReduce = 0.85f;

	particleTrailLimit = 5;		// How many particles to spawn each frame

	as_setParticleColor(PARTICLE_TYPE_SPARK, 175, 175, 175, 255);
	as_setParticleColor(PARTICLE_TYPE_EXPLOSION, 255, 0, 0, 255);
	as_setParticleColor(PARTICLE_TYPE_TRAIL, 255, 75, 75, as_getCappedRandomNum (150));

	particleMass = 0.05f;
	particleFriction = 0.2f;
	particleElastic = 0.9f;
	particleSize = 4.0f;

	particleBlendSrc = 2;
	particleBlendDst = 3;
}


/*
   ALLEGRO_ZERO                = 0,
   ALLEGRO_ONE                 = 1,
   ALLEGRO_ALPHA               = 2,
   ALLEGRO_INVERSE_ALPHA       = 3,
   ALLEGRO_SRC_COLOR           = 4,
   ALLEGRO_DEST_COLOR          = 5,
   ALLEGRO_INVERSE_SRC_COLOR   = 6,
   ALLEGRO_INVERSE_DEST_COLOR  = 7,
   ALLEGRO_CONST_COLOR         = 8,
   ALLEGRO_INVERSE_CONST_COLOR = 9,
*/
