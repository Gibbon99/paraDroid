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

list<_emitter> emitter;


int particleRenderType; // Set from start script

int particleVelocityExplosion;
int particleVelocityExplosionAdd;
int particleVelocitySpark;
int particleVelocitySparkAdd;
int particleNumberExplosion;
int particleNumberExplosionAdd;
int particleNumberTrail;
int particleNumberTrailAdd;
int particleTrailAlphaReset;
int particleTrailAlphaResetAdd;
int particleNumberSpark;
int particleNumberSparkAdd;
float particleExplosionReduce;
float particleSparkReduce;
float particleTrailReduce;
int particleTrailLimit;

float particleMass; //   0.05f
float particleFriction; //  0.2f
float particleElastic; //  0.9f
float particleSize; //  2.0f

double profileParticleRenderStart;
double profileParticleRenderEnd;
double profileParticleThinkStart;
double profileParticleThinkEnd;
bool profileParticles; // Set from startup script

int particleBlendSrc;
int particleBlendDst;

ALLEGRO_COLOR particleSparkColor;
ALLEGRO_COLOR particleExplosionColor;
ALLEGRO_COLOR particleTrailColor;

typedef struct
{
	cpVect 	worldPos;
	int		type;
	int		whichBullet;
} _emitterQueue;

vector<_emitterQueue>					emitterQueue;

//------------------------------------------------------------------
//
// Add an emitter to the queue for processing
void par_addEmitter ( cpVect worldPos, int type, int whichBullet )
//------------------------------------------------------------------
{
	_emitterQueue	tempQueueItem;

	tempQueueItem.worldPos = worldPos;
	tempQueueItem.type = type;
	tempQueueItem.whichBullet = whichBullet;

	emitterQueue.push_back ( tempQueueItem );
}

//------------------------------------------------------------------
//
// Process emitter queue - done outside of physics stepWorld
void par_processEmitterQueue()
//------------------------------------------------------------------
{
	if ( 0 == emitterQueue.size() )
		return;		// Nothing in queue to process

	for (auto itr = begin(emitterQueue); itr != end(emitterQueue); ++itr)
		{
			par_initEmitter ( itr->worldPos, itr->type, itr->whichBullet );
		}
	emitterQueue.clear();
}

//------------------------------------------------------------------
//
// Set the particle colors from the script

void par_hostSetParticleColor ( int whichParticle, int red, int green, int blue, int alpha )
//------------------------------------------------------------------
{
	switch ( whichParticle )
		{
			case PARTICLE_TYPE_EXPLOSION:
				particleExplosionColor = al_map_rgba ( red, green, blue, alpha );
				break;

			case PARTICLE_TYPE_SPARK:
				particleSparkColor = al_map_rgba ( red, green, blue, alpha );
				break;

			case PARTICLE_TYPE_TRAIL:
				particleTrailColor = al_map_rgba ( red, green, blue, alpha );
				break;
		}
}

//------------------------------------------------------------------
//
// Debug the particle emitters

void par_debugParticles()
//------------------------------------------------------------------
{
	std::list<_particle>::iterator itr;
	std::list<_emitter>::iterator emitterItr;

	con_print ( true, false, "Emitter size [ %i ]", emitter.size() );
	if ( emitter.size() > 0 )
		{
			for ( emitterItr = emitter.begin(); emitterItr != emitter.end(); ++emitterItr )
				{
					con_print ( true, false, "Num particles [ %i ]", emitterItr->particle.size() );
					con_print ( true, false, "Bullet link [ %i ]", emitterItr->bulletLink );
					con_print ( true, false, "Emitter type [ %i ]", emitterItr->type );
					con_print ( true, false, "-----------------------");
				}
		}
}

//------------------------------------------------------------------
//
// Get random angle in a circle shape
cpVect par_getCircleAngle ( int particleType )
//------------------------------------------------------------------
{
	float angle, velocity;
	cpVect direction;
	float a = 2.0f * 3.1415f;

	angle = ( ( float ) sys_genRand_int32() / ( float ) ( RAND_MAX ) ) * a;
	switch ( particleType )
		{
			case PARTICLE_TYPE_EXPLOSION:
				velocity = sys_genRand_int32() % particleVelocityExplosion + particleVelocityExplosionAdd;
				break;

			case PARTICLE_TYPE_SPARK:
				velocity = sys_genRand_int32() % particleVelocitySpark + particleVelocitySparkAdd;
				break;
		}

	direction.x = velocity * cos ( angle );
	direction.y = velocity * sin ( angle );

	return direction;
}

