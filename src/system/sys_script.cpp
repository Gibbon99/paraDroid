/*
This file is part of paraDroid.

    paraDroid is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    paraDroid is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with paraDroid.  If not, see <http://www.gnu.org/licenses/>.

Copyright 2017 David Berry
*/

#include "../../hdr/sys_globals.h"

#include "../../hdr/script/scriptbuilder.h"

// Print messages from script compiler to console
void scr_Output ( const asSMessageInfo *msg, void *param );

ALLEGRO_CONFIG *config;
cpVect position;

//--------------------------------------------------------
//
// Test object output
void as_testObject()
//--------------------------------------------------------
{
	con_print ( true, true, "INFO: Object [ %3.2f %3.2f ]", position.x, position.y );
}

//--------------------------------------------------------
//
// ScriptEngine routines
//
// Creates and manages the AngelScript engine
//
//--------------------------------------------------------
int numScriptFunctions = 0; // Number of functions declared in script
bool scriptEngineStarted = false;
static char *fileLocation = NULL; // Pointer to memory to hold the scriptFile

//-----------------------------------------------------------------------------
//
// AngelScript engine
//
//-----------------------------------------------------------------------------
asIScriptEngine *scriptEngine;
asIScriptContext *context = NULL;
CScriptBuilder builder;


//-----------------------------------------------------------------------------
//
// Structure to hold all the script filenames and name
//-----------------------------------------------------------------------------

typedef struct
{
	string scriptFileName;
	string scriptName;
} _scriptInfo;

_scriptInfo scriptInfo[] =
{
	{"shutdown.sc", 		"[ Shutdown ]"},
	{"particles.sc", 		"[ Particle Values ]"},
	{"conCommands.sc", 		"[ Console commands ]"},
	{"gui_main.sc", 		"[ GUI Main ]"},
	{"transferGame.sc", 	"[ Transfer setup ]"},
	{"transferDroidAI.sc", 	"[ Transfer Droid AI ]"},
	{"startup.sc", 			"[ Startup ]"},
	{"textEnglish.sc", 		"[ English strings ]"},
	{"textItalian.sc", 		"[ Italian strings ]"},
	{"", ""},
};

//-----------------------------------------------------------------------------
//
// Struct to hold host function mapping to script function names
//
//-----------------------------------------------------------------------------

