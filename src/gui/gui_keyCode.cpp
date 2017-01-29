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

float		keyCodeGap;

//-----------------------------------------------------------------------------
//
// Draw the keycode gui elements
void gui_drawKeycode ( int whichObject, bool hasFocus )
//-----------------------------------------------------------------------------
{

	float startDrawX, startDrawY, nameDrawX, nameDrawY;
	string drawText, nameText;
	int longestTextLine;

	startDrawX = guiKeyCodes[whichObject].attributes.boundingBox.x;
	startDrawY = guiKeyCodes[whichObject].attributes.boundingBox.y;
	longestTextLine = 0;
	drawText.clear();
	nameText.clear();

	for ( int i = 0; i != (int)guiKeyCodes[whichObject].element.size(); i++ )
		{
			if ( true == hasFocus )
				{
					if ( i == guiKeyCodes[whichObject].elementFocus )
						{
							// This element is selected
							sys_setFontColor ( guiKeyCodes[whichObject].attributes.activeCol.r, guiKeyCodes[whichObject].attributes.activeCol.g, guiKeyCodes[whichObject].attributes.activeCol.b, guiKeyCodes[whichObject].attributes.activeCol.a );
						}
					else
						{
							// Element not selected
							sys_setFontColor ( guiKeyCodes[whichObject].attributes.inActiveCol.r, guiKeyCodes[whichObject].attributes.inActiveCol.g, guiKeyCodes[whichObject].attributes.inActiveCol.b, guiKeyCodes[whichObject].attributes.inActiveCol.a );
						}
				}
			else
				{
					sys_setFontColor ( guiKeyCodes[whichObject].attributes.inActiveCol.r, guiKeyCodes[whichObject].attributes.inActiveCol.g, guiKeyCodes[whichObject].attributes.inActiveCol.b, guiKeyCodes[whichObject].attributes.inActiveCol.a );
				}

			drawText = guiKeyCodes[whichObject].element[i].keyName;

			if  ( ( i & 1 ) == 0 )
				{
					startDrawX = guiKeyCodes[whichObject].attributes.boundingBox.x;
					startDrawY += al_get_font_line_height ( font[currentFont].ttfFont );

					nameDrawX = ( ( winWidth / 2 ) - keyCodeGap );
					nameDrawX -= al_get_text_width ( font[currentFont].ttfFont, al_keycode_to_name ( guiKeyCodes[whichObject].element[i].keyCode ) );
					nameDrawX -= keyCodeGap * 2;
					nameDrawY = startDrawY;
				}
			else
				{
					startDrawX += ( winWidth / 2 );	// Element is odd - draw on left
					nameDrawY = startDrawY;
					nameText = "[ ";
					nameText.append ( al_keycode_to_name ( guiKeyCodes[whichObject].element[i].keyCode ) );
					nameText.append ( " ]" );
					nameDrawX = winWidth - ( keyCodeGap * 2 ) - al_get_text_width ( font[currentFont].ttfFont, nameText.c_str() );
					if ( al_get_text_width ( font[currentFont].ttfFont, nameText.c_str() ) > longestTextLine )
						longestTextLine = al_get_text_width ( font[currentFont].ttfFont, nameText.c_str() );
				}

			sys_printStringExt ( startDrawX, startDrawY, "%s", drawText.c_str() );
			sys_printStringExt ( nameDrawX, nameDrawY, "[ %s ]", al_keycode_to_name ( guiKeyCodes[whichObject].element[i].keyCode ) );
		}

	//
	// Draw the highlight box
	//
	if ( true == hasFocus )
		{
			startDrawY += keyCodeGap;
			startDrawX += keyCodeGap;

			al_draw_rounded_rectangle ( guiKeyCodes[whichObject].attributes.boundingBox.x - keyCodeGap,
			                            guiKeyCodes[whichObject].attributes.boundingBox.y + al_get_font_line_height ( font[currentFont].ttfFont ) - keyCodeGap,
			                            winWidth - keyCodeGap,
			                            startDrawY + al_get_font_line_height ( font[currentFont].ttfFont ),  buttonCurveX, buttonCurveY, al_map_rgba_f ( guiKeyCodes[whichObject].attributes.activeCol.r, guiKeyCodes[whichObject].attributes.activeCol.g, guiKeyCodes[whichObject].attributes.activeCol.b, guiKeyCodes[whichObject].attributes.activeCol.a ), 2.0f );
		}
}

//-----------------------------------------------------------------------------
//
// Add a new element to the keyCode object
void as_hostAddKeyCodeElement ( string objectID, string elementLabel, int elementKeyCode )
//-----------------------------------------------------------------------------
{
	_keyCodeElement		keyCodeTmp;

	keyCodeTmp.keyName = elementLabel;
	keyCodeTmp.keyCode = elementKeyCode;

	for ( vector<_guiKeyCode>::iterator it = guiKeyCodes.begin(); it != guiKeyCodes.end(); ++it )
		{
			if ( it->attributes.objectID == objectID )
				{
					it->element.push_back ( keyCodeTmp );
					return;
				}
		}
	con_print ( true, false, "GUI Error: Keycode objectID not found [ %s ]", objectID.c_str() );
}

//-----------------------------------------------------------------------------
//
// Copy the contents of the key array into the keycode vector array
//
// The vector array must have the correct number of members first
void gui_copyKeysInto ( string objectID )
//-----------------------------------------------------------------------------
{
	_keyCodeElement		keyCodeTmp;

	for ( vector<_guiKeyCode>::iterator it = guiKeyCodes.begin(); it != guiKeyCodes.end(); ++it )
		{
			if ( it->attributes.objectID == objectID )
				{
					//
					// Check to make sure it's the right size first
					//
					if ( it->element.size() != NUMBER_ACTIONS )
						{
							printf ( "Error: GUI: keyCodes size does not match [ %i ] to [ %i ]\n", (int)it->element.size(), NUMBER_ACTIONS );
							return;
						}

					for ( int i = 0; i != NUMBER_ACTIONS; i++ )
						{
							it->element[i].keyName = inputAction[i].stringValue.c_str();
							it->element[i].keyCode = inputAction[i].keyValue;
						}

					it->elementFocus = 0;	// Start on the first element
					return;	// Finished this one
				}
		}
}

//-----------------------------------------------------------------------------
//
// Copy the contents of the GUI array into the keys array
void gui_copyGUIIntoKeys ( string objectID )
//-----------------------------------------------------------------------------
{
	for ( vector<_guiKeyCode>::iterator it = guiKeyCodes.begin(); it != guiKeyCodes.end(); ++it )
		{
			if ( it->attributes.objectID == objectID )
				{
					//
					// Check we have the right number of elements
					//
					if ( NUMBER_ACTIONS != it->element.size() )
						{
							printf ( "ERROR: GUI: Keycode array is wrong size\n" );
							return;
						}

					for ( int i = 0; i != NUMBER_ACTIONS; i++ )
						{
							inputAction[i].stringValue = it->element[i].keyName;
							inputAction[i].keyValue = it->element[i].keyCode;
						}
					return;
				}
		}
}
