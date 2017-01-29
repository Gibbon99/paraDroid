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
#include "../../hdr/game/gam_transfer.h"

int					debugCurrentCircuitType = -1;
double				LOSTimeStart;
double				LOSTimeStop;
int					debugAStarIndex = 0;

//-----------------------------------------------------------------------------
//
// Return a string based on current AI CurrentState
char *sys_getAIString(int ai_currentState)
//-----------------------------------------------------------------------------
{
	switch (ai_currentState)
	{
		case AI_STATE_HEALTH:
			return "AI_STATE_HEALTH";
			break;

		case AI_STATE_PATROL:
			return "AI_STATE_PATROL";
			break;
	}
}

//-----------------------------------------------------------------------------
//
// Show stats and states for droids on level
void sys_debugDroidStats()
//-----------------------------------------------------------------------------
{
	con_print(true, false, "Level [ %i ] Droids alive [ %i ]", currentLevel, shipLevel[currentLevel].numEnemiesAlive);
	for (int i = 0; i != shipLevel[currentLevel].numDroids; i++)
	{
		if (true == shipLevel[currentLevel].droid[i].isAlive)
		{
			con_print(true, false, "Type [ %s ]", dataBaseEntry[shipLevel[currentLevel].droid[i].droidType].description.c_str());
//			con_print(true, false, "Droid [ %i ] Collision ignore Count [ %3.2f ] Collide Count [ %i ]", i, shipLevel[currentLevel].droid[i].ignoreCollisionsCounter, shipLevel[currentLevel].droid[i].collisionCount);
			con_print(true, false, "BeenShot [ %s ] Speed [ %3.3f ] Shoot [ %2.3f ] Shoot [ %3.3f ]", shipLevel[currentLevel].droid[i].beenShotByPlayer == true ? "True" : "False", shipLevel[currentLevel].droid[i].currentSpeed, shipLevel[currentLevel].droid[i].chanceToShoot, shipLevel[currentLevel].droid[i].chanceToShoot);
//			con_print(true, false, "AI_State [ %s ] Collided [ %s ] Ignore Collide [ %s ]", sys_getAIString(shipLevel[currentLevel].droid[i].ai_currentState), shipLevel[currentLevel].droid[i].hasCollided == true ? "True" : "False", shipLevel[currentLevel].droid[i].ignoreCollisions == true ? "True" : "False");
			con_print(true, false, "------------------------");
		}
	}
}

//-----------------------------------------------------------------------------
//
// Show the tile grid
void sys_debugShowTileGrid()
//-----------------------------------------------------------------------------
{
	ALLEGRO_COLOR		lineColor;
	cpVect				lineStart, lineEnd;

	lineColor = al_map_rgb(128,128,128);

	for (int i = 0; i != winWidth / TILE_SIZE; i++)
	{
		for (int j = 0; j != winHeight / TILE_SIZE; j++)
		{
			lineStart.x = 0; lineEnd.x = winWidth;
			lineStart.y = j * TILE_SIZE; lineEnd.y = j * TILE_SIZE;

			al_draw_line(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, lineColor, 2);

			lineStart.x = i * TILE_SIZE; lineEnd.x = i * TILE_SIZE;
			lineStart.y = 0; lineEnd.y = winHeight;

			al_draw_line(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, lineColor, 2);
		}
	}
}
//-----------------------------------------------------------------------------
//
// Show middle of the screen
void sys_displayScreenMiddle()
//-----------------------------------------------------------------------------
{
	ALLEGRO_COLOR		lineColor;
	cpVect				lineStart, lineEnd;

	lineColor = al_map_rgb(255,255,255);

	lineStart.x = winWidth / 2;
	lineStart.y = 0;
	lineEnd.x = winWidth / 2;
	lineEnd.y = winHeight;

	al_draw_line(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, lineColor, 2);

	lineStart.x = 0;
	lineStart.y = winHeight / 2;
	lineEnd.x = winWidth;
	lineEnd.y = winHeight / 2;

	al_draw_line(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, lineColor, 2);
}

