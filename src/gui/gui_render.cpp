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

float rasterPosY;

//-----------------------------------------------------------------------------
//
// Draw a slider object
void gui_drawslider ( int whichObject, bool hasFocus )
//-----------------------------------------------------------------------------
{
	float 		selectorX;
	string 		printLabel;
	string 		printValue;
	cpVect		printLabelPos;
	cpVect		printValuePos;

	if ( guiSliders[whichObject].currentStep == 0 )
		selectorX = guiSliders[whichObject].attributes.boundingBox.x;
	else if ( guiSliders[whichObject].currentStep == (int)guiSliders[whichObject].element.size() )
		selectorX = guiSliders[whichObject].attributes.boundingBox.x + guiSliders[whichObject].attributes.boundingBox.w;	// position at the end
	else
		{
			selectorX = ( ( guiSliders[whichObject].attributes.boundingBox.w / ( guiSliders[whichObject].element.size() - 1 ) ) * guiSliders[whichObject].currentStep ) + guiSliders[whichObject].attributes.boundingBox.x;
		}

	printLabel = guiSliders[whichObject].attributes.label ;
	printValue = guiSliders[whichObject].element[guiSliders[whichObject].currentStep].label;

	printValuePos.x = guiSliders[whichObject].attributes.boundingBox.x + ( ( guiSliders[whichObject].attributes.boundingBox.w - al_get_text_width ( font[currentFont].ttfFont, printValue.c_str() ) ) / 2 );
	printLabelPos.x = guiSliders[whichObject].attributes.boundingBox.x + ( ( guiSliders[whichObject].attributes.boundingBox.w - al_get_text_width ( font[currentFont].ttfFont, printLabel.c_str() ) ) / 2 );

	printValuePos.y = guiSliders[whichObject].attributes.boundingBox.y + guiSliders[whichObject].attributes.boundingBox.h + 2;
	printLabelPos.y = guiSliders[whichObject].attributes.boundingBox.y - ( al_get_font_line_height ( font[currentFont].ttfFont ) + 2 );
	//
	// Draw the slider bar
	//
	if ( true == hasFocus )
		{
			al_draw_filled_rounded_rectangle ( guiSliders[whichObject].attributes.boundingBox.x, guiSliders[whichObject].attributes.boundingBox.y,
			                                   guiSliders[whichObject].attributes.boundingBox.x + guiSliders[whichObject].attributes.boundingBox.w,
			                                   guiSliders[whichObject].attributes.boundingBox.y + guiSliders[whichObject].attributes.boundingBox.h,
			                                   sliderCurveX, sliderCurveY, guiSliders[whichObject].attributes.activeCol );

			al_draw_filled_circle ( selectorX, guiSliders[whichObject].attributes.boundingBox.y + ( guiSliders[whichObject].attributes.boundingBox.h / 2 ), sliderSelectorRadius, guiSliders[whichObject].attributes.activeCol );
			sys_setFontColor ( guiSliders[whichObject].attributes.activeCol.r, guiSliders[whichObject].attributes.activeCol.g, guiSliders[whichObject].attributes.activeCol.b, guiSliders[whichObject].attributes.activeCol.a );
		}
	else
		{
			al_draw_filled_rounded_rectangle ( guiSliders[whichObject].attributes.boundingBox.x, guiSliders[whichObject].attributes.boundingBox.y,
			                                   guiSliders[whichObject].attributes.boundingBox.x + guiSliders[whichObject].attributes.boundingBox.w,
			                                   guiSliders[whichObject].attributes.boundingBox.y + guiSliders[whichObject].attributes.boundingBox.h,
			                                   sliderCurveX, sliderCurveY, guiSliders[whichObject].attributes.inActiveCol );

			al_draw_filled_circle ( selectorX, guiSliders[whichObject].attributes.boundingBox.y + ( guiSliders[whichObject].attributes.boundingBox.h / 2 ), sliderSelectorRadius, guiSliders[whichObject].attributes.inActiveCol );
			sys_setFontColor ( guiSliders[whichObject].attributes.inActiveCol.r, guiSliders[whichObject].attributes.inActiveCol.g, guiSliders[whichObject].attributes.inActiveCol.b, guiSliders[whichObject].attributes.inActiveCol.a );
		}

	sys_printStringExt ( printLabelPos.x, printLabelPos.y, "%s", printLabel.c_str() );
	sys_printStringExt ( printValuePos.x, printValuePos.y, "%s", printValue.c_str() );

}

