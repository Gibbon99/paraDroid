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

#define BACKGROUND_BORDER            10
#define BACKGROUND_START_OFFSET      20
#define SIDEBAR_WIDTH	             10

#define CIRCUIT_HEIGHT		         4

#define BLOCK_HEIGHT	 	         10
#define BLOCK_WIDTH 		         6
#define BLOCK_STEP 			         2

#define BLOCK_YELLOW		         0
#define BLOCK_PURPLE		         1

#define BLOCK_LEFT			         0
#define BLOCK_RIGHT			         1

int backGroundStartX;
int backGroundStartY;
int backGroundWidth;
int backGroundHeight;

ALLEGRO_COLOR                           currentWinner;

//------------------------------------------------------------
//
// Draw a long block used in split circuits
void tran_drawLongBlock ( int whichSide, int whichCell, int posX )
//------------------------------------------------------------
{
	int startX, startY, endY;

	startX = posX;
	startY = transferCells[whichCell].startY - ( squareHeight );
	startY += BLOCK_STEP;
	endY = transferCells[whichCell].startY + ( squareHeight * 2 );
	endY -= BLOCK_STEP;

	if ( LEFT_SIDE == whichSide )
		{
			al_draw_filled_rectangle ( startX, startY, startX + SIDEBAR_WIDTH, endY, al_map_rgba_f ( COL_YELLOW ) );

			al_draw_rectangle ( startX, startY, startX + SIDEBAR_WIDTH, endY, al_map_rgba_f ( COL_BLACK ), 2.0f );
		}

	if ( RIGHT_SIDE == whichSide )
		{
			al_draw_filled_rectangle ( startX, startY, startX - SIDEBAR_WIDTH, endY, al_map_rgba_f ( COL_PURPLE ) );

			al_draw_rectangle ( startX, startY, startX - SIDEBAR_WIDTH, endY, al_map_rgba_f ( COL_BLACK ), 2.0f );
		}
}
//------------------------------------------------------------
//
// Draw random pixels contained within boundaries
void tran_drawCircuitEffect ( int startX, int startY, int length, int height, int whichSide )
//------------------------------------------------------------
{
#define NUM_PARTICLES_BASE	50

	ALLEGRO_VERTEX			*transferParticles = NULL;

	static vector<cpVect>	leftEffectPoints;
	vector<cpVect>			rightEffectPoints;
	cpVect					tempEffectPoint;
	static int				randNumberJoints;
	int						randomStartX;
	static int				randomStartY;
	int						currentStartX;
	static float			effectSpeedCount = 10.0;

	int numParticles;

	if ( NULL != transferParticles )
		{
			free ( transferParticles );
			transferParticles = NULL;
		}
	//
	// See if it's time to get new coordinates for the lines
	//
	if ( LEFT_SIDE == whichSide )
		{
			leftEffectPoints.clear();

			tempEffectPoint.x = startX + BLOCK_WIDTH * 2;
			tempEffectPoint.y = startY + ( height / 2 );
			leftEffectPoints.push_back ( tempEffectPoint );	// First point

			randNumberJoints = ( rand() % 3 ) + 3;	// Change according to circuit length

			currentStartX = startX;

			for ( int i = 0; i != randNumberJoints; i++ )
				{
					randomStartY = ( startY - 8 ) + ( rand() % ( height + 12 ) );

					randomStartX = rand() % ( ( length - currentStartX ) );
					currentStartX += randomStartX;

					tempEffectPoint.x = currentStartX;
					tempEffectPoint.y = randomStartY;

					leftEffectPoints.push_back ( tempEffectPoint );
				}

			for ( int i = 0; i != randNumberJoints; i++ )
				{
					al_draw_line ( leftEffectPoints[i].x, leftEffectPoints[i].y, leftEffectPoints[i + 1].x, leftEffectPoints[i + 1].y, al_map_rgb ( 0.0f, 1.0f, 0.0f ), 3 );
				}
			al_draw_line ( leftEffectPoints[randNumberJoints - 1].x, leftEffectPoints[randNumberJoints - 1].y, startX + length, randomStartY, al_map_rgb ( 0.0f, 1.0f, 0.0f ), 3 );

		}

	rightEffectPoints.clear();

	if ( NULL != transferParticles )
		{
			free ( transferParticles );
			transferParticles = NULL;
		}

	if ( RIGHT_SIDE == whichSide )
		{
			length -= BLOCK_WIDTH * 2;
			numParticles = NUM_PARTICLES_BASE + ( length / 10 );

			transferParticles = ( ALLEGRO_VERTEX * ) malloc ( numParticles * sizeof ( ALLEGRO_VERTEX ) );

			for ( int i = 0; i != numParticles; i++ )
				{
					transferParticles[i].x = rand() % ( ( startX + length ) - startX ) + startX;
					transferParticles[i].y = rand() % ( ( startY + height ) - startY ) + startY;
					transferParticles[i].z = 0;
					transferParticles[i].color.r = 1.0f;
					transferParticles[i].color.g = 1.0f;
					transferParticles[i].color.b = 1.0f;
				}
			al_draw_prim ( transferParticles, NULL, NULL, 0, numParticles, ALLEGRO_PRIM_POINT_LIST );
		}
	//
	// Free memory
	if ( NULL != transferParticles )
		{
			free ( transferParticles );
			transferParticles = NULL;
		}
}

