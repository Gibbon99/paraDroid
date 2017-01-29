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

#pragma once

enum
{
    flagKeyColor 	= 1,
    flagFullScreen 	= 2,
    flagCenterX 	= 4,
    flagCenterY 	= 8
};

enum
{
    TILESET_CLASSIC	= 0,
    TILESET_FUTURE,
    TILESET_RETRO,
    TILESET_RED,
    TILESET_GREEN,
    TILESET_BLUE,
    TILESET_DARK,
    TILESET_GREY,
    TILESET_YELLOW,
    TILESET_TURQUOISE,
    TILESET_HELP
};

enum
{
    tiles = 0,
    hud,
    sprite_001,
    sprite_123,
    sprite_139,
    sprite_247,
    sprite_249,
    sprite_296,
    sprite_302,
    sprite_329,
    sprite_420,
    sprite_476,
    sprite_493,
    sprite_516,
    sprite_571,
    sprite_598,
    sprite_614,
    sprite_615,
    sprite_629,
    sprite_711,
    sprite_742,
    sprite_751,
    sprite_821,
    sprite_834,
    sprite_883,
    sprite_999,
    splashImg,
    bulletType1,
    bulletType2,
    bulletType3,
    explosion,
    dbImage,
    spotLightImg,
    tilesSmall,
	particleSmoke,
	particleSpark,
	static_1,
	static_2,
	static_3,
	static_4,
	static_5,
	static_6,
	static_7,
	static_8,
	static_9,
    numImages
};

struct _images
{
    ALLEGRO_BITMAP	*image;
    const char		*fileName;
    bool			loadedOk;
    int				imageFlags;
};

extern _images		image[];

extern int			currentTileType;
extern int			currentTileColor;
extern string		currentTileTypeStr;
extern string		currentTileColorStr;

// Load an image file
bool io_loadImage(int index, const char *fileName);

// Load all the images files
bool io_loadAllImages();

// Load a bitmap font
bool io_loadBitmapFont();

// Convert bitmaps from memory to video card format
void io_convertImages();

// Set the tile set to use
//
// Attrib - Classic, Retro, Future
// Color - Red, Green, Blue, Dark, Grey, Yellow, Turquoise
void io_setTileSetType(string tileType);

// Set a new tile color
void io_setTileSetColor(string tileColor);

bool io_screenShot(const char *destination_path, const char *gamename);

// Load a new tile set
void io_loadNewTileSet();