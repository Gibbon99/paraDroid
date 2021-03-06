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

#define PHYSIC_TYPE_WALL			0x01
#define PHYSIC_TYPE_ENEMY			0x02
#define PHYSIC_TYPE_PLAYER			0x04
#define PHYSIC_TYPE_BULLET			0x08
#define PHYSIC_TYPE_DOOR			0x16
#define PHYSIC_TYPE_PARTICLE		0x32

// Setup all the collision handlers
void sys_setupCollisionHandlers();