_hostScriptFunctions hostScriptFunctions[] =
{
	{"void sys_printConInt(string &in, uint param)", 				( const void * ) &sys_scriptPrintInt},
	{"void sys_printConStr(string &in, string &in)", 				( const void * ) &sys_scriptPrintStr},
	{"void sys_printString(float posX, float posY, string &in)", 	( const void * ) &sys_printString},

	{"void sys_addScriptCommand(string &in, string &in, string &in, bool setParam)", ( const void * ) &con_addScriptCommand},
	//
	// ---------------- Use Allegro functions from within script
	//
	{"void al_set_new_display_option(int option, int value, int importance)", 	( const void * ) &al_set_new_display_option},
	{"void al_set_new_display_flags(int option)", 								( const void * ) &al_set_new_display_flags},
	{"void al_set_new_display_refresh_rate(int refresh_rate)", 					( const void * ) &al_set_new_display_refresh_rate},
	//
	// ---------------- Config file routines
	//
	{"void io_loadConfigFile			(string &in)", 					( const void * ) &io_loadConfigFile},
	{"void io_closeConfigFile			()", 							( const void * ) &io_closeConfigFile},
	{"void io_saveConfigFile			(string &in)", 					( const void * ) &io_saveConfigFile},
	{"int io_readConfigValuesInt		(string &in)", 					( const void * ) &io_readConfigValuesInt},
	{"string io_readConfigValuesString	(string &in)", 					( const void * ) &io_readConfigValuesString},
	{"float io_readConfigValuesFloat 	(string &in)", 					( const void * ) &io_readConfigValuesFloat},
	{"void io_saveConfigValueString		(string &in, string &in)", 		( const void * ) &io_saveConfigValueString},
	{"void io_saveConfigValueFloat		(string &in, float keyValue)", 	( const void * ) &io_saveConfigValueFloat},
	{"void io_saveConfigValueInt		(string &in, int keyValue)", 	( const void * ) &io_saveConfigValueInt},
	//
	// --------------- Load keybindings ----------------
	//
	{"bool io_doKeyBindings(string &in)", 								( const void * ) &io_doKeyBindings},
	//
	// ------------- Draw from within the script
	//
	{"void gam_drawBitmapFromScript(int index, float posX, float posY, float red, float green, float blue, float alpha)", ( const void * ) &gam_drawBitmapFromScript},
	{"void gam_drawSpriteFromScript(int index, float posX, float posY, float red, float green, float blue, float alpha)", ( const void * ) &gam_drawSpriteFromScript},
	//
	// ---------------- Font routines
	//
	{"void sys_setFont			(int newFont)", 							( const void * ) &sys_setFont},
	{"bool io_loadttfFont		(int fontIndex, int fontSize, string &in)", ( const void * ) &io_loadttfFont},
	{"void sys_setFontColor		(float r, float g, float b, float a)", 		( const void * ) &sys_setFontColor},

	//
	// ---------------- Play sound from script
	//
	{"void sys_playSoundFromScript(int whichSound, float pan, bool loopIt)", ( const void * ) &sys_playSoundFromScript},
	//
	// -------------- Set tile options
	//
	{"void io_setTileSetColor(string &in)", 								( const void * ) &io_setTileSetColor},
	{"void io_setTileSetType(string &in)", 									( const void * ) &io_setTileSetType},
	//
	// ------------- GUI Functions called from script
	//
	{"void as_guiCreateNewScreen		(string &in)", 						( const void * ) &gui_hostCreateNewScreen},
	{"void as_guiCreateObject			(int guiObjectType, string &in)", 	( const void * ) &gui_hostCreateObject},
	{"int as_guiFindIndex				(int guiObjectType, string &in)", 	( const void * ) &gui_findIndex},
	{"void as_gui_changeToGUIScreen		(int newScreen)", 					( const void * ) &gui_changeToGUIScreen},

	{"void as_guiSetObjectColor    (int guiObjectType, string &in, int whichColor, float red, float green, float blue, float alpha)", 		( const void * ) &gui_hostSetObjectColor},
	{"void as_guiSetObjectPosition (int guiObjectType, string &in, int coordType, float startX, float startY, float width, float height)", 	( const void * ) &gui_hostSetObjectPosition},
	
	{"void as_guiSetObjectLabel    (int guiObjectType, string &in, int labelPos, string &in)", 		( const void * ) &gui_hostSetObjectLabel},
	{"void as_guiSetObjectFunctions(int guiObjectType, string &in, string &in, string &in)", 		( const void * ) &gui_hostSetObjectFunctions},
	{"void as_guiAddObjectToScreen (int guiObjectType, string &in, string &in)", 					( const void * ) &gui_hostAddObjectToScreen},
	{"void as_guiSetCheckboxGroup  (string &in, int whichGroup)", 									( const void * ) &gui_hostSetCheckboxGroup},
	{"void as_guiSetCheckboxTick   (string &in, int whichGroup, bool ticked)", 						( const void * ) &gui_hostSetCheckboxTick},
	{"void as_guiSetupDBText()", 																	( const void * ) &gam_setupDBText},
	{"void as_guiSetImageFilename(string &in, string &in)", 										( const void * ) &gui_hostSetImageFilename},
	{"string gui_getString(string &in)", 															( const void * ) &gui_getString},
	{"void gui_addKeyAndText(string &in, string &in)", 												( const void * ) &gui_addKeyAndText},
	{"void as_guiSetObjectFocus(string &in)", 														( const void * ) &gui_setObjectFocus},
	{"void as_guiAddSliderElement(string &in, int elementType, string &in, string &in)", 			( const void * ) &as_hostAddSliderElement},
	{"string gui_IntToString(int intValue)", 														( const void * ) &gui_IntToString},
	{"int gui_StringToInt(string &in)", 															( const void * ) &gui_StringToInt},
	{"string gui_IntToChar(int intValue)", 															( const void * ) &gui_IntToChar},
	{"string as_guiGetSliderValue(string &in)", 													( const void * ) &as_hostGetSliderValue},
	{"void as_guiSetSliderValue(string &in, string &in)", 											( const void * ) &as_hostSetSliderValue},
	{"void io_loadNewTileSet()", 																	( const void * ) &io_loadNewTileSet},
	{"void as_guiAddKeyCodeElement(string &in, string &in, int elementKeyCode)", 					( const void * ) &as_hostAddKeyCodeElement},
	{"void as_guiCopyKeysInto(string &in)", 														( const void * ) &gui_copyKeysInto},
	{"void as_guiCopyGUIIntoKeys(string &in)", 														( const void * ) &gui_copyGUIIntoKeys},
	{"bool io_saveKeyBindings (string &in)", 														( const void * ) &io_saveKeyBindings},
	{"bool gam_loadDroidModel (int whichDroid)", 													( const void * ) &gam_loadDroidModel},
	//
	// ------Transfer routines for scripts
	//
	{"void as_transSetDefaultValues(int cellIndex)", 												( const void * ) &host_transSetDefaultValues},
	{"void as_transSetCellPos(int cellIndex, int posX, int posY)", 									( const void * ) &host_transSetCellPos},
	{"void as_transSetCellColor(int whichCell, int colType, float red, float green, float blue, float alpha)", ( const void * ) &host_transSetCellColor},
	{"void as_tranSetCircuitType(int whichCell, int whichSide, int typeSet)", 						( const void * ) &host_tranSetCircuitType},
	{"void as_tranSetEffectType(int whichCell, int whichSide, int effect)", 						( const void * ) &host_transSetEffectType},
	{"int as_getCappedRandomNum(int cap)", 															( const void * ) &host_getCappedRandomNum},
	{"bool as_circuitNotPowered(int whichCell, int whichSide)", 									( const void * ) &host_circuitPowered},
	{"int as_getCellColor(int whichCell, int whichSide)", 											( const void * ) &host_getCellColor},
	{"void trn_activateCircuit(int whichCell, int whichSide)", 										( const void * ) &trn_activateCircuit},
	{"void trn_placeToken(int whichCell, int whichSide, int playerOrDroid)", 						( const void * ) &trn_placeToken},
	//
	// ------------ Misc routines called from script
	//
	{"void as_printVariables()", 										( const void * ) &sys_printVariables},
	{"void as_changeGameMode(int newMode, bool useFade)", 				( const void * ) &sys_changeMode},
	{"void gam_changeToLevelFromScript(int newLevel, bool firstTime)", 	( const void * ) &gam_changeToLevelFromScript},
	//
	// ------------- Set particle colors from script
	//
	{"void as_setParticleColor(int whichParticle, int red, int green, int blue, int alpha)",( const void * ) &par_hostSetParticleColor},
	//
	// -------------- Set score initials from script
	//
	{"void gam_addNewInitials()",				( const void * ) &gam_addNewInitials},
	{"void gam_resetScore()",					( const void * ) &gam_resetScore},
	{"void gui_populateScoreTableGUI()",		( const void * ) &gui_populateScoreTableGUI},
	//
	// ---------------------- End of the list - nothing past this
	//
	{"", (const void *)""},
};

