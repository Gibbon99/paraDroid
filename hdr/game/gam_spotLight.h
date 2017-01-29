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