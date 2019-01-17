#include "PlayerShip.h"
#include "ParticleManager.h"
#include "Utility/OrbitsMath.h"
#include "Game/SoundManager.h"
#include <iostream>

using namespace sf;

const float SHIP_WIDTH = 2.0f,				// width of player's ship in meters
			SHIP_HEIGHT = 2.0f,				// height of player's ship in meters
			SHIP_MASS = 3.0f,				// mass of player's ship (without any fuel or resources on it)
			SHIP_MAX_ANGULAR = 5.0f,		// limits the maximum angular velocity of the ship 
			FUEL_CAPACITY = 100.0f,			// maximum fuel capacity of player's ship
			FUEL_START_AMOUNT = 100.0f,		// amount of fuel player start's game with
			FUEL_CONSUME_RATE = 1.5f,		// rate at which fuel is consumed by each thruster, per second
			FUEL_MASS_PER_UNIT = 0.01f,		// mass per each unit of fuel
			RESOURCE_MASS_PER_UNIT = 0.04f; // mass per unit of resource
const int	RESOURCE_CAPACITY = 25,			// capacity for acquiring resources
			SHIP_MAX_HEALTH = 100;			// starting health of the player's ship 			

const int	BASE_DAMAGE  = 10,				// damage dealt to player on collision, altered by angle of collision and SPEED_DAMAGE scale.
			SPEED_DAMAGE = 10,				// used to scale damage (speed of the ship / SPEED_DAMAGE)
			MIN_SPEED_DIFF_TO_DAMAGE = 3;	// the minimum difference in speed between the ship and player for damage calculation to begin.
const float TIME_INTERVAL_DAMAGE = .5f,		// used to prevent continual damage
			OVERALL_DAMAGE_SCALE = 2.0f;	// used to scale damage after it's calculated based on velocities and angles

const float GRAVITY_CONSTANT = 23000.0f;	// controls the gravity coefficient with the home planet

const float SHIP_THRUST = 15.0f,			// how much thrust is applied 
			THRUST_XDIST = 1.0f,			// controls how far apart thrusters are, which changes how much angular vs. linear velocity is added
			THRUST_YDIST = 0.75f;			// controls how far back thrusters are set, also effects angular vs. linear velocity

const int   LEFT_HARPOON  = 0,				// indexes into the harpoon array
			RIGHT_HARPOON = 1;

const int	NOT_DOCKING = 0,				// not within docking radius
			DOCKING_BAD_TRAJ = 2,			// within radius, but going too fast, or too slow, or heading straight into planet
			DOCKING_ORBITING = 3,			// within radius and ship is being slowed down
			DOCKING_LANDED = 4,				// on planet, exchanging fuel and resources
			DOCKING_LAUNCHING = 5;			// ship being launched from planet

const float DOCKING_CRIT_ANGLE = 40.0f,		// this denotes the lowest angle ship can approach and be acceptable (90 degrees is optimal angle)
			DOCKING_SPEED_MAX = 30.0f,
			DOCKING_SPEED_MIN = 10.0f,
			DOCKING_LANDED_TIME	= 2.0f,		// how long ship stays landed on the planet, seconds
			DOCKING_LAUNCHING_TIME = 5.0f,	// how long the launching stage lasts, seconds
			DOCKING_LAUNCH_SPEED = 13.0f;	// how fast ship is moving when done launching

// TODO: Create input system to better handle toggling. -Kenneth
const float INPUT_TOGGLE_INTERVAL = .2f;


// RESOURCE TYPES //////////////////
const int	NUM_RESOURCE_TYPES  = 3;
const float	RESOURCE_RATIO[]   = { 0.55f, 0.35f, 0.10f };	// these ratio's control the distribution of types among all asteroids, must add to 1.0
const float RESOURCE_TO_FUEL[] = { 4.0f, 8.0f,  20.0f };	// these are the conversion amount of resource type to fuel
const Color RESOURCE_COLOR[] = { Color(255, 50, 50),
								 Color(50, 255, 50),
								 Color(50, 50, 255) };

PlayerShip::PlayerShip() {
	type = PLAYER_SHIP;
	dockStage = NOT_DOCKING;
}
PlayerShip::~PlayerShip() {
}