_hostScriptFunctions hostVariables[] =
{
	{"int winWidth", &winWidth},
	{"int winHeight", &winHeight},
	{"int currentMode", &currentMode},
	{"bool gameIsRunning", &gameIsRunning},
	{"int TICKS_PER_SECOND", &TICKS_PER_SECOND},
	{"int MAX_FRAMESKIP", &MAX_FRAMESKIP},
	{"float SKIP_TICKS", &SKIP_TICKS},
	{"float baseGameSpeed", &baseGameSpeed},
	{"bool useHoldBitmap", &useHoldBitmap},
	{"bool useBackingBitmap", &useBackingBitmap},
	{"bool useCollisionDetection", &useCollisionDetection},
	{"bool debugShowStats", &debugShowStats},
	{"int fullScreenValue", &fullScreenValue},
	{"int inputMethod", &inputMethod},
	{"int numJoysticks", &numJoysticks},
	{"bool confirmExit", &confirmExit},
	{"float fadeSpeed", &fadeSpeed},
	{"float gravity", &gravity},
	{"float doorFrameDelay", &doorFrameDelay},
	{"float playerMaxVelocity", &playerMaxVelocity},
	{"float playerMaxSpeed", &playerMaxSpeed},
	{"float playerAcceleration", &playerAcceleration},
	{"float playerTransferTimeSet", &playerTransferTimeSet},
	{"float playerTransferTimeAmount", &playerTransferTimeAmount},
	{"float playerHealthWarningFactor", &playerHealthWarningFactor},
	{"float playerTransferWarning",	&playerTransferWarning},
	{"float playerFlashAmount", &playerFlashAmount},
	{"bool doClampPlayerVelocity", &doClampPlayerVelocity},
	{"int currentLevel", &currentLevel},
	{"float g_LineOfSightStep", &g_LineOfSightStep},
	{"bool allDroidsVisible", &allDroidsVisible},
	{"float progressBarLoadTimes", &progressBarLoadTimes},
	{"float wayPointDestinationSize", &wayPointDestinationSize},
	{"float bulletTravelSpeed", &bulletTravelSpeed},
	{"float explodeFrameDelay", &explodeFrameDelay},
	{"float baseDroidAnimateValue", &baseDroidAnimateValue},
	{"float maxDistanceFromPath", &maxDistanceFromPath},
	{"float moveBackToLineSegment", &moveBackToLineSegment},
	{"int currentTermMenu", &currentTermMenu},
	{"int numStars", &numStars},
	{"int currentGUIScreen", &currentGUIScreen},
	{"int currentObjectSelected", &currentObjectSelected},
	{"int checkBoxTickSize", &checkBoxTickSize},
	{"int numSoundDevices", &numSoundDevices},
	{"bool playSounds", &playSounds},
	{"float buttonCurveX", &buttonCurveX},
	{"float buttonCurveY", &buttonCurveY},
	{"float keyCodeGap", &keyCodeGap},
	{"float sliderSelectorRadius", &sliderSelectorRadius},
	{"float sliderCurveX", &sliderCurveX},
	{"float sliderCurveY", &sliderCurveY},
	{"float introStartX", &introStartX},
	{"float introStartY", &introStartY},
	{"float introWidth", &introWidth},
	{"float introHeight", &introHeight},
	{"float introScrollspeed", &introScrollspeed},
	{"float introBackRed", &introBackRed},
	{"float introBackGreen", &introBackGreen},
	{"float introBackBlue", &introBackBlue},
	{"float introBackAlpha", &introBackAlpha},
	{"float introFontRed", &introFontRed},
	{"float introFontGreen", &introFontGreen},
	{"float introFontBlue", &introFontBlue},
	{"float introFontAlpha", &introFontAlpha},
	{"float introScrollDelay", &introScrollDelay},

	{"float dbStartX", &dbStartX},
	{"float dbStartY", &dbStartY},
	{"float dbWidth", &dbWidth},
	{"float dbHeight", &dbHeight},
	{"float dbScrollspeed", &dbScrollspeed},
	{"float dbBackRed", &dbBackRed},
	{"float dbBackGreen", &dbBackGreen},
	{"float dbBackBlue", &dbBackBlue},
	{"float dbBackAlpha", &dbBackAlpha},
	{"float dbFontRed", &dbFontRed},
	{"float dbFontGreen", &dbFontGreen},
	{"float dbFontBlue", &dbFontBlue},
	{"float dbFontAlpha", &dbFontAlpha},
	{"float dbScrollDelay", &dbScrollDelay},
	{"float scrollPauseDelayValue", &scrollPauseDelayValue},

	{"int currentTileType", &currentTileType},
	{"int currentTileColor", &currentTileColor},
	{"string currentTileTypeStr", &currentTileTypeStr},
	{"string currentTileColorStr", &currentTileColorStr},
	{"string newInitials", &newInitials},

	{"float dbImagePositionX", &dbImagePositionX},
	{"float dbImagePositionY", &dbImagePositionY},
	{"float statusTextX", &statusTextX},
	{"float statusTextY", &statusTextY},
	{"float scoreTextX", &scoreTextX},

	{"int squareWidth", &squareWidth},
	{"int squareHeight", &squareHeight},
	{"float indicatorAnimSpeed", &indicatorAnimSpeed},

	{"float scoreDelay", &scoreDelay},
	{"float scoreDelayValue", &scoreDelayValue},

	{"int numConsoleCommands", &numConsoleCommands},

	{"int as_numSamples", &as_numSamples},
	{"int as_numMultiSamples", &as_numMultiSamples},
	{"bool as_useSound", &as_useSound},
//	{"float volumeLevel", &volumeLevel},
	{"string volumeLevelStr", &volumeLevelStr},
	{"int transferDroidPosX", &transferDroidPosX},
	{"int transferDroidPosY", &transferDroidPosY},
	{"int transferTextPosX", &transferTextPosX},
	{"int transferTextPosY", &transferTextPosY},
	{"int transferTextHeight", &transferTextHeight},
	{"int transferTitlePosX", &transferTitlePosX},
	{"int transferTitlePosY", &transferTitlePosY},
	{"float	transferPlayCountDown",	&transferPlayCountDown},
	{"int transferPlayCountLeftDefault", &transferPlayCountLeftDefault},

	{"int droidOnSide", &droidOnSide},
	{"int droidBlockPos", &droidBlockPos},
	{"int numDroidTokens", &numDroidTokens},
	{"int droidToTransferInto", &droidTypeToTransferInto},
	{"float circuitTimeAlive", &circuitTimeAlive},
	{"int droidTypeDBIndex",	&droidTypeDBIndex},
	{"int playerDroidTypeDBIndex",	&playerDroidTypeDBIndex},

	{"int yellowAlertLevel", &yellowAlertLevel},
	{"int redAlertLevel", &redAlertLevel},

	{"float bulletDisruptSpeed", &bulletDisruptSpeed},

	{"float healingRate", &healingRate},
	{"float healingTimer", &healingTimer},

	{"int collisionDamageInflicted", &collisionDamageInflicted},
	{"int collisionExplosionDamage", &collisionExplosionDamage},
	//
	// Variables for physics
	//
	{"float shipDamping", &shipDamping},
	{"float collisionSlop", &collisionSlop},
	{"float wallFriction", &wallFriction},
	{"float wallRadius", &wallRadius},

	{"float playerMass", &playerMass},
	{"float playerRadius", &playerRadius},
	{"float playerFriction", &playerFriction},
	{"float playerElastic", &playerElastic},
	{"float bulletMass", &bulletMass},
	{"int levelBonus", &levelBonus},
	{"int numAStarPaths", &numAStarPaths},

	{"bool profileParticles", &profileParticles},
	{"int particleRenderType", &particleRenderType},
	{"int particleVelocityExplosion", &particleVelocityExplosion},
	{"int particleVelocityExplosionAdd", &particleVelocityExplosionAdd},
	{"int particleVelocitySpark", &particleVelocitySpark},
	{"int particleVelocitySparkAdd", &particleVelocitySparkAdd},
	{"int particleNumberExplosion", &particleNumberExplosion},
	{"int particleNumberExplosionAdd", &particleNumberExplosionAdd},
	{"int particleNumberTrail", &particleNumberTrail},
	{"int particleNumberTrailAdd", &particleNumberTrailAdd},
	{"int particleTrailAlphaReset", &particleTrailAlphaReset},
	{"int particleTrailAlphaResetAdd", &particleTrailAlphaResetAdd},
	{"int particleNumberSpark", &particleNumberSpark},
	{"int particleNumberSparkAdd", &particleNumberSparkAdd},
	{"float particleExplosionReduce", &particleExplosionReduce},
	{"float particleSparkReduce", &particleSparkReduce},
	{"float particleTrailReduce", &particleTrailReduce},
	{"int particleTrailLimit", &particleTrailLimit},
	{"float particleMass", &particleMass},
	{"float particleFriction", &particleFriction},
	{"float particleElastic", &particleElastic},
	{"float particleSize", &particleSize},
	{"int particleBlendSrc", &particleBlendSrc},
	{"int particleBlendDst", &particleBlendDst},
	{"int currentLanguage", &currentLanguage},

	{"float droidBeenShotValue", &droidBeenShotValue},
	{"float witnessShootValue", &witnessShootValue},
	{"float witnessTransferValue", &witnessTransferValue},
	{"float ai_beenShot", &ai_beenShot},
	{"float ai_healthAmount", &ai_healthAmount},
	{"float ai_playerVisible", &ai_playerVisible},
	{"float ai_witnessShoot", &ai_witnessShoot},
	{"float ai_witnessTransfer", &ai_witnessTransfer},
	{"float ai_greenFactor", &ai_greenFactor},
	{"float ai_yellowFactor", &ai_yellowFactor},
	{"float ai_redFactor", &ai_redFactor},

	{"float staticScreenShowTime", &staticScreenShowTime},
	{"float staticImageCounterTime", &staticImageCounterTime},
	{"float endScreenCounterTime", &endScreenCounterTime},

	{"float as_soundPlayDelay", &as_soundPlayDelay},

	{"float repeatKeysDelay", &repeatKeysDelay},
	
	{"", NULL},
};

