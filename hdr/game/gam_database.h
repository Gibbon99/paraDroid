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
