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

cpVect  viewableScreenCoord;
float   startStatusX = 0.0f;
float   startScoreX = 0.0f;

typedef struct
{
	float	x;
	float	y;
	float	width;
	float	height;
} _myBoundingBox;

//-----------------------------------------------------------------------------
//
// Calculate the display position of the text within the HUD graphic
void sys_calcHudTextPosition ( int hudWidth, int hudHeight )
//-----------------------------------------------------------------------------
{
	statusTextY = ( ( hudHeight - font[FONT_LARGE].fontSize ) / 2 ) - 4;
	//
	// Get locations to print text
	startStatusX = hudWidth * ( statusTextX / 100 );
	startScoreX = hudWidth * ( scoreTextX / 100 );
}

//-----------------------------------------------------------------------------
//
// Change to new currentMode
void sys_changeMode ( int newMode, bool useFade )
//-----------------------------------------------------------------------------
{
	static int previousMode;

//	con_print ( true, false, "Change to mode [ %i ]", newMode );

	if ( true == useFade )
		{
			processFadeValue = true;
			currentFadeAction = FADE_OFF;
			fadeValue = 0.0f;
			modeAfterFadeOff = newMode;
		}

	if ( ( -1 == newMode ) && ( true == useFade ) )
		{
			modeAfterFadeOff = previousMode;
			return;
		}

	previousMode = currentMode;

	//
	// this is changed by the processFade function
	if ( false == useFade )
		currentMode = newMode;

	io_resetKeyboardStates();

	sys_stopPlayerMovement();
}

//----------------------------------------------------------------------
//
// Is an object visible on the screen
bool sys_visibleOnScreen ( cpVect worldCoord, int shapeSize )
//----------------------------------------------------------------------
{
	_myBoundingBox	screenArea;

	if ( ( worldCoord.x < 0 ) || ( worldCoord.y < 0 ) )
		return false;

	screenArea.x = viewableScreenCoord.x - ( shapeSize * 2 );
	screenArea.y = viewableScreenCoord.y - ( shapeSize * 2 );
	screenArea.width = winWidth;
	screenArea.height = winHeight;

	if ( worldCoord.x < screenArea.x )
		return false;

	if ( worldCoord.y < screenArea.y )
		return false;

	if ( worldCoord.x > screenArea.x + ( screenArea.width + ( shapeSize * 2 ) ) )
		return false;

	if ( worldCoord.y > screenArea.y + ( screenArea.height + ( shapeSize * 2 ) ) )
		return false;

	return true;
}

//-----------------------------------------------------------------------------
//
// Convert worldPosition coords to screen coords
cpVect sys_worldToScreen ( cpVect worldPos, int shapeSize )
//-----------------------------------------------------------------------------
{
	cpVect  screenCoords;

	if ( true == sys_visibleOnScreen ( worldPos, shapeSize ) )
		{
			screenCoords.x = worldPos.x - viewableScreenCoord.x;
			screenCoords.y = worldPos.y - viewableScreenCoord.y;
		}
	else
		{
			screenCoords.x = -1;
			screenCoords.y = -1;

			screenCoords.x = worldPos.x - viewableScreenCoord.x;
			screenCoords.y = worldPos.y - viewableScreenCoord.y;
		}
	return screenCoords;
}

//-----------------------------------------------------------------------------
// Safely concatenate two strings.
size_t                  /* O - Length of string */
strlcat ( char       *dst,      /* O - Destination string */
          const char *src,      /* I - Source string */
          size_t     size )     /* I - Size of destination string buffer */
//-----------------------------------------------------------------------------
{
	size_t    srclen;         /* Length of source string */
	size_t    dstlen;         /* Length of destination string */
//
// Figure out how much room is left...
//
	dstlen = strlen ( dst );
	size   -= dstlen + 1;

	if ( !size )
		return ( dstlen );      // No room, return immediately
//
// Figure out how much room is needed...
//
	srclen = strlen ( src );

//
// Copy the appropriate amount...
//
	if ( srclen > size )
		srclen = size;

	memcpy ( dst + dstlen, src, srclen );
	dst[dstlen + srclen] = '\0';

	return ( dstlen + srclen );
}

//-----------------------------------------------------------------------------
// Safely copy two strings.
//
// From http://stackoverflow.com/questions/18547251/when-i-use-strlcpy-function-in-c-the-compilor-give-me-an-error
//
size_t                  /* O - Length of string */
strlcpy ( char       *dst,      /* O - Destination string */
          const char *src,      /* I - Source string */
          size_t      size )    /* I - Size of destination string buffer */
//-----------------------------------------------------------------------------
{
	size_t    srclen;         /* Length of source string */
//
// Figure out how much room is needed...
//
	size --;
	srclen = strlen ( src );
//
// Copy the appropriate amount...
//

	if ( srclen > size )
		srclen = size;

	memcpy ( dst, src, srclen );
	dst[srclen] = '\0';

	return ( srclen );
}
