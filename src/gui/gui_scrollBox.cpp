#include "../../hdr/sys_globals.h"

_scrollBox	    introScrollBox;
float          viewScrollY;
float          previousScrollY;
float 			scrollPauseDelayValue;

//----------------------------------------------------------
//
// draw a scrollbox
bool gui_drawScrollBox(_scrollBox *scrollBox)
//----------------------------------------------------------
{
	char        currentLineOfText[128];
	int         LINE_HEIGHT;

	LINE_HEIGHT = al_get_font_line_height(font[currentFont].ttfFont);

	sys_setFont(FONT_GUI);
	sys_setFontColor(scrollBox->fontCol.r, scrollBox->fontCol.g, scrollBox->fontCol.b, scrollBox->fontCol.a);

	for (int i = 0; i != scrollBox->linesToPrint; i++)
		{
			if (strlen(scrollBox->line[i].text) > 128)
				strcpy(currentLineOfText, "String to long.");
			else
				strcpy(currentLineOfText, scrollBox->line[i].text);

			sys_printStringExt(scrollBox->startX, (scrollBox->height - (i * LINE_HEIGHT)) - viewScrollY, "%s", currentLineOfText);
		}
	return true;
}

//-------------------------------------------------- Non drawing routines under here -------------------------------------------------------------

//----------------------------------------------------------
//
// Setup a area with scrolling text
//
// Pass in the function to run when the text buffer needs to be refilled
void gui_setupScrollBox(int whichScrollBox, _scrollBox *scrollBox, string sourceText, ExternFunc funcPtr)
//----------------------------------------------------------
{
	switch (whichScrollBox)
		{
			case SCROLLBOX_INTRO:

				scrollBox->startX = introStartX;
				//
				// TODO: Fix
				scrollBox->startY = al_get_bitmap_height(image[hud].image); //gui_getConfigInt("introScroll:startY");

				scrollBox->width =  introWidth;
				scrollBox->height = introHeight;
				scrollBox->scrollSpeed = introScrollspeed;
				scrollBox->backGroundCol = al_map_rgba_f(introBackRed, introBackGreen, introBackBlue, introBackAlpha);
				scrollBox->fontCol = al_map_rgba_f(introFontRed, introFontGreen, introFontBlue, introFontAlpha);

				scrollBox->scrollDelay = introScrollDelay;    // default setting
				break;

			case SCROLLBOX_DB:
				scrollBox->startX = dbStartX;
				scrollBox->startY = dbStartY;

				scrollBox->width =  dbWidth;
				scrollBox->height = dbHeight;
				scrollBox->scrollSpeed = dbScrollspeed;
				scrollBox->backGroundCol = al_map_rgba_f(dbBackRed, dbBackGreen, dbBackBlue, dbBackAlpha);
				scrollBox->fontCol = al_map_rgba_f(dbFontRed, dbFontGreen, dbFontBlue, dbFontAlpha);

				scrollBox->scrollDelay = dbScrollDelay;    // default setting

				break;
		}


	scrollBox->scrollY = scrollBox->height;       // scroll starting position
	scrollBox->scrollDelayCount = 0.0f;           // counter for smooth scrolling
	scrollBox->linesToPrint = 1;                  // how many to display
	scrollBox->charPtr = 0;                       // start of the text

	// TODO Check after change to strings
	scrollBox->sourceText = sourceText;			  // String to use
	scrollBox->funcPtr = funcPtr;		          // Callback function to get more text

	for (int i = 0; i != MAX_LINES_IN_SCREEN; i++)
		strcpy(scrollBox->line[i].text, "");

	strcpy(scrollBox->line[8].text, "Accessing help system...");
}

