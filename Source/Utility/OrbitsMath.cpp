
#include "OrbitsMath.h"
#include <math.h>

using namespace sf;

const double PI		  = 3.1415926535897932384626433832795;
const double PI_OVER2 = 1.5707963267948966192313216916398;
const double DEG_TO_RAD = PI / 180.0;
const double RAD_TO_DEG = 180.0 / PI;

// treating a as a vector, returns the distance to the origin
double getLength(const Vector2f& a) {
	return sqrt(a.x * a.x + a.y * a.y);
}

// treating a as a vector, returns the distance^2 to the origin
double getLengthSquared(const Vector2f& a) {
	return a.x * a.x + a.y * a.y;
}

// treats the point as a vector, returning a vector of the same direction but length of 1
Vector2f getNormalized(const Vector2f& a) {
	double length = getLength(a);
	return Vector2f((float)(a.x / length), (float)(a.y / length));
}


// returns the straight-line distance between two points
float getDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
	float x = abs(a.x - b.x);
	float y = abs(a.y - b.y);
	return sqrt(x * x + y * y);
}


// returns the straight-line distance between two points squared
float getDistanceSquared(const sf::Vector2f& a, const sf::Vector2f& b) {
	float x = abs(a.x - b.x);
	float y = abs(a.y - b.y);
	return (x * x + y * y);
}

// stolen from : http://wiki.processing.org/w/Find_which_side_of_a_line_a_point_is_on
// check if above/below 0.0 to know which side of a line a point is on
float checkLineSide( const Vector2f& p1, const Vector2f& p2, const Vector2f& p3 ) {
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);   
}

// returns a random floating point r where (0 <= r < n)
float randFloat(float n) {
	return (float)(((rand() % RAND_MAX) / (double)RAND_MAX) * n);
}

// conversion from radians to degrees
float getDegrees(float radians) { return (float)(radians * RAD_TO_DEG); }

// conversion from degrees to radians
float getRadians(float degrees) { return (float)(degrees * DEG_TO_RAD); }

// conversion from box2d vector to SFML vector
sf::Vector2f getSFVector(const b2Vec2& v) { return sf::Vector2f(v.x, v.y); }

// conversion from SFML vector to box2d vector
b2Vec2 getB2Vector(const sf::Vector2f& v) { return b2Vec2(v.x, v.y); }

// returns dot product of 2 Vector2s.
float dotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

// returns the angle[-pi,pi] in radians of a vector relative to some origin
float getAngleOfVectorRadians(const sf::Vector2f& origin, const sf::Vector2f& pos) {
	return atan2(pos.y - origin.y, pos.x - origin.x);	// atan2 uses sign of coordinates to determines exact quadrant
}

// returns the angle[-180,180] in degrees of a vector relative to some origin
float getAngleOfVectorDegrees(const sf::Vector2f& origin, const sf::Vector2f& pos) {
	return getDegrees(getAngleOfVectorRadians(origin, pos));
}

// returns the angle to a target in radians given the forward direction of targeter,
// position of targeter, and position of target.
float getAngleToTargetRadians(const sf::Vector2f forwardDir, const sf::Vector2f& pos, const sf::Vector2f& targetPos)
{
	sf::Vector2f forwardDirNorm =	getNormalized(forwardDir);
	sf::Vector2f targetDirNorm	=	getNormalized(targetPos - pos);

	return acos(dotProduct(forwardDirNorm, targetDirNorm));
}

// returns the angle to a target in degrees given the forward direction of targeter,
// position of targeter, and position of target.
float getAngleToTargetDegrees(const sf::Vector2f forwardDir, const sf::Vector2f& pos, const sf::Vector2f& targetPos)
{
	return getDegrees(getAngleToTargetRadians(forwardDir, pos, targetPos));
}

// returns the distance (in degrees) between two angles (accounts for angles over +-360), (-2, 3) returns 5, (362, -2) returns 4, etc
float getAngleDistanceDegrees(float a1, float a2) 
{
	// first convert both angles to (-180, 180)
	if (abs(a1) > 360.0f) 
		a1 -= (int)(a1 / 360.0) * 360;
	if (abs(a1) > 180.0f)
		a1 += (a1 >= 0.0f ? -360.0f : 360.0f);

	if (abs(a2) > 360.0f) 
		a2 -= (int)(a2 / 360.0) * 360;
	if (abs(a2) > 180.0f)
		a2 += (a2 >= 0.0f ? -360.0f : 360.0f);

	float dist = abs(a1 - a2);

	if (dist <= 180.0f)
		return dist;
	else
		return (360.0f - dist);
}

// max/min functions
float  max(float a, float b)   { return (a > b ? a : b); }
double max(double a, double b) { return (a > b ? a : b); }
int	   max(int a, int b)	   { return (a > b ? a : b); }

float  min(float a, float b)   { return (a < b ? a : b); }
double min(double a, double b) { return (a < b ? a : b); }
int	   min(int a, int b)	   { return (a < b ? a : b); }

float  clamp(float value, float low, float high) {
	if (value < low)
		return low;
	else if (value > high)
		return high;
	else
		return value;
}