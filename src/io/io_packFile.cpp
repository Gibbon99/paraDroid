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

bool fileSystemReady;

//--------------------------------------------------------
//
// Get archive types supported by this version
bool io_getArchivers(void)
//--------------------------------------------------------
{
    const PHYSFS_ArchiveInfo **rc = PHYSFS_supportedArchiveTypes();
    const PHYSFS_ArchiveInfo **i;

    io_logToFile("Packfile: Supported archive types.");

    if (*rc == NULL)
	{
        io_logToFile("Packfile: Error: No archive types found.");
		return false;
	}
    else
    {
        for (i = rc; *i != NULL; i++)
        {
            io_logToFile("Packfile: [ %s ] : [ %s ]",(*i)->extension, (*i)->description);
        }
    }
	return true;
}

// ---------------------------------------------------------------------------
//
// Start the packfile system
bool io_startFileSystem(bool showArchivers)
// ---------------------------------------------------------------------------
{
    PHYSFS_Version compiled;
    PHYSFS_Version linked;

    PHYSFS_VERSION(&compiled);
    PHYSFS_getLinkedVersion(&linked);

    io_logToFile((char *)"Compiled against PhysFS version %d.%d.%d.", compiled.major, compiled.minor, compiled.patch);
    io_logToFile((char *)"Linked against PhysFS version %d.%d.%d.",  linked.major, linked.minor, linked.patch);

    if (PHYSFS_init(NULL) == 0)
        {
            io_logToFile((char *)"Error: Filesystem failed to start - [ %s ]", PHYSFS_getLastError());
            fileSystemReady = false;
            return false;
        }

    //
    // Setup directory to write if needed
    if (0 == PHYSFS_setWriteDir ("data"))
    {
        io_logToFile("ERROR: Failed to set write path [ %s ]", PHYSFS_getLastError());
        fileSystemReady = false;
        return false;
    }
	/*
    //
    // Add directory for saving screen captures
    if (0 == PHYSFS_setWriteDir ("data//screenshots"))
    {
        io_logToFile("ERROR: Failed to set write path - screenshots [ %s ]", PHYSFS_getLastError());
        fileSystemReady = false;
        return false;
    }
	*/
    //
    // Set base directory
    if (0 == PHYSFS_addToSearchPath ("data",1))
    {
        io_logToFile("ERROR: Failed to set search path - data [ %s ]", PHYSFS_getLastError());
        fileSystemReady = false;
        return false;
    }
    //
    // Add directory for loading scripts - move to archive file
#ifdef _WIN32
    if (0 == PHYSFS_addToSearchPath ("data\\scripts",1))
#else
	if (0 == PHYSFS_addToSearchPath ("data//scripts", 1))
#endif
    {
        io_logToFile("ERROR: Failed to set search path - scripts [ %s ]", PHYSFS_getLastError());
        fileSystemReady = false;
        return false;
    }


	//
	// Add directory for files - Bug in windows doesn't allow use of zip data file
#ifdef _WIN32
	if (0 == PHYSFS_addToSearchPath ("data\\winData", 1))
	{
		io_logToFile ("ERROR: Failed to set search path - scripts [ %s ]", PHYSFS_getLastError ());
		fileSystemReady = false;
		return false;
	}
#endif

    //
    // Add archive file
#ifdef _WIN32
    if (0 == PHYSFS_addToSearchPath ("data\\data.zip",1))
#else
	if (0 == PHYSFS_addToSearchPath ("data//data.zip", 1))
#endif
    {
        io_logToFile("ERROR: Failed to set search path - data.zip - [ %s ]", PHYSFS_getLastError());
        fileSystemReady = false;
        return false;
    }
    //
    // What compression types are available
    if (true == showArchivers)
        io_getArchivers();

    fileSystemReady = true;

    return true;
}

