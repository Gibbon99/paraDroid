#pragma once

//-----------------------------------------------------------------------------
//
// Enum different types for the keyboard actions keys
//
//-----------------------------------------------------------------------------

extern int *keystate;         // holds all the keystates

struct _inputAction
{
	int             keyValue;
	bool            currentlyDown;
	string          stringValue;
};

extern  _inputAction            inputAction[NUMBER_ACTIONS];

// assign the keyvalues used for actions
void io_mapActionKeys();

// Save the current key bindings for the keyboard
bool io_saveKeyBindings(string fileName);

// Read in the keybindings used from the file
bool io_readKeyBindings(string fileName);

// Perform the keybind functions - if keybind file does not exist then populate
// the inputActions array and save it, then reload it
bool io_doKeyBindings(string fileName);

// Set all keys to false state
void io_resetKeyboardStates();
