#include "OrbitsContactListener.h"
#include <iostream>

void OrbitsContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	GameObject* objectA = (GameObject*)(contact->GetFixtureA()->GetBody()->GetUserData());
	GameObject* objectB = (GameObject*)(contact->GetFixtureB()->GetBody()->GetUserData());
	
	// needed to find the world location of the contact point
	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);

	if(objectA != NULL)
		objectA->handleCollision(objectB, getSFVector(worldManifold.points[0]));
	if(objectB != NULL)
		objectB->handleCollision(objectA, getSFVector(worldManifold.points[0]));
}

// Used for sensors.
void OrbitsContactListener::BeginContact(b2Contact* contact) {
	GameObject* objectA = (GameObject*)(contact->GetFixtureA()->GetBody()->GetUserData());
	GameObject* objectB = (GameObject*)(contact->GetFixtureB()->GetBody()->GetUserData());

	if(objectA != NULL)
		objectA->beginCollision(objectB);
	if(objectB != NULL)
		objectB->beginCollision(objectA);
}

void OrbitsContactListener::EndContact(b2Contact* contact) {
	GameObject* objectA = (GameObject*)(contact->GetFixtureA()->GetBody()->GetUserData());
	GameObject* objectB = (GameObject*)(contact->GetFixtureB()->GetBody()->GetUserData());

	if(objectA != NULL)
		objectA->endCollision(objectB);
	if(objectB != NULL)
		objectB->endCollision(objectA);
}