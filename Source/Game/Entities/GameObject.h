#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Utility/OrbitsMath.h"

// All physical game objects that adhere to physics should derive from this class
// so asteroids, player, planets, harpoons?
class GameObject {

	protected :
		enum GameObjectType {
			ASTEROID, HARPOON, PLANET, PLAYER_SHIP, ROPE_LINK, COLLECTOR_BEAM, MINE
		};

		b2Body*   physicsBody;			// pointer to the body object used by box2d
		GameObjectType type;			// useful so other game object's can find out the type when they only have a GameObject*
		bool isDead;

	public :

		GameObject() { physicsBody = NULL; isDead = false; };
		~GameObject() {};

		// getters
		sf::Vector2f getPosition()	{ return getSFVector(physicsBody->GetWorldCenter()); }
		sf::Vector2f getVelocity()	{ return getSFVector(physicsBody->GetLinearVelocity()); }
		float		 getRotation()	{ return getDegrees(physicsBody->GetAngle()); }
		GameObjectType getType()	{ return type; }

		// setters (these might be unnecessary or need to update physics body as well)
		void setPosition(float x, float y)		{ physicsBody->SetTransform(b2Vec2(x, y), physicsBody->GetAngle()); }
		void setPosition(const sf::Vector2f& p) { setPosition(p.x, p.y); }
		void setVelocity(float x, float y)		{ physicsBody->SetLinearVelocity(b2Vec2(x, y)); }
		void setVelocity(const sf::Vector2f& v) { setVelocity(v.x, v.y); }		
		void setRotation(float a)				{ physicsBody->SetTransform(physicsBody->GetWorldCenter(), getRadians(a)); }

		// Child classes must override this to react to collisions.
		virtual void handleCollision(GameObject* objectHit, const sf::Vector2f& contactPoint) {};
		// Child classes must override this to react to starting collisions.
		virtual void beginCollision(GameObject* objectHit) {};
		// Child classes must override this to react to ending collisions.
		virtual void endCollision(GameObject* objectHit) {};

		b2Body* getPhysicsBody() { return physicsBody; }

		bool getIsDead() { return isDead; }
};