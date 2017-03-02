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

#define DEBUG_GUI_SETUP     1

_guiObject          tmp_guiButton;
_guiCheckBox        tmp_checkBox;
_guiObject          tmp_textLabel;
_guiImage			tmp_guiImage;
_guiObject			tmp_textBox;
_screenObject       tmp_screen;
_guiSlider			tmp_slider;
_guiKeyCode			tmp_keyCode;

//-----------------------------------------------------------------------------
//
// Find the objectID on the current screen and make it selected
void gui_setObjectFocus(string objectID)
//-----------------------------------------------------------------------------
{
	int indexCount = 0;

	for (indexCount = 0; indexCount != (int)guiScreens[currentGUIScreen].objectIDIndex.size(); indexCount++)
		{
			switch (guiScreens[currentGUIScreen].objectType[indexCount])
				{
					case GUI_OBJECT_BUTTON:
						if (guiButtons[guiScreens[currentGUIScreen].objectIDIndex[indexCount]].objectID == objectID)
							guiScreens[currentGUIScreen].selectedObject = indexCount;
						return;
						break;

					case GUI_OBJECT_CHECKBOX:
						if (guiCheckBoxes[guiScreens[currentGUIScreen].objectIDIndex[indexCount]].attributes.objectID == objectID)
							guiScreens[currentGUIScreen].selectedObject = indexCount;
						return;
						break;

					case GUI_OBJECT_SLIDER:
						if (guiSliders[guiScreens[currentGUIScreen].objectIDIndex[indexCount]].attributes.objectID == objectID)
							guiScreens[currentGUIScreen].selectedObject = indexCount;
						return;
						break;

					case GUI_OBJECT_KEYCODE:
						if(guiKeyCodes[guiScreens[currentGUIScreen].objectIDIndex[indexCount]].attributes.objectID == objectID)
							guiScreens[currentGUIScreen].selectedObject = indexCount;
						return;
						break;
				}
			indexCount++;	//TODO: Check this is needed
		}
	con_print(true, false, "GUI Error: ObjectID [ %s ] not found", objectID.c_str());
}

//-----------------------------------------------------------------------------
//
// Look through the relevant vector to locate the index of the objectID
//
// returns index value, or -1 if not found
int gui_findIndex(int guiObjectType, string objectID)
//-----------------------------------------------------------------------------
{
	int indexCount = 0;

	indexCount = 0;

	switch (guiObjectType)
		{
			case GUI_OBJECT_SCREEN:
				for (vector<_screenObject>::iterator it = guiScreens.begin(); it != guiScreens.end(); ++it)
					{
						if (it->screenID == objectID)
							{
								return indexCount;
							}
						indexCount++;
					}
				return -1;
				break;

			case GUI_OBJECT_TEXT_LABEL:
				for (vector<_guiObject>::iterator it = guiTextLabels.begin(); it != guiTextLabels.end(); ++it)
					{
						if (it->objectID == objectID)
							{
								return indexCount;
							}
						indexCount++;
					}
				return -1;  // Didn't find a match
				break;

			case GUI_OBJECT_BUTTON:
				for (vector<_guiObject>::iterator it = guiButtons.begin(); it != guiButtons.end(); ++it)
					{
						if (it->objectID == objectID)
							{
								return indexCount;
							}
						indexCount++;
					}
				return -1;  // Didn't find a match
				break;

			case GUI_OBJECT_CHECKBOX:
				for (vector<_guiCheckBox>::iterator it = guiCheckBoxes.begin(); it != guiCheckBoxes.end(); ++it)
					{
						if (it->attributes.objectID == objectID)
							{
								return indexCount;
							}
						indexCount++;
					}
				return -1;
				break;

			case GUI_OBJECT_IMAGE:
				for (vector<_guiImage>::iterator it = guiImages.begin(); it != guiImages.end(); ++it)
					{
						if (it->attributes.objectID == objectID)
							{
								return indexCount;
							}
						indexCount++;
					}
				return -1;
				break;

			case GUI_OBJECT_TEXT_BOX:
				for (vector<_guiObject>::iterator it = guiTextBoxes.begin(); it != guiTextBoxes.end(); ++it)
					{
						if (it->objectID == objectID)
							{
								return indexCount;
							}
						indexCount++;
					}
				return -1;
				break;

			case GUI_OBJECT_SLIDER:
				for (vector<_guiSlider>::iterator it = guiSliders.begin(); it != guiSliders.end(); ++it)
					{
						if (it->attributes.objectID == objectID)
							{
								return indexCount;
							}
						indexCount++;
					}
				return -1;
				break;

			case GUI_OBJECT_KEYCODE:
				for (vector<_guiKeyCode>::iterator it = guiKeyCodes.begin(); it != guiKeyCodes.end(); ++it)
					{
						if (it->attributes.objectID == objectID)
							{
								return indexCount;
							}
						indexCount++;
					}
				return -1;
				break;
		}
	return -1;
}

