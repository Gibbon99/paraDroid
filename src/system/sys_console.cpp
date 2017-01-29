#include "../../hdr/sys_globals.h"
#include <sstream>
#include <iostream>
//#include <vector>

#define CON_COMMAND	1
#define CON_USAGE	2
#define CON_FUNC	3
#define CON_PARAM   4

#define CON_FONTSIZE 16.0f

int		conNumInHistory = 0;

//-----------------------------------------------------------------------------
//
// Variables for the console
//
//-----------------------------------------------------------------------------

vector<_conCommand>			conCommands;
int			    conCurrentNumCommands = 0;

_conLine		conLines[NUM_CON_LINES];
_conLine		conPrevCommands[NUM_MAX_CON_COMMANDS];
_conLine		conCurrentLine;
_conLine		conCurrentPrompt;
int		        conCurrentCharCount = 0;
int		        conBackSpaceDown = 0;
bool	        conCursorIsOn = true;
float		    conCursorCount = 0;
int			    conStartY;
float			conBackSpaceDelay;
int		        conHistoryPtr = 0;			// Which history command are we at
ALLEGRO_COLOR	currentConLineColor;		// Print line in this color

//-----------------------------------------------------------------------------
//
// Add all the host based console commands here
void con_addConsoleCommands()
//-----------------------------------------------------------------------------
{
	con_addCommand ( "help",		        "List out available commands",		( ExternFunc ) sys_conCommandHelp );
	con_addCommand ( "tileset",			"Set new tile set",					( ExternFunc ) io_setTileSetType );
	con_addCommand ( "tilecolor",			"Set new tile color",				( ExternFunc ) io_setTileSetColor );
	con_addCommand ( "imageInfo",         "List info about images",           ( ExternFunc ) sys_conImageInfo );
	con_addCommand ( "soundInfo",			"List info about sounds",			( ExternFunc ) sys_conSoundInfo );
	con_addCommand ( "scriptRestart",     "Restart the script engine",        ( ExternFunc ) con_scriptEngineRestart );
	con_addCommand ( "showNumHealing",    "Display number healing tiles",     ( ExternFunc ) sys_conShowNumHealingTiles );
	con_addCommand ( "listVars",			"List editable variables",			( ExternFunc ) sys_conListVariables );
	con_addCommand ( "setVar",			"Set a variable",					( ExternFunc ) sys_conSetVariable );
	con_addCommand ( "getVar",			"Get a variable",					( ExternFunc ) sys_conGetVariable );
	con_addCommand ( "debugDroids",		"Show droid stats",					( ExternFunc ) sys_debugDroidStats );
	con_addCommand ( "debugAStar",		"Which droid to debug",				( ExternFunc ) sys_setAStarDebugIndex );
	con_addCommand ( "debugParticles",	"Show particles info",				( ExternFunc ) sys_debugParticles );
	con_addCommand ( "debugPath",			"Show pathfinding info",			( ExternFunc ) sys_debugPathFinding );
	con_addCommand ( "gameVersion",		"Show game version",				( ExternFunc ) sys_printGameVersion );
	con_addCommand ( "debugDrawPhysics",	"Show physics bodies",				( ExternFunc ) sys_debugPhysicsShapes );
}

//-----------------------------------------------------------------------------
//
// Autocompletion for console commands
void con_completeCommand ( string lookFor )
//-----------------------------------------------------------------------------
{
	string	lookForKeep;

	lookForKeep = lookFor;
	//
	// Check each of the commands
	for ( int i = 0; i != conCurrentNumCommands; i++ )
		{
			if ( conCommands[i].command.find ( lookFor, 0 ) != string::npos )
				{
					con_print ( false, true, "[ %s ]", conCommands[i].command.c_str() );
					//strcpy (conCurrentLine.conLine, "");
					conCurrentLine.conLine = "";
					//strcpy (conCurrentLine.conLine, conCommands[i].command);
					conCurrentLine.conLine = conCommands[i].command;
					//conCurrentCharCount = (int)strlen(conCommands[i].command);
					conCurrentCharCount = conCommands[i].command.length();
				}
		}
}

