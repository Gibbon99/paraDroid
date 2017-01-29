#include "../../hdr/sys_globals.h"

int				currentGUIScreen;
int          	currentObjectSelected;
float			buttonCurveX;
float			buttonCurveY;
float			sliderSelectorRadius;
float			sliderCurveX;
float			sliderCurveY;

int				checkBoxTickSize;

bool    		guiReady;

vector<_screenObject>   guiScreens;
vector<_guiObject>      guiButtons;
vector<_guiCheckBox>    guiCheckBoxes;
vector<_guiObject>      guiTextLabels;
vector<_guiImage>		guiImages;
vector<_guiObject>		guiTextBoxes;
vector<_guiSlider>		guiSliders;
vector<_guiKeyCode>		guiKeyCodes;

//-----------------------------------------------------------------------------
//
// Call this first before using GUI
bool gui_initGUI()
//-----------------------------------------------------------------------------
{
	//
	// Start by calling script to setup the buttons
	if (false == sys_executeScriptFunction("scr_guiSetupGUI", ""))
		{
			con_print(true, true, "Error: gui_setupGUI: Failed to execute script to start GUI");
			guiReady = false;
			return false;
		}

	guiScreens[0].selectedObject = 0;
	currentGUIScreen = 0;
	guiReady = true;

//	gui_debugGUI();

	return true;
}

//-----------------------------------------------------------------------------
//
// Return the text value of an ASCII index
string gui_IntToChar(size_t intValue)
//-----------------------------------------------------------------------------
{
	std::string const charIndex ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	string returnValue;

	returnValue = charIndex.at(intValue);
	return returnValue;
}

//-----------------------------------------------------------------------------
//
// Convert a string to a Int value
int gui_StringToInt(string value)
//-----------------------------------------------------------------------------
{
	return atoi(value.c_str());
}

//-----------------------------------------------------------------------------
//
// Convert int value to string and return
string gui_IntToString(int intValue)
//-----------------------------------------------------------------------------
{
	return std::to_string(intValue);
}

//-----------------------------------------------------------------------------
//
// Change to a new GUI screen
void gui_changeToGUIScreen(int newScreen)
//-----------------------------------------------------------------------------
{
	currentGUIScreen = newScreen;
}

//-----------------------------------------------------------------------------
//
// Return if an object can be selected or not
bool gui_canObjectBeSelected(int objectType)
//-----------------------------------------------------------------------------
{
	switch (objectType)
		{
			case GUI_OBJECT_BUTTON:
			case GUI_OBJECT_CHECKBOX:
			case GUI_OBJECT_SLIDER:
			case GUI_OBJECT_KEYCODE:
				return true;
		}
	return false;
}

