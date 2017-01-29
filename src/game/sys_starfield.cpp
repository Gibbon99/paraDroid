#include "../../hdr/sys_globals.h"

ALLEGRO_VERTEX		*stars = NULL;
int					numStars;
bool				useStarfield;

#define randomX ((rand() % winWidth))
#define randomY ((rand() % winHeight))
#define randomSpeed (((rand() % 5) + 1) * 10)

// ----------------------------------------------------------------------------
//
// Free memory used by starfield
void gam_freeStarField()
// ----------------------------------------------------------------------------
{
	if (NULL != stars)
		free(stars);
}

// ----------------------------------------------------------------------------
//
// Get some memory to hold the starfield data
//
// .u field is used for speed - z needs to be 0
bool gam_setupStarField()
// ----------------------------------------------------------------------------
{
float pickSpeed;

	if (NULL != stars)
		free(stars);
	//
	// need to allocate the memory needed for the starfield
	//
	stars = (ALLEGRO_VERTEX *) malloc (numStars * sizeof(ALLEGRO_VERTEX));
	if (NULL == stars)
	{
		useStarfield = false;
		io_logToFile("Error: Couldn't get memory for starfield. Disabled.");
		return false;
	}
	for (int i = 0; i != numStars; i++)
	{
		stars[i].x = randomX;
		stars[i].y = randomY;
		stars[i].z = 0;	// 2D only

		pickSpeed = randomSpeed;
		if (pickSpeed < 60)
		{
			stars[i].color.r = 1.0f; stars[i].color.g = 1.0f; stars[i].color.b = 1.0f;
			stars[i].u = 100.0f;
		}

		if (pickSpeed < 50)
		{
			stars[i].color.r = 0.8f; stars[i].color.g = 0.8f; stars[i].color.b = 0.8f;
			stars[i].u = 80.0f;
		}

		if (pickSpeed < 40)
		{
			stars[i].color.r = 0.6f; stars[i].color.g = 0.6f; stars[i].color.b = 0.6f;
			stars[i].u = 65.0f;
		}

		if (pickSpeed < 30)
		{
			stars[i].color.r = 0.5f; stars[i].color.g = 0.5f; stars[i].color.b = 0.5f;
			stars[i].u = 50.0f;
		}

		if (pickSpeed < 20)
		{
			stars[i].color.r = 0.4f; stars[i].color.g = 0.4f; stars[i].color.b = 0.4f;
			stars[i].u = 30.0f;
		}

		if (pickSpeed < 10)
		{
			stars[i].color.r = 0.3f; stars[i].color.g = 0.3f; stars[i].color.b = 0.3f;
			stars[i].u = 10.0f;
		}
	}
	return true;
}

// ----------------------------------------------------------------------------
//
// Animate movement of starfield
void gam_animateStarField(float interpolate)
// ----------------------------------------------------------------------------
{
	for (int i = 0; i != numStars; i++)
	{
		stars[i].x -= stars[i].u * interpolate;
		if (stars[i].x < 0)
		{
			stars[i].x = winWidth;
			stars[i].y = randomY;
		}
	}
}
