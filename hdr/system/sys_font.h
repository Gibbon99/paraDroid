#pragma once



#define NUM_FONTS		3

typedef struct
{
ALLEGRO_FONT		*ttfFont;
bool				valid;
char				fileName[MAX_STRING_SIZE];
int                 fontSize;
} _font;

extern _font		font[];
extern int			currentFont;

// State of the current font
extern ALLEGRO_COLOR	fontColor;

// Load a TTF font from scripts
bool io_loadttfFont(int fontIndex, int fontSize, string fontName);

// Start the TTF fonts to known values
void sys_initTTFSystem();

// Set the font color
void sys_setFontColor(float r, float g, float b, float a);

// Draw text string to main screen
void sys_printString(float posX, float posY, string textIn);

// If chosen font is not valid, use builtin font
void sys_printStringExt(float posX, float posY, const char *format, ...);

// Set the current font
void sys_setFont(int newFont);
