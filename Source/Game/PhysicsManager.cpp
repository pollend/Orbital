#include <iostream>
#include "PhysicsManager.h"
#include "../../ProjectOrbital/OrbitsMath.h"
#include "../../ProjectOrbital/Harpoon.h"

// these masks are used to control what collides with what
uint16 PLAYER_SHIP_MASK  = 0x01,
	   ASTEROID_MASK	 = 0x02,
	   PLANET_FIELD_MASK = 0x04,
	   PLANET_MASK		 = 0x08,
	   HARPOON_MASK		 = 0x10,
	   CHAIN_MASK		 = 0x20,
	   COLLECT_BEAM_MASK = 0x40,
	   MINE_MASK		 = 0x80;

const float ROPE_WIDTH		 = 0.5f,	// lowering this causes lots of bugs, but it can look strange since the rope sis narrow
			ROPE_FRICTION	 = 0.01f,
			ROPE_DENSITY	 = 0.1f,
			ROPE_RESTITUTION = 0.0f;

const float MINE_MASS		 = 0.03f;

PhysicsManager::PhysicsManager()
{
	Load();
}

PhysicsManager::~PhysicsManager()
{
	// When a world is deleted, all memory reserved for bodies, fixtures,
	// and joints is freed automatically.
	delete mWorld;

	mWorld = 0;
}

void PhysicsManager::Load() 
{
	playerShip = NULL;
	//b2Vec2 gravity(0.0f, GRAVITY);
	b2Vec2 gravity(0.0f, 0.0f);

	fTimeStep = UPDATE_INTERVAL;
	mVelocityIterations = VELOCITY_ITERATIONS;
	mPositionIterations = POSITION_ITERATIONS;

	mWorld = new b2World(gravity);
	mWorld->SetAllowSleeping(true);	
	mContactListener = OrbitsContactListener();
	mWorld->SetContactListener(&mContactListener);
}

void PhysicsManager::Reload() 
{
	delete mWorld;
	mWorld = 0;

	Load();
}

void PhysicsManager::Update(float DeltaT)
{
	fTimeAccumulator += DeltaT;

	//std::cout << "Physics Time Accumulated = " << fTimeAccumulator << "\n";

	// For best results, timestep should be fixed when updating physics.
	if(fTimeAccumulator >= fTimeStep * MAX_CYCLES_PER_FRAME)
		fTimeAccumulator = fTimeStep;

	while(fTimeAccumulator >= fTimeStep)
	{
		fTimeAccumulator -= fTimeStep;
		mWorld->Step(fTimeStep, mVelocityIterations, mPositionIterations);				
	}

	// Prevent time accumulated from becoming negative.
	if(fTimeAccumulator < 0)
		fTimeAccumulator = 0;
}

b2Body* PhysicsManager::AddPlayer(GameObject* userData, float width, float height, sf::Vector2f position)
{
	this->playerShip = userData;		// save for use with harpoon's

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);

	b2PolygonShape shipShape;
	shipShape.SetAsBox(width/2.0f, height/2.0f);
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shipShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.01f;			
	fixtureDef.filter.categoryBits = PLAYER_SHIP_MASK;
	fixtureDef.filter.maskBits	   = (ASTEROID_MASK | PLANET_MASK | MINE_MASK);

	b2Body* body = mWorld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetUserData((void*)userData);

	return body;
}

// this creates the force field around a planet
b2Body* PhysicsManager::AddPlanetField(GameObject* userData, sf::Vector2f position, float fieldRadius) {

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(position.x, position.y);

	b2CircleShape shape;
	shape.m_radius = fieldRadius;
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 1.0f;			
	fixtureDef.filter.categoryBits = PLANET_FIELD_MASK;
	fixtureDef.filter.maskBits	   = (ASTEROID_MASK | MINE_MASK);

	b2Body* body = mWorld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetUserData((void*)userData);

	return body;
}