//-----------------------------------------------------------------------------
//
// Print the display inside the confines of a text box
void gui_printTextBox ( int whichObject )
//-----------------------------------------------------------------------------
{
	float printStartY = guiTextBoxes[whichObject].boundingBox.y;
	string nextLine;
	int textLength;
	char *p = NULL;
	string textCopy;

	nextLine.clear ();
	textLength = 0;

//TODO: Set font color

	printStartY = guiTextBoxes[whichObject].boundingBox.y;
	//
	// strtok modifies the string - make a copy
	textCopy = guiTextBoxes[whichObject].label;
	p = strtok ( ( char * ) textCopy.c_str (), " " );

	while ( p != 0 )
		{
			textLength = al_get_text_width ( font[currentFont].ttfFont, nextLine.c_str () ) + al_get_text_width ( font[currentFont].ttfFont, p );

			if ( *p == '!' )
				{
					textLength = guiTextBoxes[whichObject].boundingBox.w + 1;
				}

			if ( textLength < guiTextBoxes[whichObject].boundingBox.w )
				{
					nextLine.append ( p );
					nextLine.append ( " " );
				}
			else
				{
					sys_printStringExt ( guiTextBoxes[whichObject].boundingBox.x, printStartY, "%s", nextLine.c_str () );
					printStartY += al_get_font_line_height ( font[currentFont].ttfFont );
					nextLine.clear ();
					if ( *p != '!' )
						nextLine.append ( p );
					nextLine.append ( " " );
				}
			p = strtok ( NULL, " " );
		}
	sys_printStringExt ( guiTextBoxes[whichObject].boundingBox.x, printStartY, "%s", nextLine.c_str () );
}