//-----------------------------------------------------------------------------
//
// Add a gui object to it's relevant vector array
//
void gui_hostCreateObject(int guiObjectType, string objectID)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	io_logToFile("Added new object - [ %s ]", objectID.c_str());
#endif // DEBUG_GUI_SETUP

	switch (guiObjectType)
		{
			case GUI_OBJECT_BUTTON:
				tmp_guiButton.objectID = objectID;
				tmp_guiButton.canFocus = true;
				guiButtons.push_back(tmp_guiButton);

				break;

			case GUI_OBJECT_CHECKBOX:
				tmp_checkBox.attributes.objectID = objectID;
				tmp_checkBox.attributes.canFocus = true;
				guiCheckBoxes.push_back(tmp_checkBox);
				guiCheckBoxes.back().ticked = false;
				guiCheckBoxes.back().attributes.selected = false;
				guiCheckBoxes.back().whichGroup = -1;
				guiCheckBoxes.back().textGap = -1;
				guiCheckBoxes.back().checkGap = -1;
#ifdef DEBUG_GUI_SETUP
				io_logToFile("GUI_OBJECT_CHECKBOX size is now - [ %i ]", guiCheckBoxes.size());
#endif // DEBUG_GUI_SETUP

				break;

			case GUI_OBJECT_TEXT_LABEL:
				tmp_textLabel.objectID = objectID;
				tmp_textLabel.canFocus = false;
				guiTextLabels.push_back(tmp_textLabel);
				break;

			case GUI_OBJECT_IMAGE:
				tmp_guiImage.attributes.objectID = objectID;
				tmp_guiImage.attributes.canFocus = false;
				tmp_guiImage.imageLoaded = false;
				guiImages.push_back(tmp_guiImage);
				break;

			case GUI_OBJECT_TEXT_BOX:
				tmp_textBox.objectID = objectID;
				tmp_textBox.canFocus = false;
				guiTextBoxes.push_back(tmp_textBox);
				break;

			case GUI_OBJECT_SLIDER:
				tmp_slider.attributes.objectID = objectID;
				tmp_slider.attributes.canFocus = true;
				guiSliders.push_back(tmp_slider);
				break;

			case GUI_OBJECT_KEYCODE:
				tmp_keyCode.attributes.objectID = objectID;
				tmp_keyCode.attributes.canFocus = true;
				guiKeyCodes.push_back(tmp_keyCode);
				break;
		}
}