void PlayerShip::Load(PhysicsManager *physics, Planet* homePlanet, Vector2f position) {

	this->physics = physics;
	this->homePlanet = homePlanet;
	isDead = false;
	leftLetUp = rightLetUp = oneLetUp = twoLetUp = threeLetUp = true;
	reverseThrust = false;

	// create player's sprite
	shipTexture.loadFromFile("Assets//playerShip.png");
	ship.setTexture(shipTexture);
	// set center of player ship to middle of texture (may change this to center of mass in future)
	ship.setOrigin((shipTexture.getSize().x / 2.0f), (shipTexture.getSize().y / 2.0f));	
	float scale = SHIP_WIDTH / shipTexture.getSize().x;
	ship.setScale(scale, scale);
	ship.setPosition(position);

	// add player to physics engine
	physicsBody = physics->AddPlayer(this, SHIP_WIDTH, SHIP_HEIGHT, position);

	area			= physicsBody->GetMass();
	healthCount	    = SHIP_MAX_HEALTH;
	damageLastFrame = 0;
	fuelCount	    = FUEL_START_AMOUNT;
	for (int i = 0; i < NUM_RESOURCE_TYPES; i++)
		resourceCount[i] = 0;
	updateMass();

	// points where thrust will be applied in local coordinates
	leftThrustPoint.Set( -THRUST_XDIST, THRUST_YDIST);
	midThrustPoint.Set(   0.0f,		    THRUST_YDIST);
	rightThrustPoint.Set( THRUST_XDIST, THRUST_YDIST);

	// collector beam
	collectorBeam.Load(physics, this, this->getPosition());
	toggleTimer = INPUT_TOGGLE_INTERVAL;

	// harpoons
	harpoon[0].Load(physics, this);
	harpoon[1].Load(physics, this);

	// start ship launching from planet
	dockStage = DOCKING_LANDED;
	dockTime = 0.0f;
	physicsBody->SetActive(false);
	
	damageTimer = TIME_INTERVAL_DAMAGE;
}

void PlayerShip::Update(float delta) {

	damageLastFrame = 0;

	if(!isDead) {
		damageTimer -= delta;

		// check for thruster input and apply impulses
		updateThrusters(delta);

		// update fuel count
		if (leftThrustOn || rightThrustOn) {

			if (leftThrustOn)  fuelCount -= FUEL_CONSUME_RATE * delta;
			if (rightThrustOn) fuelCount -= FUEL_CONSUME_RATE * delta;
			fuelCount = max(fuelCount, 0.0f);
			updateMass();
		}

		// start new harpoons?

		if (dockStage == NOT_DOCKING) {

			int index = -1;
			if (leftLetUp && Mouse::isButtonPressed(Mouse::Left))  {
				index = LEFT_HARPOON;
				leftLetUp = false;
			}
			if (rightLetUp && Mouse::isButtonPressed(Mouse::Right)) {
				index = RIGHT_HARPOON;
				rightLetUp = false;
			}

			if (index != -1) {
				if (harpoon[index].getState() == HARPOON_INACTIVE)
					startHarpoon(index);
				else
					harpoon[index].kill();
			}
		}


		if (!Mouse::isButtonPressed(Mouse::Left))  leftLetUp  = true;
		if (!Mouse::isButtonPressed(Mouse::Right)) rightLetUp = true;

		// gravity = coefficient / distance^2 (we could uses masses but it would end up being nearly constant anyways, and this is easier to control)
		Vector2f toPlanet = homePlanet->getPosition() - getPosition();
		float g = (float)(getGravityMagnitude() * delta);						
		physicsBody->ApplyLinearImpulse(getB2Vector( getNormalized( toPlanet ) * g ), physicsBody->GetWorldCenter(),true);
	
		// update docking procedures
		updateDocking(delta);			
	
		// update beam
		collectorBeam.Update(delta);
		toggleTimer -= delta;
		if(Keyboard::isKeyPressed(Keyboard::Space) && toggleTimer <= 0) {
			if (collectorBeam.IsBeamActive())
				collectorBeam.DeactiveBeam();
			else	
				collectorBeam.ActivateBeam();

			toggleTimer = INPUT_TOGGLE_INTERVAL;
		}

		// check for resource->fuel conversion
		checkForResourceToFuel();
	}

	// limit the ship's angular velocity
	float angVel = physicsBody->GetAngularVelocity();
	if (angVel > SHIP_MAX_ANGULAR || angVel < -SHIP_MAX_ANGULAR)
		physicsBody->SetAngularVelocity(clamp(physicsBody->GetAngularVelocity(), -SHIP_MAX_ANGULAR, SHIP_MAX_ANGULAR));

	// update sprite position and rotation from physics engine
	ship.setRotation( getRotation() );
	ship.setPosition( getPosition() );

	// update harpoons
	for (int i=0; i < 2; i++)
		harpoon[i].Update(delta);
}