//-----------------------------------------------------------------------------
//
// Display debug values
void sys_displayDebug()
//-----------------------------------------------------------------------------
{
	sys_printStringExt(winWidth - 180, 75, "FPS : %i", fpsPrint);

	al_draw_textf(builtInFont, al_map_rgb_f(1.0f, 1.0f, 1.0f), 1.0f, winHeight - 50, ALLEGRO_ALIGN_LEFT, "AlertDistance [ %3.2f ] Index [ %i ] Transfer  [ %3.3f ]", alertLevelDistance, playerDroidTypeDBIndex, playerTransferTimeRemaining);
	al_draw_textf(builtInFont, al_map_rgb_f(1.0f, 1.0f, 1.0f), 1.0f, winHeight - 40, ALLEGRO_ALIGN_LEFT, "tilePosX [ %i ] tilePosY [ %i ] Pixel [ %3.3f %3.3f ]", tilePosX, tilePosY, pixelX, pixelY);
	al_draw_textf(builtInFont, al_map_rgb_f(1.0f, 1.0f, 1.0f), 1.0f, winHeight - 30, ALLEGRO_ALIGN_LEFT, "Particle timing Think [ %3.5f ] Render [ %3.5f ]", profileParticleThinkEnd - profileParticleThinkStart, profileParticleRenderStart - profileParticleRenderEnd);
	al_draw_textf(builtInFont, al_map_rgb_f(1.0f, 1.0f, 1.0f), 1.0f, winHeight - 20, ALLEGRO_ALIGN_LEFT, "Frame [ %3.5f ] FPS [ %i ] Think [ %i ]", profileRenderFrameEnd - profileRenderFrameStart, fpsPrint, thinkFpsPrint);
	al_draw_textf(builtInFont, al_map_rgb_f(1.0f, 1.0f, 1.0f), 1.0f, winHeight - 10, ALLEGRO_ALIGN_LEFT, "Tile [ %i ] FPS [ %i ] Calcs Skipped [ %i ] Drawn [ %i ] Time [ %2.6f ]", tileCollision, fpsPrint, profileTileCalcSkipped, profileTotalTileDrawn, profileTimeEnd - profileTimeStart);
}

//---------------------------------------------------------
//
//	Show the droids destination point
void gam_debugShowDestination()
//---------------------------------------------------------
{
	cpVect		startLine;
	cpVect		endLine;

	for (int i = 0; i != shipLevel[currentLevel].numDroids; i++)
	{
		if (shipLevel[currentLevel].droid[i].isAlive == true)
		{
			startLine = shipLevel[currentLevel].droid[i].worldPos;
			endLine = shipLevel[currentLevel].droid[i].destinationCoords;

			startLine = sys_worldToScreen(startLine, 100);
			endLine = sys_worldToScreen(endLine, 100);

			al_draw_line(startLine.x, startLine.y, endLine.x, endLine.y, al_map_rgb_f(0.0f, 0.0f, 1.0f), 2);
		}
	}
}

//---------------------------------------------------------
//
// Show waypoint lines
void gam_debugShowWaypoints()
//---------------------------------------------------------
{
	cpVect	screenPosStart;
	cpVect	screenPosEnd;

	if (false == debugDrawPhysics)
		return;

	for(int i = 0; i < shipLevel[currentLevel].numWaypoints; i++)
		{

			screenPosStart.x = shipLevel[currentLevel].wayPoints[i].x;
			screenPosStart.y = shipLevel[currentLevel].wayPoints[i].y;
			screenPosStart = sys_worldToScreen(screenPosStart, 100);
			i++;
			screenPosEnd.x = shipLevel[currentLevel].wayPoints[i].x;
			screenPosEnd.y = shipLevel[currentLevel].wayPoints[i].y;
			screenPosEnd = sys_worldToScreen(screenPosEnd, 100);

			if(i == shipLevel[currentLevel].numWaypoints)
				{
					screenPosEnd.x = shipLevel[currentLevel].wayPoints[0].x;
					screenPosEnd.y = shipLevel[currentLevel].wayPoints[0].y;
					screenPosEnd = sys_worldToScreen(screenPosEnd, 100);
				}

			i--;

			al_draw_line(screenPosStart.x, screenPosStart.y, screenPosEnd.x, screenPosEnd.y, al_map_rgb_f(1.0f, 0.0f, 1.0f), 2);
		}

	screenPosEnd.x = shipLevel[currentLevel].wayPoints[shipLevel[currentLevel].numWaypoints].x;
	screenPosEnd.y = shipLevel[currentLevel].wayPoints[shipLevel[currentLevel].numWaypoints].y;
	screenPosEnd = sys_worldToScreen(screenPosEnd, 100);

	screenPosStart.x = shipLevel[currentLevel].wayPoints[0].x;
	screenPosStart.y = shipLevel[currentLevel].wayPoints[0].y;
	screenPosStart = sys_worldToScreen(screenPosStart, 100);

//	al_draw_line(screenPosStart.x, screenPosStart.y, screenPosEnd.x, screenPosEnd.y, al_map_rgb_f(1.0f, 0.0f, 1.0f), 2);
}
