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

#define NUM_FONTS		3

typedef struct
{
ALLEGRO_FONT		*ttfFont;
bool				valid;
char				fileName[MAX_STRING_SIZE];
int                 fontSize;
} _font;

extern _font		font[];
extern int			currentFont;

// State of the current font
extern ALLEGRO_COLOR	fontColor;

// Load a TTF font from scripts
bool io_loadttfFont(int fontIndex, int fontSize, string fontName);

// Start the TTF fonts to known values
void sys_initTTFSystem();

// Set the font color
void sys_setFontColor(float r, float g, float b, float a);

// Draw text string to main screen
void sys_printString(float posX, float posY, string textIn);

// If chosen font is not valid, use builtin font
void sys_printStringExt(float posX, float posY, const char *format, ...);

// Set the current font
void sys_setFont(int newFont);
