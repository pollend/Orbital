	
#include "Mine.h"
#include "Game/TextureManager.h"
#include "Game/ParticleManager.h"
#include "Game/SoundManager.h"
#include <iostream>

using namespace sf;

const float MINE_RADIUS = 2.0f,
			MAX_DAMAGE = 40.0f,		// maximum amount of damage mine does to player
			DAMAGE_FALL_OFF = 1.5f,	// larger = damage dealt to player is even lower when further away (1.0 = straight meters to damage conversion)
			MAX_PUSH = 70.0f,
			MAX_PUSH_DIST = 30.0f,
			MAX_VELOCITY = 4.0f,
			MAX_ANGULAR_VELOCITY = 25.0f,
			MINE_ACTIVE_DIST = 150.0f,
			MINE_ACTIVE_DIST_SQUARED = MINE_ACTIVE_DIST * MINE_ACTIVE_DIST;

Mine::Mine() { 
	
	type = MINE;

	mineState = MINE_NEEDS_TO_SPAWN; 

	mineShape.setRadius(MINE_RADIUS);
	mineShape.setPointCount(15);

	Texture* mineTexture = TextureManager::LoadAndRetrieveTexture("Assets\\mine.png");
	mineShape.setTexture(mineTexture);
	mineShape.setOrigin(MINE_RADIUS, MINE_RADIUS);	

}

Mine::~Mine() { }

void Mine::Load(PhysicsManager* physics, PlayerShip* playerShip, const Vector2f &position)
{
	this->physics    = physics;
	this->playerShip = playerShip;
	physicsBody = physics->AddMine(this, position, MINE_RADIUS);
	mineState = MINE_NORMAL;

	// give it a small random velocity
	Vector2f vel(randFloat() - 0.5f, randFloat() - 0.5f);
	vel = getNormalized(vel) * randFloat(MAX_VELOCITY);
	physicsBody->SetLinearVelocity(getB2Vector(vel));
	
	float angular = randFloat(MAX_ANGULAR_VELOCITY);
	physicsBody->SetAngularVelocity(getRadians(rand() % 2 == 0 ? angular : -angular));
}

void Mine::Update(float delta)
{
	if (mineState == MINE_EXPLODING) {
		physics->RemoveBody(physicsBody);
		mineState = MINE_NEEDS_TO_SPAWN;
		return;
	}

	if (mineState == MINE_NEEDS_TO_SPAWN)
		return;

	float dist = getDistanceSquared(getPosition(), playerShip->getPosition());
	bool lessThan = dist < MINE_ACTIVE_DIST_SQUARED;

	if (physicsBody->IsActive()) {

		if (!lessThan)
			physicsBody->SetActive(false);
	}
	else if (lessThan)
		physicsBody->SetActive(true);

	// get new position & rotation from box2D
	if (physicsBody->IsActive()) {
		mineShape.setPosition(getPosition());
		mineShape.setRotation(getRotation());
	}
}

void Mine::Draw(RenderWindow* Window) 
{
	if (mineState == MINE_NORMAL && isVisible(Window))
		Window->draw(mineShape);
}

bool Mine::isVisible(RenderWindow* Window) {

	Vector2f wp = Window->getView().getCenter();		// center of screen 
	Vector2f ws = Window->getView().getSize();			// width & height of screen in world coordinates
	ws += Vector2f(MINE_RADIUS * 2.0f, MINE_RADIUS * 2.0f);
	FloatRect wr(wp.x - ws.x / 2.0f, wp.y - ws.y / 2.0f, ws.x, ws.y);

	if ( wr.contains(getPosition()) )
		return true;
	else
		return false;
}

void Mine::handleCollision(GameObject* objectHit, const sf::Vector2f& contactPoint) {

	if (mineState != MINE_NORMAL)
		return;

	if (objectHit->getType() == ASTEROID || objectHit->getType() == PLAYER_SHIP || objectHit->getType() == MINE) {
		
		// find distance to player
		double dist = getLength(playerShip->getPosition() - getPosition());
		if (dist < 4.0f) dist = 0.0f;

		// calculate damage
		int damage = max(0, (int)(MAX_DAMAGE - dist*DAMAGE_FALL_OFF));
		playerShip->updateHealth(damage);
	
	//	std::cout << "mine damage to player = " << damage << "\n";

		// bump player
		if (dist < MAX_PUSH_DIST) {
			float speed = (float)((MAX_PUSH_DIST - dist) / MAX_PUSH_DIST) * MAX_PUSH;
			std::cout << "mine push player speed = " << speed << "\n";
			b2Vec2 dir = getB2Vector( getNormalized(playerShip->getPosition() - getPosition()) * speed );
			playerShip->getPhysicsBody()->ApplyLinearImpulse(dir, playerShip->getPhysicsBody()->GetWorldCenter(),true);
		}

		mineState = MINE_EXPLODING;

		if (getDistance(playerShip->getPosition(), getPosition()) < 200.0f) {
			ParticleManager::addMineExplosionParticle(getPosition());
			SoundManager::play(SoundManager::EXPLOSION_LARGE);
		}
	}
}