// this is the physical planet body
b2Body* PhysicsManager::AddPlanet(GameObject* userData, sf::Vector2f position, float radius) {

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(position.x, position.y);

	b2CircleShape shape;
	shape.m_radius = radius;
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f;			
	fixtureDef.filter.categoryBits = PLANET_MASK;
	fixtureDef.filter.maskBits	   = (PLAYER_SHIP_MASK);

	b2Body* body = mWorld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetUserData((void*)userData);

	return body;
}

b2Body* PhysicsManager::AddAsteroid(GameObject* userData, const sf::Vector2f* verts, int numVertices, sf::Vector2f position, sf::Vector2f velocity, float density)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);	

	b2PolygonShape asteroidShape;
	b2Vec2* vec2Verts = new b2Vec2[numVertices];
	for(int i = 0; i < numVertices; ++i)
	{
		vec2Verts[i] = getB2Vector(verts[i]);
	}
	asteroidShape.Set(vec2Verts, numVertices);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &asteroidShape;
	fixtureDef.density = density;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 1.0f;			// without highly elastic collisions, velocities will decrease over time
	fixtureDef.filter.categoryBits = ASTEROID_MASK;
	fixtureDef.filter.maskBits	   = (PLAYER_SHIP_MASK | PLANET_FIELD_MASK | ASTEROID_MASK | HARPOON_MASK | CHAIN_MASK | COLLECT_BEAM_MASK | MINE_MASK);		

	b2Body* body = mWorld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
	body->SetUserData((void*)userData);

	delete vec2Verts;

	return body;
}

b2Body* PhysicsManager::AddMine(GameObject* userData, const sf::Vector2f position, float radius) 
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);	

	b2CircleShape shape;
	shape.m_radius = radius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = (float)(MINE_MASS / (radius * radius * PI));
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 1.0f;			
	fixtureDef.filter.categoryBits = MINE_MASK;
	fixtureDef.filter.maskBits	   = (PLAYER_SHIP_MASK | PLANET_FIELD_MASK | ASTEROID_MASK | HARPOON_MASK | MINE_MASK);		

	b2Body* body = mWorld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	body->SetUserData((void*)userData);

	return body;
}

// for right now, harpoon is just the claw flying through space, once an asteroid is hit, the rope will need to be created
b2Body* PhysicsManager::AddHarpoon(GameObject* userData, sf::Vector2f position, float radius, sf::Vector2f velocity) 
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);

	b2CircleShape shape;
	shape.m_radius = radius;
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;	
	fixtureDef.density = 0.3f;
	fixtureDef.restitution = 1.0f;
	fixtureDef.filter.categoryBits = HARPOON_MASK;
	fixtureDef.filter.maskBits	   = (ASTEROID_MASK | MINE_MASK);

	b2Body* body = mWorld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetLinearVelocity(getB2Vector(velocity));
	body->SetUserData((void*)userData);

	return body;
}

// for right now, we are just going to create a rope joint between the center of player's ship and the object that was stuck with harpoon
// at some point we may want to add a segmented physical rope using revolute joints in addition to the rope joint (like in the testbed)
b2RopeJoint* PhysicsManager::AddHarpoonJoint(b2Body* stuckObject, sf::Vector2f stuckPoint) {

	b2RopeJointDef jointDef;
	jointDef.bodyA = playerShip->getPhysicsBody();
	jointDef.bodyB = stuckObject;
	jointDef.localAnchorA.Set(0.0f, 0.0f),											// anchor to center of player ship
	jointDef.localAnchorB = stuckObject->GetLocalPoint(getB2Vector(stuckPoint));	// and place where harpon stuck into asteroid
	jointDef.collideConnected = true;
	jointDef.maxLength = getDistance( playerShip->getPosition(), stuckPoint ) - 0.001f;

	b2RopeJoint* joint = (b2RopeJoint*)mWorld->CreateJoint(&jointDef);

	return joint;
}