//------------------------------------------------------------
//
// Get the length of the circuit
int tran_getCircuitLength ( int whichSide, int whichCell, int type )
//------------------------------------------------------------
{
	int startX, startY, length;

	switch ( whichSide )
		{
		case LEFT_SIDE:
			{
				startX = BACKGROUND_BORDER + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH;
				startY = transferCells[whichCell].startY + ( ( squareHeight - CIRCUIT_HEIGHT ) / 2 );
				length = ( transferCells[whichCell].startX - startX ) - ( BLOCK_WIDTH * 2 );
				switch ( type )
					{
					case CIRCUIT_ONE_INTO_TWO:
						startX = ( BACKGROUND_BORDER + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH ) + length * 0.25;
						length = length * 0.75;
						break;

					case CIRCUIT_NORMAL:
						length = length;
						break;

					case CIRCUIT_THREEQUARTERS:
						length = length * 0.75;
						length -= 6;
						break;

					case CIRCUIT_HALF:
						length = length / 2;
						break;

					case CIRCUIT_QUARTER:
						length = length * 0.25;
						break;

					case CIRCUIT_ONE_INTO_TWO_SHORT:
						length = length * 0.25;
						length -= 10;
						tran_drawLongBlock ( LEFT_SIDE, whichCell, startX + length );
						break;
					}
				return length;
				break;
			}
		case RIGHT_SIDE:
			{
				startX = ( backGroundWidth - ( BACKGROUND_BORDER * 2 ) ) - SIDEBAR_WIDTH;
				startY =  transferCells[whichCell].startY + ( ( squareHeight - CIRCUIT_HEIGHT ) / 2 );
				length = ( startX - ( transferCells[whichCell].startX + squareWidth ) ) - SIDEBAR_WIDTH;
				switch ( type )
					{
					case CIRCUIT_ONE_INTO_TWO:
						startX = startX - ( length * 0.25 );
						length = length * 0.75;
						break;

					case CIRCUIT_NORMAL:
						length = length;
						break;

					case CIRCUIT_THREEQUARTERS:
						length = length * 0.75;
						length -= 6;
						break;

					case CIRCUIT_HALF:
						length = length / 2;
						break;

					case CIRCUIT_QUARTER:
						length = length * 0.25;
						break;

					case CIRCUIT_ONE_INTO_TWO_SHORT:
						length = length * 0.25;
						length -= 10;
						tran_drawLongBlock ( RIGHT_SIDE, whichCell, startX - length );
						break;
					}
				return length;
				break;
			}
		}
}

