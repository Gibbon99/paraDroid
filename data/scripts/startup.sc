
#include "data/scripts/commonDefines.h"

int vsyncOption;
int swapMethod;

enum ALLEGRO_DISPLAY_OPTIONS
{
    ALLEGRO_RED_SIZE,
    ALLEGRO_GREEN_SIZE,
    ALLEGRO_BLUE_SIZE,
    ALLEGRO_ALPHA_SIZE,
    ALLEGRO_RED_SHIFT,
    ALLEGRO_GREEN_SHIFT,
    ALLEGRO_BLUE_SHIFT,
    ALLEGRO_ALPHA_SHIFT,
    ALLEGRO_ACC_RED_SIZE,
    ALLEGRO_ACC_GREEN_SIZE,
    ALLEGRO_ACC_BLUE_SIZE,
    ALLEGRO_ACC_ALPHA_SIZE,
    ALLEGRO_STEREO,
    ALLEGRO_AUX_BUFFERS,
    ALLEGRO_COLOR_SIZE,
    ALLEGRO_DEPTH_SIZE,
    ALLEGRO_STENCIL_SIZE,
    ALLEGRO_SAMPLE_BUFFERS,
    ALLEGRO_SAMPLES,
    ALLEGRO_RENDER_METHOD,
    ALLEGRO_FLOAT_COLOR,
    ALLEGRO_FLOAT_DEPTH,
    ALLEGRO_SINGLE_BUFFER,
    ALLEGRO_SWAP_METHOD,
    ALLEGRO_COMPATIBLE_DISPLAY,
    ALLEGRO_UPDATE_DISPLAY_REGION,
    ALLEGRO_VSYNC,
    ALLEGRO_MAX_BITMAP_SIZE,
    ALLEGRO_SUPPORT_NPOT_BITMAP,
    ALLEGRO_CAN_DRAW_INTO_BITMAP,
    ALLEGRO_SUPPORT_SEPARATE_ALPHA,
    ALLEGRO_AUTO_CONVERT_BITMAPS,
    ALLEGRO_SUPPORTED_ORIENTATIONS,
    ALLEGRO_DISPLAY_OPTIONS_COUNT
};

enum importance
{
    ALLEGRO_DONTCARE,
    ALLEGRO_REQUIRE,
    ALLEGRO_SUGGEST
};

/* Possible bit combinations for the flags parameter of al_create_display. */
enum ALLEGRO_DISPLAY_ENUM
{
    ALLEGRO_WINDOWED                    = 1 << 0,
    ALLEGRO_FULLSCREEN                  = 1 << 1,
    ALLEGRO_OPENGL                      = 1 << 2,
    ALLEGRO_DIRECT3D_INTERNAL           = 1 << 3,
    ALLEGRO_RESIZABLE                   = 1 << 4,
    ALLEGRO_FRAMELESS                   = 1 << 5,
    ALLEGRO_NOFRAME                     = ALLEGRO_FRAMELESS, /* older synonym */
    ALLEGRO_GENERATE_EXPOSE_EVENTS      = 1 << 6,
    ALLEGRO_OPENGL_3_0                  = 1 << 7,
    ALLEGRO_OPENGL_FORWARD_COMPATIBLE   = 1 << 8,
    ALLEGRO_FULLSCREEN_WINDOW           = 1 << 9,
    ALLEGRO_MINIMIZED                   = 1 << 10,
    ALLEGRO_PROGRAMMABLE_PIPELINE       = 1 << 11,
    ALLEGRO_GTK_TOPLEVEL_INTERNAL       = 1 << 12
};

//-----------------------------------------------------------------------------
//
// Load the font files
void as_loadFonts()
//-----------------------------------------------------------------------------
{
	io_loadttfFont(FONT_MEDIUM, 14.0f, "Digital.ttf");
	io_loadttfFont(FONT_LARGE, 	32.0f, "Digital.ttf");
	io_loadttfFont(FONT_GUI,	24.0f, "Digital.ttf");
}

