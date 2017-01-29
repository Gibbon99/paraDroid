#pragma once

extern int		currentLanguage;

// Return a string based on the key
string gui_getString(string key);

// Add a key and string to the map
void gui_addKeyAndText(string key, string textString);