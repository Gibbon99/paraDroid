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

// ----------------------------------------------------------------------------
//
// load the sideview data from the external file
bool io_loadSideViewData(const char *sideviewFileName)
// ----------------------------------------------------------------------------
    {
    int numberLevels;
    int count;
    int buf[1];
    unsigned char levelCount[1];

    ALLEGRO_FILE		*fpSideView;
    uint				returnCode;

    fpSideView = al_fopen(sideviewFileName, "rb");
    if (fpSideView == NULL)
        return false;

    returnCode = al_fread(fpSideView, &levelCount, sizeof(unsigned char));
    if (returnCode != 1)
        {
        con_print(true, false, "ERROR: Loading sideView: Error: [ %s ]", "UNKNOWN");
        return false;
        }

    numberLevels = levelCount[0];

    if (MAX_LEVELS != numberLevels)
        {
        io_logToFile("ERROR: Read wrong data from sideview file.");
        sys_errorFatal(__FILE__, __LINE__, "Data corruption reading sideview file.");
        }
//  numberLevels = 29; //buf[0];

    for (count = 0; count != numberLevels; count++)
        {
        al_fread(fpSideView, (void *)&buf, sizeof(sideviewLevels[count].x1));
        sideviewLevels[count].x1 = buf[0];

        al_fread(fpSideView, (void *)&buf, sizeof(sideviewLevels[count].y1));
        sideviewLevels[count].y1 = buf[0] - 100;

        al_fread(fpSideView, (void *)&buf, sizeof(sideviewLevels[count].x2));
        sideviewLevels[count].x2 = buf[0];

        al_fread(fpSideView, (void *)&buf, sizeof(sideviewLevels[count].y2));
        sideviewLevels[count].y2 = buf[0] - 100;
        }
    al_fclose(fpSideView);

    gam_setupTunnels();

    return true;
    }