//-----------------------------------------------------------------------------
//
// Structure to hold all the functions in the script
//-----------------------------------------------------------------------------

typedef struct
{
	asIScriptFunction *funcID;
	bool fromScript; // Is this created from a console script
	string functionName;
	string scriptName;
	bool param1; // Does this function need something passed to it
} _scriptFunctionName;

//
// This is how we call a script from the Host program
//
_scriptFunctionName scriptFunctionName[] =
{
	// 				Name of function in script						Name we call from host
	{0, false, "void as_initGameVars()", 			"as_initGameVars", 				false},
	{0, false, "void as_addAllScriptCommands()", 	"as_addAllScriptCommands", 		false},
	{0, false, "void as_shutdown()", 				"as_shutdown", 					false},
	{0, false, "void as_loadFonts()", 				"as_loadFonts", 				false},
	{0, false, "void as_guiSetupGUI()", 			"scr_guiSetupGUI", 				false},
	{0, false, "void as_setIntroValues()", 			"as_setIntroValues", 			false},
	{0, false, "void as_setdbValues()", 			"as_setdbValues", 				false},
	{0, false, "void setupDefaultCellValues()", 	"as_setupTransferCellValues", 	false},
	{0, false, "void as_processTransferAI()", 		"as_processTransferAI", 		false},
	{0, false, "void as_initParticleValues()", 		"as_initParticleValues", 		false},
	{0, false, "void as_setLanguageStrings()", 		"as_setLanguageStrings", 		false},

	{0, false, "void as_guiHandleButtonPress(string &in)", "scr_guiHandleButtonPress", true},
	{0, false, "void as_guiHandleMouseOver(string &in)", "scr_guiHandleMouseOver", 	false},

	{0, true, "", "", false},
};