//------------------------------------------------------------------
//
// Start an emitter at the passed in location
int par_initEmitter ( cpVect worldPos, int type, int whichBullet )
//------------------------------------------------------------------
{

	_emitter tempEmitter;
	_particle tempParticle;
	int particleNumber;
	int particleNumberAdd;
	cpVect particlePosition;

	tempEmitter.type = type;
	tempEmitter.image = particleSpark;

	if ( PARTICLE_RENDER_OFF == particleRenderType )
		return -1;

	switch ( type )
		{
			case PARTICLE_TYPE_EXPLOSION:
				tempEmitter.bulletLink = -1;
				tempEmitter.usePhysics = true;
				tempEmitter.useDirections = true;
				particleNumber = particleNumberExplosion;
				particleNumberAdd = particleNumberExplosionAdd;
				particlePosition = worldPos;
				tempParticle.color = particleExplosionColor;
				//al_map_rgba(particleExplosionColor.r, particleExplosionColor.g, particleExplosionColor.b, particleExplosionColor.a);
				break;

			case PARTICLE_TYPE_TRAIL:
				tempEmitter.usePhysics = false;
				tempEmitter.useDirections = false;
				tempEmitter.bulletLink = whichBullet;
				particleNumber = particleNumberTrail;
				particleNumberAdd = particleNumberTrailAdd;
				particlePosition = bullet[whichBullet].worldPos;
				tempParticle.color = particleTrailColor; //al_map_rgba(particleTrailColor.r, particleTrailColor.g, particleTrailColor.b, sys_genRand_int32() % 150);
				tempParticle.color.a = -1;
				break;

			case PARTICLE_TYPE_SPARK:
				tempEmitter.usePhysics = true;
				tempEmitter.useDirections = true;
				tempEmitter.bulletLink = -1;
				particleNumber = particleNumberSpark;
				particleNumberAdd = particleNumberSparkAdd;
				particlePosition = worldPos;
				tempParticle.color = particleSparkColor; //al_map_rgba(particleSparkColor.r, particleSparkColor.g, particleSparkColor.b, particleSparkColor.a);
				break;
		}

	tempEmitter.numParticlesAlive = sys_genRand_int32() % particleNumber + particleNumberAdd;
	for ( unsigned int i = 0; i != tempEmitter.numParticlesAlive; ++i )
		{
			tempParticle.worldPos = particlePosition;

			if ( true == tempEmitter.useDirections )
				{
					switch ( tempEmitter.type )
						{
							case PARTICLE_TYPE_EXPLOSION:
								tempParticle.direction = par_getCircleAngle ( PARTICLE_TYPE_EXPLOSION );
								break;

							case PARTICLE_TYPE_SPARK:
								tempParticle.direction = par_getCircleAngle ( PARTICLE_TYPE_SPARK );
								break;
						}
				}

			if ( true == tempEmitter.usePhysics )
				{
					tempParticle.body = cpSpaceAddBody ( space, cpBodyNew ( particleMass, cpMomentForCircle ( particleMass, 0.0f, particleSize, cpvzero ) ) );

					cpBodySetMass ( tempParticle.body, particleMass );
					cpBodySetPosition ( tempParticle.body, worldPos );

					tempParticle.shape = cpSpaceAddShape ( space, cpCircleShapeNew ( tempParticle.body, particleSize, cpvzero ) );
					cpShapeSetCollisionType ( tempParticle.shape, PHYSIC_TYPE_PARTICLE );

					cpShapeSetFriction ( tempParticle.shape, particleFriction );
					cpShapeSetElasticity ( tempParticle.shape, particleElastic );
				}
			tempEmitter.particle.push_back ( tempParticle );
		}
	emitter.push_back ( tempEmitter );
	//
	// Link an emitter with a bullet index
	//
	if ( type == PARTICLE_TYPE_TRAIL )
		return emitter.size(); // return index into emitter array
	else
		return -1;
}