void PlayerShip::Draw(sf::RenderWindow* Window) {

	for (int i=0; i < 2; i++)		// harpoons
		harpoon[i].Draw(Window);

	if(!isDead)
		collectorBeam.Draw(Window);	// collector beam
	
	Window->draw(ship);				// player ship

	mouseWorldCoordinates = Window->mapPixelToCoords(Mouse::getPosition(*Window));
}

// looks for input and controls ship acceleration
void PlayerShip::updateThrusters(float delta) {

	// don't use thrusters if player is out of fuel or in a automated docking stage
	if (fuelCount <= 0.0f || (dockStage != NOT_DOCKING && dockStage != DOCKING_BAD_TRAJ)) {
		leftThrustOn = rightThrustOn = false;
		updateThrusterSounds(leftThrustOn, rightThrustOn);
		return;
	}

	reverseThrust = false;

	// find the thrust direction vectors			
	b2Vec2 thrustMagnitude ( 0.0f, -SHIP_THRUST * delta);		// controls thrust power
	b2Vec2 thrustMagnitude2( 0.0f, -SHIP_THRUST * 2.0f * delta);// for combining thrust

	// now transform thrust points and thrust direction to world coordinates
	b2Vec2 leftPoint( physicsBody->GetWorldPoint( leftThrustPoint) );
	b2Vec2 midPoint( physicsBody->GetWorldPoint(  midThrustPoint) );
	b2Vec2 rightPoint( physicsBody->GetWorldPoint(rightThrustPoint) );
	b2Vec2 thrust( physicsBody->GetWorldVector(   thrustMagnitude) );
	b2Vec2 thrust2 (physicsBody->GetWorldVector(  thrustMagnitude2) );

	// note : if both thrusters are on, we need to apply one force in middle since impulses are applied immediately after being called
	leftThrustOn  = Keyboard::isKeyPressed(Keyboard::A);
	rightThrustOn = Keyboard::isKeyPressed(Keyboard::D);

	if (Keyboard::isKeyPressed(Keyboard::W)) {
		if (leftThrustOn)
			rightThrustOn = false;
		else if (rightThrustOn)
			leftThrustOn = false;
		else 
			leftThrustOn = rightThrustOn = true;
	}
	else if (Keyboard::isKeyPressed(Keyboard::S)) {

		reverseThrust = true;
		thrust = -thrust;
		thrust2 = -thrust2;

		// when pressing reverse and a direction, we need to use the opposite side compared to normal
		if (leftThrustOn) {	
			rightThrustOn = true;
			leftThrustOn = false;
		}
		else if (rightThrustOn) {
			rightThrustOn = false;
			leftThrustOn = true;
		}
		else 
			leftThrustOn = rightThrustOn = true;
	}

	if (leftThrustOn && rightThrustOn) 
		physicsBody->ApplyLinearImpulse(thrust2, midPoint,true);			// both thrusters
	else if (leftThrustOn)
		physicsBody->ApplyLinearImpulse(thrust, rightPoint,true);		// left thruster
	else if (rightThrustOn)
		physicsBody->ApplyLinearImpulse(thrust, leftPoint,true);			// right thruster

	// add particles as the engine exhaust, and update sounds
	addThrusterParticles(delta, leftThrustOn, rightThrustOn);
	updateThrusterSounds(leftThrustOn, rightThrustOn);

}

void PlayerShip::addThrusterParticles(float delta, bool left, bool right) {

	b2Vec2 leftPoint( physicsBody->GetWorldPoint( leftThrustPoint) );
	b2Vec2 rightPoint( physicsBody->GetWorldPoint(rightThrustPoint) );

	if (left)
		ParticleManager::addExhaustParticles(delta, getSFVector(rightPoint));
	if (right)
		ParticleManager::addExhaustParticles(delta, getSFVector(leftPoint));
}

void PlayerShip::updateThrusterSounds(bool left, bool right) {
	SoundManager::setLeftThrust(left);
	SoundManager::setRightThrust(right);
}

// returns a number between 0 and 1 denoting health count / maximum health
float PlayerShip::getHealthCountRatio() {
	return (float)healthCount / SHIP_MAX_HEALTH;
}

// returns a number between 0 and 1 denoting fuel count / fuel capacity
float PlayerShip::getFuelCountRatio() {
	return fuelCount / FUEL_CAPACITY;
}

