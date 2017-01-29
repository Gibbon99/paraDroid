#include "../../hdr/sys_globals.h"

double cosTable[360];
double sinTable[360];

//-----------------------------------------------------------------------------
//
// Precalculate the SIN and COS lookup tables
void sys_setupTables()
//-----------------------------------------------------------------------------
{
  for (int i = 0; i < 360; i++)
    {
      cosTable[i] = cos(i * (3.14 / 180.0f));
      sinTable[i] = sin(i * (3.14 / 180.0f));
    }
}

//-----------------------------------------------------------------------------
//
// Rotate a point around an origin point
cpVect sys_rotatePoint(cpVect point, cpVect origin, int degree)
//-----------------------------------------------------------------------------
{
  cpVect displacement;

  displacement.x = point.x - origin.x;
  displacement.y = point.y - origin.y;

  point.x = displacement.x * cosTable[degree] + displacement.y * sinTable[degree];
  point.y = displacement.y * cosTable[degree] - displacement.x * sinTable[degree];

  point.x = point.x + origin.x;
  point.y = point.y + origin.y;

  return point;
}

//-----------------------------------------------------------------------------
//
// Heads towards destination
cpVect sys_getDirection(cpVect sourcePoint, cpVect destPoint)
//-----------------------------------------------------------------------------
{
	cpVect	tempDirection;

	tempDirection = cpvsub(sourcePoint, destPoint);
	tempDirection = cpvnormalize(tempDirection);

	return tempDirection;
}

//-----------------------------------------------------------------------------
//
// Get the distance between two points
int sys_getDistance(cpVect point1, cpVect point2)
//-----------------------------------------------------------------------------
{
	cpVect tempPoint;
	int         distance;

	tempPoint.x = point2.x - point1.x;
	tempPoint.y = point2.y - point1.y;

	distance = sqrt(tempPoint.x * tempPoint.x + tempPoint.y * tempPoint.y);

	return distance;
}

//-----------------------------------------------------------------------------
//
// Get the direction of a vector
cpVect sys_getVectorDirection(cpVect vectA, cpVect vectB)
//-----------------------------------------------------------------------------
{
	cpVect direction;

	direction.x = vectB.x - vectA.x;
	direction.y = vectB.y - vectA.y;

	return direction;
}

//-----------------------------------------------------------------------------
//
// Dot product between two vectors
float sys_dot(cpVect v1, cpVect v2)
//-----------------------------------------------------------------------------
{
	return (v1.x*v2.x) + (v1.y*v2.y);
}

//-----------------------------------------------------------------------------
//
// If that point parameter is less than 0, then it is behind the start of the segment,
// if it is more than 1 then it is in front of the end of the segment.
// If it is between those two then it is on the segment.
//
float sys_distancetoLineSegment(cpVect pt1, cpVect pt2, cpVect point)
//-----------------------------------------------------------------------------
{
//
// (x1,y1) and (x2,y2) are any two(non-coinciding) on the line.

	float A = point.x - pt1.x;
	float B = point.y - pt1.y;

	float C = pt2.x - pt1.x;
	float D = pt2.y - pt1.y;

	float dot = A * C + B * D;
	float len_sq = C * C + D * D;
	float param = dot / len_sq;

	float xx,yy;

	if(param < 0)
	{
		xx = pt1.x;
		yy = pt1.y;
	}
	else if (param > 1)
	{
		xx = pt2.x;
		yy = pt2.y;
	}
	else
	{
		xx = pt1.x + param * C;
		yy = pt1.y + param * D;
	}

	cpVect temp;
	temp.x = xx;
	temp.y = yy;

	float dist = cpvdist(point, temp);

	return dist;
}

//-----------------------------------------------------------------------------
//
// Find the closest point on a line segment from a point
cpVect sys_closestPointOnLine(cpVect ptStart, cpVect ptEnd, cpVect ptCheck)
//-----------------------------------------------------------------------------
{
	cpVect newPoint;

	float A1 = ptEnd.y - ptStart.y;
	float B1 = ptStart.x - ptEnd.x;
	double C1 = (ptEnd.y - ptStart.y) * ptStart.x + (ptStart.x - ptEnd.x) * ptStart.y;
	double C2 = -B1*ptCheck.x + A1*ptCheck.y;
	double det = A1*A1 - -B1*B1;

	newPoint.x = 0;
	newPoint.y = 0;

	if(det != 0)
	{
		newPoint.x = (float)((A1*C1 - B1*C2)/det);
		newPoint.y = (float)((A1*C2 - -B1*C1)/det);
	}
	else
	{
		newPoint.x = ptCheck.x;
		newPoint.y = ptCheck.y;
	}
	return newPoint;
}

//-----------------------------------------------------------------------------
//
// Findif two points are moving towards each other or away
bool sys_movingTowards(cpVect point1, cpVect point2, cpVect point1Velocity)
//-----------------------------------------------------------------------------
{
	double 	D;
	cpVect 		C;
	cpVect 		N;

// Normalize the movevec
	N = cpvnormalize(point1Velocity);

// Find C, the vector from the center of the moving
// circle A to the center of B
	C = cpvsub(point2, point1);
	D = sys_dot(N, C);

// Another early escape: Make sure that A is moving
// towards B! If the dot product between the movevec and
// B.center - A.center is less that or equal to 0,
// A isn't isn't moving towards B

	if(D <= 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
