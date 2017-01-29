#pragma once

extern ALLEGRO_VERTEX		*stars;
extern int					numStars;
extern bool				useStarfield;

// Free memory used by starfield
void gam_freeStarField();

// Get some memory to hold the starfield data
//
// .u field is used for speed - z needs to be 0
bool gam_setupStarField();

// Animate movement of starfield
void gam_animateStarField(float interpolate);
