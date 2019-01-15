#pragma once
#include "PlayerShip.h"
#include <SFML\Graphics.hpp>
#include <list>

class Particle
{
public:
	sf::Sprite	 sprite;
	sf::Vector2f velocity;
	double		 aliveTime;			// time, in seconds that particle has been alive
	double		 lifeTime;			// time, in seconds of the life of the particle
	bool		 linearColor;		// interpolate color linearly over lifespan of particle
	bool		 defaultView;		// if true, particle uses the default view (screen coordinates) rather than world coordinates

	sf::Color startColor, endColor;		// for linear color interpolation

	Particle() { 
		aliveTime = 0.0;
		defaultView = false;
		linearColor = false;
	}
};

// completely static class
class ParticleManager
{
	private:

		static std::list<Particle> particleList;	// list of active particles, once they die, they are removed from list
		static sf::Texture plus1Texture,
						   particleTexture,
						   asteroidDustTexture;

		static PlayerShip* playerShip;

		static sf::View	    lastFrameView;
		static sf::Vector2f windowSize;

	public:

		static void load();
		static void destroy();			
		static void clearAllParticles();
		static void update(float delta);
		static void draw(sf::RenderWindow* window);

		static void setPlayerShip(PlayerShip* ship) { playerShip = ship; }

		static void addPlus1Particle(sf::Color color);
		static void addExhaustParticles(float delta, const sf::Vector2f& startPosition);
		static void addAsteroidDustParticles(const sf::Vector2f& location);
		static void addMineExplosionParticle(const sf::Vector2f location);
		static void addSpecParticles(const sf::Vector2f& location, int numSpecs, const sf::Color& color);
		static void addSpecParticles(const sf::Vector2f& location, int numSpecs);
};