//-----------------------------------------------------------------------------
//
// Display the current prompt
void con_processBackspaceKey ( float frameInterval )
//-----------------------------------------------------------------------------
{
	_conLine	conTempLine;
	//
	// Process the backspace key as it repeats
	if ( 1 == conBackSpaceDown )
		{
			conBackSpaceDelay += 30.0f * thinkInterval;
			if ( conBackSpaceDelay > 1.0f )
				{
					if ( conCurrentCharCount > 0 )
						conCurrentCharCount--;
					conBackSpaceDelay = 0.0f;

					conTempLine.conLine = conCurrentLine.conLine.substr ( 0, conCurrentCharCount );
					conCurrentLine.conLine = conTempLine.conLine;
				}
		}
}

//-----------------------------------------------------------------------------
//
// Process the cursor
void con_processCursor ( float frameInterval )
//-----------------------------------------------------------------------------
{
	conCursorCount += ( float ) ( 15.0f * frameInterval );
	if ( conCursorCount > 1.0f )
		{
			conCursorCount = 0;
			if ( false == conCursorIsOn )
				conCursorIsOn = true;
			else
				conCursorIsOn = false;
		}
}

//-----------------------------------------------------------------------------
//
// Pop a command from the history buffer
void con_popHistoryCommand()
//-----------------------------------------------------------------------------
{
	conCurrentLine.conLine = conPrevCommands[conHistoryPtr].conLine;
	conCurrentCharCount = conPrevCommands[conHistoryPtr].conLine.length();
}

//-----------------------------------------------------------------------------
//
// Add a valid command to the history buffer
void con_addHistoryCommand ( string command )
//-----------------------------------------------------------------------------
{
	conNumInHistory++;
	if ( conNumInHistory > NUM_MAX_CON_COMMANDS )
		conNumInHistory = NUM_MAX_CON_COMMANDS;

	for ( int i = NUM_MAX_CON_COMMANDS - 1; i != 0; i-- )
		{
			conPrevCommands[i].conLine = conPrevCommands[i - 1].conLine;
		}
	conPrevCommands[0].conLine = command;
	conHistoryPtr = 0;
}

//-----------------------------------------------------------------------------
//
// Add a command to the console command list
bool con_addCommand ( string command, string usage, ExternFunc functionPtr )
//-----------------------------------------------------------------------------
{
	_conCommand			tempConCommand;

	//
	// Check values going in
	//
	if ( 0 == command.length() )
		return false;

	if ( 0 == usage.length() )
		usage = "";
	//
	// Next slot
	//

	tempConCommand.command = 		command;
	tempConCommand.usage = 			usage;
	tempConCommand.conFunc = 		functionPtr;
	tempConCommand.type = 			CON_COMMAND_HOST;

	conCommands.push_back ( tempConCommand );

	conCurrentNumCommands++;

	return true;
}

//-----------------------------------------------------------------------------
//
// Init the console
void con_initConsole()
//-----------------------------------------------------------------------------
{
	int i;

	for ( i = 0; i < NUM_CON_LINES; i++ )
		{
			//strcpy(conLines[i].conLine, "");
			conLines[i].conLine = "";
			conLines[i].conLineColor.r = 1.0f;
			conLines[i].conLineColor.g = 1.0f;
			conLines[i].conLineColor.b = 1.0f;
			conLines[i].conLineColor.a = 1.0f;
		}

	currentConLineColor.r = 1.0f;
	currentConLineColor.g = 1.0f;
	currentConLineColor.b = 1.0f;
	currentConLineColor.a = 1.0f;

	for ( i = NUM_MAX_CON_COMMANDS - 1; i != 0; i-- )
		conPrevCommands[i].conLine = "";

	conHistoryPtr = 0;
	conNumInHistory = 0;


	conCurrentPrompt.conLine = "]_";
	conCurrentLine.conLine = "";

	conCurrentCharCount = 0;

	con_addConsoleCommands();
}

