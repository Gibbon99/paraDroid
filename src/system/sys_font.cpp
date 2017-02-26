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

ALLEGRO_COLOR	fontColor;

_font		    font[NUM_FONTS];
int			    currentFont;

//-----------------------------------------------------------------------------
//
// Set the font color
void sys_setFontColor(float r, float g, float b, float a)
//-----------------------------------------------------------------------------
{
	fontColor = al_map_rgba_f(r, g, b, a);
}

//-----------------------------------------------------------------------------
//
// Start the TTF fonts to known values
void sys_initTTFSystem()
//-----------------------------------------------------------------------------
{
	for (int i = 0; i != NUM_FONTS; i++)
	{
		font[i].valid = false;
		font[i].ttfFont = NULL;
		strcpy(font[i].fileName, "");
	}
}

//-----------------------------------------------------------------------------
//
// Load a TTF font from scripts
bool io_loadttfFont(int fontIndex, int fontSize, string fontName)
//-----------------------------------------------------------------------------
{
	strcpy(font[fontIndex].fileName, fontName.c_str());

	font[fontIndex].ttfFont = al_load_ttf_font(font[fontIndex].fileName, fontSize, 0);
	if (NULL == font[fontIndex].ttfFont)
	{
		io_logToFile("Error: Font [ %s ] not found", font[fontIndex].fileName);
		font[fontIndex].valid = false;
	}
	font[fontIndex].valid = true;
	font[fontIndex].fontSize = fontSize;

	return true;
}

//-----------------------------------------------------------------------------
//
// Set the current font
void sys_setFont(int newFont)
//-----------------------------------------------------------------------------
{
	currentFont = newFont;
}

//-----------------------------------------------------------------------------
//
// Draw text string to main screen
//
// If chosen font is not valid, use builtin font
void sys_printString(float posX, float posY, string textIn)
//-----------------------------------------------------------------------------
{
	if (true == useHoldBitmap)
	  al_hold_bitmap_drawing(true);

//	al_draw_textf (false == font[currentFont].valid ? builtInFont : font[currentFont].ttfFont, fontColor, posX, posY, ALLEGRO_ALIGN_LEFT, "%s", textIn.c_str());

	if (false == font[currentFont].valid)
		al_draw_textf (builtInFont, fontColor, posX, posY, ALLEGRO_ALIGN_LEFT, "%s", textIn.c_str());
	else
		al_draw_textf (font[currentFont].ttfFont, fontColor, posX, posY, ALLEGRO_ALIGN_LEFT, "%s", textIn.c_str());

	if (true == useHoldBitmap)
	  al_hold_bitmap_drawing(false);
}

//-----------------------------------------------------------------------------
//
// If chosen font is not valid, use builtin font
void sys_printStringExt(float posX, float posY, const char *format, ...)
//-----------------------------------------------------------------------------
{
	va_list		args;
	char		stringText[MAX_STRING_SIZE];
	string		passText;
	//
	// get out the passed in parameters
	//
	va_start(args, format);
	vsnprintf(stringText, MAX_STRING_SIZE, format, args);
	va_end(args);

	passText = stringText;
	sys_printString(posX, posY, passText);
}
