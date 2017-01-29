#pragma once

extern cpVect 		viewableScreenCoord;
extern float        startStatusX;
extern float        startScoreX;

// Change to new currentMode
void sys_changeMode(int newMode, bool useFade);

// Convert worldPosition coords to screen coords
cpVect sys_worldToScreen(cpVect worldPos, int shapeSize);

// Is an object visible on the screen
bool sys_visibleOnScreen(cpVect worldCoord, int shapeSize);

// Calculate the display position of the text within the HUD graphic
void sys_calcHudTextPosition(int hudWidth, int hudHeight);

// Safely copy two strings.
size_t strlcpy(char *dst, const char *src, size_t size);

size_t strlcat(char *dst, const char *src, size_t size);