//-----------------------------------------------------------------------------
//
// Draw a key based on passed in color and mode
void gui_drawObject ( int objectType, int whichObject, bool hasFocus )
//-----------------------------------------------------------------------------
{
	switch ( objectType )
		{
			case GUI_OBJECT_IMAGE:
				if ( true == guiImages[whichObject].imageLoaded )
					{
						if ( GUI_COORD_TYPE_PERCENT == guiImages[whichObject].attributes.coordType )
							al_draw_tinted_bitmap ( guiImages[whichObject].bitmap, guiImages[whichObject].tint,
							                        guiImages[whichObject].attributes.boundingBox.x - ( al_get_bitmap_width ( guiImages[whichObject].bitmap ) / 2 ), guiImages[whichObject].attributes.boundingBox.y, 0 );
						else
							al_draw_tinted_bitmap ( guiImages[whichObject].bitmap, guiImages[whichObject].tint, guiImages[whichObject].attributes.boundingBox.x, guiImages[whichObject].attributes.boundingBox.y, 0 );
					}
				break;

			case GUI_OBJECT_BUTTON:

				if ( true == hasFocus )
					{
						al_draw_rounded_rectangle ( guiButtons[whichObject].boundingBox.x, guiButtons[whichObject].boundingBox.y, guiButtons[whichObject].boundingBox.x + guiButtons[whichObject].boundingBox.w, guiButtons[whichObject].boundingBox.y + guiButtons[whichObject].boundingBox.h, buttonCurveX, buttonCurveY, al_map_rgba_f ( guiButtons[whichObject].activeCol.r, guiButtons[whichObject].activeCol.g, guiButtons[whichObject].activeCol.b, guiButtons[whichObject].activeCol.a ), 2.0f );

						sys_setFontColor ( guiButtons[whichObject].inActiveLabelCol.r, guiButtons[whichObject].inActiveLabelCol.g, guiButtons[whichObject].inActiveLabelCol.b, guiButtons[whichObject].inActiveLabelCol.a );
					}
				else
					{
						al_draw_rounded_rectangle ( guiButtons[whichObject].boundingBox.x, guiButtons[whichObject].boundingBox.y, guiButtons[whichObject].boundingBox.x + guiButtons[whichObject].boundingBox.w, guiButtons[whichObject].boundingBox.y + guiButtons[whichObject].boundingBox.h, buttonCurveX, buttonCurveY, al_map_rgba_f ( guiButtons[whichObject].inActiveCol.r, guiButtons[whichObject].inActiveCol.g, guiButtons[whichObject].inActiveCol.b, guiButtons[whichObject].inActiveCol.a ), 2.0f );

						sys_setFontColor ( guiButtons[whichObject].activeLabelCol.r, guiButtons[whichObject].activeLabelCol.g, guiButtons[whichObject].activeLabelCol.b, guiButtons[whichObject].activeLabelCol.a );
					}
				sys_printStringExt ( guiButtons[whichObject].labelStartX, guiButtons[whichObject].labelStartY, "%s", guiButtons[whichObject].label.c_str () );
				break;

			case GUI_OBJECT_CHECKBOX:

				int boxSize;

				boxSize = al_get_font_line_height ( font[currentFont].ttfFont );

				if ( true == hasFocus )
					{
						al_draw_rounded_rectangle ( guiCheckBoxes[whichObject].attributes.boundingBox.x, guiCheckBoxes[whichObject].attributes.boundingBox.y, guiCheckBoxes[whichObject].attributes.boundingBox.x + boxSize, guiCheckBoxes[whichObject].attributes.boundingBox.y + boxSize, buttonCurveX, buttonCurveY, al_map_rgba_f ( guiCheckBoxes[whichObject].attributes.activeCol.r, guiCheckBoxes[whichObject].attributes.activeCol.g, guiCheckBoxes[whichObject].attributes.activeCol.b, guiCheckBoxes[whichObject].attributes.activeCol.a ), 2.0f );
						if ( true == guiCheckBoxes[whichObject].ticked )
							al_draw_filled_rounded_rectangle ( guiCheckBoxes[whichObject].attributes.boundingBox.x + checkBoxTickSize, guiCheckBoxes[whichObject].attributes.boundingBox.y + checkBoxTickSize, guiCheckBoxes[whichObject].attributes.boundingBox.x + boxSize - checkBoxTickSize, guiCheckBoxes[whichObject].attributes.boundingBox.y + boxSize - checkBoxTickSize, buttonCurveX, buttonCurveY, al_map_rgba_f ( guiCheckBoxes[whichObject].attributes.activeCol.r, guiCheckBoxes[whichObject].attributes.activeCol.g, guiCheckBoxes[whichObject].attributes.activeCol.b, guiCheckBoxes[whichObject].attributes.activeCol.a ) );

						sys_setFontColor ( guiCheckBoxes[whichObject].attributes.activeLabelCol.r, guiCheckBoxes[whichObject].attributes.activeLabelCol.g, guiCheckBoxes[whichObject].attributes.activeLabelCol.b, guiCheckBoxes[whichObject].attributes.activeLabelCol.a );
					}
				else
					{
						al_draw_rounded_rectangle ( guiCheckBoxes[whichObject].attributes.boundingBox.x, guiCheckBoxes[whichObject].attributes.boundingBox.y, guiCheckBoxes[whichObject].attributes.boundingBox.x + boxSize, guiCheckBoxes[whichObject].attributes.boundingBox.y + boxSize, buttonCurveX, buttonCurveY, al_map_rgba_f ( guiCheckBoxes[whichObject].attributes.inActiveCol.r, guiCheckBoxes[whichObject].attributes.inActiveCol.g, guiCheckBoxes[whichObject].attributes.inActiveCol.b, guiCheckBoxes[whichObject].attributes.inActiveCol.a ), 2.0f );
						if ( true == guiCheckBoxes[whichObject].ticked )
							al_draw_filled_rounded_rectangle ( guiCheckBoxes[whichObject].attributes.boundingBox.x + checkBoxTickSize, guiCheckBoxes[whichObject].attributes.boundingBox.y + checkBoxTickSize, guiCheckBoxes[whichObject].attributes.boundingBox.x + boxSize - checkBoxTickSize, guiCheckBoxes[whichObject].attributes.boundingBox.y + boxSize - checkBoxTickSize, buttonCurveX, buttonCurveY, al_map_rgba_f ( guiCheckBoxes[whichObject].attributes.inActiveCol.r, guiCheckBoxes[whichObject].attributes.inActiveCol.g, guiCheckBoxes[whichObject].attributes.inActiveCol.b, guiCheckBoxes[whichObject].attributes.inActiveCol.a ) );

						sys_setFontColor ( guiCheckBoxes[whichObject].attributes.inActiveLabelCol.r, guiCheckBoxes[whichObject].attributes.inActiveLabelCol.g, guiCheckBoxes[whichObject].attributes.inActiveLabelCol.b, guiCheckBoxes[whichObject].attributes.inActiveLabelCol.a );
					}
				sys_printStringExt ( guiCheckBoxes[whichObject].attributes.labelStartX + guiCheckBoxes[whichObject].textGap, guiCheckBoxes[whichObject].attributes.labelStartY, "%s", guiCheckBoxes[whichObject].attributes.label.c_str () );
				break;

			case GUI_OBJECT_TEXT_LABEL:
				sys_setFontColor ( guiTextLabels[whichObject].activeCol.r, guiTextLabels[whichObject].activeCol.g, guiTextLabels[whichObject].activeCol.b, guiTextLabels[whichObject].activeCol.a );
				sys_printStringExt ( guiTextLabels[whichObject].labelStartX, guiTextLabels[whichObject].labelStartY, "%s", guiTextLabels[whichObject].label.c_str () );
				break;

			case GUI_OBJECT_TEXT_BOX:
				sys_setFontColor ( guiTextBoxes[whichObject].activeCol.r, guiTextBoxes[whichObject].activeCol.g, guiTextBoxes[whichObject].activeCol.b, guiTextBoxes[whichObject].activeCol.a );
				gui_printTextBox ( whichObject );
				break;

			case GUI_OBJECT_SLIDER:
				gui_drawslider ( whichObject, hasFocus );
				break;

			case GUI_OBJECT_KEYCODE:
				gui_drawKeycode ( whichObject, hasFocus );
				break;
		}
}