int PlayerShip::getResourceCapacity() {
	return RESOURCE_CAPACITY;		// might change in future (ship upgrades)
}

// based on distance to planet
double PlayerShip::getGravityMagnitude() {
	Vector2f direction = homePlanet->getPosition() - getPosition();
	return (GRAVITY_CONSTANT / getLengthSquared(direction));	
}

void PlayerShip::handleCollision(GameObject* objectHit, const Vector2f& contactPoint)
{
	std::cout << "Ship collided with object# " << objectHit <<
	"\nShip Position: " << physicsBody->GetWorldCenter().x << ", " << physicsBody->GetWorldCenter().y <<
	"\nShip Speed: " << physicsBody->GetLinearVelocity().Length() << 
	"\nShip Velocity: (x: " << physicsBody->GetLinearVelocity().x << " y: " << physicsBody->GetLinearVelocity().y <<
	")" << std::endl;

	if (objectHit->getType() == MINE)		// damage will be calculated by mine
		return;

	if( ( (abs(physicsBody->GetLinearVelocity().x - objectHit->getPhysicsBody()->GetLinearVelocity().x) > MIN_SPEED_DIFF_TO_DAMAGE
		|| abs(physicsBody->GetLinearVelocity().y - objectHit->getPhysicsBody()->GetLinearVelocity().y) > MIN_SPEED_DIFF_TO_DAMAGE)
		&& damageTimer <= 0) || objectHit->getType() == PLANET ) {

		damageTimer = TIME_INTERVAL_DAMAGE;
		float angle = getAngleToTargetDegrees(getVelocity(), getPosition(), objectHit->getPosition());
		int	 damage = calculateDamage(angle, physicsBody->GetLinearVelocity().Length());

		if(objectHit->getType() == PLANET) {
			std::cout << "Player crashed into planet = instant death (does not bounce off)" << "\n";
			damage = SHIP_MAX_HEALTH * 2;
		}

		if(damage > 0) {

			std::cout << "Angle: " << angle << std::endl;
			std::cout << "Estimated Damage: " << damage << std::endl;
			updateHealth(damage);

			if      (damage < 8)  SoundManager::play(SoundManager::PLAYER_COLLISION_LIGHT);
			else if (damage < 20) SoundManager::play(SoundManager::PLAYER_COLLISION_MEDIUM);
			else				  SoundManager::play(SoundManager::PLAYER_COLLISION_STRONG);
		}
	}		
	else if (damageTimer <= 0) {	// if hit wasn't strong enough to damage player, we still need to wait before another one happens
		SoundManager::play(SoundManager::PLAYER_COLLISION_LIGHT);		
		damageTimer = TIME_INTERVAL_DAMAGE;
	}
}

void PlayerShip::startHarpoon(int i) {
	Vector2f directionToMouse = getNormalized(mouseWorldCoordinates - getPosition());
	Vector2f velocity		  = directionToMouse * HARPOON_SPEED;

	harpoon[i].Launch(getPosition(), velocity + this->getVelocity(), (i == LEFT_HARPOON ? true : false));
}

// call when fuel or resource count is changed
void PlayerShip::updateMass() {			
	mass = SHIP_MASS + (fuelCount * FUEL_MASS_PER_UNIT) + (getResourceCountAll() * RESOURCE_MASS_PER_UNIT);
	// must set density of fixture then have body recalculate mass
	physicsBody->GetFixtureList()->SetDensity(mass / area);
	physicsBody->ResetMassData();	
	//std::cout << "Player ship new mass = " << (mass) << "\n";
}

int PlayerShip::calculateDamage(float degreeAngle, float speedMagnitude) {
	int damage = int(BASE_DAMAGE * abs((90 - degreeAngle)/90) * (speedMagnitude / SPEED_DAMAGE));
	return (int)(damage * OVERALL_DAMAGE_SCALE);
}

void PlayerShip::updateHealth(int damage) {
	healthCount = max(0, healthCount - damage);
	damageLastFrame += damage;

	if(healthCount <= 0) {
		isDead = true;
		std::cout << "YOU ARE DEAD" << std::endl;
	}
}

// returns the count of all resources of all types
int	PlayerShip::getResourceCountAll() {
	int sum = 0; 
	for (int i = 0; i < NUM_RESOURCE_TYPES; i++)
		sum += resourceCount[i];
	return sum;
}

// returns the capacity of the ship to carry resources
float PlayerShip::getResourceCapacityRatio() {
	return ((float)getResourceCountAll() / RESOURCE_CAPACITY);
}

