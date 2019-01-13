#pragma once

#include "GameObject.h"
#include "PhysicsManager.h"
#include "PlayerShip.h"
#include "Planet.h"
#include <SFML/Graphics.hpp>

#define TEXT_BUFFER_SIZE 100

class Asteroid : public GameObject {

	public:

		Asteroid();
		~Asteroid();

		void Load(sf::Texture* texture, PhysicsManager* physics, PlayerShip* playerShip, Planet* planet, int fieldX, int fieldY, int fieldWidth, int fieldHeight);
		void Update(float delta);
		void Draw(sf::RenderWindow* window);
		bool isInWindow(sf::RenderWindow* window);
		void handleCollision(GameObject* objectHit, const sf::Vector2f& contactPoint);
		void transferResourceToPlayer(int amount);
		void updateColor();

		int	 getResourceCount() { return resourceCount; }
		int	 getResourceType()  { return resourceType; }

	private:

		sf::ConvexShape asteroid;		// the polygon that represents the asteroid

		PlayerShip*		playerShip;
		bool			visibleLastFrame;
		float			bumpTimer;

		float mass;						// mass of the asteroid based on surface area
		int	  resourceType;				// the index of the type of resource this asteroid contains
		int	  resourceCount;			// amount of resources asteroid has currently

};