#pragma once

#define PHYSIC_TYPE_WALL			0x01
#define PHYSIC_TYPE_ENEMY			0x02
#define PHYSIC_TYPE_PLAYER			0x04
#define PHYSIC_TYPE_BULLET			0x08
#define PHYSIC_TYPE_DOOR			0x16
#define PHYSIC_TYPE_PARTICLE		0x32

// Setup all the collision handlers
void sys_setupCollisionHandlers();
