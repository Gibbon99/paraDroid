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

int		currentLanguage;

typedef struct
{
	string		key;
	string		textString;
} _textStrings;

vector<_textStrings>		textStrings;

//--------------------------------------------------------
//
// Return a string based on the key
string gui_getString(string key)
//--------------------------------------------------------
{
	for (vector<_textStrings>::iterator itr = textStrings.begin(); itr != textStrings.end(); ++itr)
	{
		if (key == itr->key)
		{
			return itr->textString;
		}
	}
	return key + " not found";
}

//--------------------------------------------------------
//
// Add a key and string to the map
void gui_addKeyAndText(string key, string textString)
//--------------------------------------------------------
{
	_textStrings		tmpString;

	tmpString.key = key;
	tmpString.textString = textString;

	textStrings.push_back(tmpString);
}