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

ALLEGRO_CONFIG* configFile = NULL;
ALLEGRO_CONFIG* databaseFile = NULL;

//--------------------------------------------------------
//
// Open a config file from the script

void io_loadConfigFile ( string fileName )
//--------------------------------------------------------
{
	configFile = al_load_config_file ( fileName.c_str() );
	if ( NULL == configFile )
		{
			printf ( "Error: Could not open config file. Exiting\n" );
			sys_errorFatal ( "io_loadConfigFile", __LINE__, "Error: Couldn't open config file. Exiting" );
		}

	//	io_saveConfigFile(fileName.c_str());

	//	al_save_config_file(fileName.c_str(), configFile);
}

//--------------------------------------------------------
//
// Close config file

void io_closeConfigFile()
//--------------------------------------------------------
{
	al_destroy_config ( configFile );
	configFile = NULL;
}

//--------------------------------------------------------
//
// Save the current configuration to the config file

void io_saveConfigFile ( string fileName )
//--------------------------------------------------------
{
	//    return;

	if ( configFile == NULL )
		{
			con_print ( true, false, "Invalid config file pointer" );
			return;
		}

	if ( false == al_save_config_file ( fileName.c_str(), configFile ) )
		{
			con_print ( true, false, "Error: Could not save config values to file [ %s ]", fileName.c_str() );
			//		sys_errorFatal("io_saveConfigFile", __LINE__, "Error: Could not save config values to file\n");
		}
}

//--------------------------------------------------------
//
// Read config values

int io_readConfigValuesInt ( string varName )
//--------------------------------------------------------
{
	const char* returnValue;

	returnValue = al_get_config_value ( configFile, "Options", varName.c_str() );
	if ( NULL == returnValue )
		{
			printf ( "Error: Could not get value from config file [ %s ]. Exiting\n", varName.c_str() );
			sys_errorFatal ( __FILE__, __LINE__, "Error: Could not get value from config file. Exiting\n" );
		}

	//	io_saveConfigValueInt(varName, returnValue);

	return atoi ( returnValue );
}

//--------------------------------------------------------
//
// Read config value - string
string io_readConfigValuesString ( string varName )
//--------------------------------------------------------
{
	const char* returnValue;

	returnValue = al_get_config_value ( configFile, "Options", varName.c_str() );
	if ( NULL == returnValue )
		{
			printf ( "Error: Could not get value from config file [ %s ]. Exiting\n", varName.c_str() );
			sys_errorFatal ( __FILE__, __LINE__, "Error: Could not get value from config file. Exiting" );
		}

	return returnValue;
}

//--------------------------------------------------------
//
// Read config values

float io_readConfigValuesFloat ( string varName )
//--------------------------------------------------------
{
	float returnValue;

	returnValue = atof ( al_get_config_value ( configFile, "Options", varName.c_str() ) );
	if ( NULL == returnValue )
		{
			printf ( "Error: Could not get value from config file. Exiting\n" );
			sys_errorFatal ( __FILE__, __LINE__, "Error: Could not get value from config file. Exiting\n" );
		}

	io_saveConfigValueFloat ( varName, returnValue );

	return returnValue;
}

//--------------------------------------------------------
//
// Set a config value to be saved

void io_saveConfigValueString ( string varName, string varValue )
//--------------------------------------------------------
{
	if ( configFile != NULL )
		al_set_config_value ( configFile, "Options", varName.c_str(), varValue.c_str() );
	else
		io_logToFile ( "Error: io_saveConfigValueString: configFile is NULL" );
}

//--------------------------------------------------------
//
// Set a config value to be saved

void io_saveConfigValueFloat ( string varName, float varValue )
//--------------------------------------------------------
{
	char tempValue[MAX_STRING_SIZE];

	sprintf ( tempValue, "%3.2f", varValue );

	al_set_config_value ( configFile, "Options", varName.c_str(), tempValue );
}

//--------------------------------------------------------
//
// Set a config value to be saved

void io_saveConfigValueInt ( string varName, int varValue )
//--------------------------------------------------------
{
	char tempValue[MAX_STRING_SIZE];

	sprintf ( tempValue, "%i", varValue );

	al_set_config_value ( configFile, "Options", varName.c_str(), tempValue );
}

//----------  Database routines -----------

//--------------------------------------------------------
//
// Set a config value to be saved