//
// This is used to hold the function ID and is used when creating functions
// from within a running script. A copy of scriptFunctionName is used as
// the source data.
//
vector<_scriptFunctionName> scriptFunctions;

int numConsoleCommands;

//-----------------------------------------------------------------------------
//
// Error codes for AngelScript
const char *sys_getScriptError ( int errNo )
//-----------------------------------------------------------------------------
{
	switch ( errNo )
		{
			case 0:
				return ( "asSUCCESS" );
				break;

			case -1:
				return ( "asERROR" );
				break;

			case -2:
				return ( "asCONTEXT_ACTIVE" );
				break;

			case -3:
				return ( "asCONTEXT_NOT_FINISHED" );
				break;

			case -4:
				return ( "asCONTEXT_NOT_PREPARED" );
				break;

			case -5:
				return ( "asINVALID_ARG" );
				break;

			case -6:
				return ( "asNO_FUNCTION" );
				break;

			case -7:
				return ( "asNOT_SUPPORTED" );
				break;

			case -8:
				return ( "asINVALID_NAME" );
				break;

			case -9:
				return ( "asNAME_TAKEN" );
				break;

			case -10:
				return ( "asINVALID_DECLARATION" );
				break;

			case -11:
				return ( "asINVALID_OBJECT" );
				break;

			case -12:
				return ( "asINVALID_TYPE" );
				break;

			case -13:
				return ( "asALREADY_REGISTERED" );
				break;

			case -14:
				return ( "asMULTIPLE_FUNCTIONS" );
				break;

			case -15:
				return ( "asNO_MODULE" );
				break;

			case -16:
				return ( "asNO_GLOBAL_VAR" );
				break;

			case -17:
				return ( "asINVALID_CONFIGURATION" );
				break;

			case -18:
				return ( "asINVALID_INTERFACE" );
				break;

			case -19:
				return ( "asCANT_BIND_ALL_FUNCTIONS" );
				break;

			case -20:
				return ( "asLOWER_ARRAY_DIMENSION_NOT_REGISTERED" );
				break;

			case -21:
				return ( "asAPP_CANT_INTERFACE_DEFAULT_ARRAY" );
				break;

			default:
				return ( "Unknown error type." );
		}
}

//--------------------------------------------------------
//
// Clean up memory for script engine
void sys_cleanupScriptEngine ( bool restart )
//--------------------------------------------------------
{
	if ( true == scriptEngineStarted )
		{
			// Clean up
			//        context->Release();
			//        scriptEngine->Release();
		}

	if ( false == restart )
		{
			if ( scriptFunctionName )
				{
					//			al_free(scriptFunctions);
					//			scriptFunctions = NULL;
				}
		}
}

//-----------------------------------------------------------------------------
//
// Add global variables to be used by the scriptEngine

bool sys_registerVariables()
//-----------------------------------------------------------------------------
{
	int result = 0;
	int count = 0;

	if ( NULL == scriptEngine )
		{
			con_print ( true, true, "Script: Failed to register variables. ScriptEngine is not ready." );
			return false;
		}

	while ( hostVariables[count].scriptFunctionName.size() > 1 )
		{
			result = scriptEngine->RegisterGlobalProperty ( hostVariables[count].scriptFunctionName.c_str(), (void *)hostVariables[count].hostFunctionPtr );

			if ( result < 0 )
				{
					con_print ( true, true, "Script: Error: Couldn't register variable - [ %s ]", hostVariables[count].scriptFunctionName.c_str() );
					return false;
				}

			sys_setFontColor ( 0.0f, 1.0f, 0.0f, 1.0f );
			con_print ( true, true, "Script: Registered variable - [ %s ]", hostVariables[count].scriptFunctionName.c_str() );
			count++;
		}

	return true;
}

//-----------------------------------------------------------------------------
//
// Test setup passing a struct and values

