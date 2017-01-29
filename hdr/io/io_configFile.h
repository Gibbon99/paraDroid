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
