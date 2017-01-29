#pragma once

extern float				loadingBarPercent;
extern char					loadingStatus[MAX_STRING_SIZE];

extern bool					gamePaused;
extern double				profileRenderFrameStart, profileRenderFrameEnd;

// Routine to display stuff onto the screen
//
// Pass in interpolation to draw screen
void sys_displayFrame (float interpolation);

// Routine to handle all non graphics routines
void sys_updateFrame();
