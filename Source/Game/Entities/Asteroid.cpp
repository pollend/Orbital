#include "Asteroid.h"
#include "Utility/OrbitsMath.h"
#include "Game/SoundManager.h"
#include "Game/ParticleManager.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <stdio.h>

using namespace sf;

// these constants are all parameters that define how to create the convex polygons that are asteroids

const float RADIUS_EXTENT = 0.5f,		// the difference between this and MIN_RADIUS will control the amount of possible asymmetry
			MIN_RADIUS    = 0.5f,		// should try to keep this + RADIUS_EXTENT equal to 1
			INC_STEP      = 0.15f,		// used to modify one vertex until it becomes convex with its neighbors
			MIN_SCALE     = 1.66f,		// this is the smallest scale factor to scale asteroid by
			SCALE_EXTENT  = 5.0f;		// this + MIN_SCALE determines the largest possible scale factor
const int	MIN_VERTICES  = 7,			// minimum number of vertices per asteroid
			MAX_VERTICES  = 8;			// maximum number of vertices per asteroid

const float MAX_VELOCITY	   = 5.0f,		// this is the maximum starting velocity (in either x or y direction)
			ROTATION_SPEED	   = 5000.0f,	// controls average angular velocity, will be divided by mass of asteroid
			MAX_ROTATION_SPEED = 100.0f,	// limits angular velocity
			ASTEROID_DENSITY   = 7.5f,		// all asteroids have same density
			MASS_TO_RESOURCE   = 0.025f;	// controls conversion of mass to resource amount (there is some randomness also)

const float CAMERA_FIELD_CLIP = 0.75f;		// when checking the distance from camera to asteroid and comparing to camera width * this value 
const float ASTEROID_ACTIVE_DIST = 100.0f;	// distance from player at which asteroids go to sleep
const float ASTEROID_ACTIVE_DIST_SQUARED = ASTEROID_ACTIVE_DIST * ASTEROID_ACTIVE_DIST;

const float MAX_RESOURCE_COLOR = 25.0f,		// controls the amount of resources needed for an asteroid to have max resource color
			MIN_RESOURCE_COLOR = 5.0f;		// controls the color jump between an asteroid with 1 resource and 0 resources (higher number = bigger jump)

Asteroid::Asteroid() {
	type = ASTEROID;
}
Asteroid::~Asteroid() {
}

// creates a randomized asteroid
void Asteroid::Load(Texture* texture, PhysicsManager *physics, PlayerShip* playerShip, Planet* planet, int fieldX, int fieldY, int fieldWidth, int fieldHeight) {
	
	this->playerShip = playerShip;
	bumpTimer = 0.0f;

	// we are going to create a randomized convex polygon for the asteroid
	// first determine how many vertices
	int numVerts		= rand() % (MAX_VERTICES - MIN_VERTICES + 1) + MIN_VERTICES;	
	Vector2f* verts = new Vector2f[numVerts];
	
	// create a polygon by creating vertices at regular intervals around a circle, and giving them random radius's
	// it won't be convex but we will fix that
	Transform rotation;
	rotation.rotate(360.0f / numVerts);

	Vector2f normalPoint(0.0f, -1.0);
	for (int i = 0; i < numVerts; i++) {
		verts[i] = normalPoint * (MIN_RADIUS + randFloat(RADIUS_EXTENT));
		normalPoint = rotation.transformPoint(normalPoint);
	}
	
	/*  * first find the line that connects two neighbor vertices (L1)
        * see which side of line we are on
		* if on the wrong side, move in direction of normal to line until just on the other side*/
	bool done = false;
	while (!done) {		// keep iterating over all vertices until no adjustments need to be made
		done = true;

		for (int i = 0; i < numVerts; i++) {

			int v1 = (i != 0 ? i-1 : numVerts-1);
			int v2 = (i != numVerts -1 ? i+1 : 0);
			Vector2f normal( getNormalized(verts[i]) * INC_STEP );

			// keep moving vertex until it is concave with its neighbors
			while (checkLineSide(verts[v1], verts[v2], verts[i]) > 0.0) {
				verts[i] += normal;
				done = false;
			}
		}
	}

	// randomly scale entire asteroid, also use scale to compute mass
	float astScale = MIN_SCALE + randFloat(SCALE_EXTENT);
	for (int i = 0; i < numVerts; i++) 
		verts[i] *= astScale;

	// now create asteroid from polygon
	asteroid.setPointCount(numVerts);
	for (int i = 0; i < numVerts; i++)
		asteroid.setPoint(i, verts[i]);

	// texture
	asteroid.setTexture(texture);

	// find random position that is not within planet force field radius
	Vector2f position;
	do {
		position.x = (float)(rand() % fieldWidth + fieldX);
		position.y = (float)(rand() % fieldHeight + fieldY);
		if(planet == 0)break;
	} while ( getDistance(planet->getPosition(), position) < (planet->getFieldRadius() + astScale * 2.0f) );
	asteroid.setPosition(position);
	
	// generate randomized starting velocity
	sf::Vector2f velocity;
	velocity.x = randFloat(MAX_VELOCITY * 2.0f) - MAX_VELOCITY;
	velocity.y = randFloat(MAX_VELOCITY * 2.0f) - MAX_VELOCITY;

	// create physics body
	physicsBody = physics->AddAsteroid(this, verts, numVerts, position, velocity, ASTEROID_DENSITY);
	mass = physicsBody->GetMass();

	// find angular velocity based on mass and randomness
	float angular = min(randFloat(ROTATION_SPEED) / mass, MAX_ROTATION_SPEED);		// speed should generally be slower for larger asteroids
	physicsBody->SetAngularVelocity(getRadians(rand() % 2 == 0 ? angular : -angular));

	// resource count will be based on mass but with some randomness also
	resourceCount = (int)(mass*MASS_TO_RESOURCE + randFloat(mass*MASS_TO_RESOURCE));
	
	// randomize resource type
	resourceType = 0;				// just in case
	float randNum = randFloat();
	float sum = 0.0f;
	for (int i = 0; i < NUM_RESOURCE_TYPES; i++) {
		if (randNum < (sum + RESOURCE_RATIO[i])) {
			resourceType = i;
			break;
		}
		sum += RESOURCE_RATIO[i];
	}
	updateColor();
	
	// if main menu asteroid, set active
	if (playerShip == 0)
		physicsBody->SetActive(true);

	delete[] verts;
}

