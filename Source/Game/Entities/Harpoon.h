#pragma once

#include "Game/entities/GameObject.h"
#include "../source/game/PhysicsManager.h"
#include <SFML/Graphics.hpp>

class PlayerShip;

extern const int HARPOON_INACTIVE, HARPOON_FLYING, HARPOON_STICKING, HARPOON_STUCK;
extern const float HARPOON_SPEED, RESOURCE_GATHER_INTERVAL;

class RopeLink : public GameObject {

	public :
		RopeLink(b2Body* body) { 
			type = ROPE_LINK;
			this->physicsBody = body; 
		}
};

// This object I imagine to be the physical claw or grapple that is shot from the ship and attaches to an asteroid.
class Harpoon : public GameObject {

	private:

		sf::CircleShape  harpoon;				// for right now, just a circle			
		int				 state;					// state of harpoon (inactive, flying, or stuck)

		GameObject*		 stuckObject;			// object that harpoon gets stuck to
		sf::Vector2f	 stuckPosition;			// position (in local coordinates of asteroid that we stuck into)
		float			 stuckRotation;			// rotation of object when harpoon stuck into it
		b2RopeJoint*	 ropeJoint;				// distance joint between player ship and stuck harpoon
		RopeLink*		 ropeLink;				// this refers to the first link in the chain
		sf::Vertex*		 ropeVertices;			// used to draw the rope each frame

		PlayerShip*		 playerShip;
		PhysicsManager*	 physics;

	public:

		Harpoon();
		~Harpoon();

		void Load(PhysicsManager *physics, PlayerShip* playerShip);
		void Launch(sf::Vector2f position, sf::Vector2f velocity, bool isLeftHarpoon);
		void Update(float delta);
		void Draw(sf::RenderWindow* Window);
		void handleCollision(GameObject* objectHit, const sf::Vector2f& contactPoint);
		void kill();

		int getState() { return state; }
};

