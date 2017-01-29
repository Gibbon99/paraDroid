#pragma once

extern int      TICKS_PER_SECOND;
extern float    SKIP_TICKS;
extern int      MAX_FRAMESKIP;

extern double	 thinkInterval;

// Calculate the frame rate for the capped portion
void sys_CalculateThinkFrameRate(double timeValue);

// This function calculates the overall frame rate and time taken to draw a frame
void sys_CalculateFrameRate(double timeValue);

