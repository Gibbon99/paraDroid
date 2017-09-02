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

_images		image[] =
{
	{NULL, "alltiles.bmp", false, 0},
	{NULL, "hud.tga", 	false, flagKeyColor | flagCenterX},
	{NULL, "001.bmp", 	false, flagKeyColor},
	{NULL, "123.bmp", 	false, flagKeyColor},
	{NULL, "139.bmp", 	false, flagKeyColor},
	{NULL, "247.bmp", 	false, flagKeyColor},
	{NULL, "249.bmp", 	false, flagKeyColor},
	{NULL, "296.bmp", 	false, flagKeyColor},
	{NULL, "302.bmp", 	false, flagKeyColor},
	{NULL, "329.bmp", 	false, flagKeyColor},
	{NULL, "420.bmp", 	false, flagKeyColor},
	{NULL, "476.bmp", 	false, flagKeyColor},
	{NULL, "493.bmp", 	false, flagKeyColor},
	{NULL, "516.bmp", 	false, flagKeyColor},
	{NULL, "571.bmp", 	false, flagKeyColor},
	{NULL, "598.bmp", 	false, flagKeyColor},
	{NULL, "614.bmp", 	false, flagKeyColor},
	{NULL, "615.bmp", 	false, flagKeyColor},
	{NULL, "629.bmp", 	false, flagKeyColor},
	{NULL, "711.bmp", 	false, flagKeyColor},
	{NULL, "742.bmp", 	false, flagKeyColor},
	{NULL, "751.bmp", 	false, flagKeyColor},
	{NULL, "821.bmp", 	false, flagKeyColor},
	{NULL, "834.bmp", 	false, flagKeyColor},
	{NULL, "883.bmp", 	false, flagKeyColor},
	{NULL, "999.bmp", 	false, flagKeyColor},
	{NULL, "splash.png",		false,	flagFullScreen},
	{NULL, "bullet_001.bmp",   	false, flagKeyColor},
	{NULL, "bullet_476.bmp",   	false, flagKeyColor},
	{NULL, "bullet_821.bmp",   	false, flagKeyColor},
	{NULL, "explosion.bmp",		false, flagKeyColor},
	{NULL, "db_001.jpg",		false, flagKeyColor},
	{NULL, "spotlight.png",		false, flagKeyColor},
	{NULL, "alltiles_128.bmp",	false, 0},
	{NULL, "particleSmoke.png",	false, flagKeyColor},
	{NULL, "particleSpark.png",	false, flagKeyColor},
	{NULL, "static_1.png", 		false, flagFullScreen},
	{NULL, "static_2.png", 		false, flagFullScreen},
	{NULL, "static_3.png", 		false, flagFullScreen},
	{NULL, "static_4.png", 		false, flagFullScreen},
	{NULL, "static_5.png", 		false, flagFullScreen},
	{NULL, "static_6.png", 		false, flagFullScreen},
	{NULL, "static_7.png", 		false, flagFullScreen},
	{NULL, "static_8.png", 		false, flagFullScreen},
	{NULL, "static_9.png", 		false, flagFullScreen},
};

int			currentTileType = TILESET_CLASSIC;
int			currentTileColor = TILESET_GREY;
string		currentTileTypeStr;
string		currentTileColorStr;


//--------------------------------------------------------
//
// Load an image file
bool io_loadImage ( int index, const char *fileName )
//--------------------------------------------------------
{
	ALLEGRO_COLOR	keyColor;

	image[index].image = al_load_bitmap ( fileName );
	
	if ( NULL == image[index].image )
		{
			io_logToFile ( "Loading image failed [ %s ]", fileName );
			return false;
		}

	io_logToFile ( "Loaded image [ %s ]", fileName );

	if ( ( image[index].imageFlags & flagKeyColor ) == flagKeyColor )
		{
			keyColor = al_get_pixel ( image[index].image, 0, 0 );

			al_convert_mask_to_alpha ( image[index].image, keyColor );
		}
	return true;
}

