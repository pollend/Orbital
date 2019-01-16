#include "CollectorBeam.h"
#include "Game/entities/Asteroid.h"
#include "../source/game/PlayerShip.h"
#include "../source/game/ParticleManager.h"
#include "Utility/OrbitsMath.h"

#define BEAM_RADIUS		 20.0f
#define BEAM_ANGLE_WIDTH 30.0f
#define BEAM_NUM_VERTS	 8

#define RESOURCE_GATHER_INTERVAL 2.0f

CollectorBeam::CollectorBeam() {
	type = GameObject::COLLECTOR_BEAM;
	currentGatherInterval = RESOURCE_GATHER_INTERVAL;
}

CollectorBeam::~CollectorBeam() {

}

void CollectorBeam::Load(PhysicsManager* physics, PlayerShip* player, sf::Vector2f position) {	
	playerShip = player;

	sf::Vector2f* verts = new sf::Vector2f[BEAM_NUM_VERTS];
	verts[0] = sf::Vector2f(0, 0);
	for(int i = 0; i < BEAM_NUM_VERTS - 1; ++i)
	{
		float angle = getRadians(i / 6.0f * BEAM_ANGLE_WIDTH) - getRadians(90 + BEAM_ANGLE_WIDTH / 2);// - (3 * PI / 4);
		verts[i + 1] =  sf::Vector2f(BEAM_RADIUS * cosf(angle), BEAM_RADIUS * sinf(angle));
	}

	beamShape.setPointCount(BEAM_NUM_VERTS);
	for (int i = 0; i < BEAM_NUM_VERTS; ++i)
		beamShape.setPoint(i, verts[i]);

	beamShape.setFillColor(sf::Color(255,0,0,70));
	beamShape.setPosition(position);
	beamShape.setOutlineThickness(0.3f);
	beamShape.setOutlineColor(sf::Color::Transparent);

	physicsBody = physics->AddCollectorBeam(this, position, verts, BEAM_NUM_VERTS, BEAM_RADIUS, BEAM_ANGLE_WIDTH);
	physicsBody->SetUserData(this);
	physicsBody->SetActive(false);

	asteroidList.clear();
	pulseCount = 0.0f;
}

void CollectorBeam::Update(float delta) {

	pulseCount += delta;

	// clear any asteroids from list that have lost all resources
	for(unsigned int i = 0; i < asteroidList.size(); ++i) {
		if(asteroidList[i]->getResourceCount() == 0) {
			asteroidList.erase(asteroidList.begin() + i);
			i--;
		}
	}

	if(IsBeamActive()) {
		currentGatherInterval -= delta;

		physicsBody->SetTransform(playerShip->getPhysicsBody()->GetPosition(),
			playerShip->getPhysicsBody()->GetAngle());

		if(currentGatherInterval <= 0) {
			currentGatherInterval = RESOURCE_GATHER_INTERVAL;

			for(unsigned int i = 0; i < asteroidList.size(); i++) 
				asteroidList[i]->transferResourceToPlayer(1);
		}

		if (!asteroidList.empty()) {

			double a = sin(fmod(pulseCount * PI, PI));
			beamShape.setOutlineColor(sf::Color(100,100,255,(sf::Uint8)(a * 175.0  + 80.0)));
		}
		else
			beamShape.setOutlineColor(sf::Color::Transparent);
	}
	else {
		currentGatherInterval = RESOURCE_GATHER_INTERVAL;
		beamShape.setOutlineColor(sf::Color::Transparent);
	}

	beamShape.setRotation( getRotation() );
	beamShape.setPosition( getPosition() );
}

void CollectorBeam::Draw(sf::RenderWindow* Window) {
	if(IsBeamActive())
		Window->draw(beamShape);
}

void CollectorBeam::ActivateBeam() {
	physicsBody->SetActive(true);
}

void CollectorBeam::DeactiveBeam() {
	physicsBody->SetActive(false);
	asteroidList.clear();
}

bool CollectorBeam::IsBeamActive() {
	return physicsBody->IsActive();
}

void CollectorBeam::beginCollision(GameObject* objectHit) {
	if(IsBeamActive()) {
		if(objectHit->getType() == GameObject::ASTEROID) {
			for(unsigned int i = 0; i < asteroidList.size(); ++i) {
				if(asteroidList[i] == objectHit)
					return;
			}
			asteroidList.push_back(((Asteroid*)objectHit));
			//std::cout << "resource : " << playerShip->getResourceCount() << std::endl;
		}
	}
}

void CollectorBeam::endCollision(GameObject* objectHit) {
	for(unsigned int i = 0; i < asteroidList.size(); ++i) {
		if(asteroidList[i] == objectHit) {
			asteroidList.erase(asteroidList.begin() + i);
			return;
		}
	}
}