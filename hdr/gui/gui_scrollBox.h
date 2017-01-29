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
	char	text[MAX_NUM_CHARS_IN_LINE];
} _scrollLine;

typedef struct
	{
	int         	startX;
	int         	startY;
	int         	width;
	int         	height;
	int         	whichFont;
	ALLEGRO_COLOR 	backGroundCol;
	ALLEGRO_COLOR 	fontCol;
	int         	linesToPrint;   // how far up have we scrolled
	int         	charPtr;        // which character are we up to
	float       	scrollY;		// How many fine pixel lines scrolled
	float       	scrollSpeed;
	float       	scrollDelayCount;
	float		    scrollDelay;	// Adjustable by text
	_scrollLine		line[MAX_LINES_IN_SCREEN];
	string			sourceText;
	ExternFunc		funcPtr;
	} _scrollBox;

extern _scrollBox		introScrollBox;
extern _scrollBox   	databaseScrollBox;

extern float 			scrollPauseDelayValue;

// Pass in the function to run when the text buffer needs to be refilled
void gui_setupScrollBox(int whichScrollBox, _scrollBox *scrollBox, string sourceText, ExternFunc funcPtr);

// draw a scrollbox
bool gui_drawScrollBox(_scrollBox *scrollBox);

// Scroll the scrollbox - seperate scrolling from drawing
void gui_scrollScrollBox(_scrollBox *scrollBox);

