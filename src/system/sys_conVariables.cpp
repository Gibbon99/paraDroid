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
int sys_conGetVarType ( string whichType )
//-----------------------------------------------------------------------------
{
	if ( 0 == whichType.size() )
		return VAR_TYPE_INT;

	if ( whichType == "int" )
		return VAR_TYPE_INT;

	if ( whichType == "float" )
		return VAR_TYPE_FLOAT;

	if ( whichType == "bool" )
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
	asUINT n;

	// List the application registered variables
	con_print ( true, false, "Application variables" );
	for ( n = 0; n < ( asUINT ) scriptEngine->GetGlobalPropertyCount(); n++ )
	{
		const char *name;
		int typeId;
		bool isConst;
		scriptEngine->GetGlobalPropertyByIndex ( n, &name, 0, &typeId, &isConst );
		string decl = isConst ? " const " : " ";
		decl += scriptEngine->GetTypeDeclaration ( typeId );
		decl += " ";
		decl += name;
		con_print ( true, false, "[ %s ]", decl.c_str() );
	}

	// List the user variables in the module
	asIScriptModule *mod = scriptEngine->GetModule ( "startup" );
	if ( mod )
	{
		cout << endl;
		con_print ( true, false, "User variables" );
		for ( n = 0; n < ( asUINT ) mod->GetGlobalVarCount(); n++ )
		{
			con_print ( true, false, "[ %s ]", mod->GetGlobalVarDeclaration ( n ) );
		}
	}

	return true;
}

//-----------------------------------------------------------
//
// Search for a variable name within all the global names
// return the index if found
// otherwise return -1
int util_getVariableIndex(string whichVar)
//-----------------------------------------------------------
{
	const char 	*name;
	int 		typeID;
	string 		variableName;
	
	for (unsigned int i = 0; i < (asUINT)scriptEngine->GetGlobalPropertyCount(); i++)
	{
		scriptEngine->GetGlobalPropertyByIndex(i, &name, 0, &typeID);
		variableName = name;
		if (variableName == whichVar)
			return i;	// Found a match - return index
	}
	return -1;	// Not found
}


//-----------------------------------------------------------------------------
//
//! \brief Get the value of a single variable
//! \param Pass in the name of the variable to query
bool sys_conGetVariable ( string whichVar )
//-----------------------------------------------------------------------------
{
	int		variableIndex;
	
	variableIndex = util_getVariableIndex(whichVar);
	
	if (-1 == variableIndex)
	{
		con_print(true, false, "ERROR: Could not locate variable [ %s ]", whichVar.c_str());
		return false;
	}
	
	const char 	*name;
	int 		typeID;
	bool 		isConst;
	void 		*varPointer;
	string 		varType;
	
	//
	// Get the Global properties
	scriptEngine->GetGlobalPropertyByIndex(variableIndex, &name, 0, &typeID, &isConst, 0, &varPointer);

	varType = scriptEngine->GetTypeDeclaration(typeID);
	if (varType == "int")
	{
		con_print(true, false, "Value of %s %s is %i", varType.c_str(), whichVar.c_str(), *(int *)varPointer);
		return true;
	}
	else if (varType == "float")
	{
		con_print(true, false, "Value of %s %s is %5.5f", varType.c_str(), whichVar.c_str(), *(float *)varPointer);
		return true;
	}
	else if (varType == "bool")
	{
		con_print(true, false, "Value of %s %s is %s", varType.c_str(), whichVar.c_str(), *(bool *)varPointer ? "true" : "false");
		return true;
	}
	else if (varType == "string")
	{
		string printString;
		
		printString = *(string *)varPointer;
		con_print(true, false, "Value of %s %s is %s", varType.c_str(), whichVar.c_str(), printString.c_str());
	}
	else
		con_print(true, false, "Unknown type [ %s ] for [ %s ]", varType.c_str(), whichVar.c_str());
		
	return true;
}

//-----------------------------------------------------------------------------
//
//! \brief Set the value of a single variable
//! \param Pass in the name and the new value
bool sys_conSetVariable ( string whichVar, string newValue )
//-----------------------------------------------------------------------------
{
	int		variableIndex;
	
	variableIndex = util_getVariableIndex(whichVar);
	if (-1 == variableIndex)
	{
		con_print(true, false, "ERROR: Could not locate variable [ %s ]", whichVar.c_str());
		return false;
	}
	
	const char 	*name;
	int 		typeID;
	bool 		isConst;
	void 		*varPointer;
	string 		varType;
	
	//
	// Get the Global properties
	scriptEngine->GetGlobalPropertyByIndex(variableIndex, &name, 0, &typeID, &isConst, 0, &varPointer);

	varType = scriptEngine->GetTypeDeclaration(typeID);
	if (varType == "int")
	{
		con_print(true, false, "Set varible to value [ %i ]", atoi(newValue.c_str()));
		*(int *)varPointer = atoi(newValue.c_str());
	}
	else if (varType == "float")
	{
		con_print(true, false, "Set varible to value [ %f ]", atof(newValue.c_str()));
		*(float *)varPointer = atof(newValue.c_str());
	}
	else if (varType == "bool")
	{
		if ((newValue == "true") || (newValue == "1"))
		{
			con_print(true, false, "Set variable to value [ %s ]", newValue.c_str());
			*(bool *)varPointer = true;
		}
		else
		{
			con_print(true, false, "Set variable to value [ %s ]", "false");
			*(bool *)varPointer = false;
		}
	}
	
	return true;
}