//------------------------------------------------------------
//
// Draw a normal circuit type - length based on type
void tran_drawCircuitNormal ( int whichSide, int whichCell, int type, bool drawBlock )
//------------------------------------------------------------
{
	int startX, startY, length;

	if ( LEFT_SIDE == whichSide )
		{
			startX = BACKGROUND_BORDER + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH;
			startY = transferCells[whichCell].startY + ( ( squareHeight - CIRCUIT_HEIGHT ) / 2 );
			length = tran_getCircuitLength ( whichSide, whichCell, type );

//
// Draw the circuit
//
			if ( type == CIRCUIT_ONE_INTO_TWO )
				{
					if ( ( ( transferCells[whichCell].circuitTypeLeft == CIRCUIT_SPLIT_HALF ) && ( true == transferCells[whichCell - 1].powerOnLeft ) && ( CIRCUIT_ONE_INTO_TWO == transferCells[whichCell - 1].circuitTypeLeft ) )
					        || ( ( transferCells[whichCell].circuitTypeLeft == CIRCUIT_SPLIT_HALF ) && ( true == transferCells[whichCell + 1].powerOnLeft ) && ( CIRCUIT_ONE_INTO_TWO == transferCells[whichCell + 1].circuitTypeLeft ) ) )
						{
							al_draw_filled_rectangle ( startX, startY, startX + BLOCK_WIDTH * 2, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_BLACK ) );
							startX += BLOCK_WIDTH * 2;
							length -= BLOCK_WIDTH * 2;
							al_draw_filled_rectangle ( startX - ( BLOCK_WIDTH * 2 ), startY, startX + length, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_YELLOW ) );

							tran_drawCircuitEffect ( startX - ( BLOCK_WIDTH ) * 2, startY, length, CIRCUIT_HEIGHT, LEFT_SIDE );
						}
					else
						{
							al_draw_filled_rectangle ( startX, startY, startX + length, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_BLACK ) );
						}
				}
			else if ( false == transferCells[whichCell].powerOnLeft )
				al_draw_filled_rectangle ( startX, startY, startX + length, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_BLACK ) );
			else
				{
					al_draw_filled_rectangle ( startX, startY, startX + BLOCK_WIDTH * 2, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_BLACK ) );
					startX += BLOCK_WIDTH * 2;
					length -= BLOCK_WIDTH * 2;
					al_draw_filled_rectangle ( startX, startY, startX + length, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_YELLOW ) );
					tran_drawCircuitBlock ( BLOCK_RIGHT, whichCell, startX, COL_YELLOW );

					tran_drawCircuitEffect ( startX, startY, length, CIRCUIT_HEIGHT, LEFT_SIDE );
				}

			if ( true == drawBlock )
				{
					if ( type != CIRCUIT_NORMAL )
						tran_drawCircuitBlock ( BLOCK_LEFT, whichCell, startX + length, COL_YELLOW );
				}
			return;
		}

	if ( RIGHT_SIDE == whichSide )
		{
			startX = ( backGroundWidth - ( BACKGROUND_BORDER * 2 ) ) - SIDEBAR_WIDTH;
			startY =  transferCells[whichCell].startY + ( ( squareHeight - CIRCUIT_HEIGHT ) / 2 );
			length = tran_getCircuitLength ( whichSide, whichCell, type );

//
// Draw the circuit
//
			if ( type == CIRCUIT_ONE_INTO_TWO )
				{
					if ( ( ( transferCells[whichCell].circuitTypeRight == CIRCUIT_SPLIT_HALF ) && ( true == transferCells[whichCell - 1].powerOnRight ) && ( CIRCUIT_ONE_INTO_TWO == transferCells[whichCell - 1].circuitTypeRight ) )
					        || ( ( transferCells[whichCell].circuitTypeRight == CIRCUIT_SPLIT_HALF ) && ( true == transferCells[whichCell + 1].powerOnRight ) && ( CIRCUIT_ONE_INTO_TWO == transferCells[whichCell + 1].circuitTypeRight ) ) )
						{
							al_draw_filled_rectangle ( startX, startY, startX - length, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_BLACK ) );
							al_draw_filled_rectangle ( startX, startY, startX - length, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_PURPLE ) );

							tran_drawCircuitEffect ( startX - length, startY, length + ( BLOCK_WIDTH * 2 ), CIRCUIT_HEIGHT, RIGHT_SIDE );
						}
					else
						{
							al_draw_filled_rectangle ( startX, startY, startX - length, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_BLACK ) );
						}
				}
			else if ( false == transferCells[whichCell].powerOnRight )
				al_draw_filled_rectangle ( startX, startY, startX - length, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_BLACK ) );
			else
				{
					al_draw_filled_rectangle ( startX, startY, startX - length, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_BLACK ) );
					startX -= BLOCK_WIDTH * 2;
					length -= BLOCK_WIDTH * 2;
					al_draw_filled_rectangle ( startX, startY, startX - length, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_PURPLE ) );
					tran_drawCircuitBlock ( BLOCK_LEFT, whichCell, startX, COL_PURPLE );

					tran_drawCircuitEffect ( startX - length, startY, length, CIRCUIT_HEIGHT, RIGHT_SIDE );
				}

			if ( true == drawBlock )
				{
					if ( type != CIRCUIT_NORMAL )
						tran_drawCircuitBlock ( BLOCK_RIGHT, whichCell, startX - length, COL_PURPLE );
				}
		}
}

//------------------------------------------------------------
//
// Draw a circuit block
// posX is the location along the whichCell circuit
void tran_drawCircuitBlock ( int blockDirection, int whichCell, int posX, float red, float green, float blue, float alpha )
//------------------------------------------------------------
{
	int startX, startY;

	if ( whichCell == TOKEN_POS_HIDE )
		return;

	if ( BLOCK_RIGHT == blockDirection )
		{
			startX = posX;
			if ( -1 == whichCell )
				{
					startY = transferCells[0].startY + ( ( squareHeight - BLOCK_HEIGHT ) / 2 );
					startY -= squareHeight;
					startY -= BLOCK_HEIGHT / 4;
				}
			else
				startY = transferCells[whichCell].startY + ( ( squareHeight - BLOCK_HEIGHT ) / 2 );

			al_draw_filled_rectangle ( startX, startY, startX + BLOCK_WIDTH, startY + BLOCK_HEIGHT, al_map_rgba_f ( red, green, blue, alpha ) );

			al_draw_filled_rectangle ( startX + BLOCK_WIDTH, startY + BLOCK_STEP, startX + ( BLOCK_WIDTH * 2 ), startY + ( BLOCK_HEIGHT - BLOCK_STEP ), al_map_rgba_f ( red, green, blue, alpha ) );

			al_draw_rectangle ( startX, startY, startX + BLOCK_WIDTH, startY + BLOCK_HEIGHT, al_map_rgba_f ( COL_BLACK ), 2.0f );

			al_draw_rectangle ( startX + BLOCK_WIDTH, startY + BLOCK_STEP, startX + ( BLOCK_WIDTH * 2 ), startY + ( BLOCK_HEIGHT - BLOCK_STEP ), al_map_rgba_f ( COL_BLACK ), 2.0f );
		}


	if ( BLOCK_LEFT == blockDirection )
		{
			startX = posX;
			if ( -1 == whichCell )
				{
					startY = transferCells[0].startY + ( ( squareHeight - BLOCK_HEIGHT ) / 2 );
					startY -= squareHeight;
					startY -= BLOCK_HEIGHT / 4;
				}
			else
				startY = transferCells[whichCell].startY + ( ( squareHeight - BLOCK_HEIGHT ) / 2 );

			al_draw_filled_rectangle ( startX, startY, startX - BLOCK_WIDTH, startY + BLOCK_HEIGHT, al_map_rgba_f ( red, green, blue, alpha ) );

			al_draw_filled_rectangle ( startX - BLOCK_WIDTH, startY + BLOCK_STEP, startX - ( BLOCK_WIDTH * 2 ), startY + ( BLOCK_HEIGHT - BLOCK_STEP ), al_map_rgba_f ( red, green, blue, alpha ) );

			al_draw_rectangle ( startX, startY, startX - BLOCK_WIDTH, startY + BLOCK_HEIGHT, al_map_rgba_f ( COL_BLACK ), 2.0f );

			al_draw_rectangle ( startX - BLOCK_WIDTH, startY + BLOCK_STEP, startX - ( BLOCK_WIDTH * 2 ), startY + ( BLOCK_HEIGHT - BLOCK_STEP ), al_map_rgba_f ( COL_BLACK ), 2.0f );
		}
}

