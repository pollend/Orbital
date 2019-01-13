#include "ParticleManager.h"
#include "OrbitsMath.h"
#include <iostream>

using namespace sf;


const float PLUS1_SIZE = 35.0f,			// size of the plus 1 particle
			PLUS1_LIFE = 1.7f,			// lifetime of the plus 1 particle
			PLUS1_SPEED = 60.0f,
			PLUS1_SPAN = 1.0f;			

const int	NUM_EXHAUST_PARTICLES = 8;	// per thruster per frame
const float EXHAUST_SIZE = 0.25f,		// size of exhaust particles
			EXHAUST_LIFE = 0.15f,		// in seconds
			EXHAUST_SPAN = 0.4f,		// controls how much exhaust angle can vary from straight behind ship
			EXHAUST_SPEED = 20.0f;		// how fast exhaust shoots out of back of ship

const int   NUM_DUST_PARTICLES = 15;
const float DUST_SIZE = 0.4f,
			DUST_LIFE = 0.75f,
			DUST_SPEED = 1.25f;

const int	NUM_MINE_PARTICLES = 150;
const float MINE_LIFE = 1.5f,
			MINE_SIZE = 0.25f,
			MINE_AVG_SPEED = 20.0f,
			MINE_VAR_SPEED = 30.0f;	

const float SPEC_LIFE = 5.0f,
			SPEC_SIZE = 0.15f,
			SPEC_AVG_SPEED = 20.0f,
			SPEC_VAR_SPEED = 10.0f;	

std::list<Particle> ParticleManager::particleList;	// list of active particles, once they die, they are removed from list
Texture		ParticleManager::plus1Texture;
Texture		ParticleManager::particleTexture;
Texture		ParticleManager::asteroidDustTexture;
PlayerShip* ParticleManager::playerShip;
View		ParticleManager::lastFrameView;
Vector2f	ParticleManager::windowSize;

void ParticleManager::load() {
	plus1Texture.loadFromFile("Assets//plus1_texture.png");
	particleTexture.loadFromFile("Assets//particle_texture.png");
	asteroidDustTexture.loadFromFile("Assets//dust_texture.png");
}

void ParticleManager::destroy() {
	particleList.clear();
}

void ParticleManager::clearAllParticles() {
	particleList.clear();
}

void ParticleManager::update(float delta) {

	for (std::list<Particle>::iterator it = particleList.begin(); it != particleList.end(); /* done inside loop*/) {

		// check for particle life over
		it->aliveTime += delta;
		if (it->aliveTime >= it->lifeTime) {
			it = particleList.erase(it);
			continue;
		}

		// update position
		it->sprite.move(it->velocity * delta);

		// update color value if needed
		if (it->linearColor) {
			double t = it->aliveTime / it->lifeTime;	// t starts at 1 -> 0
			Color c = it->startColor;
			c.r += (Uint8)((it->endColor.r - it->startColor.r) * t);
			c.g += (Uint8)((it->endColor.g - it->startColor.g) * t);
			c.b += (Uint8)((it->endColor.b - it->startColor.b) * t);
			c.a += (Uint8)((it->endColor.a - it->startColor.a) * t);
			it->sprite.setColor(c);
		}

		// goto next particle
		it++;	
	}

}

void ParticleManager::draw(sf::RenderWindow* window) {

	// render all particles that use the game world view
	for (std::list<Particle>::iterator it = particleList.begin(); it != particleList.end(); it++) {
		if (!it->defaultView)
			window->draw(it->sprite);
	}

	lastFrameView = window->getView();
	windowSize	  = window->getDefaultView().getSize();

	// render all particles that use the default view (screen coordinates)
	window->setView(window->getDefaultView());
	for (std::list<Particle>::iterator it = particleList.begin(); it != particleList.end(); it++) {
		if (it->defaultView)
			window->draw(it->sprite);
	}

	window->setView(lastFrameView);
}

void ParticleManager::addPlus1Particle(Color color) {

	Particle p;
	p.lifeTime = PLUS1_LIFE;	
	p.linearColor = true;
	p.startColor = color;
	p.endColor	 = Color(color.r, color.g, color.b, 0);

	p.sprite.setTexture(plus1Texture);
	p.sprite.setOrigin(plus1Texture.getSize().x / 2.0f, plus1Texture.getSize().y / 2.0f);
	p.sprite.setColor(color);

	// find a velocity that comes out the back of the ship
	double dir = (getRadians(playerShip->getRotation()) - PI_OVER2) + (randFloat(PLUS1_SPAN) - PLUS1_SPAN/2.0);
	p.velocity = playerShip->getVelocity() + Vector2f((float)-cos(dir), (float)-sin(dir)) * PLUS1_SPEED;		
	
	// find transformed origin point in screen coordinates
	p.defaultView = true;
	Vector2f upperLeft = lastFrameView.getCenter() - lastFrameView.getSize() / 2.0f;
	Transform t;
	t.scale(windowSize.x / lastFrameView.getSize().x, windowSize.y / lastFrameView.getSize().y);
	t.translate(-upperLeft);
	p.sprite.setPosition( t.transformPoint(playerShip->getPosition()) );

	float scale = PLUS1_SIZE / plus1Texture.getSize().x;
	p.sprite.setScale(scale, scale);

	particleList.push_front(p);		// add particle to list
}

