#pragma once

#include "Game/Entities/GameObject.h"
#include "Game/Mission.h"
#include "PhysicsManager.h"
#include <SFML/Graphics.hpp>

class Planet : public GameObject {

	private:

		b2Body*			 planetBody;			// the physicsBody is used for planet field

		sf::CircleShape  planet;				// this is the actual planet body				
		sf::CircleShape  planetDock;			// this is the docking range around the planet
		sf::CircleShape  planetField;			// this is the force field that separates planet from asteroids

		float radius, dockRadius, fieldRadius;

		Mission currentMission;

		int resourceCount[10];					// amount of resources currently obtained from player of each type (10 is arbitrary)
		int resourcesNeeded[10];				// current goal resource count of each type

	public:

		Planet();
		~Planet();

		void Load(sf::Texture* planetTexture, PhysicsManager *physics, sf::Vector2f position, float radius, float dockRadius, float forceFieldRadius);
		void Update(float delta);
		void Draw(sf::RenderWindow* Window);

		void setResourcesNeeded(int index, int count);
		void addToResourceCount(int index, int amount);

		int getResourceCount(int index);
		int getResourcesNeeded(int index);
		int getResourcesCurrNeeded(int index);

		bool getIsMissionComplete() { return currentMission.GetIsMissionComplete(); }

		float getRadius()		{ return radius; }
		float getDockRadius()	{ return dockRadius; }
		float getFieldRadius()	{ return fieldRadius; }

};