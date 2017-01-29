#include "hdr/sys_globals.h"

ALLEGRO_DISPLAY      		*display = NULL;
ALLEGRO_FONT         		*builtInFont = NULL;

ALLEGRO_KEYBOARD_STATE  	*keyboardState = NULL;
ALLEGRO_EVENT_QUEUE  		*eventQueue = NULL;
ALLEGRO_EVENT        		event;
ALLEGRO_BITMAP				*bitmapFBO;

string				gameVersion;
float				baseGameSpeed;

float				progressBarLoadTimes;
bool 				gameIsRunning = true;
bool				useHoldBitmap = true;
bool				useBackingBitmap = true;

bool				useCollisionDetection = false;

double				next_game_tick;
int					loops;
float				interpolation;

int					winWidth;
int					winHeight;
int					fullScreenValue;

int 				tileCollision = false;

//-----------------------------------------------------------------------------
//
// Start
int main (int argc, char **argv)
//-----------------------------------------------------------------------------
	{
	double singleTimeValue;

	gameVersion = "v0.8";

	if (false == sys_startOnce() )
			{
			exit (-1);
			}

	renderTick1 = 0.0f;
	thinkTick1 = 0.0f;

	next_game_tick = al_get_time();

	while ( true == gameIsRunning )
			{
			loops = 0;
			singleTimeValue = al_get_time();

			while (singleTimeValue > next_game_tick && loops < MAX_FRAMESKIP)
					{
					sys_updateFrame();
					next_game_tick += SKIP_TICKS;
					loops++;
					sys_CalculateThinkFrameRate (singleTimeValue);
					}

			io_readKeyboard();	// Best in 30fps loop - or as fast as possible??

			interpolation = float (singleTimeValue + SKIP_TICKS - next_game_tick) / float (SKIP_TICKS);

			sys_displayFrame ( interpolation );

			sys_CalculateFrameRate (singleTimeValue);
			}

	sys_shutDown();

	return 0;
	}