//-----------------------------------------------------------------------------
// Host function called by script to set position values
void gui_hostSetObjectPosition(int guiObjectType, string objectID, int coordType, float startX, float startY, float width, float height)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	io_logToFile("Setting position for - [ %s ]", objectID.c_str());
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = gui_findIndex(guiObjectType, objectID);
	if (-1 == objectIndex)
		{
			io_logToFile("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str());
			return;
		}

	switch (guiObjectType)
		{
			case GUI_OBJECT_TEXT_LABEL:
				guiTextLabels[objectIndex].coordType = coordType;
				if (GUI_COORD_TYPE_PERCENT == guiTextLabels[objectIndex].coordType)
					{
						guiTextLabels[objectIndex].boundingBox.x = winWidth  * (startX / 100);
						guiTextLabels[objectIndex].boundingBox.y = winHeight * (startY / 100);
						guiTextLabels[objectIndex].boundingBox.w = winWidth  * (width  / 100);
						guiTextLabels[objectIndex].boundingBox.h = winHeight * (height / 100);
					}
				else
					{
						guiTextLabels[objectIndex].boundingBox.x = startX;
						guiTextLabels[objectIndex].boundingBox.y = startY;
						guiTextLabels[objectIndex].boundingBox.w = width;
						guiTextLabels[objectIndex].boundingBox.h = height;
					}
				break;

			case GUI_OBJECT_BUTTON:
				guiButtons[objectIndex].coordType = coordType;
				if (GUI_COORD_TYPE_PERCENT == guiButtons[objectIndex].coordType)
					{
						guiButtons[objectIndex].boundingBox.x = winWidth  * (startX / 100);
						guiButtons[objectIndex].boundingBox.y = winHeight * (startY / 100);
						guiButtons[objectIndex].boundingBox.w = winWidth  * (width  / 100);
						guiButtons[objectIndex].boundingBox.h = winHeight * (height / 100);
					}
				else
					{
						guiButtons[objectIndex].boundingBox.x = startX;
						guiButtons[objectIndex].boundingBox.y = startY;
						guiButtons[objectIndex].boundingBox.w = width;
						guiButtons[objectIndex].boundingBox.h = height;
					}
				break;


			case GUI_OBJECT_CHECKBOX:
				guiCheckBoxes[objectIndex].attributes.coordType = coordType;
				guiCheckBoxes[objectIndex].textGap = SLIDER_LABEL_GUTTER;
				guiCheckBoxes[objectIndex].labelGap = SLIDER_LABEL_GUTTER;

				if (GUI_COORD_TYPE_PERCENT == guiCheckBoxes[objectIndex].attributes.coordType)
					{
						guiCheckBoxes[objectIndex].attributes.boundingBox.x = winWidth  * (startX / 100);
						guiCheckBoxes[objectIndex].attributes.boundingBox.y = winHeight * (startY / 100);
						guiCheckBoxes[objectIndex].attributes.boundingBox.w = winWidth  * (width / 100);
						guiCheckBoxes[objectIndex].attributes.boundingBox.h = winHeight * (height / 100);
					}
				else
					{
						guiCheckBoxes[objectIndex].attributes.boundingBox.x = startX;
						guiCheckBoxes[objectIndex].attributes.boundingBox.y = startY;
						guiCheckBoxes[objectIndex].attributes.boundingBox.w = width;
						guiCheckBoxes[objectIndex].attributes.boundingBox.h = height;
					}
				break;


			case GUI_OBJECT_IMAGE:
				guiImages[objectIndex].attributes.coordType = coordType;
				if (GUI_COORD_TYPE_PERCENT == guiImages[objectIndex].attributes.coordType)
					{
						if (true == guiImages[objectIndex].imageLoaded)
							guiImages[objectIndex].attributes.boundingBox.x = (winWidth * (startX / 100) - (al_get_bitmap_width(guiImages[objectIndex].bitmap) / 2));
						else
							guiImages[objectIndex].attributes.boundingBox.x = winWidth * (startX / 100);

						guiImages[objectIndex].attributes.boundingBox.y = winHeight * (startY / 100);
					}
				else
					{
						guiImages[objectIndex].attributes.boundingBox.x = startX;
						guiImages[objectIndex].attributes.boundingBox.y = startY;
					}
				break;

			case GUI_OBJECT_TEXT_BOX:
				guiTextBoxes[objectIndex].coordType = coordType;
				if (GUI_COORD_TYPE_PERCENT == guiTextBoxes[objectIndex].coordType)
					{
						guiTextBoxes[objectIndex].boundingBox.x = winWidth * (startX / 100);
						guiTextBoxes[objectIndex].boundingBox.y = winHeight * (startY / 100);
						guiTextBoxes[objectIndex].boundingBox.w = winWidth * (width / 100);
						guiTextBoxes[objectIndex].boundingBox.h = winHeight * (height / 100);
					}
				else
					{
						guiTextBoxes[objectIndex].boundingBox.x = startX;
						guiTextBoxes[objectIndex].boundingBox.y = startY;
						guiTextBoxes[objectIndex].boundingBox.w = width;
						guiTextBoxes[objectIndex].boundingBox.h = height;
					}
				break;

			case GUI_OBJECT_SLIDER:
				guiSliders[objectIndex].attributes.coordType = coordType;
				if (GUI_COORD_TYPE_PERCENT == guiSliders[objectIndex].attributes.coordType)
					{
						guiSliders[objectIndex].attributes.boundingBox.x = winWidth * (startX / 100);
						guiSliders[objectIndex].attributes.boundingBox.y = winHeight * (startY / 100);
						guiSliders[objectIndex].attributes.boundingBox.w = winWidth * (width / 100);
						guiSliders[objectIndex].attributes.boundingBox.h = winHeight * (height / 100);
					}
				else
					{
						guiSliders[objectIndex].attributes.boundingBox.x = startX;
						guiSliders[objectIndex].attributes.boundingBox.y = startY;
						guiSliders[objectIndex].attributes.boundingBox.w = width;
						guiSliders[objectIndex].attributes.boundingBox.h = height;
					}
				break;

			case GUI_OBJECT_KEYCODE:
				guiKeyCodes[objectIndex].attributes.coordType = coordType;
				if (GUI_COORD_TYPE_PERCENT == guiKeyCodes[objectIndex].attributes.coordType)
					{
						guiKeyCodes[objectIndex].attributes.boundingBox.x = winWidth * (startX / 100);
						guiKeyCodes[objectIndex].attributes.boundingBox.y = winHeight * (startY / 100);
						guiKeyCodes[objectIndex].attributes.boundingBox.w = winWidth * (width / 100);
						guiKeyCodes[objectIndex].attributes.boundingBox.h = winHeight * (height / 100);
					}
				else
					{
						guiKeyCodes[objectIndex].attributes.boundingBox.x = startX;
						guiKeyCodes[objectIndex].attributes.boundingBox.y = startY;
						guiKeyCodes[objectIndex].attributes.boundingBox.w = width;
						guiKeyCodes[objectIndex].attributes.boundingBox.h = height;
					}
				break;
		}
}