void io_saveDBValueString ( string varName, string varValue )
//--------------------------------------------------------
{
	if ( databaseFile != NULL )
		al_set_config_value ( databaseFile, "droidInfo", varName.c_str(), varValue.c_str() );
	else
		io_logToFile ( "Error: io_saveConfigValueString: configFile is NULL" );
}

//--------------------------------------------------------
//
// Set a config value to be saved

void io_saveDBValueFloat ( string varName, float varValue )
//--------------------------------------------------------
{
	char tempValue[MAX_STRING_SIZE];

	sprintf ( tempValue, "%3.2f", varValue );

	al_set_config_value ( databaseFile, "droidInfo", varName.c_str(), tempValue );
}

//--------------------------------------------------------
//
// Set a config value to be saved

void io_saveDBValueInt ( string varName, int varValue )
//--------------------------------------------------------
{
	char tempValue[MAX_STRING_SIZE];

	sprintf ( tempValue, "%i", varValue );

	al_set_config_value ( databaseFile, "droidInfo", varName.c_str(), tempValue );
}

//------------------------------------------------------------
//
// Get a float value from the config file

float io_getDBValueInt ( const char* whichKey )
//------------------------------------------------------------
{
	const char* value;

	if ( NULL == databaseFile )
		sys_errorFatal ( __FILE__, __LINE__, "Error: Config file no longer valid" );

	value = al_get_config_value ( databaseFile, "droidInfo", whichKey );
	if ( NULL == value )
		sys_errorFatal ( "io_getDBValueInt", __LINE__, "Error: No key value [ %s ]", whichKey );

	return atoi ( value );
}

//------------------------------------------------------------
//
// Get a float value from the config file

float io_getDBValueFloat ( const char* whichKey )
//------------------------------------------------------------
{
	const char* value;

	if ( NULL == databaseFile )
		sys_errorFatal ( __FILE__, __LINE__, "Error: Config file no longer valid" );

	value = al_get_config_value ( databaseFile, "droidInfo", whichKey );
	if ( NULL == value )
		sys_errorFatal ( __FILE__, __LINE__, "Error: No key value [ %s ]", whichKey );

	return atof ( value );
}

//------------------------------------------------------------
//
// Get a float value from the config file