void ParticleManager::addExhaustParticles(float delta, const Vector2f& startPosition) {

	double shipDir = playerShip->getPhysicsBody()->GetAngle() - PI_OVER2;

	if (playerShip->isReverseThrustOn())
		shipDir += PI;

	int numParticles = (int)(delta * 800.0);

	for (int i = 0; i < numParticles; i++) {

		Particle p;
		p.lifeTime = EXHAUST_LIFE + randFloat(EXHAUST_LIFE / 2.0f);

		// both particle speed and direction get a little bit of randomness
		double dir	  = shipDir + (randFloat(EXHAUST_SPAN) - EXHAUST_SPAN/2.0);
		float speed   = EXHAUST_SPEED + randFloat(EXHAUST_SPEED / 2.0f) - (EXHAUST_SPEED / 4.0f);
		p.velocity    = playerShip->getVelocity() + Vector2f((float)-cos(dir), (float)-sin(dir)) * speed;		

		p.linearColor = true;
		p.startColor = Color(255, 255, 255, 255);
		p.endColor	 = Color(255, 255, 255, 0);

		p.sprite.setTexture(particleTexture);
		p.sprite.setOrigin(particleTexture.getSize().x / 2.0f, particleTexture.getSize().y / 2.0f);
		p.sprite.setPosition(startPosition);	// a little randomness here too (but only in direction perpendicular to shipdirection)

		float scale = EXHAUST_SIZE / particleTexture.getSize().x;
		p.sprite.setScale(scale, scale);

		particleList.push_front(p);		// add particle to list
	}

	if (randFloat() < delta)  {

		Particle p;
		p.lifeTime = 3.0f;

		double dir	  = shipDir + (randFloat(EXHAUST_SPAN) - EXHAUST_SPAN/2.0);
		float speed   = 7.5f + randFloat(3.0f) - 1.5f;
		p.velocity    = playerShip->getVelocity() + Vector2f((float)-cos(dir), (float)-sin(dir)) * speed;	

		p.linearColor = true;
		p.startColor = Color(200,200,200);
		p.endColor   = Color(200,200,200,0);

		p.sprite.setTexture(particleTexture);
		p.sprite.setOrigin(particleTexture.getSize().x / 2.0f, particleTexture.getSize().y / 2.0f);
		p.sprite.setPosition(startPosition);	// a little randomness here too (but only in direction perpendicular to shipdirection)

		float scale = SPEC_SIZE / particleTexture.getSize().x;
		p.sprite.setScale(scale, scale);

		particleList.push_front(p);		// add particle to list
	}
}

void ParticleManager::addAsteroidDustParticles(const Vector2f& location) {

	for (int i = 0; i < NUM_DUST_PARTICLES; i++) {

		Particle p;
		p.lifeTime = DUST_LIFE;

		// both particle speed and direction get a little bit of randomness
		double dir	  = randFloat((float)(PI * 2.0));
		float speed   = DUST_SPEED + randFloat(DUST_SPEED) - (DUST_SPEED / 2.0f);
		p.velocity    = Vector2f((float)-cos(dir), (float)-sin(dir)) * speed;	

		p.linearColor = true;
		p.startColor = Color(200, 200, 200, 200);
		p.endColor	 = Color(128, 128, 128, 0);

		p.sprite.setTexture(asteroidDustTexture);
		p.sprite.setOrigin(asteroidDustTexture.getSize().x / 2.0f, asteroidDustTexture.getSize().y / 2.0f);
		p.sprite.setPosition(location);		

		float scale = DUST_SIZE / asteroidDustTexture.getSize().x;
		p.sprite.setScale(scale, scale);

		particleList.push_front(p);		// add particle to list
	}

}

void ParticleManager::addMineExplosionParticle(const sf::Vector2f location) {
	
	for (int i = 0; i < NUM_MINE_PARTICLES; i++) {

		Particle p;
		p.lifeTime = MINE_LIFE + randFloat(1.5f) - 0.75f;

		float speed = MINE_AVG_SPEED + randFloat(MINE_VAR_SPEED) - (MINE_VAR_SPEED/2.0f);
		p.velocity  = getNormalized(Vector2f(randFloat() - 0.5f, randFloat() - 0.5f)) * speed;

		p.sprite.setTexture(particleTexture);
		p.sprite.setOrigin(particleTexture.getSize().x / 2.0f, particleTexture.getSize().y / 2.0f);
		p.sprite.setPosition(location);		

		p.linearColor = true;
		p.startColor = Color(255, 50, 50, 255);
		p.endColor   = Color(255, 0, 0, 0);

		float scale = MINE_SIZE / particleTexture.getSize().x;
		p.sprite.setScale(scale, scale);

		particleList.push_front(p);		// add particle to list
	}

}

void ParticleManager::addSpecParticles(const sf::Vector2f& location, int numSpecs, const Color& color) {

	for (int i = 0; i < numSpecs; i++) {

		Particle p;
		p.lifeTime = SPEC_LIFE;

		float speed = SPEC_AVG_SPEED + randFloat(SPEC_VAR_SPEED) - (SPEC_VAR_SPEED/2.0f);
		p.velocity  = getNormalized(Vector2f(randFloat() - 0.5f, randFloat() - 0.5f)) * speed;

		p.sprite.setTexture(particleTexture);
		p.sprite.setOrigin(particleTexture.getSize().x / 2.0f, particleTexture.getSize().y / 2.0f);
		p.sprite.setPosition(location);		

		p.linearColor = true;
		p.startColor = color;
		p.endColor   = Color(color.r, color.g, color.b, 0);

		float scale = SPEC_SIZE / particleTexture.getSize().x;
		p.sprite.setScale(scale, scale);

		particleList.push_front(p);		// add particle to list
	}

}

void ParticleManager::addSpecParticles(const sf::Vector2f& location, int numSpecs) {
	addSpecParticles(location, numSpecs, Color::White);
}