//-----------------------------------------------------------------------------
//
// Set label string and position for an object
//
// Need to check that setPosition has been called before this - otherwise boundingbox isn't set
void gui_hostSetObjectLabel(int guiObjectType, string objectID, int labelPos, string newLabel)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	io_logToFile("Setting label for - [ %s ]", objectID.c_str());
#endif // DEBUG_GUI_SETUP

	if (objectID == "buttonVideoOptionsApply")
		io_logToFile("Started setLabel [ %s ]", objectID.c_str());

	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = gui_findIndex(guiObjectType, objectID);
	if (-1 == objectIndex)
		{
			io_logToFile("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str());
			return;
		}

	sys_setFont(FONT_GUI);

	switch (guiObjectType)
		{
			case GUI_OBJECT_BUTTON:
//				if (strlen(newLabel.c_str()) > GUI_LABEL_SIZE)
//					guiButtons[objectIndex].label = "Length error";
//				else
					guiButtons[objectIndex].label = newLabel;

				guiButtons[objectIndex].labelPos = labelPos;
				guiButtons[objectIndex].labelStartY = guiButtons[objectIndex].boundingBox.y + (guiButtons[objectIndex].boundingBox.h - al_get_font_line_height(font[currentFont].ttfFont)) / 4;

				switch (labelPos)
					{
						case GUI_LABEL_CENTER:
							guiButtons[objectIndex].labelStartX = guiButtons[objectIndex].boundingBox.x + ((guiButtons[objectIndex].boundingBox.w - al_get_text_width(font[currentFont].ttfFont, guiButtons[objectIndex].label.c_str())) / 2);
							break;

						case GUI_LABEL_LEFT:
							guiButtons[objectIndex].labelStartX = guiButtons[objectIndex].boundingBox.x;
							break;

						case GUI_LABEL_RIGHT:
							guiButtons[objectIndex].labelStartX = (guiButtons[objectIndex].boundingBox.x + guiButtons[objectIndex].boundingBox.w) - al_get_text_width(font[currentFont].ttfFont, guiButtons[objectIndex].label.c_str());
							break;
					}
				break;

			case GUI_OBJECT_CHECKBOX:

//				if (strlen(newLabel.c_str()) > GUI_LABEL_SIZE)
//					guiCheckBoxes[objectIndex].attributes.label = "Length error";
//				else
					guiCheckBoxes[objectIndex].attributes.label = newLabel;

				guiCheckBoxes[objectIndex].attributes.labelStartX = guiCheckBoxes[objectIndex].attributes.boundingBox.x + al_get_font_line_height(font[currentFont].ttfFont) + guiCheckBoxes[objectIndex].labelGap;
				guiCheckBoxes[objectIndex].attributes.labelStartY = guiCheckBoxes[objectIndex].attributes.boundingBox.y;

				break;

			case GUI_OBJECT_TEXT_LABEL:

//				if (strlen(newLabel.c_str()) > GUI_LABEL_SIZE)
//					guiTextLabels[objectIndex].label = "Length error";
//				else
					guiTextLabels[objectIndex].label = newLabel;

				if (GUI_COORD_TYPE_PERCENT == guiTextLabels[objectIndex].coordType)
					{
						guiTextLabels[objectIndex].labelStartX = guiTextLabels[objectIndex].boundingBox.x - (al_get_text_width(font[currentFont].ttfFont, guiTextLabels[objectIndex].label.c_str()) / 2);
						guiTextLabels[objectIndex].labelStartY = guiTextLabels[objectIndex].boundingBox.y;
					}
				else
					{
						guiTextLabels[objectIndex].labelStartX = guiTextLabels[objectIndex].boundingBox.x;
						guiTextLabels[objectIndex].labelStartY = guiTextLabels[objectIndex].boundingBox.y;
					}

				break;

			case GUI_OBJECT_TEXT_BOX:
				guiTextBoxes[objectIndex].label = newLabel;
				break;

			case GUI_OBJECT_SLIDER:
				guiSliders[objectIndex].attributes.label = newLabel;
				break;

			case GUI_OBJECT_KEYCODE:
				guiKeyCodes[objectIndex].attributes.label = newLabel;
				break;
		}
}


