#include <iostream>
#include "Asteroid.h"
#include "Harpoon.h"
#include "../source/game/PlayerShip.h"
#include "../source/game/ParticleManager.h"
#include "SoundManager.h"
#include "TextureManager.h"

using namespace sf;

const int HARPOON_INACTIVE = 0,
		  HARPOON_FLYING   = 1,
		  HARPOON_STICKING = 2,				// this is just a transition from flying to stuck, but we can't create a joint while handling the collision
		  HARPOON_STUCK	   = 3,
		  HARPOON_DYING	   = 4;

const float HARPOON_SPEED = 65.0f,
			HARPOON_RADIUS = 0.8f,
			HARPOON_MAX_DIST = 50.0f,		// harpoon can only travel this far from player before being destroyed
			RESOURCE_GATHER_INTERVAL = 2.0f;

const int NUM_ROPE_LINKS = 30;

Harpoon::Harpoon() { 

	type = HARPOON;
	physicsBody = NULL;
	ropeVertices = new Vertex[NUM_ROPE_LINKS + 2];
}

Harpoon::~Harpoon() { 
	delete[] ropeVertices;
}

void Harpoon::Load( PhysicsManager* physics, PlayerShip* playerShip )
{
	state = HARPOON_INACTIVE;
	this->playerShip = playerShip;
	this->physics = physics;

	harpoon.setTexture(TextureManager::LoadAndRetrieveTexture("Assets\\harpoon.png"));

	stuckObject = NULL;
	ropeJoint = NULL;
	ropeLink = NULL;
}

void Harpoon::Launch(Vector2f position, Vector2f velocity, bool isLeftHarpoon) {

	harpoon.setRadius(HARPOON_RADIUS);
	harpoon.setOrigin(HARPOON_RADIUS, HARPOON_RADIUS);
	harpoon.setFillColor( (isLeftHarpoon) ? Color(255,50,100) : Color(50,100,255) );
	harpoon.setPosition(position);
	
	physicsBody = physics->AddHarpoon(this, position, HARPOON_RADIUS, velocity);

	// im thinking the harpoon shouldn't weigh much
	physicsBody->GetFixtureList()->SetDensity(0.001f);
	physicsBody->ResetMassData();	

	if (isLeftHarpoon) SoundManager::play(SoundManager::HARPOON_FIRE_1);
	else			   SoundManager::play(SoundManager::HARPOON_FIRE_2);

	state = HARPOON_FLYING;
	physicsBody->SetActive(true);
}

void Harpoon::Update(float delta) {

	if (state == HARPOON_INACTIVE)
		return;

	if (state == HARPOON_FLYING) {

		// too far from player?
		if ( getDistance(getPosition(), playerShip->getPosition()) > HARPOON_MAX_DIST) {
			kill();
			return;
		}
	}
	else if (state == HARPOON_STICKING) {

		// create joint between player and object that was stuck on by harpoon
		Vector2f pos = stuckObject->getPosition() + stuckPosition;
		ropeJoint = physics->AddHarpoonJoint( stuckObject->getPhysicsBody(), pos );
		ropeLink  = physics->AddHarpoonChain( stuckObject->getPhysicsBody(), pos, NUM_ROPE_LINKS );
		state = HARPOON_STUCK;
		physicsBody->SetActive(false);
	}
	else if (state == HARPOON_STUCK) {

		// match the asteroids position and rotation
		Transform rotate;
		rotate.rotate(stuckObject->getRotation() - stuckRotation);
		Vector2f newPosition = rotate.transformPoint(stuckPosition) + stuckObject->getPosition();
		setPosition(newPosition);
		setRotation(stuckObject->getRotation() - stuckRotation);

	}
	else if (state == HARPOON_DYING) {
		kill();
		return;
	}

	// get state from physics
	harpoon.setPosition(getPosition());
	harpoon.setRotation(getRotation());
}

void Harpoon::Draw(sf::RenderWindow* Window) {

	if (state == HARPOON_INACTIVE || state == HARPOON_DYING)
		return;

	// should we draw the rope?
	if (state == HARPOON_FLYING || state == HARPOON_STICKING) {

		Vertex line[2] = { playerShip->getPosition(), getPosition() };
		Window->draw(line, 2, sf::Lines);
	}
	else if (state == HARPOON_STUCK && ropeLink != NULL) {
	
		RopeLink* link = ropeLink;

		ropeVertices[0].position = getPosition();
		for (int i = 1; i < NUM_ROPE_LINKS+1; i++) {
			ropeVertices[i].position = link->getPosition();
			link = (RopeLink*)link->getPhysicsBody()->GetUserData();
		}
		ropeVertices[NUM_ROPE_LINKS+1] = playerShip->getPosition();

		Window->draw(ropeVertices, NUM_ROPE_LINKS+2, sf::LinesStrip);
	}

	// draw hapoon
	Window->draw(harpoon);
}

void Harpoon::handleCollision(GameObject* objectHit, const Vector2f& contactPoint) {

	if (state != HARPOON_FLYING)
		return;

	if (objectHit->getType() == MINE) {
		state = HARPOON_DYING;
		return;
	}

	// note : we cannot create a joint while a time step is being performed by box2D, so we must wait until 
	// the harpoon's usual update function is called to create the joint
	state = HARPOON_STICKING;
	this->stuckObject = objectHit;

	// convert collision location into local coordinates for the asteroid that was hit
	stuckPosition = getPosition() - objectHit->getPosition();
	stuckRotation = objectHit->getRotation();

	SoundManager::play(SoundManager::HARPOON_HIT);
}

void Harpoon::kill() {

	state = HARPOON_INACTIVE;
	stuckObject = NULL;

	SoundManager::play(SoundManager::HARPOON_CUT);

	if (physicsBody != NULL) {
		physics->GetWorld()->DestroyBody(physicsBody);
		physicsBody = NULL;
	}

	if (ropeJoint != NULL) {
		physics->GetWorld()->DestroyJoint(ropeJoint);
		ropeJoint = NULL;
	}

	// delete chain by deleting every body in the chain (joints should be deleted when an attached body is deleted)
	if (ropeLink != NULL) {
		RopeLink* next = NULL;

		while (ropeLink != NULL) {
			next = (RopeLink*)ropeLink->getPhysicsBody()->GetUserData();
			physics->GetWorld()->DestroyBody(ropeLink->getPhysicsBody());
			delete ropeLink;
			ropeLink = next;
		}

		ropeLink = NULL;
	}
}