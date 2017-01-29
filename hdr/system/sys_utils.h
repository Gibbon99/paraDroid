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

extern cpVect 		viewableScreenCoord;
extern float        startStatusX;
extern float        startScoreX;

// Change to new currentMode
void sys_changeMode(int newMode, bool useFade);

// Convert worldPosition coords to screen coords
cpVect sys_worldToScreen(cpVect worldPos, int shapeSize);

// Is an object visible on the screen
bool sys_visibleOnScreen(cpVect worldCoord, int shapeSize);

// Calculate the display position of the text within the HUD graphic
void sys_calcHudTextPosition(int hudWidth, int hudHeight);

// Safely copy two strings.
size_t strlcpy(char *dst, const char *src, size_t size);

size_t strlcat(char *dst, const char *src, size_t size);