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

//-----------------------------------------------------------------------------
//
// Set the slider to the passed in value
void as_hostSetSliderValue(string objectID, string value)
//-----------------------------------------------------------------------------
{
	int indexCount = 0;

	for (vector<_guiSlider>::iterator it = guiSliders.begin(); it != guiSliders.end(); ++it)
	{
		if (it->attributes.objectID == objectID)
		{
			for (vector<_sliderElement>::iterator stepItr = it->element.begin(); stepItr != it->element.end(); ++stepItr)
			{
				switch (stepItr->type)
				{
					case GUI_SLIDER_STRING:
					if (stepItr->value == value)
					{
						it->currentStep = indexCount;
						return;
					}
					break;
				}
				indexCount += 1;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
// Get the value of the current slot in a slider
string as_hostGetSliderValue(string objectID)
//-----------------------------------------------------------------------------
{
	for (vector<_guiSlider>::iterator it = guiSliders.begin(); it != guiSliders.end(); ++it)
	{
		if (it->attributes.objectID == objectID)
		{
			return it->element[it->currentStep].value;
		}
	}
	return "Slider ID not found";
}

//-----------------------------------------------------------------------------
//
// Add a new element to the names slider object
void as_hostAddSliderElement(string objectID, int elementType, string elementValue, string elementLabel)
//-----------------------------------------------------------------------------
{
	_sliderElement sliderElementTmp;

	sliderElementTmp.type = elementType;
	sliderElementTmp.value = elementValue;
	sliderElementTmp.label = elementLabel;

	for (vector<_guiSlider>::iterator it = guiSliders.begin(); it != guiSliders.end(); ++it)
		{
			if (it->attributes.objectID == objectID)
				{
					it->element.push_back(sliderElementTmp);
					return;
				}
		}
	con_print(true, false, "GUI Error: Slider objectID not found [ %s ]", objectID.c_str());
}
