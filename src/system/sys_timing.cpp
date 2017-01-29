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

int      fpsPrint;
int      thinkFpsPrint;
double   renderTick1;
double   thinkTick1;

int      TICKS_PER_SECOND;
float    SKIP_TICKS;
int      MAX_FRAMESKIP;

double	 thinkInterval;

//-----------------------------------------------------------------------------
//
// Calculate the frame rate for the capped portion
void sys_CalculateThinkFrameRate(double timeValue)
//-----------------------------------------------------------------------------
{
    static int	      thinkFps = 0;
    static double	   thinkTick2 = 0.0f;
    static float	   thinkDelayCounter = 0.0f;

    thinkTick2 = timeValue;

    thinkInterval += ( (thinkTick2 - thinkTick1) - thinkInterval) * 0.1f;

    thinkTick1 = thinkTick2;

    thinkFps++;

    thinkDelayCounter += 1.0f * thinkInterval;
    if (thinkDelayCounter > 1.0f)
        {
            thinkDelayCounter = 0.0f;
            thinkFpsPrint = thinkFps;
            thinkFps = 0;
        }
}

//-----------------------------------------------------------------------------
//
// This function calculates the overall frame rate and time taken to draw a frame
void sys_CalculateFrameRate(double timeValue)
//-----------------------------------------------------------------------------
{
    static int	   fps = 0;	// Remove global fps variable
    static double	renderTick2 = 0.0f;
    static float	tickDelayCounter = 0.0f;
    static float	frameDelayCounter = 0.0f;
//
// Collect stats
//    static int	   numFramesRendered = 0;
    static float	totalRenderTime = 0.0f;
//    static float	averagefps = 0.0f;

    renderTick2 = timeValue;

    frameDelayCounter += ( (renderTick2 - renderTick1) - frameDelayCounter) * 0.1f;

    renderTick1 = renderTick2;

// Increase the frame counter
    fps++;

    tickDelayCounter += 1.0f * frameDelayCounter;
    if (tickDelayCounter > 1.0f)
        {
            tickDelayCounter = 0.0f;
            fpsPrint = fps;     // Value to display
            fps = 0;
        }
    totalRenderTime += frameDelayCounter;
}