//-----------------------------------------------------------------------------
//
// Add a new line to the console - move all the others up
// 0 is the new line added
void con_incLine ( string newLine )
//-----------------------------------------------------------------------------
{
	int i;

	for ( i = NUM_CON_LINES - 1; i != 0; i-- )
		{
			conLines[i].conLine = conLines[i - 1].conLine;
			conLines[i].conLineColor = conLines[i - 1].conLineColor;
		}
	conLines[0].conLine = newLine;
	conLines[0].conLineColor = currentConLineColor;
}

//-----------------------------------------------------------------------------
//
// Print text to console
void con_print ( bool logToFile, bool updateNow, const char *format, ... )
//-----------------------------------------------------------------------------
{
	va_list		args;
	char		logText[MAX_STRING_SIZE];
	//
	// get out the passed in parameters
	//
	va_start ( args, format );
	vsnprintf ( logText, MAX_STRING_SIZE, format, args );
	va_end ( args );

	if ( true == logToFile )
		io_logToFile ( "%s", logText );

	con_incLine ( logText );

	if ( true == updateNow )
		con_drawScreen ( true, thinkInterval );

}

//-----------------------------------------------------------------------------
//
// Print to console from the scripts - INT version
void sys_scriptPrintInt ( string *msgText, int *intParam )
//-----------------------------------------------------------------------------
{
	con_print ( true, true, "[ %s ] [ %i ]", msgText->c_str(), intParam );
}

//-----------------------------------------------------------------------------
//
// Print to console from the scripts - String version
void sys_scriptPrintStr ( string *msgText, string *msgParam )
//-----------------------------------------------------------------------------
{
	con_print ( true, true, "[ %s ] [ %s ]", msgText->c_str(), msgParam->c_str() );
}


//-----------------------------------------------------------------------------
//
// Process a entered command
void con_processCommand ( string comLine )
//-----------------------------------------------------------------------------
{
	int                 i;
	vector<string>     	tokens;   // one command and one param
	string              buffer;

	string              command;
	string              param;
	string				param2;
	bool    			conMatchFound = false;
	//
	// Start the string with known empty value
	command = "";
	param = "";
	//
	// Get the command and any parameters
	//
	// Insert the string into a stream
	istringstream iss ( comLine, istringstream::in );
	//
	// Put each word into the vector
	while ( iss >> buffer )
		{
			tokens.push_back ( buffer );
		}
	command = tokens[0];
	//
	// Make sure there is a paramater to use
	if ( tokens.size() > 1 )
		param = tokens[1];
	else
		param = "";

	if ( tokens.size() > 2 )
		param2 = tokens[2];
	else
		param2 = "";
	//
	// Check each of the commands
	for ( i = 0; i != conCurrentNumCommands; i++ )
		{
			if ( conCommands[i].command == command )
				{
					con_addHistoryCommand ( comLine );
					con_incLine ( comLine );
					if ( CON_COMMAND_HOST == conCommands[i].type )
						conCommands[i].conFunc ( param, param2 );
					else
						{
							con_print ( true, true, "Matching [ %s ] to run [ %s %s]", conCommands[i].command.c_str(), conCommands[i].scriptFunc.c_str(), param.c_str() );
							sys_executeScriptFunction ( conCommands[i].command, param );

						}
					conMatchFound = true;
					break;
					// TODO: Break out early when found ?
				}
		}
	if ( false == conMatchFound )
		con_print ( false, true, "Command [ %s ] not found.", comLine.c_str() );

	//
	// Clear out the string so it doesn't show
	conCurrentLine.conLine = "";

	conCurrentCharCount = 0;
}

//-----------------------------------------------------------------------------
//
// Add a line to the console and update the display
void con_printUpdate ( bool fileLog, const char *printText, ... )
//-----------------------------------------------------------------------------
{
	va_list		    args;
	char		    conText[MAX_STRING_SIZE];
	//
	// get out the passed in parameters
	//
	va_start ( args, printText );
	vsnprintf ( conText, MAX_STRING_SIZE, printText, args );
	va_end ( args );

	con_incLine ( conText );

	if ( true == fileLog )
		io_logToFile ( "%s", conText );
}