//-----------------------------------------------------
//
// Reset all emitters ready for new game to start
void par_resetAllEmitters()
//-----------------------------------------------------
{
	if ( PARTICLE_RENDER_OFF == particleRenderType )
		return;

//	for ( emitterItr = emitter.begin(); emitterItr != emitter.end(); emitterItr++ )
	for ( auto emitterItr = emitter.begin(); emitterItr != emitter.end();)
		{
			if ( true == emitterItr->usePhysics )
				{
					for ( auto thisIter = emitterItr->particle.begin(); thisIter != emitterItr->particle.end(); )
//			for ( itr = emitterItr->particle.begin(); itr != emitterItr->particle.end(); itr++ )
						{
							if ( ( cpTrue == cpSpaceContainsShape ( space, thisIter->shape ) ) &&
							        (cpTrue == cpSpaceContainsBody ( space, thisIter->body ) ) )
								{
									cpSpaceRemoveShape ( space, thisIter->shape );
									cpShapeFree ( thisIter->shape );

									cpSpaceRemoveBody ( space, thisIter->body );
									cpBodyFree ( thisIter->body );
									thisIter = emitterItr->particle.erase ( thisIter );
								}
							else
								{
									++thisIter;
								}
//					itr = emitterItr->particle.erase ( itr );
						}
				}
				emitterItr = emitter.erase ( emitterItr );
//		++emitterItr;
		}
}

//-----------------------------------------------------
//
// Remove an emitter attached to a bullet
void par_removeEmitter ( int whichBullet )
//-----------------------------------------------------
	{
	std::list<_particle>::iterator itr;
	std::list<_emitter>::iterator emitterItr;

		if ( PARTICLE_RENDER_OFF == particleRenderType )
			return;

		for ( emitterItr = emitter.begin(); emitterItr != emitter.end(); ++emitterItr )
			{
				if ( whichBullet == emitterItr->bulletLink )
					{
						for ( itr = emitterItr->particle.begin(); itr != emitterItr->particle.end(); ++itr )
							{
								if ( true == emitterItr->usePhysics )
									{
										if (cpTrue == cpSpaceContainsShape ( space, itr->shape ) )
											{
												cpSpaceRemoveShape ( space, itr->shape );
												cpShapeFree ( itr->shape );
											}

										if (cpTrue == cpSpaceContainsBody ( space, itr->body ) )
											{
												cpSpaceRemoveBody ( space, itr->body );
												cpBodyFree ( itr->body );
											}
									}
								itr = emitterItr->particle.erase ( itr );
								break;
							}
						emitterItr = emitter.erase ( emitterItr );
						return;
					}
			}
	}