bool sys_registerObject()
//-----------------------------------------------------------------------------
{
	int returnCode;

	position.x = 50.0f;
	position.y = 60.0f;

	returnCode = scriptEngine->RegisterObjectType ( "cpVect", sizeof ( cpVect ), asOBJ_VALUE | asOBJ_POD ); //asOBJ_REF | asOBJ_NOCOUNT );
//	returnCode = scriptEngine->RegisterObjectType("cpVect", sizeof (cpVect), asOBJ_REF | asOBJ_NOCOUNT );

	if ( returnCode < 0 )
		{
			con_print ( true, true, "Error RegisterObjectType [ %s ]", sys_getScriptError ( returnCode ) );
			return false;
		}

	returnCode = scriptEngine->RegisterObjectProperty ( "cpVect", "float x", asOFFSET ( cpVect, x ) );

	if ( returnCode < 0 )
		{
			con_print ( true, true, "Error RegisterObjectProperty [ %s ]", sys_getScriptError ( returnCode ) );
			return false;
		}

	returnCode = scriptEngine->RegisterObjectProperty ( "cpVect", "float y", asOFFSET ( cpVect, y ) );

	if ( returnCode < 0 )
		{
			con_print ( true, true, "Error RegisterObjectProperty 1 [ %s ]", sys_getScriptError ( returnCode ) );
			return false;
		}

	returnCode = scriptEngine->RegisterGlobalProperty ( "cpVect position", ( void * ) &position );

	if ( returnCode < 0 )
		{
			con_print ( true, true, "Error RegisterGlobalProperty [ %s ]", sys_getScriptError ( returnCode ) );
			return false;
		}

	return true;
}

//-----------------------------------------------------------------------------
//
// Register all the functions to make available to the script

bool sys_registerFunctions()
//-----------------------------------------------------------------------------
{
	int 		r = 0;
	int 		count = 0;
	asDWORD		callType;

	if ( NULL == scriptEngine )
		{
			con_print ( true, true, "Failed to register functions. ScriptEngine is not ready." );
			return false;
		}

	if ( !strstr ( asGetLibraryOptions(), "AS_MAX_PORTABILTY" ) )
		{
			callType = asCALL_CDECL;
			con_print(true, true, "Script: Call type [ %s ]", "asCALL_CDECL");
		}
	else
		{
			callType = asCALL_STDCALL;
			con_print(true, true, "Script: Call type [ %s ]", "asCALL_STDCALL");
		}

	while ( hostScriptFunctions[count].scriptFunctionName.size() > 1 )
		{
			r = scriptEngine->RegisterGlobalFunction ( hostScriptFunctions[count].scriptFunctionName.c_str(), asFUNCTION ( hostScriptFunctions[count].hostFunctionPtr ), callType );

			if ( r < 0 )
				{
					con_print ( true, true, "Failed to registerGlobalFunction [ %s ]", hostScriptFunctions[count].scriptFunctionName.c_str() );
					con_print ( true, true, "Error [ %s ]", sys_getScriptError ( r ) );
					return false;
				}

			sys_setFontColor ( 0.0f, 0.0f, 1.0f, 1.0f );
			con_print ( true, true, "Script: Registered function - [ %s ]", hostScriptFunctions[count].scriptFunctionName.c_str() );
			count++;
		}

	return true;
}

//-------------------------------------------------------------------------------
//
// Load into memory the file to be read
//
bool sys_fileIntoMemory ( char *whichFile )
//-------------------------------------------------------------------------------
{
//	ALLEGRO_FILE 	*fileHandle;
	int64_t 		fileSize;
	char 			fileName[MAX_PATH];
	ALLEGRO_FILE*	fileHandle;

	strcpy ( fileName, whichFile );

	fileHandle = al_fopen ( fileName, "r" );

	if ( NULL == fileHandle )
		{
			con_printUpdate ( true, "Open error [ %s ]", fileName );
			return false;
		}

	fileSize = al_fsize ( fileHandle );
	con_print ( true, true, "Size of script [ %i ] - [ %s ]", fileSize, fileName );

	//
	// if memory already allocated - free and remalloc it
	if ( fileLocation )
		{
			free ( fileLocation );
			fileLocation = NULL;
		}


	fileLocation = ( char * ) malloc ( sizeof ( char ) * ( int ) fileSize );

	if ( NULL == fileLocation )
		{
			sys_errorFatal ( ( char * ) __FILE__, __LINE__, ( char * ) "Memory allocation failed for [ %s ]", fileName );
			return false;
		}

	if ( al_fread ( fileHandle, ( void * ) fileLocation, ( size_t ) fileSize ) <= 0 )
		sys_errorFatal ( "sys_fileIntoMemory", __LINE__, ( char * ) "Read error [ %s ]", fileName );

	al_fclose ( fileHandle );

	fileLocation[fileSize - 1] = '\0';
	return true;
}

//-----------------------------------------------------------------------------
//
// Load the scripts into memory
bool sys_loadAndCompileScripts()
//-----------------------------------------------------------------------------
{
	int fileCounter = 0;
	int retCode = 0;

	retCode = builder.StartNewModule ( scriptEngine, 0 );

	if ( retCode < 0 )
		{
			con_print ( true, true, "Error: Failed to start new module" );
			return false;
		}

	while ( scriptInfo[fileCounter].scriptFileName.size() > 1 )
		{
			//
			// Load the script file
			//
			if ( sys_fileIntoMemory ( ( char* ) scriptInfo[fileCounter].scriptFileName.c_str() ) == false )
				{
					con_print ( true, true, "Error: Failed to load scriptFile [ %s ]", scriptInfo[fileCounter].scriptFileName.c_str() );
					return false;
				}

			retCode = builder.AddSectionFromMemory ( scriptInfo[fileCounter].scriptName.c_str(), fileLocation, ( int ) strlen ( fileLocation ), 0 );

			if ( retCode < 0 )
				{
					con_print ( true, true, "Error: Failed to add script file [ %s ]", scriptInfo[fileCounter].scriptFileName.c_str() );
					return false;
				}

			fileCounter++;
		}

	//
	// Build the script from the loaded setions using ScriptBuilder
	//
	if ( builder.BuildModule() < 0 )
		{
			con_print ( true, true, "Error: Failed to build the module." );
			return false;
		}

	//
	// Free memory used to hold scripts while they are loaded into engine
	// if memory already allocated - free and remalloc it
	//
	if ( fileLocation )
		{
			free ( fileLocation );
			fileLocation = NULL;
		}

	return true;
}

