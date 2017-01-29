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

extern int      TICKS_PER_SECOND;
extern float    SKIP_TICKS;
extern int      MAX_FRAMESKIP;

extern double	 thinkInterval;

// Calculate the frame rate for the capped portion
void sys_CalculateThinkFrameRate(double timeValue);

// This function calculates the overall frame rate and time taken to draw a frame
void sys_CalculateFrameRate(double timeValue);