//-----------------------------------------------------------------------------
//
// Set the script function to run when clicked or mouse is over
void gui_hostSetObjectFunctions(int guiObjectType, string objectID, string clickFunction, string mouseFunction)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	io_logToFile("Setting the function call for [ %s ] - [ %s ]", objectID.c_str(), clickFunction.c_str());
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = gui_findIndex(guiObjectType, objectID);
	if (-1 == objectIndex)
		{
			io_logToFile("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str());
			return;
		}

	switch (guiObjectType)
		{
				//
				// Actions for a object
			case GUI_OBJECT_BUTTON:
				guiButtons[objectIndex].action = clickFunction;
//        strcpy(guiButton[whichObject].action, mouseFunction->c_str());
				break;

			case GUI_OBJECT_CHECKBOX:
				guiCheckBoxes[objectIndex].attributes.action = clickFunction;
//        strcpy(guiCheckBox[whichObject].attributes.action, mouseFunction->c_str());
				break;

			case GUI_OBJECT_KEYCODE:
				guiKeyCodes[objectIndex].attributes.action = clickFunction;
				break;
		}
}

//-----------------------------------------------------------------------------
//
// Set the color for the passed in element for this object
void gui_setObjectColorByIndex(int guiObjectType, int objectIndex, int whichColor, float red, float green, float blue, float alpha)
//-----------------------------------------------------------------------------
{
	switch (guiObjectType)
		{
				//
				// Colors for guiButton
			case GUI_OBJECT_BUTTON:

				switch(whichColor)
					{
						case GUI_BUTTON_ACTIVE_COL:
							guiButtons[objectIndex].activeCol = al_map_rgba_f(red, green, blue, alpha);
							break;

						case GUI_BUTTON_INACTIVE_COL:
							guiButtons[objectIndex].inActiveCol = al_map_rgba_f(red, green, blue, alpha);
							break;

						case GUI_BUTTON_ACTIVE_EDGE_COL:
							guiButtons[objectIndex].edgeActiveCol = al_map_rgba_f(red, green, blue, alpha);
							break;

						case GUI_BUTTON_INACTIVE_EDGE_COL:
							guiButtons[objectIndex].edgeInActiveCol = al_map_rgba_f(red, green, blue, alpha);
							break;

						case GUI_BUTTON_ACTIVE_LABEL_COL:
							guiButtons[objectIndex].activeLabelCol = al_map_rgba_f(red, green, blue, alpha);
							break;

						case GUI_BUTTON_INACTIVE_LABEL_COL:
							guiButtons[objectIndex].inActiveLabelCol = al_map_rgba_f(red, green, blue, alpha);
							break;
					}
				break;
				//
				// Colors for guiCheckBox
			case GUI_OBJECT_CHECKBOX:

				switch(whichColor)
					{
						case GUI_CHECKBOX_TICKED_COL:
							guiCheckBoxes[objectIndex].attributes.activeCol = al_map_rgba_f(red, green, blue, alpha);
							guiCheckBoxes[objectIndex].attributes.activeLabelCol = al_map_rgba_f(red, green, blue, alpha);
							break;

						case GUI_CHECKBOX_NOT_TICKED_COL:
							guiCheckBoxes[objectIndex].attributes.inActiveCol = al_map_rgba_f(red, green, blue, alpha);
							guiCheckBoxes[objectIndex].attributes.inActiveLabelCol = al_map_rgba_f(red, green, blue, alpha);
							break;
							//
							// Values to be set by default
							guiCheckBoxes[objectIndex].ticked = false;
					}
				break;

				//
				// Colors for guiCheckBox
			case GUI_OBJECT_TEXT_LABEL:

				switch(whichColor)
					{
						case GUI_OBJECT_TEXT_LABEL_COLOR:
							guiTextLabels[objectIndex].activeCol = al_map_rgba_f(red, green, blue, alpha);
							break;
					}
				break;

			case GUI_OBJECT_IMAGE:
				guiImages[objectIndex].tint = al_map_rgba_f(red, green, blue, alpha);
				break;

			case GUI_OBJECT_TEXT_BOX:
				switch (whichColor)
					{
						case GUI_OBJECT_TEXT_LABEL_COLOR:
							guiTextBoxes[objectIndex].activeCol = al_map_rgba_f(red, green, blue, alpha);
							break;
					}
				break;

			case GUI_OBJECT_SLIDER:
				switch (whichColor)
					{
						case GUI_SLIDER_BAR_COL_ACTIVE:
							guiSliders[objectIndex].attributes.activeCol = al_map_rgba_f(red, green, blue, alpha);
							break;

						case GUI_SLIDER_BAR_COL_NOT_ACTIVE:
							guiSliders[objectIndex].attributes.inActiveCol = al_map_rgba_f(red, green, blue, alpha);
							break;

						case GUI_SLIDER_SELECTOR_COL_ACTIVE:
							break;

						case GUI_SLIDER_SELECTOR_COL_NOT_ACTIVE:
							break;

						case GUI_SLIDER_LABEL_COL_ACTIVE:
							guiSliders[objectIndex].attributes.activeLabelCol = al_map_rgba_f(red, green, blue, alpha);
							break;

						case GUI_SLIDER_LABEL_COL_NOT_ACTIVE:
							guiSliders[objectIndex].attributes.inActiveLabelCol = al_map_rgba_f(red, green, blue, alpha);
							break;

						case GUI_SLIDER_VALUE_COL_ACTIVE:
							break;

						case GUI_SLIDER_VALUE_COL_NOT_ACTIVE:
							break;
					}
					break;

			case GUI_OBJECT_KEYCODE:
				switch(whichColor)
					{
						case GUI_BUTTON_ACTIVE_COL:
							guiKeyCodes[objectIndex].attributes.activeCol = al_map_rgba_f(red, green, blue, alpha);
							break;

						case GUI_BUTTON_INACTIVE_COL:
							guiKeyCodes[objectIndex].attributes.inActiveCol = al_map_rgba_f(red, green, blue, alpha);
							break;
					}
					break;


				/*
				        case GUI_OBJECT_POPUP:

				            switch (whichColor)
				                {
				                case GUI_OBJECT_TEXT_LABEL_COLOR:
				//                    guiPopUp.attributes.activeCol = al_map_rgba_f(red, green, blue, alpha);
				                    break;
				                }
								 * */
		}
}


