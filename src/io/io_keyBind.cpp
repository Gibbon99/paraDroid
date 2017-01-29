
#include "../../hdr/sys_globals.h"

ALLEGRO_FILE	*filePtr;

_inputAction	inputAction[NUMBER_ACTIONS];

// ---------------------------------------------------------------------------
//
// Show the keybinds set
bool io_printKeyBind()
// ---------------------------------------------------------------------------
{
	string keyTest;

	static int count = 0;

	con_print(true, false, "Called from io_printKeyBind - [ %i ]", count);
	con_print(true, false, "------------------------------------");

	for (int i = 0; i != NUMBER_ACTIONS; i++)
		{
			con_setColor(0.0f, 1.0f, 0.0f, 1.0f);
			keyTest = inputAction[i].stringValue.c_str();
			keyTest.append(" : [ ");
			keyTest.append(al_keycode_to_name(inputAction[i].keyValue));
			keyTest.append("]");
			con_print(true, false, "[ %s ]", keyTest.c_str());
//			con_print(true, false, "Keybind: [ %s ] = [ %i ]", inputAction[i].stringValue.c_str(), inputAction[i].keyValue);
		}

	return true;
}

//-----------------------------------------------------------------------------
//
// Set all keys to false state
void io_resetKeyboardStates()
//-----------------------------------------------------------------------------
{
	for (int i = 0; i != NUMBER_ACTIONS; i++)
		inputAction[i].currentlyDown = false;
}

// ---------------------------------------------------------------------------
//
// assign the keyvalues used for actions
void io_mapActionKeys()
// ---------------------------------------------------------------------------
{
	inputAction[gameLeft].keyValue                  = ALLEGRO_KEY_LEFT;
	inputAction[gameLeft].stringValue = "Game Left";

	inputAction[gameRight].keyValue                 = ALLEGRO_KEY_RIGHT;
	inputAction[gameRight].stringValue = "Game Right";

	inputAction[gameUp].keyValue                    = ALLEGRO_KEY_UP;
	inputAction[gameUp].stringValue = "Game Up";

	inputAction[gameDown].keyValue                  = ALLEGRO_KEY_DOWN;
	inputAction[gameDown].stringValue = "Game Down";

	inputAction[gameFire].keyValue                  = ALLEGRO_KEY_LCTRL;
	inputAction[gameFire].stringValue = "Game action";

	inputAction[gameEscape].keyValue                = ALLEGRO_KEY_ESCAPE;
	inputAction[gameEscape].stringValue = "Exit the game";

	inputAction[actionScreenShot].keyValue          = ALLEGRO_KEY_F12;
	inputAction[actionScreenShot].stringValue = "Save Screenshot";

	inputAction[consoleAction].keyValue             = ALLEGRO_KEY_TILDE;
	inputAction[consoleAction].stringValue = "Show console";

	inputAction[menuUp].keyValue                    = ALLEGRO_KEY_UP;
	inputAction[menuUp].stringValue = "Menu Up";

	inputAction[menuDown].keyValue                  = ALLEGRO_KEY_DOWN;
	inputAction[menuDown].stringValue = "Menu Down";

	inputAction[menuLeft].keyValue					= ALLEGRO_KEY_LEFT;
	inputAction[menuLeft].stringValue = "Menu Left";

	inputAction[menuRight].keyValue					= ALLEGRO_KEY_RIGHT;
	inputAction[menuRight].stringValue = "Menu Right";

	inputAction[menuAction].keyValue                = ALLEGRO_KEY_ENTER;
	inputAction[menuAction].stringValue = "Menu action";

	inputAction[gamePause].keyValue                 = ALLEGRO_KEY_P;
	inputAction[gamePause].stringValue = "Pause Game";
	//
	// Clear out the state
	//
	io_resetKeyboardStates();
}

//-----------------------------------------------------------------------------
//
// Read in the keybindings used from the file
bool io_readKeyBindings (string fileName)
//-----------------------------------------------------------------------------
{
	uint		numRead = 0;
	size_t		stringSize;

	filePtr = al_fopen(fileName.c_str(), "rb");
	if (NULL == filePtr)
		sys_errorFatal(__FILE__, __LINE__, "Keybind: Error: Could not open file [ %s ] for reading.", fileName.c_str());

	for (int i = 0; i != NUMBER_ACTIONS; i++)
		{
			al_fread(filePtr, &inputAction[i].keyValue, sizeof(inputAction[i].keyValue));
			al_fread(filePtr, &stringSize, sizeof(stringSize));
			numRead = al_fread(filePtr, inputAction[i].stringValue.c_str(), stringSize);

			if (numRead != stringSize)
				{
					con_print(true, false, "Keybind: Error: Read error [ %s ]", "UNKNOWN");
					al_fclose(filePtr);
					return false;
				}
		}
	al_fclose(filePtr);

	con_print(true, false, "Keybind: Info: Key states read from file.");

	io_printKeyBind();

	return true;
}

//-----------------------------------------------------------------------------
//
// Save the current key bindings for the keyboard
bool io_saveKeyBindings (string fileName)
//-----------------------------------------------------------------------------
{
	uint		numWrite = 0;
	size_t		stringSize;
	char		tempString[32];

	filePtr = al_fopen(fileName.c_str(), "wb");
	if (NULL == filePtr)
		{
			con_print(true, false, "Keybind: Error: Write error [ %s ]", "UNKNOWN");
			sys_errorFatal(__FILE__, __LINE__, "Keybind: Error: Could not open file [ %s ] for writing.", fileName.c_str());
		}

	for (int i = 0; i != NUMBER_ACTIONS; i++)
		{
			//stringSize = inputAction[i].stringValue.size();
			//
			// Insert hack to get strings writing out correctly

			strcpy(tempString, inputAction[i].stringValue.c_str());
			stringSize = strlen(tempString);

			al_fwrite(filePtr, &inputAction[i].keyValue, sizeof(inputAction[i].keyValue));
			al_fwrite(filePtr, &stringSize, sizeof(stringSize));
			numWrite = al_fwrite(filePtr, tempString, strlen(tempString));

			if (numWrite != stringSize)
				{
					con_print(true, false, "Keybind: Error: Write error [ %s ]", "UNKNOWN");
					al_fclose(filePtr);
					return false;
				}
		}
	al_fclose(filePtr);

	con_print(true, false, "Keybind: Info: Key states saved to file.");
	return true;
}

//-----------------------------------------------------------------------------
//
// Perform the keybind functions - if keybind file does not exist then populate
// the inputActions array and save it, then reload it
bool io_doKeyBindings(string fileName)
//-----------------------------------------------------------------------------
{
	if (NULL == al_fopen(fileName.c_str(), "rb"))
		{
			con_print(true, false, "Keybind: Error: Keybind file not found. Creating default.");
			io_mapActionKeys();
			if (false == io_saveKeyBindings(fileName.c_str()))
				{
					con_print(true, false, "Keybind: Error: Couldn't create default keybind file.");
					return false;
				}
			if (false == io_readKeyBindings(fileName.c_str()))
				{
					con_print(true, false, "Keybind: Error: Couldn't read default keybind file.");
					return false;
				}
		}
	else	// file already exists so just read it in
		{
			if (false == io_readKeyBindings(fileName.c_str()))
				{
					con_print(true, false, "Keybind: Error: Couldn't read keybind file.");
					io_mapActionKeys();	// Use default ones
					return false;
				}
		}
	return true;
}
