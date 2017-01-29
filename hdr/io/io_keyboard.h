#pragma once

extern float		useKeysDelay;	// From scripts
extern float		consoleKeysDelay;

// Read the keyboard
void io_readKeyboard();

// Read a key for keycode capture
void io_readKeyCodeInput(ALLEGRO_EVENT event);
