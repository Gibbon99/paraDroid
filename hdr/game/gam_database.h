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

struct _dataBaseEntry
{
	int      	maxHealth;
	float      	maxSpeed;
	float    	accelerate;
	int      	score;
	int      	bounceDamage;
	bool     	canShoot;
	int      	bulletType;
	int      	chanceToShoot;
	int      	bulletDamage;
	bool     	disrupterImmune;
	float    	rechargeTime;
	int      	tokenCount;
	int			imageIndex;
	char	 	dbImageFileName	[MAX_STRING_SIZE];
	char    	height		[MAX_STRING_SIZE];
	char     	weight		[MAX_STRING_SIZE];

	string     	description;
	string     	className;
	string     	drive;
	string     	brain;
	string     	weapon;
	string     	sensor1;
	string     	sensor2;
	string     	sensor3;
	string     	notes;
};

extern _dataBaseEntry dataBaseEntry[];

// Get and build up information for droid database files
bool gam_getDBInformation();