//-----------------------------------------------------------------------------
//
// Main routine for drawing all the GUI screens
void gui_drawGUI ()
//-----------------------------------------------------------------------------
{
	sys_setFont ( FONT_GUI );

//	sys_printStringExt ( 10.0f, 30.0f, "currentGUIScreen [ %s ] Selected [ %i ] Size [ %i ]", guiScreens[currentGUIScreen].screenID.c_str(), guiScreens[currentGUIScreen].selectedObject, guiScreens[currentGUIScreen].objectIDIndex.size() );
	//
	// Draw GUI object elements
	int indexCount = 0;

	indexCount = 0;

	for ( vector<int>::iterator it = guiScreens[currentGUIScreen].objectIDIndex.begin (); it != guiScreens[currentGUIScreen].objectIDIndex.end (); ++it )
		{
			if ( guiScreens[currentGUIScreen].selectedObject == indexCount )
				gui_drawObject ( guiScreens[currentGUIScreen].objectType[indexCount], guiScreens[currentGUIScreen].objectIDIndex[indexCount], true );
			else
				gui_drawObject ( guiScreens[currentGUIScreen].objectType[indexCount], guiScreens[currentGUIScreen].objectIDIndex[indexCount], false );
			indexCount++;
		}
	gui_drawRasterLines ();
}

//-----------------------------------------------------------------------------
//
// Draw the moving raster line for effect
void gui_animateRasterLine ()
//-----------------------------------------------------------------------------
{
	rasterPosY++;
	if ( rasterPosY > winHeight )
		rasterPosY = al_get_bitmap_height ( image[hud].image );
}

//-----------------------------------------------------------------------------
//
// Draw the lines background
void gui_drawRasterLines ()
//-----------------------------------------------------------------------------
{
	for ( int j = al_get_bitmap_height ( image[hud].image ); j < winHeight; j++ )
		{
			al_draw_line ( 0, j++, winWidth, j++, al_map_rgba_f ( 0.0, 0.0, 0.2, 0.3 ), 1.0f );

			if ( j == rasterPosY )
				al_draw_line ( 0, j++, winWidth, j++, al_map_rgba_f ( 0.0, 0.0, 0.6, 0.3 ), 1.0f );
			if ( j == rasterPosY - 1 )
				al_draw_line ( 0, j++, winWidth, j++, al_map_rgba_f ( 0.0, 0.0, 0.5, 0.3 ), 1.0f );
			if ( j == rasterPosY - 2 )
				al_draw_line ( 0, j++, winWidth, j++, al_map_rgba_f ( 0.0, 0.0, 0.4, 0.3 ), 1.0f );
		}
}