//------------------------------------------------------------
//
// Draw a two into one circuitTypeLeft - Do repeater as well ???
void tran_drawCircuitTwoIntoOne ( int whichSide, int whichCell )
//------------------------------------------------------------
{
	int startXMiddle, startYMiddle, length;

	length = transferCells[whichCell].startX - ( BACKGROUND_BORDER + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH );
	length += ( BLOCK_WIDTH * 2 );
	length = length * 0.25;

	if ( LEFT_SIDE == whichSide )
		{
			startXMiddle = ( transferCells[whichCell].startX - length ) - ( BLOCK_WIDTH * 2 );
			startYMiddle = transferCells[whichCell].startY + ( ( squareHeight - CIRCUIT_HEIGHT ) / 2 );
//
// Draw the circuit

			if ( ( true == transferCells[whichCell - 1].powerOnLeft ) && ( true == transferCells[whichCell + 1].powerOnLeft ) )
				{
					al_draw_filled_rectangle ( startXMiddle, startYMiddle, transferCells[whichCell].startX - ( BLOCK_WIDTH * 2 ), startYMiddle + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_YELLOW ) );
					tran_drawCircuitEffect ( startXMiddle, startYMiddle, length, CIRCUIT_HEIGHT, LEFT_SIDE );
				}
			else
				al_draw_filled_rectangle ( startXMiddle, startYMiddle, transferCells[whichCell].startX - ( BLOCK_WIDTH * 2 ), startYMiddle + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_BLACK ) );

			tran_drawLongBlock ( LEFT_SIDE, whichCell, startXMiddle );
		}

	if ( RIGHT_SIDE == whichSide )
		{
			startXMiddle = transferCells[whichCell].startX + squareWidth + ( BLOCK_WIDTH * 2 );
			startYMiddle = transferCells[whichCell].startY + ( ( squareHeight - CIRCUIT_HEIGHT ) / 2 );
//
// Draw the circuit

			if ( ( true == transferCells[whichCell - 1].powerOnRight ) && ( true == transferCells[whichCell + 1].powerOnRight ) )
				{
					al_draw_filled_rectangle ( startXMiddle, startYMiddle, startXMiddle + length, startYMiddle + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_PURPLE ) );
					tran_drawCircuitEffect ( startXMiddle, startYMiddle, length, CIRCUIT_HEIGHT, RIGHT_SIDE );
				}
			else
				al_draw_filled_rectangle ( startXMiddle, startYMiddle, startXMiddle + length, startYMiddle + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_BLACK ) );

			tran_drawLongBlock ( RIGHT_SIDE, whichCell, startXMiddle + length );
		}
}

//------------------------------------------------------------
//
// Draw a one into two circuit
void tran_drawCircuitOneIntoTwo ( int whichSide, int whichCell )
//------------------------------------------------------------
{

	int length; //, startX, startY;

	length = transferCells[whichCell].startX - ( BACKGROUND_BORDER + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH );
	length += ( BLOCK_WIDTH * 2 );
	length = length * 0.25;

	if ( LEFT_SIDE == whichSide )
		{
			tran_drawCircuitNormal ( whichSide, whichCell, CIRCUIT_ONE_INTO_TWO_SHORT, false );
		}

	if ( RIGHT_SIDE == whichSide )
		{
			tran_drawCircuitNormal ( whichSide, whichCell, CIRCUIT_ONE_INTO_TWO_SHORT, false );
		}
}