// returns amount of resources that were unable to add (due to full capacity)
int PlayerShip::addToResourceCount(int index, int amount) {

	int currTotal = getResourceCountAll();
	int overflow = 0;
	
	if (currTotal + amount > RESOURCE_CAPACITY) {
		overflow = (currTotal + amount) - RESOURCE_CAPACITY;
		amount -= overflow;
		updateMass();
	}

	resourceCount[index] += amount;
	updateMass();
	//std::cout << "Player resources : " << resourceCount[0] << "," << resourceCount[1] << "," << resourceCount[2] << "\n";
	return overflow;	// usually will be zero
}

// checks if player wants to convert any resources they have into fuel
void PlayerShip::checkForResourceToFuel() {

	if ( oneLetUp && (Keyboard::isKeyPressed(Keyboard::Num1) || Keyboard::isKeyPressed(Keyboard::Numpad1)) ) {
		oneLetUp = false;
		convertResourceToFuel(0);
	}
	if ( twoLetUp && (Keyboard::isKeyPressed(Keyboard::Num2) || Keyboard::isKeyPressed(Keyboard::Numpad2)) ) {
		twoLetUp = false;
		convertResourceToFuel(1);
	}
	if ( threeLetUp && (Keyboard::isKeyPressed(Keyboard::Num3) || Keyboard::isKeyPressed(Keyboard::Numpad3)) ) {
		threeLetUp = false;
		convertResourceToFuel(2);
	}

	// check for keys let up
	if (!Keyboard::isKeyPressed(Keyboard::Num1)) oneLetUp = true;
	if (!Keyboard::isKeyPressed(Keyboard::Num2)) twoLetUp = true;
	if (!Keyboard::isKeyPressed(Keyboard::Num3)) threeLetUp = true;
}

void PlayerShip::convertResourceToFuel(int index) {

	// make sure there is at least one resource, and fuel is not at capacity
	if (resourceCount[index] != 0 && fuelCount < FUEL_CAPACITY) {
		resourceCount[index]--;
		fuelCount = min(FUEL_CAPACITY, fuelCount + RESOURCE_TO_FUEL[index]);
		updateMass();	// changes when fuel or resource changes
	}
}

bool PlayerShip::isWithinPlanetFieldRadius() {

	if (getDistance(getPosition(), homePlanet->getPosition()) < homePlanet->getFieldRadius())
		return true;
	else
		return false;
}