//-----------------------------------------------------
//
// Move the particles
void par_animateParticles()
//-----------------------------------------------------
	{
		float particleReduce = 0.0f;
		int particleTrailCount = 0;
		std::list<_particle>::iterator itr;
		std::list<_emitter>::iterator emitterItr;


		if ( PARTICLE_RENDER_OFF == particleRenderType )
			return;

		if ( true == profileParticles )
			profileParticleThinkStart = al_get_time();

		particleTrailCount = 0;

		for ( emitterItr = emitter.begin(); emitterItr != emitter.end(); ++emitterItr )
			{
				switch ( emitterItr->type )
					{
						case PARTICLE_TYPE_EXPLOSION:
							particleReduce = particleExplosionReduce;
							break;

						case PARTICLE_TYPE_TRAIL:
							particleReduce = particleTrailReduce;
							emitterItr->worldPos = bullet[emitterItr->bulletLink].worldPos;
							break;

						case PARTICLE_TYPE_SPARK:
							particleReduce = particleSparkReduce;
							break;
					}

				for ( itr = emitterItr->particle.begin(); itr != emitterItr->particle.end(); ++itr )
					{
						itr->color.a -= particleReduce * thinkInterval;
						if ( itr->color.a < 0.0f )
							{
								switch ( emitterItr->type )
									{
										case PARTICLE_TYPE_EXPLOSION:
										case PARTICLE_TYPE_SPARK:
											if (cpTrue == cpSpaceContainsShape ( space, itr->shape ) )
												{
													cpSpaceRemoveShape ( space, itr->shape );
													cpShapeFree ( itr->shape );
												}

											if (cpTrue == cpSpaceContainsBody ( space, itr->body ) )
												{
													cpSpaceRemoveBody ( space, itr->body );
													cpBodyFree ( itr->body );
												}

											io_logToFile ("About to free spark particle [ %i ]", emitterItr->particle.size());

											itr = emitterItr->particle.erase ( itr );

											if (itr == emitterItr->particle.end ())
												return;

											break;

										case PARTICLE_TYPE_TRAIL:
											itr->worldPos = emitterItr->worldPos;
											itr->color = al_map_rgba ( 255, 0, 0, sys_genRand_int32() % particleTrailAlphaReset + particleTrailAlphaResetAdd );
											int randomX, randomY;

											randomX = sys_genRand_int32() % 10;
											randomX -= 5;
											randomY = sys_genRand_int32() % 10;
											randomY -= 5;
											itr->worldPos.x += randomX;
											itr->worldPos.y += randomY;
											particleTrailCount++;
//											if ( particleTrailCount > particleTrailLimit )
											if ( particleTrailCount > particleTrailLimit )
												{
//													itr = emitterItr->particle.end();
												}
											//	cpBodySetPosition (itr->body, itr->worldPos);
											break;
									}
							}
						else
							{
								switch ( emitterItr->type )
									{
										case PARTICLE_TYPE_EXPLOSION:
										case PARTICLE_TYPE_SPARK:
										if (cpTrue == cpSpaceContainsBody(space, itr->body))
												cpBodySetForce ( itr->body, itr->direction );
											break;
									}
							}

						if ( emitterItr->particle.size() == 1 )
							{
								io_logToFile ("Freeing particle emitterItr.");

								emitterItr = emitter.erase ( emitterItr ); // emitterItr now points to the one after the deleted one
								return;
							}
					}
			}
		if ( true == profileParticles )
			profileParticleThinkEnd = al_get_time();
	}

//-----------------------------------------------------
//
// Draw all emitters and particles
void par_drawParticles()
//-----------------------------------------------------
	{
		cpVect vecPos1;
		std::list<_particle>::iterator itr;
		std::list<_emitter>::iterator emitterItr;

		int op, src, dst;

		if ( PARTICLE_RENDER_OFF == particleRenderType )
			return;

		if ( true == profileParticles )
			profileParticleRenderStart = al_get_time();

		if ( PARTICLE_RENDER_BITMAP == particleRenderType )
			{
				if ( true == useHoldBitmap )
					al_hold_bitmap_drawing ( true );
			}

		al_get_blender ( &op, &src, &dst );
		//	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ALPHA);
		al_set_blender ( ALLEGRO_ADD, particleBlendSrc, particleBlendDst );

		for ( emitterItr = emitter.begin(); emitterItr != emitter.end(); ++emitterItr )
			{
				for ( unsigned int i = 0; i != emitterItr->particle.size(); i++ )
					{
						for ( itr = emitterItr->particle.begin(); itr != emitterItr->particle.end(); ++itr )
							{
								if ( itr->color.a > 0 )
									{
										if ( true == emitterItr->usePhysics )
											vecPos1 = cpBodyGetPosition ( itr->body );
										else
											vecPos1 = itr->worldPos;
										//
										// Translate from world to screen position
										vecPos1 = sys_worldToScreen ( vecPos1, particleSize );

										switch ( particleRenderType )
											{
												case PARTICLE_RENDER_BITMAP:
													al_draw_tinted_bitmap ( image[emitterItr->image].image, itr->color, vecPos1.x, vecPos1.y, 0 );
													break;

												case PARTICLE_RENDER_PRIMITIVE:
													al_draw_filled_circle ( vecPos1.x, vecPos1.y, 2, itr->color );
													break;
											}
									}
							}
					}
			}

		al_set_blender ( op, src, dst );

		if ( PARTICLE_RENDER_BITMAP == particleRenderType )
			{
				if ( true == useHoldBitmap )
					al_hold_bitmap_drawing ( false );
			}

		if ( true == profileParticles )
			profileParticleRenderEnd = al_get_time();
	}