//------------------------------------------------------------
//
// Draw the repeat or reverse block
void tran_drawEffect ( int whichSide, int whichCell, int type )
//------------------------------------------------------------
{
	int posX, startX, startY;

	if ( type == CIRCUIT_REPEATER )
		{
			if ( LEFT_SIDE == whichSide )
				{
					startX = BACKGROUND_BORDER + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH;
					posX = ( transferCells[whichCell].startX - startX ) - ( BLOCK_WIDTH * 2 );
					posX = posX * 0.75;
					tran_drawCircuitBlock ( BLOCK_RIGHT, whichCell, posX, COL_YELLOW );
					if ( true == transferCells[whichCell].repeaterLeftActivated )
						{
							startY = transferCells[whichCell].startY + ( ( squareHeight - CIRCUIT_HEIGHT ) / 2 );
							al_draw_filled_rectangle ( posX + ( BLOCK_WIDTH * 2 ), startY, ( posX + ( transferCells[whichCell].startX - posX ) - BLOCK_WIDTH * 2 ), startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_YELLOW ) );
							tran_drawCircuitEffect ( posX, startY, ( ( transferCells[whichCell].startX - posX ) - BLOCK_WIDTH * 2 ), CIRCUIT_HEIGHT, LEFT_SIDE );
						}
				}
			if ( RIGHT_SIDE == whichSide )
				{
					startX = ( backGroundWidth - ( BACKGROUND_BORDER * 2 ) ) - SIDEBAR_WIDTH;
					posX = ( startX - ( transferCells[whichCell].startX + squareWidth ) ) - SIDEBAR_WIDTH;
					posX = posX * 0.75;
					tran_drawCircuitBlock ( BLOCK_LEFT, whichCell, startX - posX, COL_PURPLE );
					if ( true == transferCells[whichCell].repeaterRightActivated )
						{
							startY = transferCells[whichCell].startY + ( ( squareHeight - CIRCUIT_HEIGHT ) / 2 );
							al_draw_filled_rectangle ( transferCells[whichCell].startX + squareWidth + ( BLOCK_WIDTH * 2 ), startY, ( startX - posX ) - ( BLOCK_WIDTH * 2 ), startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_PURPLE ) );
							tran_drawCircuitEffect ( transferCells[whichCell].startX + squareWidth + ( BLOCK_WIDTH * 2 ), startY,
							                         ( ( startX - posX ) - ( BLOCK_WIDTH * 2 ) ) - ( transferCells[whichCell].startX + squareWidth ), CIRCUIT_HEIGHT, RIGHT_SIDE );

						}
				}
		}

	if ( type == CIRCUIT_REVERSED )
		{
			if ( LEFT_SIDE == whichSide )
				{
					startX = BACKGROUND_BORDER + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH;
					posX = ( transferCells[whichCell].startX - startX ) - ( BLOCK_WIDTH * 2 );
					posX = posX * 0.75;
					tran_drawCircuitBlock ( BLOCK_RIGHT, whichCell, posX, COL_PURPLE );

					if ( true == transferCells[whichCell].reversedLeftActivated )
						{
							startY = transferCells[whichCell].startY + ( ( squareHeight - CIRCUIT_HEIGHT ) / 2 );
							al_draw_filled_rectangle ( posX + ( BLOCK_WIDTH * 2 ), startY, ( posX + ( transferCells[whichCell].startX - posX ) - BLOCK_WIDTH * 2 ), startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_PURPLE ) );
							tran_drawCircuitEffect ( posX, startY, ( ( transferCells[whichCell].startX - posX ) - BLOCK_WIDTH * 2 ), CIRCUIT_HEIGHT, LEFT_SIDE );
						}
				}
			if ( RIGHT_SIDE == whichSide )
				{
					startX = ( backGroundWidth - ( BACKGROUND_BORDER * 2 ) ) - SIDEBAR_WIDTH;
					posX = ( startX - ( transferCells[whichCell].startX + squareWidth ) ) - SIDEBAR_WIDTH;
					posX = posX * 0.75;
					tran_drawCircuitBlock ( BLOCK_LEFT, whichCell, startX - posX, COL_YELLOW );

					if ( true == transferCells[whichCell].reversedRightActivated )
						{
							startY = transferCells[whichCell].startY + ( ( squareHeight - CIRCUIT_HEIGHT ) / 2 );
							al_draw_filled_rectangle ( transferCells[whichCell].startX + squareWidth + ( BLOCK_WIDTH * 2 ), startY, ( startX - posX ) - ( BLOCK_WIDTH * 2 ), startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_YELLOW ) );
							tran_drawCircuitEffect ( transferCells[whichCell].startX + squareWidth + ( BLOCK_WIDTH * 2 ), startY,
							                         ( ( startX - posX ) - ( BLOCK_WIDTH * 2 ) ) - ( transferCells[whichCell].startX + squareWidth ), CIRCUIT_HEIGHT, RIGHT_SIDE );
						}
				}
		}
}

