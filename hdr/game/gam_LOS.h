#pragma once

extern float		g_LineOfSightStep;
extern bool			allDroidsVisible;

// Master routine to check visiblity of all valid sprites
void lvl_LOS();

// Debug LOS lines
void lvl_debugLOS();