// if docking has commenced, this makes approriate updates
void PlayerShip::updateDocking(float delta) {

	if (dockStage == NOT_DOCKING)
		checkForDockingStart(); 
	else if (dockStage == DOCKING_BAD_TRAJ) {
		// check if ship passed through (outside dock radius)
		if (getDistance(getPosition(), homePlanet->getPosition()) > homePlanet->getDockRadius())
			dockStage = NOT_DOCKING;
	}
	else if (dockStage == DOCKING_ORBITING) {

		// in this stage we are going to overwrite the ships position as needed to do what we want
		dockSpeed -= dockSpeed*(1/dockDist)*1.0*delta;
		dockDist  -= dockDist *0.3*delta;
		
		Vector2f position = getPosition();
		Vector2f toPlanet = getNormalized(homePlanet->getPosition() - position);

		// move ship at a right angle from direction to planet
		Vector2f moveDir(toPlanet.y, -toPlanet.x);
		if (dockDirection == -1) { moveDir.x = -moveDir.x; moveDir.y = -moveDir.y; }
		position += moveDir * (float)(dockSpeed * delta);								

		// move ship closer to planet
		double actualDist = dockDist + SHIP_WIDTH/2.0 + homePlanet->getRadius();
		position += toPlanet * (float)( getDistance(position, homePlanet->getPosition()) - actualDist );

		// set position in box2d
		setPosition(position);

		// adjust ship's rotation so bottom of ship will touch down on planet
		float idealAngle = getAngleOfVectorDegrees(homePlanet->getPosition(), position) + 90.0f;
		float currAngle  = getRotation();

		if (getAngleDistanceDegrees(idealAngle, currAngle) < 5.0f)
			setRotation(idealAngle);
		else  {
			float rot = (dockDirection == -1 ? -30.0f : 30.0f);		// vary rotation direction based on docking direction
			setRotation(currAngle - delta * rot);
		}

		// landed ?
		if (dockDist < 0.5f) {
			std::cout << "Docking : ship has landed (refuel and unload resources happens), launch in 2 seconds" << "\n";
			dockStage = DOCKING_LANDED;
			dockTime = 0.0;
		}

	}
	else if (dockStage == DOCKING_LANDED) {
	
		float fuelInc = FUEL_CAPACITY / DOCKING_LANDED_TIME;
		fuelCount	  = min(fuelCount + fuelInc * delta, FUEL_CAPACITY);
		healthCount   = min(healthCount + 1, SHIP_MAX_HEALTH);

		// i am assuming that enough frames will always occur in this stage for all resources to be unloaded
		// could do it all at once, but this way we get a quick rising and falling of display bars
		for (int i = 0; i < NUM_RESOURCE_TYPES; i++) {
			if (resourceCount[i] > 0 && homePlanet->getResourcesCurrNeeded(i) > 0) {	// player holds onto excess
				homePlanet->addToResourceCount(i, 1);
				resourceCount[i] = max(resourceCount[i] - 1, 0);
			}
		}
		
		dockTime += delta;
		if (dockTime > DOCKING_LANDED_TIME) {
			dockStage = DOCKING_LAUNCHING;
			std::cout << "Docking : Launching......." << "\n";
			dockTime = 0.0;
			float angle = getAngleOfVectorDegrees(homePlanet->getPosition(), getPosition()) + 90.0f;
			std::cout << "launch angle : " << angle << "\n";
			setRotation( angle );
			setVelocity( 0.0f, 0.0f); 
			reverseThrust = false;
		}
	}
	else if (dockStage == DOCKING_LAUNCHING) {		

		// ship is not active yet in box2d so we need to manually update position
		// velocity is calculated based on how far into launch stage we are
		Vector2f direction = getPosition() - homePlanet->getPosition();
		setVelocity( getNormalized(direction) * (float)(dockTime / DOCKING_LAUNCHING_TIME * DOCKING_LAUNCH_SPEED) );
		setPosition(getPosition() + getVelocity() * delta);
		addThrusterParticles(delta, true, true);
		updateThrusterSounds(true, true);

		dockTime += delta;
		if (dockTime > DOCKING_LAUNCHING_TIME)  {
			dockStage = NOT_DOCKING;
			physicsBody->SetActive(true);
			physicsBody->SetAngularVelocity(0.0f);
			std::cout << "Docking : End of launching....." << "\n";
		}
	}
}

void PlayerShip::checkForDockingStart() {

	// if already docking or not within dock radius, then no change
	if (getDistance(getPosition(), homePlanet->getPosition()) > homePlanet->getDockRadius())
		return;

	// first check and make sure angle is approximately a right angle to a vector from ship to planet center
	Vector2f toPlanet = getNormalized(homePlanet->getPosition() - getPosition());
	Vector2f velocity = getNormalized(getVelocity());

	float angle  = getDegrees( acos(dotProduct(toPlanet, velocity)) );
	float velMag = (float)getLength(getVelocity());		// "speed" of ship

	if (angle < DOCKING_CRIT_ANGLE || velMag > DOCKING_SPEED_MAX || velMag < DOCKING_SPEED_MIN ) {
		dockStage = DOCKING_BAD_TRAJ;
		std::cout << "Docking : bad trajectory" << "\n";

		std::string s;

		if (velMag > DOCKING_SPEED_MAX)
			s = "Velocity is too fast to initiate auto-dock\n";
		else if (velMag < DOCKING_SPEED_MIN)
			s = "Velocity is too slow to initiate auto-dock\n";

		if (angle < DOCKING_CRIT_ANGLE)
			s += "Trajectory is too steep to initiate auto-dock";

		Notification::StartMessage((char*)s.c_str());
	}
	else {
		dockStage = DOCKING_ORBITING;
		Notification::StartMessage("Trajectory accepted : auto-dock initiated");

		// find the direction of the trajectory around the planet
		Vector2f rightAngle(toPlanet.y, -toPlanet.x);
		angle = getDegrees( acos(dotProduct(rightAngle, velocity)) );
		dockDirection = (angle < 90.0 ? 1 : -1);

		// were gonna take over the ship movement from box2d
		dockSpeed = getLength(getVelocity());
		dockDist  = getDistance(getPosition(), homePlanet->getPosition()) - homePlanet->getRadius() - SHIP_WIDTH/2.0;

		physicsBody->SetActive(false);

		std::cout << "Docking : orbiting stage begun....." << "\n";
		std::cout << "        : dock direction: " << (dockDirection == -1 ? "counter-clockwise" : "clockwise") << "\n";
	}
}
