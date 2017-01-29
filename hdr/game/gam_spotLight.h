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
	cpVect		worldPosition;
	cpVect		srcPosition;
	int			rotateAngle;
	float		rotateSpeedCount;
	int			bitmapWidth;
	int			bitmapHeight;
} _spotLight;

extern vector<_spotLight>		spotLight;

// Find all the alert tiles on the current level
void gam_findAllAlertTiles(bool clearVector);

// Animate all the alert spotlights
void gam_animateSpotLights();

// Draw all the spotlights visible on the screen
void gam_drawSpotLights();