//-----------------------------------------------------------------------------
//
// Host function called by script to set object colors
void gui_hostSetObjectColor(int guiObjectType, string objectID, int whichColor, float red, float green, float blue, float alpha)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	io_logToFile("Setting colors for - [ %s ]", objectID.c_str());
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;
	int	numObjects = 0;

	if (objectID == "ALL")	// Are we setting the colors for all of this type of object
		{
			switch (guiObjectType)	// Find out how many of this type there are
				{
					case GUI_OBJECT_BUTTON:
						numObjects = guiButtons.size();
						break;

					case GUI_OBJECT_CHECKBOX:
						numObjects = guiCheckBoxes.size();
						break;

					case GUI_OBJECT_TEXT_LABEL:
						numObjects = guiTextLabels.size();
						break;

					case GUI_OBJECT_TEXT_BOX:
						numObjects = guiTextBoxes.size();
						break;
					case GUI_OBJECT_SLIDER:
						numObjects = guiSliders.size();
						break;

					case GUI_OBJECT_KEYCODE:
						numObjects = guiKeyCodes.size();
						break;
				}

			if (numObjects == 0)
				return;

			for (int i = 0; i != numObjects; i++)	// Loop through each object and set it's color
				gui_setObjectColorByIndex(guiObjectType, i, whichColor, red, green, blue, alpha);
		}
	else
		{
			//
			// Find the index for this object
			objectIndex = gui_findIndex(guiObjectType, objectID);
			if (-1 == objectIndex)
				{
					io_logToFile("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str());
					return;
				}
			gui_setObjectColorByIndex(guiObjectType, objectIndex, whichColor, red, green, blue, alpha);
		}
}