//-----------------------------------------------------------------------------
//
// Setup colors and variables for intro scrollbox
void as_setIntroValues()
//-----------------------------------------------------------------------------
{
	introStartX = 50;
	introWidth = 740;		// Check why this is wider than the screen
	introHeight = 410;
	introScrollspeed = 0.8;
	introBackRed = 0.0;
	introBackGreen = 0.0;
	introBackBlue = 1.0;
	introBackAlpha = 1.0;
	introFontRed = 0.4;
	introFontGreen = 0.7;
	introFontBlue = 0.7;
	introFontAlpha = 0.8;
	introScrollDelay = 0.75f;
}

//-----------------------------------------------------------------------------
//
// Add all the script commands to console commands
void as_initGameVars()
//-----------------------------------------------------------------------------
{
	int		returnBool;
	int		displayFlags;

	currentMode = MODE_CONSOLE;

	io_loadConfigFile("config.ini");
	//
	// Screen dimensions
	winWidth = io_readConfigValuesInt("winWidth");
	winHeight = io_readConfigValuesInt("winHeight");
	baseGameSpeed = io_readConfigValuesFloat("baseGameSpeed");
	//
	// Use sound within program or not
	as_useSound = io_readConfigValuesInt("useSoundToggle") == 1 ? true : false;

	if (true == as_useSound)
	{
		volumeLevelStr = io_readConfigValuesString("volumeLevel");
		playSounds = io_readConfigValuesInt("playSounds") == 1 ? true : false;
		playSoundsTemp = playSounds;	// Used in GUI
	}
	else
		playSounds = false;

	TICKS_PER_SECOND = io_readConfigValuesInt("ticksPerSecond");		// How many FPS to process things at
	MAX_FRAMESKIP = io_readConfigValuesInt("maxFrameSkip");			// Minimum FPS to run at
	SKIP_TICKS = (1000.0f / TICKS_PER_SECOND) * 0.001f;

	fullScreenValue = io_readConfigValuesInt("fullScreen");

	if(1 == fullScreenValue)
		displayFlags = ALLEGRO_FULLSCREEN_WINDOW;
	else
		displayFlags = ALLEGRO_WINDOWED;

	displayFlags = (displayFlags | ALLEGRO_OPENGL);

	al_set_new_display_flags(displayFlags);

	al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 0, ALLEGRO_SUGGEST);

	vsyncOption = io_readConfigValuesInt("vsyncOption");
	swapMethod = io_readConfigValuesInt("swapMethod");

	al_set_new_display_option(ALLEGRO_VSYNC, vsyncOption, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SWAP_METHOD, swapMethod, ALLEGRO_SUGGEST);	// Prefer flipping method
	al_set_new_display_option(ALLEGRO_AUTO_CONVERT_BITMAPS, io_readConfigValuesInt("autoConvertBitmaps"), ALLEGRO_SUGGEST);
	al_set_new_display_refresh_rate(io_readConfigValuesInt("refreshRate"));	// 60 FPS
	// How much delay between each element for moving progress bar
	progressBarLoadTimes = 0.02f;
	//
	// Should al_hold_bitmap_drawing be used
	useHoldBitmap = io_readConfigValuesInt("useHoldBitmap") == 1 ? true : false;
	//
	// Use a backing bitmap ( FBO )
	useBackingBitmap = io_readConfigValuesInt("useBackingBitmap") == 1 ? true : false;
	//
	// Use collision detection
	useCollisionDetection = io_readConfigValuesInt("useCollisionDetection") == 1 ? true : false;
	//
	// Slowdown factor
	gravity = 60.0f;
	//
	// Door animation time
	doorFrameDelay = 30.0f;
	//
	// Debug for seeing all droids from LOS
	allDroidsVisible = io_readConfigValuesInt("allDroidsVisible") == 1 ? true : false;
	//
	// Type of particle, if any, to render
	particleRenderType = io_readConfigValuesInt("particleType");
	//
	// Get language setting
	currentLanguage = io_readConfigValuesInt("language");
	//
	// Clamp playervelocity speed
	playerMaxVelocity = 30.0f;
	//
	// Clamp velocity to set value
	doClampPlayerVelocity = true;
	//
	// Get tile color
	currentTileColorStr = io_readConfigValuesString("tileColor");
	currentTileTypeStr = io_readConfigValuesString("tileType");
	//
	// How much distance to check for LOS
	g_LineOfSightStep = 1.0f;
	//
	// Size of circle to see if we have reached a destination
	wayPointDestinationSize = 7.5f;
	//
	// How fast to move bullets
	bulletTravelSpeed = 10.0f + baseGameSpeed;
	//
	// How fast to animate an explosion
	explodeFrameDelay = 20.0f;
	//
	// Higher is faster base animation -affected by current health
	baseDroidAnimateValue = 35.0f;
	//
	// How far off the line segment before moving back towards it
	maxDistanceFromPath = 1.0f;
	//
	// How fast to move back to line segment if off center line
	moveBackToLineSegment = 4.0f;
	//
	// Size of starfield
	numStars = 100;

	//
	// Position for text in hud
	switch(winWidth)
		{
		case 640:
			statusTextX = 20; // Percentage along width of HUD to start text
			scoreTextX = 70; // Percentage along width of HUD to start text
			break;

		case 800:
			statusTextX = 32; // Percentage along width of HUD to start text
			scoreTextX = 83; // Percentage along width of HUD to start text
			break;
		}

	statusTextY = 0;    // Calculated from percentage of HUD height
	//
	// Speed at which score updates
	scoreDelay = 12.0f;
	scoreDelayValue = 2.0f;
	yellowAlertLevel = 450;
	redAlertLevel = 900;
	//
	// Number of sound samples
	as_numSamples = 8;
	//
	// How many already playing samples can play at the same time
	as_numMultiSamples = 8;
	//
	// Position of the droid sprite in transfer screen
	transferDroidPosX = 5;
	transferDroidPosY = 165;
	transferTextPosX = 185;
	transferTextPosY = 150;
	transferTextHeight = 30;
	transferTitlePosX = 70;
	transferTitlePosY = 110;

	transferPlayCountDown = 20.0f;
	transferPlayCountLeftDefault = 10;

	//
	// How long to flash disrupter weapon for
	bulletDisruptSpeed = 20.0f;
	//
	// How much to heal when over a healing tile
	healingRate = 5.0f;
	//
	// How fast an enemy droid heals when on healing tile
	healingTimer = 2.0f;
	//
	// How much damage when droids colliding
	collisionDamageInflicted = 2;
	//
	// How much damage from hitting an exploding droid
	collisionExplosionDamage = 3;
	//
	// Variables for physics engine
	shipDamping = 		0.2f;
	collisionSlop = 	0.9f;
	wallFriction = 		0.8f;
	wallRadius = 		2.0f;

	playerMass = 		0.6f;
	playerRadius = 		12.0f;
	playerFriction = 	0.5f;
	playerElastic = 	0.8f;
	bulletMass = 		0.8f;

	playerTransferTimeSet = 60.0f;
	playerTransferTimeAmount = 1.0f;
	playerHealthWarningFactor = 0.3f;
	playerTransferWarning = playerTransferTimeSet * 0.3f;	// When to start flashing for timeout warning
	playerFlashAmount = 5.0f;

	levelBonus = 		500;
	//
	// How many AStar paths / threads can be allocated
	numAStarPaths =		6;

	//
	// Profile the particle system or not
	profileParticles = true;

	//
	// Counter for how long droid remembers being shot
	droidBeenShotValue = 3.0f;
	witnessShootValue = 1.5f;

	//
	// Amounts to influence chance to shoot
	ai_beenShot = 0.7f;
	ai_healthAmount = 0.3f;
	ai_playerVisible = 0.2f;
	ai_witnessShoot = 0.4f;
	ai_greenFactor = 0.1f;
	ai_yellowFactor = 0.2f;
	ai_redFactor = 0.4f;

	//
	// How long to show end game static screen for
	staticScreenShowTime = 5.0f;
	staticImageCounterTime = 0.1f;
	endScreenCounterTime = 4.0f;

	//
	// Testing
	position.x = 100.0f;
	position.y = 250.0f;
}