//-----------------------------------------------------------------------------
//
// Set the current color for printing lines to the console
void con_setColor ( float red, float green, float blue, float alpha )
//-----------------------------------------------------------------------------
{
	currentConLineColor.r = red;
	currentConLineColor.g = green;
	currentConLineColor.b = blue;
	currentConLineColor.a = alpha;
}

//-----------------------------------------------------------------------------
//
// Draw the console screen
void con_drawScreen ( bool updateNow, float frameInterval )
//-----------------------------------------------------------------------------
{
	_conLine			conTempLine;
	ALLEGRO_BITMAP		*stopCrashBitmap;


	if ( NULL == builtInFont )      // Can't draw using this font until it's loaded
		return;

	conStartY = winHeight - ( CON_FONTSIZE );

	//
	// This is crashing inside Allegro - al_get_bitmap_flags is crashing due to invalid bitmap
	// Bitmap is returned from calling thread - is path finding printing out and it's a thread with no bitmap
	//
	stopCrashBitmap = al_get_target_bitmap();
	if ( NULL == stopCrashBitmap )
		return;

	if ( true == updateNow )
		{
			al_clear_to_color ( al_map_rgb ( 0,0,0 ) );
		}

	if ( true == useHoldBitmap )
		al_hold_bitmap_drawing ( true );

	sys_setFont ( FONT_MEDIUM );
	sys_setFontColor ( 1.0f, 1.0f, 1.0f, 1.0f );

	for ( int i = 0; i < winHeight / CON_FONTSIZE; i++ )
		{
			if ( false == font[currentFont].valid )
				al_draw_textf ( builtInFont, conLines[i].conLineColor, 2.0f, ( conStartY - ( ( i * CON_FONTSIZE ) + CON_FONTSIZE ) ), ALLEGRO_ALIGN_LEFT, "%s", conLines[i].conLine.c_str() );
			else
				al_draw_textf ( font[currentFont].ttfFont, conLines[i].conLineColor, 2.0f, ( conStartY - ( ( i * CON_FONTSIZE ) + CON_FONTSIZE ) ), ALLEGRO_ALIGN_LEFT, "%s", conLines[i].conLine.c_str() );
		}

	if ( true == useHoldBitmap )
		al_hold_bitmap_drawing ( false );

	if ( true == updateNow )
		{
			al_flip_display();
		}

	if ( false == updateNow )
		{
			conTempLine.conLine = conCurrentLine.conLine;
			if ( true == conCursorIsOn )
				conTempLine.conLine += "_";
			else
				conTempLine.conLine += " ";

			if ( false == font[currentFont].valid )
				al_draw_textf ( builtInFont, conLines[0].conLineColor, 1.0f, ( float ) ( winHeight - ( CON_FONTSIZE ) ), ALLEGRO_ALIGN_LEFT, "%s", conTempLine.conLine.c_str() );
			else
				al_draw_textf ( font[currentFont].ttfFont, conLines[0].conLineColor, 1.0f, ( float ) ( winHeight - ( CON_FONTSIZE ) ), ALLEGRO_ALIGN_LEFT, "%s", conTempLine.conLine.c_str() );
		}
}

//-----------------------------------------------------------------------------
//
// Add a script function to the console commands
void con_addScriptCommand ( string command, string usage, string funcPtr, bool setParam )
//-----------------------------------------------------------------------------
{
	_conCommand			tempConCommand;

	tempConCommand.type =		CON_COMMAND_SCRIPT;
	tempConCommand.command = 	command;
	tempConCommand.usage = 		usage;
	tempConCommand.scriptFunc = funcPtr;

	conCommands.push_back ( tempConCommand );
	sys_addScriptConsoleFunction ( conCommands.back().command, conCommands.back().scriptFunc, setParam );
	conCurrentNumCommands++;
}