void Asteroid::Update(float delta) {

	bumpTimer += delta;

	if(playerShip != 0)		// if this is not a main menu asteroid
	{
		// if asteroid is not close enough to camera, set it as inactive  (or awaken it if it is close enough)
		
		float dist = getDistanceSquared(getPosition(), playerShip->getPosition());
		bool lessThan = dist < ASTEROID_ACTIVE_DIST_SQUARED;

		if (physicsBody->IsActive()) {

			if (!lessThan)
				physicsBody->SetActive(false);
			else {
				asteroid.setRotation( getRotation() );
				asteroid.setPosition( getPosition() );
			}

		}
		else if (lessThan)
			physicsBody->SetActive(true);
	}
	else {
		asteroid.setRotation( getRotation() );
		asteroid.setPosition( getPosition() );
	}

}

void Asteroid::Draw(RenderWindow* window) {

	visibleLastFrame = true;
	// the physics body should become active before the asteroid comes on screen
	if (!physicsBody->IsActive() || !isInWindow(window)) {		
		visibleLastFrame = false;
		return;
	}

	// draw the asteroid
	window->draw(asteroid);
}

bool Asteroid::isInWindow(RenderWindow* window) {

	Vector2f  ap = getPosition();						// center of asteroid
	FloatRect ar = asteroid.getLocalBounds();
	ar.left += ap.x;
	ar.top  += ap.y;

	Vector2f wp = window->getView().getCenter();		// center of screen 
	Vector2f ws = window->getView().getSize();			// width & height of screen in world coordinates
	FloatRect wr(wp.x - ws.x / 2.0f, wp.y - ws.y / 2.0f, ws.x, ws.y);

	if (ar.intersects(wr))
		return true;
	else
		return false;
}

void Asteroid::handleCollision(GameObject* objectHit, const Vector2f& contactPoint) {

	if (!visibleLastFrame || objectHit == NULL || bumpTimer < 0.25f)
		return;

	bumpTimer = 0.0f;

	if (objectHit->getType() == ASTEROID) {

		ParticleManager::addAsteroidDustParticles(contactPoint);
		ParticleManager::addSpecParticles(contactPoint, 2, asteroid.getFillColor());
		
		// play a random asteroid collision sound
		int i = rand() % 3;
		if      (i == 0) SoundManager::play(SoundManager::ASTEROID_COLLISION_1);
		else if (i == 1) SoundManager::play(SoundManager::ASTEROID_COLLISION_2);
		else if (i == 2) SoundManager::play(SoundManager::ASTEROID_COLLISION_3);
	}
	else if (objectHit->getType() == HARPOON || objectHit->getType() == PLAYER_SHIP) {
		ParticleManager::addSpecParticles(contactPoint, 4, asteroid.getFillColor());
	}
}

void Asteroid::transferResourceToPlayer(int amount) {
	if(amount > resourceCount)
		amount = resourceCount;

	if (amount == 0)
		return;		

	resourceCount -= amount;
	int overflow = playerShip->addToResourceCount(resourceType, amount);
	resourceCount += overflow;

	// add the "+1" particle over player ship 
	if (overflow == 0) {
		ParticleManager::addPlus1Particle(RESOURCE_COLOR[resourceType]);
		SoundManager::play(SoundManager::MINING_SOUND);
	}

	updateColor();
}

// called when resource count is changed, changes color of asteroid to correspond to resource type and quantity
void Asteroid::updateColor() {

	Color color = RESOURCE_COLOR[resourceType];
	
	float scale = min(resourceCount + MIN_RESOURCE_COLOR, MAX_RESOURCE_COLOR) / MAX_RESOURCE_COLOR;	

	// used resource count to vary color from resource color to white
	color.r = (uint8)(255 - (255 - color.r) * scale);
	color.g = (uint8)(255 - (255 - color.g) * scale);
	color.b = (uint8)(255 - (255 - color.b) * scale);

	if (resourceCount != 0)	
		asteroid.setFillColor(color);
	else
		asteroid.setFillColor(Color::White);		// no more resources so no color
}
