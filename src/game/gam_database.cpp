#include "../../hdr/sys_globals.h"

_dataBaseEntry 		dataBaseEntry[NUM_SPRITE_TYPES];

//------------------------------------------------------------
//
// Get and build up information for droid database files
bool gam_getDBInformation()
//------------------------------------------------------------
{
    con_print(true, false, "Get droid database information.");
// TODO (dberry#1#): Error check return value
    io_getDBDroidInfo(0, "001.txt");
    io_getDBDroidInfo(1, "123.txt");
    io_getDBDroidInfo(2, "139.txt");
    io_getDBDroidInfo(3, "247.txt");
    io_getDBDroidInfo(4, "249.txt");
    io_getDBDroidInfo(5, "296.txt");
    io_getDBDroidInfo(6, "302.txt");
    io_getDBDroidInfo(7, "329.txt");
    io_getDBDroidInfo(8, "420.txt");
    io_getDBDroidInfo(9, "476.txt");
    io_getDBDroidInfo(10, "493.txt");
    io_getDBDroidInfo(11, "516.txt");
    io_getDBDroidInfo(12, "571.txt");
    io_getDBDroidInfo(13, "598.txt");
    io_getDBDroidInfo(14, "614.txt");
    io_getDBDroidInfo(15, "615.txt");
    io_getDBDroidInfo(16, "629.txt");
    io_getDBDroidInfo(17, "711.txt");
    io_getDBDroidInfo(18, "742.txt");
    io_getDBDroidInfo(19, "751.txt");
    io_getDBDroidInfo(20, "821.txt");
    io_getDBDroidInfo(21, "834.txt");
    io_getDBDroidInfo(22, "883.txt");
    io_getDBDroidInfo(23, "999.txt");

	loadingBarPercent = 0.0f;
    return true;
}
