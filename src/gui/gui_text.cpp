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