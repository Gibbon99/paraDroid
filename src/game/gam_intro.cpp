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