//------------------------------------------------------------
//
// Draw the token launch circuits
void tran_drawTokenLaunchers()
//------------------------------------------------------------
{
	int leftStartX, startY, rightStartX;

	float length;

	leftStartX = BACKGROUND_BORDER + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH;
	startY = transferCells[0].startY - ( squareHeight - CIRCUIT_HEIGHT );
	length = ( transferCells[1].startX - leftStartX ) - ( BLOCK_WIDTH * 2 );

	length = length * 0.10f;

	rightStartX = ( backGroundWidth - ( BACKGROUND_BORDER * 2 ) ) - SIDEBAR_WIDTH;

//
// Draw the circuit
	al_draw_filled_rectangle ( leftStartX, startY, leftStartX + length, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_BLACK ) );
	al_draw_filled_rectangle ( rightStartX, startY, rightStartX - length, startY + CIRCUIT_HEIGHT, al_map_rgba_f ( COL_BLACK ) );

	if ( LEFT_SIDE == playerOnSide )
		{
			if ( playerBlockPos != TOKEN_POS_HIDE )
				if ( playerBlockPos == TOKEN_POS_LAUNCH )
					tran_drawCircuitBlock ( BLOCK_RIGHT, TOKEN_POS_LAUNCH, leftStartX + length, COL_YELLOW );

			if ( droidBlockPos != TOKEN_POS_HIDE )
				if ( droidBlockPos == TOKEN_POS_LAUNCH )
					tran_drawCircuitBlock ( BLOCK_LEFT, TOKEN_POS_LAUNCH,  rightStartX - ( length - 1 ), COL_PURPLE );
		}
	else
		{
			if ( playerBlockPos != TOKEN_POS_HIDE )
				if ( playerBlockPos == TOKEN_POS_LAUNCH )
					tran_drawCircuitBlock ( BLOCK_LEFT, TOKEN_POS_LAUNCH, rightStartX - ( length - 1 ), COL_PURPLE );

			if ( droidBlockPos != TOKEN_POS_HIDE )
				if ( droidBlockPos == TOKEN_POS_LAUNCH )
					tran_drawCircuitBlock ( BLOCK_RIGHT, TOKEN_POS_LAUNCH,  leftStartX + length, COL_YELLOW );

		}
}


//------------------------------------------------------------
//
// Draw the tokens for each side
void tran_drawTokens()
//------------------------------------------------------------
{
	int leftStartX;
	int rightStartX;

	int spaceBetweenTokens = 15;

	int leftTokenCount;
	int rightTokenCount;

	leftStartX = ( backGroundStartX + ( BACKGROUND_BORDER * 2 ) ) - spaceBetweenTokens;

	rightStartX = backGroundWidth - ( BACKGROUND_BORDER * 2 ) + spaceBetweenTokens;

	if ( LEFT_SIDE == playerOnSide )	// Yellow
		{
			leftTokenCount = numPlayerTokens;
			rightTokenCount = numDroidTokens;

			if ( numDroidTokens > -1 )
				tran_drawCircuitBlock ( BLOCK_LEFT, droidBlockPos, ( backGroundWidth - ( BACKGROUND_BORDER * 2 ) ) - SIDEBAR_WIDTH, COL_PURPLE );

			if ( ( playerBlockPos > TOKEN_POS_LAUNCH ) && ( numPlayerTokens > -1 ) )
				tran_drawCircuitBlock ( BLOCK_RIGHT, playerBlockPos, BACKGROUND_BORDER + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH, COL_YELLOW );
		}
	else	// Purple
		{
			leftTokenCount = numDroidTokens;
			rightTokenCount = numPlayerTokens;

			if ( numDroidTokens > -1 )
				tran_drawCircuitBlock ( BLOCK_RIGHT, droidBlockPos, BACKGROUND_BORDER + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH, COL_YELLOW );

			if ( ( playerBlockPos > TOKEN_POS_LAUNCH ) && ( numPlayerTokens > -1 ) )
				tran_drawCircuitBlock ( BLOCK_LEFT, playerBlockPos, ( backGroundWidth - ( BACKGROUND_BORDER * 2 ) ) - SIDEBAR_WIDTH, COL_PURPLE );
		}

	if ( leftTokenCount > 0 )
		{
			for ( int i = 0; i != leftTokenCount; i++ )
				{
					tran_drawCircuitBlock ( BLOCK_RIGHT, i, leftStartX, COL_YELLOW );
				}
		}

	if ( rightTokenCount > 0 )
		{
			for ( int i = 0; i != rightTokenCount; i++ )
				{
					tran_drawCircuitBlock ( BLOCK_LEFT, i, rightStartX, COL_PURPLE );
				}
		}
}

