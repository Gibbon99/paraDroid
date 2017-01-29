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

float introStartX;
float introStartY;
float introWidth;
float introHeight;
float introScrollspeed;
float introBackRed;
float introBackGreen;
float introBackBlue;
float introBackAlpha;
float introFontRed;
float introFontGreen;
float introFontBlue;
float introFontAlpha;
float introScrollDelay;

// ----------------------------------------------------------------------------
//
// Function called from scrollBox to get more text
bool gam_introScrollGetMoreText()
// ----------------------------------------------------------------------------
{
	static int currentTextBlock = 0;

	currentTextBlock++;
	if (currentTextBlock == 5)
		currentTextBlock = 0;

	introScrollBox.charPtr = 0;
	switch (currentTextBlock)
	{
	case 0:
		introScrollBox.sourceText = gui_getString("scrollText");
		break;
	case 1:
		introScrollBox.sourceText = gui_getString("scrollText2");
		break;
	case 2:
		introScrollBox.sourceText = gui_getString("scrollText3");
		break;
	case 3:
		introScrollBox.sourceText = gui_getString("scrollText4");
		break;
	case 4:
		introScrollBox.sourceText = gui_getString("scrollText5");
		break;
	}
	return true;
}

// ----------------------------------------------------------------------------
//
// Display the intro text
void gam_displayIntroText()
// ----------------------------------------------------------------------------
	{
	static bool scrollBoxSetup = false;

	if (false == scrollBoxSetup)
		{
		scrollBoxSetup = true;
		sys_executeScriptFunction("as_setIntroValues", "");
		gui_setupScrollBox(SCROLLBOX_INTRO, &introScrollBox, gui_getString("scrollText"), (ExternFunc)gam_introScrollGetMoreText);
		}

	gui_drawScrollBox(&introScrollBox);
	}
