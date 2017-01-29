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

