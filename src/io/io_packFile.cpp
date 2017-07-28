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
bool useZlibWorkaround;

//--------------------------------------------------------
//
// Get archive types supported by this version
bool io_getArchivers ( void )
//--------------------------------------------------------
{
	const PHYSFS_ArchiveInfo **rc = PHYSFS_supportedArchiveTypes();
	const PHYSFS_ArchiveInfo **i;

	io_logToFile ( "Packfile: Supported archive types." );

	if ( *rc == NULL )
		{
			io_logToFile ( "Packfile: Error: No archive types found." );
			return false;
		}
	else
		{
			for ( i = rc; *i != NULL; i++ )
				{
					io_logToFile ( "Packfile: [ %s ] : [ %s ]", ( *i )->extension, ( *i )->description );
				}
		}
	return true;
}

// ---------------------------------------------------------------------------
//
// See if the packfile data file exists - if so, we can use the packfile method
// to open files - otherwise the game is shipped with the data files in a directory
bool is_usingPackfile ( string fileName )
// ---------------------------------------------------------------------------
{
	ALLEGRO_FILE	*packfile;

	packfile = al_fopen ( fileName.c_str(), "r" );
	if ( packfile == NULL )
		{
			// datafile not found - use directory method
			return true;
		}
	else
		{
			// zip archive file exists - use it - close open handle
			al_fclose(packfile);
			return false;
		}
}

// ---------------------------------------------------------------------------
//
// Start the packfile system
bool io_startFileSystem ( bool showArchivers )
// ---------------------------------------------------------------------------
{
	PHYSFS_Version compiled;
	PHYSFS_Version linked;

#ifdef _WIN32
	useZlibWorkaround = is_usingPackfile("data\\data.zip");
#else
	useZlibWorkaround = is_usingPackfile("data/data.zip");
#endif

	PHYSFS_VERSION ( &compiled );
	PHYSFS_getLinkedVersion ( &linked );

	io_logToFile ( ( char * ) "INFO: Compiled against PhysFS version %d.%d.%d.", compiled.major, compiled.minor, compiled.patch );
	io_logToFile ( ( char * ) "INFO: Linked against PhysFS version %d.%d.%d.",  linked.major, linked.minor, linked.patch );

	if ( PHYSFS_init ( NULL ) == 0 )
		{
			io_logToFile ( ( char * ) "Error: Filesystem failed to start - [ %s ]", PHYSFS_getLastError() );
			fileSystemReady = false;
			return false;
		}

	//
	// Setup directory to write if needed
	if ( 0 == PHYSFS_setWriteDir ( "data" ) )
		{
			io_logToFile ( "ERROR: Failed to set write path [ %s ]", PHYSFS_getLastError() );
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
	if ( 0 == PHYSFS_mount ("data", "/", 1))
//	if ( 0 == PHYSFS_addToSearchPath ( "data",0 ) )
		{
			io_logToFile ( "ERROR: Failed to set search path - data [ %s ]", PHYSFS_getLastError() );
			fileSystemReady = false;
			return false;
		}
	//
	// Add directory for loading scripts - move to archive file
#ifdef _WIN32
//	if ( 0 == PHYSFS_addToSearchPath ( "data\\scripts",1 ) )
	if ( 0 == PHYSFS_mount ( "data\\scripts", "/", 1 ) )
#else
//	if ( 0 == PHYSFS_addToSearchPath ( "data/scripts", 1 ) )
	if ( 0 == PHYSFS_mount ( "data/scripts", "/", 1 ) )		
#endif
		{
			io_logToFile ( "ERROR: Failed to set search path - scripts [ %s ]", PHYSFS_getLastError() );
			fileSystemReady = false;
			return false;
		}

//
// Work around the bug in zlib and physfs
//
	if ( true == useZlibWorkaround )
		{
			//
			// Add directory for files - Bug in zlib and physfs doesn't allow use of zip data file

			io_logToFile ( "INFO: Using file structure for data files." );

			if ( 0 == PHYSFS_mount ( "data/winData", "/", 0 ) )
				{
					io_logToFile ( "ERROR: Failed to set search path - winData [ %s ]", PHYSFS_getLastError () );
					fileSystemReady = false;
					return false;
				}

		}
	else
		{
			//
			// Add archive file

			io_logToFile ( "INFO: Using packfile for data files" );

#ifdef _WIN32
			if ( 0 == PHYSFS_mount ( "data\\data.zip", "/", 1 ) )
#else
			if ( 0 == PHYSFS_mount ( "data/data.zip", "/", 1 ) )
#endif
				{
					io_logToFile ( "ERROR: Failed to set search path - data.zip - [ %s ]", PHYSFS_getLastError() );
					fileSystemReady = false;
					return false;
				}
		}
	//
	// What compression types are available
	if ( true == showArchivers )
		io_getArchivers();

	fileSystemReady = true;
	char **i;
	//
	// Show paths set with physfs
	//
	for ( i = PHYSFS_getSearchPath(); *i != NULL; i++ )
		{
			io_logToFile ( "INFO: Data file path [ %s ]", *i );
		}
	return true;
}