//-----------------------------------------------------------------------------
//
// Print messages from script compiler to console

void scr_Output ( const asSMessageInfo *msg, void *param )
//-----------------------------------------------------------------------------
{
	const char *type = "ERR ";

	if ( msg->type == asMSGTYPE_WARNING )
		type = "WARN";

	else if ( msg->type == asMSGTYPE_INFORMATION )
		type = "INFO";

	con_print ( true, true, "%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message );

	if ( false == scriptEngineStarted )
		printf ( "%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message );
}

//--------------------------------------------------------
//
// Start the script engine
bool sys_initScriptEngine()
//--------------------------------------------------------
{
	_scriptFunctionName tempFunctionName;

	scriptEngine = asCreateScriptEngine ( ANGELSCRIPT_VERSION );

	if ( 0 == scriptEngine )
		{
			scriptEngineStarted = false;
			sys_errorFatal ( __FILE__, __LINE__, "Script: Error: Failed to create script engine- [ %s ]", sys_getScriptError ( 0 ) );
		}

	io_logToFile ( "Script: Scriptengine started." );

	RegisterStdString ( scriptEngine );

	RegisterScriptArray ( scriptEngine, true );

	// The script compiler will write any compiler messages to the callback.
	scriptEngine->SetMessageCallback ( asFUNCTION ( scr_Output ), 0, asCALL_CDECL );

	// What version are we running
	io_logToFile ( "Script: ScriptEngine version - [ %s ]", asGetLibraryVersion() );

	// What options are compiled
	io_logToFile ( "Script: Options - [ %s ]", asGetLibraryOptions() );

	if ( false == sys_registerObject() )
		{
			RET_FALSE ( "Script: Error: Couldn't register objects.", "" );
		}

	// Add variables available to the script
	if ( false == sys_registerVariables() )
		{
			RET_FALSE ( "Script: Error: Couldn't register variables.", "" );
		}

	// Add all the functions that the scripts can access
	if ( false == sys_registerFunctions() )
		{
			RET_FALSE ( "Script: Error: Couldn't register functions.", "" );
		}


	sys_registerObject();

	if ( false == sys_loadAndCompileScripts() )
		RET_FALSE ( "Script: Error: Couldn't load or compile scripts.", "" );

	int count = 0;
	numScriptFunctions = 0;

	//
	// Get the number of functions we are using
	//
	while ( scriptFunctionName[count].functionName.size() > 1 )
		{
			numScriptFunctions++;
			count++;
		}

	//
	// Get memory to hold script details
	//
	scriptFunctions.reserve ( numScriptFunctions );
	con_print ( true, true, "Number of functions in script [ %i ]", numScriptFunctions );

	//
	// Get function ID's for each function we will call in the script
	//
	for ( int i = 0; i != numScriptFunctions; i++ )
		{
			tempFunctionName.funcID = scriptEngine->GetModule ( 0 )->GetFunctionByDecl ( scriptFunctionName[i].functionName.c_str() );

			if ( tempFunctionName.funcID <= 0 )
				{
					con_print ( true, true, "Failed to get function ID for [ %s ]", scriptFunctionName[i].functionName.c_str());
					return false;
				}
			else
				{
					con_print ( true, false, "Func ID for [ %s ] - [ %i ]", scriptFunctionName[i].functionName.c_str(), tempFunctionName.funcID );
				}

			//
			// Setup malloc'd memory with the funcID and name to execute
			//
			tempFunctionName.param1 = scriptFunctionName[i].param1;
			tempFunctionName.functionName = scriptFunctionName[i].functionName;
			tempFunctionName.scriptName = scriptFunctionName[i].scriptName;
			tempFunctionName.fromScript = scriptFunctionName[i].fromScript;

			scriptFunctions.push_back ( tempFunctionName );

			//		con_print (true, true, "scriptFunctions [ %s ] has ID [ %i ]", scriptFunctions[i].functionName.c_str(), scriptFunctions[i].funcID);
		}

	// Do some preparation before execution
	context = scriptEngine->CreateContext();

	scriptEngineStarted = true;
	return true;
}


//-----------------------------------------------------------------------------
//
// Execute a function from the script

bool sys_executeScriptFunction ( string functionName, string funcParam )
//-----------------------------------------------------------------------------
{
	int i = 0;

	//
	// Check for function name passed in
	if ( functionName.size() < 1 )
		{
			con_printUpdate ( true, "Error: Failed to provide function name." );
			return false;
		}

	//
	// Safety check first
	if ( NULL == scriptEngine )
		{
			con_printUpdate ( true, "Couldn't execute [ %s ]. Engine not ready.", functionName.c_str() );
			return false;
		}

	if ( NULL == context )
		{
			con_printUpdate ( true, "Couldn't execute [ %s ]. Script context not ready", functionName.c_str() );
			return false;
		}

	//
	// Find the functionID we need for the functionName
	for ( i = 0; i != numScriptFunctions; i++ )
		{
			//
			// Do a case insensitive compare - FIXME
			//
#if defined(WIN32)
			if ( _stricmp ( scriptFunctions[i].scriptName.c_str(), functionName.c_str() ) == 0 )
#else
			if ( scriptFunctions[i].scriptName == functionName )
#endif
				break;
		}

	if ( i == numScriptFunctions )
		{
			con_print ( true, false, "Couldn't find function name [ %s ]", functionName.c_str() );
			return false;
		}

	//	con_print (true, false, "Found [ %s ] to execute", scriptFunctions[i].functionName.c_str());

	//
	// Prepare the engine context to execute the function
	//
	int j = context->Prepare ( scriptFunctions[i].funcID );

	if ( j < 0 )
		{
			con_print ( true, true, "Failed to prepare the context for [ %s ].", scriptFunctions[i].functionName.c_str() );
			return false;
		}

	//	con_print (true, false, "Context for [ %s ] has been prepared.", scriptFunctions[i].functionName.c_str());
	//
	// Pass the parameter to the function if there is one
	//
	int testInt;

	if ( true == scriptFunctions[i].param1 )
		{
//		io_logToFile("Parameter to pass to script [ %s ]", funcParam.c_str());
			//
			// See if it's a number or a string - check first character of the string only
			if ( isdigit ( funcParam.c_str() [0] ) )
				{
					//
					// Paramater is a number - convert before passing in
					//
					testInt = atoi ( funcParam.c_str() );
					context->SetArgAddress ( 0, &testInt );
				}
			else
				{
					//
					// Send in string value
					//
					context->SetArgAddress ( 0, &funcParam );
				}
		}

	//
	// Now run it
	//
	int ret = context->Execute();

	if ( ret != asEXECUTION_FINISHED )
		{
			//
			// The execution didn't finish as we had planned. Determine why.
			//
			if ( ret == asEXECUTION_ABORTED )
				con_print ( true, true, "Script: Error: The script was aborted before it could finish. Probably it timed out." );

			else if ( ret == asEXECUTION_EXCEPTION )
				{
					con_print ( true, true, "Script: Error: The script ended with an exception." );
					//
					// Write some information about the script exception
					//
					asIScriptFunction *func = context->GetExceptionFunction();
					con_print ( true, true, "Func: [ %s ]", func->GetDeclaration() );
					con_print ( true, true, "Module: [ %s ]", func->GetModuleName() );
					con_print ( true, true, "Section: [ %s ]", func->GetScriptSectionName() );
					con_print ( true, true, "Line: [ %s ]", context->GetExceptionLineNumber() );
					con_print ( true, true, "Desc: [ %s ]", context->GetExceptionString() );
				}
			else
				con_print ( true, true, "The script ended for some unforeseen reason [ %i ]", ret );

			return false;
		}

	return true;
}


//-----------------------------------------------------------------------------
//
// Add a script console command to the executable list of functions
//
// funcName - what the console command is
// funcPtr - which function gets called from the console command
//
bool sys_addScriptConsoleFunction ( string funcName, string funcPtr, bool setParam )
//-----------------------------------------------------------------------------
{
	_scriptFunctionName tempScriptFunction;

	tempScriptFunction.functionName = funcPtr;
	tempScriptFunction.scriptName = funcName;
	tempScriptFunction.fromScript = true;
	tempScriptFunction.param1 = setParam;

	tempScriptFunction.funcID = scriptEngine->GetModule ( 0 )->GetFunctionByDecl ( tempScriptFunction.functionName.c_str() );

	if ( tempScriptFunction.funcID == NULL  )
		{
			con_print ( true, true, "Err: Failed to get function ID for [ %s ].", tempScriptFunction.functionName.c_str());
			return false;
		}

	con_print ( true, true, "FuncID for script function [ %s ] is [ %i ]", tempScriptFunction.functionName.c_str(), tempScriptFunction.funcID );

	// TODO: Handle adding parameter value

	scriptFunctions.push_back ( tempScriptFunction );
	numScriptFunctions++;
	return true;
}

//--------------------------------------------------------
//
// TODO - Get this working properly
//
// Restart the script engine
//
// TODO: Finish this
bool sys_restartScriptEngine()
//--------------------------------------------------------
{
	con_print ( true, true, "Restarting Scripting engine" );
	sys_cleanupScriptEngine ( true );

	numScriptFunctions = 0;
	conCurrentNumCommands = 0;

	con_addConsoleCommands();

	sys_initScriptEngine();
	sys_executeScriptFunction ( "as_initGameVars", "" );

	return true;
	//    al_free(scriptFunctions);
	//	sys_executeScriptFunction ("as_addAllScriptCommands", "");
}

//--------------------------------------------------------
//
// Call this from a running script to display the current
// variables in use

void sys_printVariables()
//--------------------------------------------------------
{
	asIScriptContext *ctx = asGetActiveContext();
	asIScriptEngine *engine = ctx->GetEngine();

	int stackLevel = 0;

	int typeId = ctx->GetThisTypeId();
	void *varPointer = ctx->GetThisPointer();

	if ( typeId )
		{
			io_logToFile ( " this = 0x%x\n", varPointer );
		}

	int numVars = ctx->GetVarCount();

	for ( int n = 0; n < numVars; n++ )
		{
			int typeId = ctx->GetVarTypeId ( n );
			void *varPointer = ctx->GetAddressOfVar ( n );

			if ( typeId == engine->GetTypeIdByDecl ( "int" ) )
				{
					io_logToFile ( " %s = %d\n", ctx->GetVarDeclaration ( n, stackLevel ), * ( int* ) varPointer );
				}
			else if ( typeId == engine->GetTypeIdByDecl ( "string" ) )
				{
					std::string *str = ( std::string* ) varPointer;

					if ( str )
						io_logToFile ( " %s = '%s'\n", ctx->GetVarDeclaration ( n, stackLevel ), str->c_str() );
					else
						io_logToFile ( " %s = <null>\n", ctx->GetVarDeclaration ( n, stackLevel ) );
				}
			else
				{
					io_logToFile ( " %s = {...}\n", ctx->GetVarDeclaration ( n, stackLevel ) );
				}
		}
}
