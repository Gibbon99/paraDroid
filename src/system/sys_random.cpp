#include "../../hdr/sys_globals.h"

//------------------------------------------------------------
//
// Generate random number

unsigned long sys_genRand_int32(void)
//------------------------------------------------------------
{
	return rand();
}

//------------------------------------------------------------
//
// Init the random number routines

void sys_initRandomNum()
//------------------------------------------------------------
{
	srand (time(NULL));
}