//-----------------------------------------------------------------------------
//
// Move focus to next object
void gui_handleFocusMove(int moveDirection, bool takeAction)
//-----------------------------------------------------------------------------
{
	int indexCount = 0;
	int selectedSlider, selectedKeyCode;

	indexCount = 0;

	selectedSlider = guiScreens[currentGUIScreen].objectIDIndex[guiScreens[currentGUIScreen].selectedObject];
	selectedKeyCode = guiScreens[currentGUIScreen].objectIDIndex[guiScreens[currentGUIScreen].selectedObject];

	if (true == takeAction)
		{
			currentObjectSelected = guiScreens[currentGUIScreen].objectIDIndex[guiScreens[currentGUIScreen].selectedObject];

			switch (guiScreens[currentGUIScreen].objectType[guiScreens[currentGUIScreen].selectedObject])
				{
					case GUI_OBJECT_BUTTON:
						sys_executeScriptFunction(guiButtons[currentObjectSelected].action.c_str(), guiButtons[currentObjectSelected].objectID.c_str());
						break;

					case GUI_OBJECT_CHECKBOX:
						sys_executeScriptFunction(guiCheckBoxes[currentObjectSelected].attributes.action.c_str(), guiCheckBoxes[currentObjectSelected].attributes.objectID.c_str());
						break;

					case GUI_OBJECT_KEYCODE:
						sys_executeScriptFunction(guiKeyCodes[currentObjectSelected].attributes.action.c_str(), guiKeyCodes[currentObjectSelected].attributes.objectID.c_str());
						break;
				}
			return;
		}


	switch (moveDirection)
		{
			case FOCUS_NEXT:
				indexCount = 1;
				if (guiScreens[currentGUIScreen].selectedObject != (int)guiScreens[currentGUIScreen].objectIDIndex.size() - 1)	// Don't go past number of objects on the screen
					{
						while (gui_canObjectBeSelected(guiScreens[currentGUIScreen].objectType[guiScreens[currentGUIScreen].selectedObject + indexCount]) == false)
							{
								indexCount++;
							}
						guiScreens[currentGUIScreen].selectedObject += indexCount;

						if (indexCount > guiScreens[currentGUIScreen].objectIDIndex.size())
							{
								indexCount = guiScreens[currentGUIScreen].objectIDIndex.size();
							}

						currentObjectSelected = guiScreens[currentGUIScreen].selectedObject;
					}
				break;

			case FOCUS_PREVIOUS:
				indexCount = 1;
				if (guiScreens[currentGUIScreen].selectedObject > 0)
					{
						while (gui_canObjectBeSelected(guiScreens[currentGUIScreen].objectType[guiScreens[currentGUIScreen].selectedObject - indexCount]) == false)
							{
								indexCount++;
								if (guiScreens[currentGUIScreen].selectedObject - indexCount < 0)
									return;
							}
						guiScreens[currentGUIScreen].selectedObject -= indexCount;

						if (guiScreens[currentGUIScreen].selectedObject < 0)
							guiScreens[currentGUIScreen].selectedObject = 0;

						currentObjectSelected = guiScreens[currentGUIScreen].selectedObject;
					}
				break;

			case FOCUS_LEFT:
				switch (guiScreens[currentGUIScreen].objectType[guiScreens[currentGUIScreen].selectedObject])
					{
						case GUI_OBJECT_KEYCODE:
							guiKeyCodes[selectedKeyCode].elementFocus -= 1;
							if (guiKeyCodes[selectedKeyCode].elementFocus < 0)
								guiKeyCodes[selectedKeyCode].elementFocus = guiKeyCodes[selectedKeyCode].element.size() - 1;
							break;

						case GUI_OBJECT_SLIDER:
							guiSliders[selectedSlider].currentStep -= 1;
							if (guiSliders[selectedSlider].currentStep < 0)
								guiSliders[selectedSlider].currentStep = 0;
							break;
					}
				break;

			case FOCUS_RIGHT:
				switch (guiScreens[currentGUIScreen].objectType[guiScreens[currentGUIScreen].selectedObject])
					{
						case GUI_OBJECT_KEYCODE:
							guiKeyCodes[selectedKeyCode].elementFocus += 1;
							if (guiKeyCodes[selectedKeyCode].elementFocus > guiKeyCodes[selectedKeyCode].element.size() - 1)
								guiKeyCodes[selectedKeyCode].elementFocus = 0;
							break;

						case GUI_OBJECT_SLIDER:
							guiSliders[selectedSlider].currentStep += 1;
							if (guiSliders[selectedSlider].currentStep > guiSliders[selectedSlider].element.size() - 1)
								guiSliders[selectedSlider].currentStep = guiSliders[selectedSlider].element.size() - 1;
							break;
					}
				break;
		}
}

//-----------------------------------------------------------------------------
//
// Populate the score table with text
void gui_populateScoreTableGUI()
//-----------------------------------------------------------------------------
{
	gui_hostSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreInitialsOne", GUI_LABEL_CENTER, gam_getScoreElement(SCORE_INITIALS, 0));
	gui_hostSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreInitialsTwo", GUI_LABEL_CENTER, gam_getScoreElement(SCORE_INITIALS, 1));
	gui_hostSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreInitialsThree", GUI_LABEL_CENTER, gam_getScoreElement(SCORE_INITIALS, 2));
	gui_hostSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreInitialsFour", GUI_LABEL_CENTER, gam_getScoreElement(SCORE_INITIALS, 3));
	gui_hostSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreInitialsFive", GUI_LABEL_CENTER, gam_getScoreElement(SCORE_INITIALS, 4));
//
// Setup text boxes for score
//
	gui_hostSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreScoreOne", GUI_LABEL_CENTER, gam_getScoreElement(SCORE_SCORE, 0));
	gui_hostSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreScoreTwo", GUI_LABEL_CENTER, gam_getScoreElement(SCORE_SCORE, 1));
	gui_hostSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreScoreThree", GUI_LABEL_CENTER, gam_getScoreElement(SCORE_SCORE, 2));
	gui_hostSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreScoreFour", GUI_LABEL_CENTER, gam_getScoreElement(SCORE_SCORE, 3));
	gui_hostSetObjectLabel ( GUI_OBJECT_TEXT_BOX, "scoreScoreFive", GUI_LABEL_CENTER, gam_getScoreElement(SCORE_SCORE, 4));

}