//------------------------------------------------------------
//
// Draw the transfer graphics
void tran_drawTransferCells ( bool justBackground )
//------------------------------------------------------------
{
	int i;
	ALLEGRO_COLOR	backGroundColor;

	backGroundStartX = BACKGROUND_BORDER;
	backGroundWidth = winWidth - ( BACKGROUND_BORDER * 2 );
	backGroundStartY = al_get_bitmap_height ( image[hud].image ) + 30; //transferCells[0].startY - (3 * squareHeight) - BACKGROUND_START_OFFSET;
	backGroundHeight = winHeight - BACKGROUND_START_OFFSET;

//
// Draw the colored background
	switch ( currentMode )
		{
		case MODE_TRANSFER_INTRO:
		case MODE_TRANSFER_INTRO_1:
		case MODE_TRANSFER_INTRO_2:
			backGroundColor = al_map_rgba_f ( COL_BLACK );
			break;

		case MODE_TRANSFER_START:
		case MODE_TRANSFER_SELECT_SIDE:  // Countdown to choose side
		case MODE_TRANSFER_SELECT:
		case MODE_TRANSFER_COPY:
		case MODE_TRANSFER_FINISH:
			backGroundColor = al_map_rgba_f ( 0.53f, 0.25f, 0.211f, 0.0f );
			break;
		}

	al_draw_filled_rectangle ( backGroundStartX, backGroundStartY, backGroundWidth, backGroundHeight, backGroundColor );

	if ( true == justBackground )
		return;
//
// Draw the colored cells
	for ( i = 0; i != NUMBER_CELLS; i++ )
		{
			al_draw_filled_rectangle ( transferCells[i].startX, transferCells[i].startY,
			                           transferCells[i].startX + squareWidth, transferCells[i].startY + squareHeight, transferCells[i].color );
		}

//
// draw the blocks on the middle bar
	for ( i = 0; i != NUMBER_CELLS; i++ )
		{
			tran_drawCircuitBlock ( BLOCK_LEFT, i, transferCells[i].startX, COL_YELLOW );
			tran_drawCircuitBlock ( BLOCK_RIGHT, i, transferCells[i].startX + squareWidth, COL_PURPLE );
		}


//
// Draw outlines
	for ( i = 0; i != NUMBER_CELLS; i++ )
		{
			al_draw_rectangle ( transferCells[i].startX, transferCells[i].startY,
			                    transferCells[i].startX + squareWidth, transferCells[i].startY + squareHeight,
			                    al_map_rgba_f ( COL_BLACK ), 2.0f );
		}

//
// draw current winning color outline
	al_draw_rectangle ( transferCells[0].startX, transferCells[0].startY - ( 2 * squareHeight ),
	                    transferCells[0].startX + squareWidth, transferCells[0].startY,
	                    al_map_rgba_f ( COL_BLACK ), 2.0f );

//
// draw current winning color
	al_draw_filled_rectangle ( transferCells[0].startX, transferCells[0].startY - ( 2 * squareHeight ),
	                           transferCells[0].startX + squareWidth, transferCells[0].startY,
	                           currentWinner );

//
// draw sidebar left
	al_draw_filled_rectangle ( backGroundStartX + ( BACKGROUND_BORDER * 2 ), transferCells[0].startY - ( 2 * squareHeight ),
	                           backGroundStartX + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH, ( transferCells[0].startY + squareHeight * ( NUMBER_CELLS + 2 ) ) - ( 2 * squareHeight ),
	                           al_map_rgba_f ( COL_YELLOW ) );

//
// draw sidebar left outlines
	al_draw_rectangle ( backGroundStartX + ( BACKGROUND_BORDER * 2 ), transferCells[0].startY - ( 2 * squareHeight ),
	                    backGroundStartX + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH, ( transferCells[0].startY + squareHeight * ( NUMBER_CELLS + 2 ) ) - ( 2 * squareHeight ),
	                    al_map_rgba_f ( COL_BLACK ), 2.0f );

//
// draw sidebar right
	al_draw_filled_rectangle ( backGroundWidth - ( BACKGROUND_BORDER * 2 ), transferCells[0].startY - ( 2 * squareHeight ),
	                           backGroundWidth - ( BACKGROUND_BORDER * 2 ) - SIDEBAR_WIDTH, ( transferCells[0].startY + squareHeight * ( NUMBER_CELLS + 2 ) ) - ( 2 * squareHeight ),
	                           al_map_rgba_f ( COL_PURPLE ) );
//
// draw sidebar right outline
	al_draw_rectangle ( backGroundWidth - ( BACKGROUND_BORDER * 2 ), transferCells[0].startY - ( 2 * squareHeight ),
	                    backGroundWidth - ( BACKGROUND_BORDER * 2 ) - SIDEBAR_WIDTH, ( transferCells[0].startY + squareHeight * ( NUMBER_CELLS + 2 ) ) - ( 2 * squareHeight ),
	                    al_map_rgba_f ( COL_BLACK ), 2.0f );


	//
	// Draw the circuits
	for ( i = 0; i != NUMBER_CELLS; i++ )
		{
			switch ( transferCells[i].circuitTypeLeft )
				{
				case CIRCUIT_NORMAL:
				case CIRCUIT_NORMAL_1:
				case CIRCUIT_NORMAL_2:
					tran_drawCircuitNormal ( LEFT_SIDE, i, CIRCUIT_NORMAL, true );
					break;
				case CIRCUIT_THREEQUARTERS:
					tran_drawCircuitNormal ( LEFT_SIDE, i, CIRCUIT_THREEQUARTERS, true );
					break;
				case CIRCUIT_HALF:
					tran_drawCircuitNormal ( LEFT_SIDE, i, CIRCUIT_HALF, true );
					break;
				case CIRCUIT_QUARTER:
					tran_drawCircuitNormal ( LEFT_SIDE, i, CIRCUIT_QUARTER, true );
					break;
				case CIRCUIT_SPLIT_HALF:
					tran_drawCircuitNormal ( LEFT_SIDE, i, CIRCUIT_ONE_INTO_TWO, false );
					break;
				case CIRCUIT_SPLIT_TWO_INTO_ONE:
					tran_drawCircuitNormal ( LEFT_SIDE, i, CIRCUIT_THREEQUARTERS, false );
					break;
				case CIRCUIT_TWO_INTO_ONE:
					tran_drawCircuitTwoIntoOne ( LEFT_SIDE, i );
					break;
				case CIRCUIT_ONE_INTO_TWO:
					tran_drawCircuitOneIntoTwo ( LEFT_SIDE, i );
					break;
				}

			if ( true == transferCells[i].isRepeaterLeft )
				tran_drawEffect ( LEFT_SIDE, i, CIRCUIT_REPEATER );

			if ( true == transferCells[i].isReversedLeft )
				tran_drawEffect ( LEFT_SIDE, i, CIRCUIT_REVERSED );

			switch ( transferCells[i].circuitTypeRight )
				{
				case CIRCUIT_NORMAL:
				case CIRCUIT_NORMAL_1:
				case CIRCUIT_NORMAL_2:
					tran_drawCircuitNormal ( RIGHT_SIDE, i, CIRCUIT_NORMAL, true );
					break;
				case CIRCUIT_THREEQUARTERS:
					tran_drawCircuitNormal ( RIGHT_SIDE, i, CIRCUIT_THREEQUARTERS, true );
					break;
				case CIRCUIT_HALF:
					tran_drawCircuitNormal ( RIGHT_SIDE, i, CIRCUIT_HALF, true );
					break;
				case CIRCUIT_QUARTER:
					tran_drawCircuitNormal ( RIGHT_SIDE, i, CIRCUIT_QUARTER, true );
					break;
				case CIRCUIT_SPLIT_HALF:
					tran_drawCircuitNormal ( RIGHT_SIDE, i, CIRCUIT_ONE_INTO_TWO, false );
					break;
				case CIRCUIT_SPLIT_TWO_INTO_ONE:
					tran_drawCircuitNormal ( RIGHT_SIDE, i, CIRCUIT_THREEQUARTERS, false );
					break;
				case CIRCUIT_TWO_INTO_ONE:
					tran_drawCircuitTwoIntoOne ( RIGHT_SIDE, i );
					break;
				case CIRCUIT_ONE_INTO_TWO:
					tran_drawCircuitOneIntoTwo ( RIGHT_SIDE, i );
					break;
				}

			if ( true == transferCells[i].isRepeaterRight )
				tran_drawEffect ( RIGHT_SIDE, i, CIRCUIT_REPEATER );

			if ( true == transferCells[i].isReversedRight )
				tran_drawEffect ( RIGHT_SIDE, i, CIRCUIT_REVERSED );
		}

	tran_drawTokenLaunchers();

	tran_drawTokens();

	cpVect drawPos;

	if ( LEFT_SIDE == playerOnSide )
		{
			drawPos.x = ( transferCells[0].startX - backGroundStartX + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH ) / 2;
			drawPos.y =  backGroundStartY + 20;

			if ( sprite_001 == playerDroidTypeImage )
				gam_drawSprite ( 0, playerDroidTypeImage, drawPos, -1, al_map_rgba_f ( 1.0f, 1.0f, 1.0f, 1.0f ) );
			else
				gam_drawSprite ( 0, playerDroidTypeImage, drawPos, -1, al_map_rgba_f ( 1.0f, 1.0f, 1.0f, 0.0f ) );
			//
			// Draw enemy droid sprite
			drawPos.x = ( ( backGroundWidth - ( BACKGROUND_BORDER * 2 ) - ( transferCells[0].startX + squareWidth ) ) / 2 ) + transferCells[0].startX;
			drawPos.y = backGroundStartY + 20;
			gam_drawSprite ( 0, droidTypeToTransferInto + 2, drawPos, -1, al_map_rgba_f ( 0.0f, 0.0f, 0.0f,1.0f ) );
		}
	else
		{
			//
			// Draw current player sprite
			drawPos.x = ( ( backGroundWidth - ( BACKGROUND_BORDER * 2 ) - ( transferCells[0].startX + squareWidth ) ) / 2 ) + transferCells[0].startX;
			drawPos.y = backGroundStartY + 20;

			if ( sprite_001 == playerDroidTypeImage )
				gam_drawSprite ( 0, playerDroidTypeImage, drawPos, -1, al_map_rgba_f ( 1.0f, 1.0f, 1.0f, 1.0f ) );
			else
				gam_drawSprite ( 0, playerDroidTypeImage, drawPos, -1, al_map_rgba_f ( 1.0f, 1.0f, 1.0f, 0.0f ) );
			//
			// Draw enemy droid sprite
			drawPos.x = ( transferCells[0].startX - backGroundStartX + ( BACKGROUND_BORDER * 2 ) + SIDEBAR_WIDTH ) / 2;
			drawPos.y =  backGroundStartY + 20;
			gam_drawSprite ( 0, droidTypeToTransferInto + 2, drawPos, -1, al_map_rgba_f ( 0.0f, 0.0f, 0.0f,1.0f ) );
		}
}