//-----------------------------------------------------------------------------
//
// Set the group that this checkbox belongs to
void gui_hostSetCheckboxGroup(string objectID, int whichGroup)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	io_logToFile("Setting checkBox group for - [ %s ]", objectID.c_str());
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = gui_findIndex(GUI_OBJECT_CHECKBOX, objectID);
	if (-1 == objectIndex)
		{
			io_logToFile("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str());
			return;
		}

	guiCheckBoxes[objectIndex].whichGroup = whichGroup;
}


//-----------------------------------------------------------------------------
//
// Set a checkbox to ticked
// Go through other members of the group and untick them
void gui_hostSetCheckboxTick(string objectID, int whichGroup, bool ticked)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	io_logToFile("Setting checkBox tick for - [ %s ]", objectID.c_str());
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = gui_findIndex(GUI_OBJECT_CHECKBOX, objectID);
	if (-1 == objectIndex)
		{
			io_logToFile("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str());
			return;
		}

	if (-1 == whichGroup)
		{
			// Not part of a group
			guiCheckBoxes[objectIndex].ticked = ticked;
			return;
		}

	// TODO: Check this is working ok

	if (true == guiCheckBoxes[objectIndex].ticked)
		{
			guiCheckBoxes[objectIndex].ticked = false;
			return;
		}

	for (unsigned int i = 0; i != guiCheckBoxes.size(); i++)
		{
			if (whichGroup == guiCheckBoxes[i].whichGroup)
				guiCheckBoxes[i].ticked = false;
		}
	//
	// Now set the checkbox to ticked
	guiCheckBoxes[objectIndex].ticked = true;
}

