#pragma once

extern double cosTable[];
extern double sinTable[];

// Precalculate the SIN and COS lookup tables
void sys_setupTables();

// Rotate a point around an origin point
cpVect sys_rotatePoint(cpVect point, cpVect origin, int degree);

// clamp(value, min, max) - limits value to the range min..max
double sys_Clamp(double d, double min, double max);

// Get the direction of a vector
cpVect sys_getVectorDirection(cpVect  vectA, cpVect  vectB);

// Get the distance between two points
int sys_getDistance(cpVect point1, cpVect point2);

// Heads towards destination
cpVect sys_getDirection(cpVect sourcePoint, cpVect destPoint);

// If that point parameter is less than 0, then it is behind the start of the segment,
// if it is more than 1 then it is in front of the end of the segment.
// If it is between those two then it is on the segment.
float sys_distancetoLineSegment(cpVect pt1, cpVect pt2, cpVect point);

// Find the closest point on a line segment from a point
cpVect sys_closestPointOnLine(cpVect ptStart, cpVect ptEnd, cpVect ptCheck);

//Returns true if the circles are touching, or false if they are not
bool sys_circleCollision(cpVect pos1, int radius1, cpVect pos2, int radius2);

// Findif two points are moving towards each other or away
bool sys_movingTowards(cpVect point1, cpVect point2, cpVect point1Velocity);
