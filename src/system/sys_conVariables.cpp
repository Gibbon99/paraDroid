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
#include "../../hdr/system/sys_script.h"
#include <sstream>
#include <iostream>

#define VAR_TYPE_INT    0
#define VAR_TYPE_FLOAT  1
#define VAR_TYPE_BOOL   2

//-----------------------------------------------------------------------------
//
// Helper function to assign a num value to a var type string
int sys_conGetVarType(string whichType)
//-----------------------------------------------------------------------------
{
	if (0 == whichType.size())
		return VAR_TYPE_INT;

	if (whichType == "int")
		return VAR_TYPE_INT;

	if (whichType == "float")
		return VAR_TYPE_FLOAT;

	if (whichType == "bool")
		return VAR_TYPE_BOOL;

	return -1;
}

//-----------------------------------------------------------------------------
//
//! \brief List the variables we can change
//! \param Not required to pass anything in
bool sys_conListVariables()
//-----------------------------------------------------------------------------
{
	int 		count = 0;
	int 		varTypeNum = 0;

	int         *testVarInt;
	float       *testVarFloat;

	int         tempValueInt = 0;
	float       tempValueFloat = 0.0f;

	string 		varType;
	string 		varName;
	//
	// Get the command and any parameters
	//
	vector<string>     	tokens;   // one command and one param
	string              buffer;

	while (hostVariables[count].scriptFunctionName.size() > 1)
	{
		//
		// Start the string with known empty value
		varType = "";
		varName = "";

		// Insert the string into a stream
		istringstream iss(hostVariables[count].scriptFunctionName, istringstream::in);
		//
		// Put each word into the vector
		while(iss >> buffer)
		{
			tokens.push_back(buffer);
		}
		varType = tokens[0];
		varName = tokens[1];
		//
		// Clear vectors and stringstream
		//
		iss.clear();
		tokens.clear();

		varTypeNum = sys_conGetVarType(varType);

		switch (varTypeNum)
		{
			case VAR_TYPE_INT:
				testVarInt = hostVariables[count].hostFunctionPtr;
				tempValueInt = *testVarInt;
				con_print(true, true, "Variable - INT -  [ %s ] - [ %i ]", varName.c_str(), tempValueInt);
				break;

			case VAR_TYPE_FLOAT:
				testVarFloat = hostVariables[count].hostFunctionPtr;
				tempValueFloat = *testVarFloat;
				con_print(true, true, "Variable - FLOAT - [ %s ] - [ %2.2f ]", varName.c_str(), tempValueFloat);
				break;

			case VAR_TYPE_BOOL:
				testVarInt = hostVariables[count].hostFunctionPtr;
				tempValueInt =  *testVarInt;
				if (0 == tempValueInt)
				{
					con_print(true, true, "Variable - BOOL - [ %s ] - [ %s ]", varName.c_str(), "false");
				}
				else
				{
					con_print(true, true, "Variable - BOOL - [ %s ] - [ %s ]", varName.c_str(), "true");
				}
				break;
		}
		count++;
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//! \brief Get the value of a single variable
//! \param Pass in the name of the variable to query
bool sys_conGetVariable(string param1)
//-----------------------------------------------------------------------------
{
	int 	count = 0;
	int 	varTypeNum = 0;
	int 	tempValue = 0;

	int		tempValueInt = 0;
	float 	tempValueFloat = 0;

	int		*testVarInt;
	float	*testVarFloat;

	string 	varType;
	string 	varName;
	//
	// Get the command and any parameters
	//
	vector<string>     	tokens;   // one command and one param
	string              buffer;

	if (param1.size() == 0)
	{
		con_print (true, true, "Error: No variable specified - listing variables");
		sys_conListVariables();
		return true;
	}

	while (hostVariables[count].scriptFunctionName.size() > 1)
	{
		//
		// Start the string with known empty value
		varType = "";
		varName = "";

		// Insert the string into a stream
		istringstream iss(hostVariables[count].scriptFunctionName, istringstream::in);
		//
		// Put each word into the vector
		while(iss >> buffer)
		{
			tokens.push_back(buffer);
		}
		varType = tokens[0];
		varName = tokens[1];
		//
		// Clear vectors and stringstream
		//
		iss.clear();
		tokens.clear();

		if (param1 == varName)
		{
			varTypeNum = sys_conGetVarType(varType);
			switch (varTypeNum)
			{
				case VAR_TYPE_INT:
					testVarInt = hostVariables[count].hostFunctionPtr;
					tempValueInt = *testVarInt;
					con_print(true, true, "Variable [ %s ] - [ %i ]", varName.c_str(), tempValueInt);
					break;

				case VAR_TYPE_FLOAT:
					testVarFloat = hostVariables[count].hostFunctionPtr;
					tempValueFloat = *testVarFloat;
					con_print(true, true, "Variable [ %s ] - [ %2.2f ]", varName.c_str(), tempValueFloat);
					break;

				case VAR_TYPE_BOOL:
					testVarInt = hostVariables[count].hostFunctionPtr;
					tempValueInt = *testVarFloat;
					if (0 == tempValue)
					{
						con_print(true, true, "Variable [ %s ] - [ %s ]", varName.c_str(), "false");
					}
					else
					{
						con_print(true, true, "Variable [ %s ] - [ %s ]", varName.c_str(), "true");
					}
					break;
			}
			return true;
		}

		count++;
	}
	con_print (true, true, "Error: No matching value for [ %s ] found", param1);
	return true;
}

//-----------------------------------------------------------------------------
//
//! \brief Set the value of a single variable
//! \param Pass in the name and the new value
bool sys_conSetVariable(string param1, string param2)
//-----------------------------------------------------------------------------
{
	int 	count = 0;
	string 	varType;
	string 	varName;

	int 	*testVarInt;
	float	*testVarFloat;

	int varTypeNum;
	int tempValue;

	vector<string>     	tokens;   // one command and one param
	string              buffer;

	if (param1.size() == 0)
	{
		con_print (true, true, "Error: No variable specified - listing variables");
		sys_conListVariables();
		return true;
	}

	if (param2.size() == 0)
	{
		con_print(true, true, "Error: Missing parameter to setVar");
		return false;
	}

	while (hostVariables[count].scriptFunctionName.size() > 1)
	{
		//
		// Start the string with known empty value
		varType = "";
		varName = "";

		// Insert the string into a stream
		istringstream iss(hostVariables[count].scriptFunctionName, istringstream::in);
		//
		// Put each word into the vector
		while(iss >> buffer)
		{
			tokens.push_back(buffer);
		}
		varType = tokens[0];
		varName = tokens[1];
		//
		// Clear vectors and stringstream
		//
		iss.clear();
		tokens.clear();

		if (param1 == varName)
		{
			varTypeNum = sys_conGetVarType(varType);
			switch (varTypeNum)
			{
				case VAR_TYPE_INT:
					testVarInt = hostVariables[count].hostFunctionPtr;
					*testVarInt = atoi(param2.c_str());
					con_print (true, true,  "Variable [ %s ] set to [ %i ]", varName.c_str(), *testVarInt);
					break;

				case VAR_TYPE_FLOAT:
					testVarFloat = hostVariables[count].hostFunctionPtr;
					*testVarFloat = atof(param2.c_str());
					con_print (true, true, "Variable [ %s ] set to [ %2.2f ]", varName.c_str(), *testVarFloat);
					break;

				case VAR_TYPE_BOOL:
					testVarInt = hostVariables[count].hostFunctionPtr;
					tempValue = atoi(param2.c_str());
					if (0 == tempValue)
					{
						con_print (true, true, "Variable [ %s ] set to [ %s ]", varName.c_str(), "false");
					}

					else
					{
						con_print (true, true, "Variable [ %s ] set to [ %s ]", varName.c_str(), "true");
					}
					break;
			}
			return true;
		}
		count++;
	}
	con_print (true, true,  "Error: No matching value for [ %s ] found", param1.c_str());
	return true;
}
