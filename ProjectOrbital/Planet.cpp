#include "Planet.h"
#include "PlayerShip.h"
#include <iostream>

using namespace sf;

Planet::Planet() {
	type = PLANET;
}

Planet::~Planet() {}

void Planet::Load(Texture* planetTexture, PhysicsManager *physics, sf::Vector2f position, float radius, float dockRadius, float forceFieldRadius) {

	this->radius	 = radius;
	this->dockRadius = dockRadius;
	this->fieldRadius = forceFieldRadius;

	// create planet body
	planet.setTexture(planetTexture);
	planet.setPosition(position);
	planet.setRadius(radius);
	planet.setOrigin(radius, radius);
	planet.setFillColor(Color(255,255,255,255));

	// create docking range
	planetDock.setPosition(position);
	planetDock.setRadius(dockRadius);
	planetDock.setOrigin(dockRadius, dockRadius);
	planetDock.setFillColor(Color(255,50,50,50));

	// create force field
	planetField.setPosition(position);
	planetField.setRadius(forceFieldRadius);
	planetField.setOrigin(forceFieldRadius, forceFieldRadius);
	planetField.setFillColor(Color(30,100,255,50));
	planetField.setPointCount(50);

	// create physics body for force field and physical planet
	physicsBody = physics->AddPlanetField(this, position, fieldRadius);
	planetBody  = physics->AddPlanet(this, position, radius);

	// clear all resource counts
	for (int i = 0; i < NUM_RESOURCE_TYPES; i++) 
		resourceCount[0] = 0;

	currentMission.Load();
}

void Planet::Update(float delta) {	
}

void Planet::Draw(sf::RenderWindow* Window) {

	Window->draw(planetField);
	Window->draw(planetDock);
	Window->draw(planet);
}

void Planet::setResourcesNeeded(int index, int count) {
	currentMission.SetResourcesNeeded(index, count);
}

void Planet::addToResourceCount(int index, int amount) {
	currentMission.AddToResourceCount(index, amount);
}

int Planet::getResourceCount(int index) { 
	return currentMission.GetResourceCount(index);
}

int Planet::getResourcesNeeded(int index) { 
	return currentMission.GetResourcesNeeded(index); 
}

int Planet::getResourcesCurrNeeded(int index) {
	return currentMission.GetResourcesCurrNeeded(index); 
}