const char* io_getDBValueString ( const char* whichKey )
//------------------------------------------------------------
{
	const char* value;

	if ( NULL == databaseFile )
		sys_errorFatal ( __FILE__, __LINE__, "Error: Config file no longer valid" );

	value = al_get_config_value ( databaseFile, "droidInfo", whichKey );
	if ( NULL == value )
		sys_errorFatal ( __FILE__, __LINE__, "Error: No key value [ %s ]", whichKey );

	return value;
}
//------------------------------------------------------------
//
// Get each field from the config file
bool io_getDBDroidInfo ( int dbIndex, const char* fileName )
//------------------------------------------------------------
{
	static int imageIndexCounter = sprite_001; // First image for first droid type

	loadingBarPercent += 1.0f / NUM_SPRITE_TYPES;
	strlcpy ( loadingStatus, fileName, strlen ( fileName ) );
	al_rest ( progressBarLoadTimes );

	databaseFile = al_load_config_file ( fileName );
	if ( NULL == databaseFile )
		sys_errorFatal ( __FILE__, __LINE__, "Error: could not open database file [ %s ]", fileName );
	// TODO (roam#1#): Seg fault if crashes when file not found - bombing because it's running in a thread - and can't
	// update console screen

	dataBaseEntry[dbIndex].maxHealth = io_getDBValueInt ( "max_health" );

	if ( 0 == dbIndex )	// 001 base droid stats
		{
			dataBaseEntry[dbIndex].maxSpeed = io_getDBValueFloat ( "max_speed" );
			dataBaseEntry[dbIndex].maxSpeed += baseGameSpeed;
			dataBaseEntry[dbIndex].accelerate = io_getDBValueFloat ( "accelerate" );
			dataBaseEntry[dbIndex].accelerate += baseGameSpeed;
		}
	else
		{
			//
			// All other droid speeds and acceleration are based on player droid values
			// Easy to change overall speed of the game
			//
			dataBaseEntry[dbIndex].maxSpeed = dataBaseEntry[0].maxSpeed + io_getDBValueFloat ( "max_speed" );
			dataBaseEntry[dbIndex].accelerate = dataBaseEntry[0].accelerate + io_getDBValueFloat ( "accelerate" );
		}

	dataBaseEntry[dbIndex].score = io_getDBValueInt ( "score" );
	dataBaseEntry[dbIndex].bounceDamage = io_getDBValueInt ( "bounce_damage" );
	dataBaseEntry[dbIndex].canShoot = io_getDBValueInt ( "can_shoot" );
	dataBaseEntry[dbIndex].rechargeTime = io_getDBValueFloat ( "rechargeTime");
	dataBaseEntry[dbIndex].bulletType = io_getDBValueInt ( "bullet_type" );
	//
	// Map the type of bullet to the sprite image for it
	//
	// Set to -1 if can_shoot == 0
	switch ( dataBaseEntry[dbIndex].bulletType )
		{
			case 0: // small double laser
				dataBaseEntry[dbIndex].bulletType = BULLET_TYPE_NORMAL;
				break;

			case 1: // Large single laser
				dataBaseEntry[dbIndex].bulletType = BULLET_TYPE_SINGLE;
				break;

			case 2: // Large double laser
				dataBaseEntry[dbIndex].bulletType = BULLET_TYPE_DOUBLE;
				break;

			case 3: // Disrupter
				dataBaseEntry[dbIndex].bulletType = BULLET_TYPE_DISRUPTER;
				break;
		}

	dataBaseEntry[dbIndex].chanceToShoot = io_getDBValueInt ( "chance_to_shoot" );
	dataBaseEntry[dbIndex].bulletDamage = io_getDBValueInt ( "bullet_damage" );
	dataBaseEntry[dbIndex].disrupterImmune = io_getDBValueInt ( "disrupter_immune" );
	dataBaseEntry[dbIndex].tokenCount = io_getDBValueInt ( "token_count" );
	strcpy ( dataBaseEntry[dbIndex].dbImageFileName, io_getDBValueString ( "dbImageFileName" ) );
	strcpy ( dataBaseEntry[dbIndex].height, io_getDBValueString ( "height" ) );
	strcpy ( dataBaseEntry[dbIndex].weight, io_getDBValueString ( "weight" ) );

	//
	// Read in key for text language
	//
	dataBaseEntry[dbIndex].description = io_getDBValueString ( "description" );
	dataBaseEntry[dbIndex].className = io_getDBValueString ( "className" );
	dataBaseEntry[dbIndex].drive = io_getDBValueString ( "drive" );
	dataBaseEntry[dbIndex].brain = io_getDBValueString ( "brain" );
	dataBaseEntry[dbIndex].weapon = io_getDBValueString ( "weapon" );
	dataBaseEntry[dbIndex].sensor1 = io_getDBValueString ( "sensor1" );
	dataBaseEntry[dbIndex].sensor2 = io_getDBValueString ( "sensor2" );
	dataBaseEntry[dbIndex].sensor3 = io_getDBValueString ( "sensor3" );
	dataBaseEntry[dbIndex].notes = io_getDBValueString ( "notes" );

	//
	// Now get the text string using that text key
	//
	dataBaseEntry[dbIndex].description = gui_getString ( dataBaseEntry[dbIndex].description );
	dataBaseEntry[dbIndex].className = gui_getString ( dataBaseEntry[dbIndex].className );
	dataBaseEntry[dbIndex].drive = gui_getString ( dataBaseEntry[dbIndex].drive );
	dataBaseEntry[dbIndex].brain = gui_getString ( dataBaseEntry[dbIndex].brain );
	dataBaseEntry[dbIndex].weapon = gui_getString ( dataBaseEntry[dbIndex].weapon );
	dataBaseEntry[dbIndex].sensor1 = gui_getString ( dataBaseEntry[dbIndex].sensor1 );
	dataBaseEntry[dbIndex].sensor2 = gui_getString ( dataBaseEntry[dbIndex].sensor2 );
	dataBaseEntry[dbIndex].sensor3 = gui_getString ( dataBaseEntry[dbIndex].sensor3 );
	dataBaseEntry[dbIndex].notes = gui_getString ( dataBaseEntry[dbIndex].notes );
	//
	// Add in index pointers
	dataBaseEntry[dbIndex].imageIndex = imageIndexCounter; // Start at first image and increment
	imageIndexCounter++;

	//
	// Close the file and free resources
	al_destroy_config ( databaseFile );

	return true;
}
