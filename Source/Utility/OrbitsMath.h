/* Common math-related functions I'm putting here since they aren't really specific to anything else */
#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

extern const double PI, PI_OVER2;

// treating a as a vector, returns the distance^2 to the origin
double getLengthSquared(const sf::Vector2f& a);

// treating a as a vector, returns the distance to the origin
double getLength(const sf::Vector2f& a);

// treats the point as a vector, returning a vector of the same direction but length of 1
sf::Vector2f getNormalized(const sf::Vector2f& a);

// returns the straight-line distance between two points
float getDistance(const sf::Vector2f& a, const sf::Vector2f& b);

// returns the straight-line distance between two points squared
float getDistanceSquared(const sf::Vector2f& a, const sf::Vector2f& b);

// check if above/below 0.0 to know which side of a line a point is on
float checkLineSide( const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3 );

// returns a random floating point r where (0 <= r < n)
float randFloat(float n = 1.0);

// conversion from radians to degrees
float getDegrees(float radians);

// conversion from degrees to radians
float getRadians(float degrees);

// conversion from box2d vector to SFML vector
sf::Vector2f getSFVector(const b2Vec2& v);

// conversion from SFML vector to box2d vector
b2Vec2 getB2Vector(const sf::Vector2f& v);

// returns dot product of 2 Vector2s.
float dotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2);

// returns the angle[-pi,pi] in radians of a vector relative to some origin
float getAngleOfVectorRadians(const sf::Vector2f& origin, const sf::Vector2f& pos);

// returns the angle[-180,180] in degrees of a vector relative to some origin
float getAngleOfVectorDegrees(const sf::Vector2f& origin, const sf::Vector2f& pos);

// returns the angle to a target in radians given the forward direction of targeter,
// position of targeter, and position of target.
float getAngleToTargetRadians(const sf::Vector2f forwardDir, const sf::Vector2f& pos, const sf::Vector2f& targetPos);

// returns the angle to a target in radians given the forward direction of targeter,
// position of targeter, and position of target.
float getAngleToTargetDegrees(const sf::Vector2f forwardDir, const sf::Vector2f& pos, const sf::Vector2f& targetPos);

// returns the distance (in degrees) between two angles (accounts for angles over +-360), (-2, 3) returns 5, (362, -2) returns 4, etc
float getAngleDistanceDegrees(float a1, float a2);

// max/min functions
float  max(float a, float b);
double max(double a, double b);
int	   max(int a, int b);

float  min(float a, float b);
double min(double a, double b);
int	   min(int a, int b);

float  clamp(float value, float low, float high);