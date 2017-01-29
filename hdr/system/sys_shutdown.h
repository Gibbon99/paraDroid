#pragma once

// Shutdown the game
bool sys_shutDown();

// Shutdown with error - fatal error
void sys_errorFatal(const char *sourcefile, int sourceline, const char *text, ...);