//--------------------------------------------------------
//
// Load a new tile set
void io_loadNewTileSet()
//--------------------------------------------------------
{
	string tileSetFileName;
	string tileSetSmallFileName;

	tileSetFileName = "";
	tileSetSmallFileName = "";

	switch ( currentTileType )
		{
			case TILESET_CLASSIC:
				tileSetFileName = "alltiles.bmp";
				if (currentTileColor == TILESET_DARK)
					tileSetFileName = "alltiles_dark.bmp";
					
				tileSetSmallFileName = "alltiles_128.bmp";
				io_loadImage ( tiles, tileSetFileName.c_str() );
				io_loadImage ( tilesSmall, tileSetSmallFileName.c_str() );
				return;
				break;

			case TILESET_FUTURE:
				tileSetFileName = "future_";
				tileSetSmallFileName = "future_";
				break;

			case TILESET_RETRO:
				tileSetFileName = "retro_";
				tileSetSmallFileName = "retro_";
				break;

			default:
				tileSetFileName = "retro_";
				tileSetSmallFileName = "retro_";
				break;
		}

	switch ( currentTileColor )
		{
			case TILESET_RED:
				tileSetFileName += "red.bmp";
				tileSetSmallFileName += "red_128.bmp";
				break;
			case TILESET_GREEN:
				tileSetFileName += "green.bmp";
				tileSetSmallFileName += "green_128.bmp";
				break;
			case TILESET_BLUE:
				tileSetFileName += "blue.bmp";
				tileSetSmallFileName += "blue_128.bmp";
				break;
			case TILESET_DARK:
				tileSetFileName += "dark.bmp";
				tileSetSmallFileName += "dark_128.bmp";
				if (TILESET_CLASSIC == currentTileType)
				{
					tileSetFileName = "alltiles_dark.bmp";
					tileSetSmallFileName = "alltiles_128.bmp";
				}
				break;
			case TILESET_GREY:
				tileSetFileName += "gray.bmp";
				tileSetSmallFileName += "gray_128.bmp";
				break;
			case TILESET_YELLOW:
				tileSetFileName += "yellow.bmp";
				tileSetSmallFileName += "yellow_128.bmp";
				break;
			case TILESET_TURQUOISE:
				tileSetFileName += "turquoise.bmp";
				tileSetSmallFileName += "turquoise_128.bmp";
				break;
			default:
				tileSetFileName += "blue.bmp";
				tileSetSmallFileName += "blue_128.bmp";
				break;
		}
	io_loadImage ( tiles, tileSetFileName.c_str() );
	io_loadImage ( tilesSmall, tileSetSmallFileName.c_str() );

	con_printUpdate ( true, "Loaded tile set [ %s ]", tileSetFileName.c_str() );
	con_printUpdate ( true, "Loaded tile set [ %s ]", tileSetSmallFileName.c_str() );
}


//--------------------------------------------------------
//
// Set a new tile color
void io_setTileSetColor ( string tileColor )
//--------------------------------------------------------
{
	static int      beforeDarkColor;

	if ( tileColor == "save" )
		{
			if ( currentTileColor != TILESET_DARK )
				beforeDarkColor = currentTileColor;
			return;
		}

	if ( tileColor == "restore" )
		currentTileColor = beforeDarkColor;

	if ( tileColor == "help" )
		{
			con_print ( false, true, "Tile colors - tileset_ [ red, green, blue, dark, gray, yellow, turquoise ]" );
			return;
		}

	if ( tileColor == "red" )
		{
			currentTileColor = TILESET_RED;
		}

	if ( tileColor == "blue" )
		{
			currentTileColor = TILESET_BLUE;
		}

	if ( tileColor == "green" )
		{
			currentTileColor = TILESET_GREEN;
		}

	if ( tileColor == "dark" )
		{
			currentTileColor = TILESET_DARK;
		}

	if ( tileColor == "gray" )
		{
			currentTileColor = TILESET_GREY;
		}

	if ( tileColor == "yellow" )
		{
			currentTileColor = TILESET_YELLOW;
		}

	if ( tileColor == "turquoise" )
		{
			currentTileColor = TILESET_TURQUOISE;
		}

	io_loadNewTileSet();
}

//--------------------------------------------------------
//
// Set the tile set to use
//
// Attrib - Classic, Retro, Future
// Color - Red, Green, Blue, Dark, Grey, Yellow, Turquoise
void io_setTileSetType ( string tileType )
//--------------------------------------------------------
{
	if ( tileType == "help" )
		{
			con_print ( false, true, "Tile types - tiletype_ [ classic, future, retro ]" );
			return;
		}

	if ( tileType == "classic" )
		{
			currentTileType = TILESET_CLASSIC;
		}

	if ( tileType == "retro" )
		{
			currentTileType = TILESET_RETRO;
		}

	if ( tileType == "future" )
		{
			currentTileType = TILESET_FUTURE;
		}
	io_loadNewTileSet();
}