//----------------------------------------------------------
//
// Get the next line of text up to the scrollBox width
char *gui_getNextTextLine(_scrollBox *scrollBox)
//----------------------------------------------------------
{
	char    nextWord[32];
// TODO (dberry#1#): Change to pointer
	char    nextLine[256];

	int previousSpace;
	bool foundLine;
	int i = 0;

	int textLength;
	int boxWidth;

	//
	// Move the text up to the next line
	for (i = MAX_LINES_IN_SCREEN - 1; i != 0; i--)
		{
			strcpy (scrollBox->line[i].text, scrollBox->line[i - 1].text);
		}

	foundLine = false;
	strcpy(nextLine, "");   // clear it out
	strcpy(nextWord, "");   // clear it out

	while (false == foundLine)
		{
			nextWord[i] = scrollBox->sourceText[scrollBox->charPtr];
			switch (nextWord[i])
				{
					case ' ': // Complete word
						{
							textLength = al_get_text_width(font[currentFont].ttfFont, nextLine) + al_get_text_width(font[currentFont].ttfFont, nextWord);
							boxWidth = scrollBox->width;
							if (textLength < boxWidth)
								{
									previousSpace = scrollBox->charPtr;    // remember this space
									nextWord[i+1] = '\0';
									strcat(nextLine, nextWord);
									strcpy(nextWord, "");
									i = -1;
								}
							else
								{
									scrollBox->charPtr = previousSpace;
									foundLine = true;
								}
						}
						break;


					case '!':		// line break
						nextWord[i] = '\0';
						strcat(nextLine, nextWord);
						strcpy(nextWord, "");
						i = -1;
						foundLine = true;
						scrollBox->charPtr++;
						break;

					case '@':		// line return - space
						nextWord[i] = '\0';
						strcat(nextLine, nextWord);
						strcpy(nextWord, "");
						i = -1;
						foundLine = true;
						scrollBox->charPtr++;
						break;

					case '#':		// next piece of text
						nextWord[i] = '\0';
						strcat(nextLine, nextWord);
						strcpy(nextWord, "");
						i = -1;
						foundLine = true;
						scrollBox->charPtr++;
						//
						// Get more text
						scrollBox->funcPtr();
						break;

					case '$':		// start a centered text
						nextWord[i] = '\0';
						scrollBox->charPtr++;
						break;

					case '%':		// end a centered text
						nextWord[i] = '\0';
						scrollBox->charPtr++;
						break;

					case '^':		// pause the scrolling text
						nextWord[i] = '\0';
						scrollBox->scrollDelay = scrollPauseDelayValue;
						scrollBox->charPtr++;
						foundLine = true;
						break;
				}

			i++;
			scrollBox->charPtr++;
			if (scrollBox->charPtr > scrollBox->sourceText.size())
				{
					strcat(nextLine, nextWord);
					foundLine = true;
				}
		}
	strcpy(scrollBox->line[0].text, nextLine);

	return nextLine;
}

//----------------------------------------------------------
//
// Move the scroll up a pixel
void gui_scrollOneLine(_scrollBox *scrollBox)
//----------------------------------------------------------
{
	int lineHeight;

	lineHeight = al_get_font_line_height(font[currentFont].ttfFont);

	previousScrollY = scrollBox->scrollY;
	//
	// Next pixel line
	scrollBox->scrollY++;

	if (scrollBox->scrollY > lineHeight)
		{
			scrollBox->scrollY = 0;
			gui_getNextTextLine(scrollBox);
			scrollBox->linesToPrint++;
			if (scrollBox->linesToPrint > scrollBox->height / lineHeight)
				scrollBox->linesToPrint = scrollBox->height / lineHeight;
		}
}

//----------------------------------------------------------
//
// Scroll the scrollbox - seperate scrolling from drawing
void gui_scrollScrollBox(_scrollBox *scrollBox)
//----------------------------------------------------------
{
	//
	// See if we need to stop scroller
	if (scrollBox->scrollDelay > 0)
		{
			scrollBox->scrollDelayCount += scrollBox->scrollSpeed;

			if (scrollBox->scrollDelayCount > scrollBox->scrollDelay)
				{
					scrollBox->scrollDelayCount = 0.0f;
					scrollBox->scrollDelay = introScrollDelay;
					gui_scrollOneLine(scrollBox);
				}
		}
	else
		{
			gui_scrollOneLine(scrollBox);
		}
//    viewScrollY = scrollBox->scrollY * g_interpolation + previousScrollY * (1 - g_interpolation);
	viewScrollY = scrollBox->scrollY;// * g_frameInterval; // + previousScrollY * (1 - g_frameInterval);
}
