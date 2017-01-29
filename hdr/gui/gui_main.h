#pragma once

#define GUI_LABEL_SIZE			32

#define SLIDER_LABEL_GUTTER		7.0f

#define LABEL_GUTTER_HEIGHT 		4
#define LABEL_GUTTER_LEFT   		15
#define LABEL_GUTTER_RIGHT  		15

#define FOCUS_NEXT						0
#define FOCUS_PREVIOUS					1
#define FOCUS_LEFT						2
#define FOCUS_RIGHT						3

extern bool    guiReady;

typedef struct
{
	int x, y, w, h;
} _objCoords;

typedef struct
{
	int              	selectedObject;
	string           	screenID;          		// Name of this screen
	vector<int>     	objectIDIndex;          // Index into object array
	vector<int>    		objectType;        		// Which object array
} _screenObject;

typedef struct
{
	bool            	selected;       // Is the object currently selected for action
	bool				canFocus;		// Can object be selected
	string             	objectID;       // Unique for each type
	int             	objectType;     // Button, checkBox etc
	int             	screenID;       // Which screen will this control be drawn on
	int             	coordType;      // Method to work out positioning
	int              	labelPos;       // Where to place labels on buttons
	ALLEGRO_COLOR     	activeCol;
	ALLEGRO_COLOR     	inActiveCol;
	ALLEGRO_COLOR     	edgeActiveCol;
	ALLEGRO_COLOR     	edgeInActiveCol;
	ALLEGRO_COLOR     	activeLabelCol;             // text color for mouse over
	ALLEGRO_COLOR     	inActiveLabelCol;           // normal text color
	_objCoords      	boundingBox;                // used for mouse detection
	float           	labelStartX;
	float           	labelStartY;
	string            	label;      // Text associated with control
	string            	action;  	// Script to run
} _guiObject;

typedef struct
{
	_guiObject      attributes;
	int             buttonOne;  // Index into button array
	int             buttonTwo;
} _guiPopUp;

extern _guiPopUp   guiPopUp;

typedef struct
{
	_guiObject      attributes;
	bool            ticked;
	int             textGap;
	int             checkGap;
	int             labelGap;
	int             whichGroup;     // Group checkboxes together
} _guiCheckBox;

typedef struct
{
	_guiObject		attributes;
	ALLEGRO_BITMAP	*bitmap;
	ALLEGRO_COLOR	tint;
	bool			imageLoaded;
	string			bitmapFileName;
} _guiImage;

typedef struct
{
	string					label;		// Text to display
	string					value;		// Value of this element
	int						type;		// What type of element - convert on query
} _sliderElement;

typedef struct
{
	vector<_sliderElement>	element;		// Value for this step
	_guiObject				attributes;		// Standard attributes
	int						currentStep;	// What is selected
	int						numberSteps;	// How many elements
	int						barThickness;	// Thickness for drawing the bar
} _guiSlider;

typedef struct
{
	string					keyName;		// String name of the key
	int						keyCode;		// Value for the key code
} _keyCodeElement;

typedef struct
{
	vector<_keyCodeElement>	element;		// Each of the key names / codes
	_guiObject				attributes;		// Standard attributes
	int						elementFocus;	// Which element is currently selected
} _guiKeyCode;


extern vector<_screenObject>	guiScreens;
extern vector<_guiCheckBox>		guiCheckBoxes;
extern vector<_guiObject>		guiButtons;
extern vector<_guiObject>		guiTextLabels;
extern vector<_guiImage>		guiImages;
extern vector<_guiObject>		guiTextBoxes;
extern vector<_guiSlider>		guiSliders;
extern vector<_guiKeyCode>		guiKeyCodes;

extern int          currentObjectSelected;  // Pass this to script to act on

extern int			currentGUIScreen;
extern int			mousePosX;
extern int			mousePosY;

extern float		buttonCurveX;
extern float		buttonCurveY;
extern float		sliderSelectorRadius;
extern float		sliderCurveX;
extern float		sliderCurveY;
extern float		keyCodeGap;

extern float		rasterPosY;

extern int			checkBoxTickSize;

// Change to a new GUI screen
void gui_changeToGUIScreen(int newScreen);

// Look through the relevant vector to location the index of the objectID
//
// returns index value, or -1 if not found
int gui_findIndex(int guiObjectType, string objectID);

// Create a new GUI screen which will contain other GUI objects
void gui_hostCreateNewScreen(string screenID);

// Add a gui object to it's vector array
void gui_hostCreateObject(int guiObjectType, string objectID);

// Associate the object to a screen, recording it's index and type
void gui_hostAddObjectToScreen(int guiObjectType, string whichObject, string whichScreen);

// Host function called by script to set position values
void gui_hostSetObjectPosition(int guiObjectType, string objectID, int coordType, float startX, float startY, float width, float height);

// Set the script function to run when clicked or mouse is over
void gui_hostSetObjectFunctions(int guiObjectType, string objectID, string clickFunction, string mouseFunction);

// Host function called by script to set object colors
void gui_hostSetObjectColor(int guiObjectType, string objectID, int whichColor, float red, float green, float blue, float alpha);

// Set label string and position for an object
void gui_hostSetObjectLabel(int guiObjectType, string objectID, int labelPos, string newLabel);

// Set the group that this checkbox belongs to
void gui_hostSetCheckboxGroup(string objectID, int whichGroup);

// Set a checkbox to ticked
// Go through other members of the group and untick them
void gui_hostSetCheckboxTick(string objectID, int whichGroup, bool ticked);

// Set the filename for a GUI image and attempt to load it
void gui_hostSetImageFilename(string objectID, string fileName);

// Call this first before using GUI
bool gui_initGUI();

// Move focus to next object
void gui_handleFocusMove(int moveDirection, bool takeAction);

// Print out the GUI system
void gui_debugGUI();

// Draw the lines background
void gui_drawRasterLines();

// Draw the moving raster line for effect
void gui_animateRasterLine();

// Find the objectID on the current screen and make it selected
void gui_setObjectFocus(string objectID);

// Add a new element to the names slider object
void as_hostAddSliderElement(string objectID, int elementType, string elementValue, string elementLabel);

// Get the value of the current slot in a slider
string as_hostGetSliderValue(string objectID);

// Set the slider to the passed in value
void as_hostSetSliderValue(string objectID, string value);

// Convert int value to string nnd return
string gui_IntToString(int intValue);

// Convert a string to a Int value
int gui_StringToInt(string value);

// Add a new element to the keyCode object
void as_hostAddKeyCodeElement(string objectID, string elementLabel, int elementKeyCode);

// Copy the contents of the key array into the keycode array
void gui_copyKeysInto(string objectID);

// Copy the contents of the GUI array into the keys array
void gui_copyGUIIntoKeys(string objectID);

// Set columns for keycode
void as_hostSetNumColumns(string objectID, int numColumns);

// Draw the keycode gui elements
void gui_drawKeycode(int whichObject, bool hasFocus);

// Populate the score table with text
void gui_populateScoreTableGUI();

// Return the text value of an ASCII index
string gui_IntToChar(size_t intValue);