RopeLink* PhysicsManager::AddHarpoonChain(b2Body* stuckObject, sf::Vector2f stuckPoint, int numLinks) {

	// create a chain out of small links, every box2d body in the chain points to the next RopeLink object
	// so the whole chain can be traversed via the first RopeLink' body
	
	b2Vec2 bStuckWorldPoint = getB2Vector(stuckPoint);//stuckObject->GetWorldPoint(getB2Vector(stuckPoint));
	float dist = getDistance( playerShip->getPosition(), stuckPoint );
	double xStep = (playerShip->getPosition().x - bStuckWorldPoint.x) / (double)numLinks;		// dist from link center to end
	double yStep = (playerShip->getPosition().y - bStuckWorldPoint.y) / (double)numLinks;

	// create a rectangle angled in right direction, with origin at one end (not in middle)
	b2PolygonShape shape;
	float angle = (float)-b2Atan2(yStep, xStep);
	shape.SetAsBox(ROPE_WIDTH * 2.0f, (dist / numLinks) * 0.5f, b2Vec2(0.0f, (float)(-dist/numLinks * 0.5)), angle);
	
	// create a single fixture to be used for all links
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.friction = ROPE_FRICTION;
	fd.density = ROPE_DENSITY;
	fd.restitution = ROPE_RESTITUTION;
	fd.filter.categoryBits = CHAIN_MASK;
	fd.filter.maskBits = (ASTEROID_MASK);	

	// start links at asteroid and go towards player

	b2Body* prevBody = stuckObject;		// first link attaches to asteroid
	b2Body* currBody = NULL;
	RopeLink* currLink = NULL;
	RopeLink* firstLink = NULL;

	double x = bStuckWorldPoint.x + xStep * 0.01;
	double y = bStuckWorldPoint.y + yStep * 0.01;

	b2RevoluteJointDef jd;
	jd.collideConnected = false;

	for (int i = 0; i <= numLinks; i++) {

		// create body
		if (i != numLinks) {
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set((float)x, (float)y);
		
			currBody = mWorld->CreateBody(&bd);
			currBody->CreateFixture(&fd);

			currLink = new RopeLink(currBody);
			if (i == 0)
				firstLink = currLink;
		}
		else {	// last joint connects last link to player ship
			currBody = playerShip->getPhysicsBody();
			x = playerShip->getPosition().x;		// not sure if this helps
			y = playerShip->getPosition().y;
		}

		// create a revolute joint from this body to the last one created
		jd.Initialize( prevBody, currBody, b2Vec2((float)x, (float)y) );
		/*jd.enableLimit = true;
		jd.lowerAngle = -PI;
		jd.upperAngle = PI;*/
		/*if (i >= numLinks-1) {	// last joints that connects to player should limit motion (so player doesn't spin independently of rope)
			jd.enableLimit = true;	
			jd.lowerAngle = -0.1f;	// ***************** im really just unsure as to whether this is desired or not, im gonna leave it out for now
			jd.upperAngle = 0.1f;
		}*/

		b2Joint* joint = mWorld->CreateJoint(&jd);

		if (i != 0) {
			if (i != numLinks)
				prevBody->SetUserData(currLink);
			else
				prevBody->SetUserData(NULL);		// dont point last body at player' ship (don't want to delete it when chain is removed)
		}

		prevBody = currBody;
		x += xStep;
		y += yStep;
	}

	// return body of first link
	return firstLink;
}

b2Body* PhysicsManager::AddCollectorBeam(GameObject* userData, sf::Vector2f position, const sf::Vector2f* verts, int numVerts, float radius, float angleWidth) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);	

	b2PolygonShape collectorBeamShape;
	b2Vec2* vec2Verts = new b2Vec2[numVerts];
	for(int i = 0; i < numVerts; ++i)
	{
		vec2Verts[i] = getB2Vector(verts[i]);
	}
	collectorBeamShape.Set(vec2Verts, 8);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &collectorBeamShape;	
	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = COLLECT_BEAM_MASK;
	fixtureDef.filter.maskBits	   = ASTEROID_MASK;		

	b2Body* body = mWorld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetUserData((void*)userData);

	delete vec2Verts;

	return body;
}

bool PhysicsManager::RemoveBody(b2Body* body)
{
	mWorld->DestroyBody(body);
	return true;
}