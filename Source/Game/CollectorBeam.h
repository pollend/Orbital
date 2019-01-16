#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Game/entities/GameObject.h"
#include "../source/game/PhysicsManager.h"

// Forward declarations to avoid circular includes.
class Asteroid;
class PlayerShip;

class CollectorBeam : public GameObject {

private:
	PlayerShip* playerShip;

	std::vector<Asteroid*> asteroidList;

	float currentGatherInterval;

	sf::ConvexShape beamShape;	
	double pulseCount;

public:
	CollectorBeam();
	~CollectorBeam();

	void Load(PhysicsManager *physics, PlayerShip* player, sf::Vector2f position);
	void Update(float delta);
	void Draw(sf::RenderWindow* Window);

	void ActivateBeam();
	void DeactiveBeam();
	bool IsBeamActive();

	// collision
	void beginCollision(GameObject* objectHit);
	void endCollision(GameObject* objectHit);
};