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

#pragma once

// Open a config file from the script
void io_loadConfigFile(string fileName);

// Close config file
void io_closeConfigFile();

// Save the current configuration to the config file
void io_saveConfigFile(string fileName);

// Read config values
int io_readConfigValuesInt(string varName);

// Read config value - string
string io_readConfigValuesString(string varName);

// Read config values
float io_readConfigValuesFloat (string varName);

// Set a config value to be saved
void io_saveConfigValueString(string varName, string varValue);

// Set a config value to be saved
void io_saveConfigValueFloat(string varName, float varValue);

// Set a config value to be saved
void io_saveConfigValueInt(string varName, int varValue);

// Get each field from the config file
bool io_getDBDroidInfo(int dbIndex, const char *fileName);