//--------------------------------------------------------
//
// Load all the images files
bool io_loadAllImages()
//--------------------------------------------------------
{
	for ( int i = 0; i != numImages; i++ )
		{
			strcpy ( loadingStatus, image[i].fileName );
			loadingBarPercent += 1.0f / numImages;
			al_rest ( progressBarLoadTimes );

			if ( true == io_loadImage ( i, image[i].fileName ) )
				{
					image[i].loadedOk = true;
				}
			else
				{
					image[i].loadedOk = false;
					con_print ( true, false, "ERROR: Failed to load image [ %s ]", image[i].fileName );
					sys_errorFatal ( __FILE__, __LINE__, "Error: Failed to load image file" );
				}
		}
	loadingBarPercent = 0.0f;
	currentTileColor = TILESET_BLUE;
	currentTileType = TILESET_RETRO;
	sys_calcHudTextPosition ( al_get_bitmap_width ( image[hud].image ), al_get_bitmap_height ( image[hud].image ) );
	return true;
}

//--------------------------------------------------------
//
// Convert bitmaps from memory to video card format
void io_convertImages()
//--------------------------------------------------------
{
	ALLEGRO_BITMAP	*tempBitmap;

	for ( int i = 0; i != numImages; i++ )
		{
			tempBitmap = al_clone_bitmap ( image[i].image );
			image[i].image = al_clone_bitmap ( tempBitmap );
		}
}

//--------------------------------------------------------
// Arguments:
// `destination_path` - Where to put the screenshot. If NULL, uses
//      ALLEGRO_USER_DOCUMENTS_PATH.
//
// `gamename` - The name of your game (only use path-valid characters).
//      If NULL, uses al_get_app_name().
//
//
// Returns:
// 0 on success, anything else on failure.
bool io_screenShot ( const char *destination_path, const char *gamename )
//--------------------------------------------------------
{
	ALLEGRO_PATH *path;
	char *filename, *filename_wp;
	struct tm *tmp;
	time_t t;
	unsigned int i;
	const char *path_cstr;

	if ( !destination_path )
		path = al_get_standard_path ( ALLEGRO_USER_DOCUMENTS_PATH );
	else
		path = al_create_path_for_directory ( destination_path );

	if ( !path )
		return false;

	if ( !gamename )
		{
			if ( ! ( gamename = al_get_app_name() ) )
				{
					al_destroy_path ( path );
					return false;
				}
		}

	t = time ( 0 );
	tmp = localtime ( &t );
	if ( !tmp )
		{
			al_destroy_path ( path );
			return false;
		}

	// Length of gamename + length of "-YYYYMMDD" + length of maximum [a-z] + NULL terminator
	if ( ! ( filename_wp = filename = ( char * ) malloc ( strlen ( gamename ) + 9 + 2 + 1 ) ) )
		{
			al_destroy_path ( path );
			return false;
		}

	strcpy ( filename, gamename );
	// Skip to the '.' in the filename, or the end.
	for ( ; *filename_wp != '.' && *filename_wp != 0; ++filename_wp );

	*filename_wp++ = '-';
	if ( strftime ( filename_wp, 9, "%Y%m%d", tmp ) != 8 )
		{
			free ( filename );
			al_destroy_path ( path );
			return false;
		}
	filename_wp += 8;

	for ( i = 0; i < 26*26; ++i )
		{
			if ( i > 25 )
				{
					filename_wp[0] = ( i / 26 ) + 'a';
					filename_wp[1] = ( i % 26 ) + 'a';
					filename_wp[2] = 0;
				}
			else
				{
					filename_wp[0] = ( i % 26 ) + 'a';
					filename_wp[1] = 0;
				}

			al_set_path_filename ( path, filename );
			al_set_path_extension ( path, ".png" );
			path_cstr = al_path_cstr ( path, ALLEGRO_NATIVE_PATH_SEP );

			if ( al_filename_exists ( path_cstr ) )
				continue;

			if ( false == al_save_bitmap ( path_cstr, al_get_target_bitmap() ) )
				{
					free ( filename );
					al_destroy_path ( path );
					return false;
				}

			free ( filename );
			al_destroy_path ( path );
			return true;
		}

	free ( filename );
	al_destroy_path ( path );

	return false;
}