//-----------------------------------------------------------------------------
//
// Set the filename for a GUI image and attempt to load it
void gui_hostSetImageFilename(string objectID, string fileName)
//-----------------------------------------------------------------------------
{
	int objectIndex;

	objectIndex = gui_findIndex(GUI_OBJECT_IMAGE, objectID);
	if (-1 == objectIndex)
		{
			io_logToFile("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str());
		}

	guiImages[objectIndex].bitmapFileName = fileName;
	if (false == guiImages[objectIndex].imageLoaded)
		{
			guiImages[objectIndex].bitmap = al_load_bitmap (guiImages[objectIndex].bitmapFileName.c_str());
			if (NULL == guiImages[objectIndex].bitmap)
				{
					guiImages[objectIndex].imageLoaded = false;
				}
			else
				{
					guiImages[objectIndex].imageLoaded = true;
				}
		}
}

//-----------------------------------------------------------------------------
//
// Create a new GUI screen which will contain other GUI objects
void gui_hostCreateNewScreen(string screenID)
//-----------------------------------------------------------------------------
{
	tmp_screen.screenID = screenID;
	tmp_screen.selectedObject = 0;      // default to first object on screen having focus

	guiScreens.push_back(tmp_screen);

	io_logToFile("Screen [ %s ] created.", screenID.c_str());
}

//-----------------------------------------------------------------------------
//
// Associate the object to a screen, recording it's index and type
void gui_hostAddObjectToScreen(int guiObjectType, string objectID, string whichScreen)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	io_logToFile("Adding [ %s ] to screen [ %s ]", objectID.c_str(), whichScreen.c_str());
#endif // DEBUG_GUI_SETUP

	int screenIndex = 0;
	int objectIndex = 0;
	//
	// Find the index for this screen
	screenIndex = gui_findIndex(GUI_OBJECT_SCREEN, whichScreen);
	if (-1 == screenIndex)
		{
			io_logToFile("ERROR: Couldn't find GUI screen index [ %s ]", whichScreen.c_str());
			return;
		}
	//
	// Find the index for this object
	objectIndex = gui_findIndex(guiObjectType, objectID);
	if (-1 == objectIndex)
		{
			io_logToFile("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str());
			return;
		}

	switch (guiObjectType)
		{
			case GUI_OBJECT_BUTTON:
			case GUI_OBJECT_CHECKBOX:
			case GUI_OBJECT_TEXT_LABEL:
			case GUI_OBJECT_IMAGE:
			case GUI_OBJECT_TEXT_BOX:
			case GUI_OBJECT_SLIDER:
			case GUI_OBJECT_KEYCODE:
				guiScreens[screenIndex].objectIDIndex.push_back(objectIndex);    // Add objectIndex to the end
				guiScreens[screenIndex].objectType.push_back(guiObjectType);

#ifdef DEBUG_GUI_SETUP
				io_logToFile("Screen [ %s ] objectID [ %i ] - Added", guiScreens[screenIndex].screenID.c_str(), guiScreens[screenIndex].objectIDIndex.back());
				io_logToFile("-------------");
#endif
				break;
		}
}

//----------------------------------------------------------------
//
// Print out the GUI system
void gui_debugGUI()
//----------------------------------------------------------------
{
	int screenCount;
	int objectCount;

	screenCount = 0;
	objectCount = 0;

	for (vector<_screenObject>::iterator it = guiScreens.begin(); it != guiScreens.end(); ++it)
		{
			// for each screen, display each objectID
			io_logToFile("Screen [ %s ] Screencount [ %i ]", guiScreens[screenCount].screenID.c_str(), screenCount);

			objectCount = 0;
			for (vector<int>::iterator obj_it = guiScreens[screenCount].objectIDIndex.begin(); obj_it != guiScreens[screenCount].objectIDIndex.end(); ++obj_it)
				{
					switch (guiScreens[screenCount].objectType[objectCount])
						{
							case GUI_OBJECT_BUTTON:
								io_logToFile("-->Button [ %s ] objectIDIndex [ %i ]", guiButtons[guiScreens[screenCount].objectIDIndex[objectCount]].objectID.c_str(), guiScreens[screenCount].objectIDIndex[objectCount]);
								break;

							case GUI_OBJECT_CHECKBOX:
								io_logToFile("-->Checkbox [ %s ] objectIDIndex [ %i ]", guiCheckBoxes[guiScreens[screenCount].objectIDIndex[objectCount]].attributes.objectID.c_str(), guiScreens[screenCount].objectIDIndex[objectCount]);
								break;

							case GUI_OBJECT_TEXT_LABEL:
								io_logToFile("-->TextLabel [ %s ] objectIDIndex [ %i ]", guiTextLabels[guiScreens[screenCount].objectIDIndex[objectCount]].objectID.c_str(), guiScreens[screenCount].objectIDIndex[objectCount]);
								break;

							case GUI_OBJECT_KEYCODE:
								io_logToFile("-->KeyLabel [ %s ] objectIDIndex [ %i ]", guiKeyCodes[guiScreens[screenCount].objectIDIndex[objectCount]].attributes.objectID.c_str(), guiScreens[screenCount].objectIDIndex[objectCount]);
						}
					objectCount++;
				}
			screenCount++;
		}

}
