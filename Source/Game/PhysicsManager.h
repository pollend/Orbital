#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "Game/OrbitsContactListener.h"

class RopeLink;

#define UPDATE_INTERVAL			1.0f / 60.0f
#define MAX_CYCLES_PER_FRAME	5
#define VELOCITY_ITERATIONS		8
#define POSITION_ITERATIONS		3
#define GRAVITY					0

class PhysicsManager
{
private:
	b2World* mWorld;
	OrbitsContactListener mContactListener;

	GameObject* playerShip;			// convenient to save and use when making joint between stuck harpoon and ship

	float	fTimeAccumulator;
	float	fTimeStep;
	int		mVelocityIterations;
	int		mPositionIterations;

public:
	PhysicsManager();

	~PhysicsManager();

	void Load();

	void Reload();

	void Update(float DeltaT);

	b2Body* AddPlayer(GameObject* userData, float width, float height, sf::Vector2f position);

	b2Body* AddPlanetField(GameObject* userData, sf::Vector2f position, float fieldRadius);
	b2Body* AddPlanet(GameObject* userData, sf::Vector2f position, float radius);

	b2Body* AddAsteroid(GameObject* userData, const sf::Vector2f* verts, int numVertices, sf::Vector2f position, sf::Vector2f velocity, float density);

	b2Body* AddMine(GameObject* userData, const sf::Vector2f position, float radius);

	b2Body* AddHarpoon(GameObject* userData, sf::Vector2f position, float radius, sf::Vector2f velocity);

	b2RopeJoint* AddHarpoonJoint(b2Body* stuckObject, sf::Vector2f stuckPoint); 

	RopeLink* AddHarpoonChain(b2Body* stuckObject, sf::Vector2f stuckPoint, int numLinks);

	b2Body* AddCollectorBeam(GameObject* userData, sf::Vector2f position, const sf::Vector2f* verts, int numVerts, float radius, float angleWidth);

	bool RemoveBody(b2Body* body);

	b2World* GetWorld() { return mWorld; }
};