#pragma once
#include "GameObject.h"
#include "PhysicsManager.h"
#include "Planet.h"
#include "Harpoon.h"
#include "CollectorBeam.h"
#include "Notification.h"
#include <SFML/Graphics.hpp>

extern const int	   NUM_RESOURCE_TYPES;
extern const float	   RESOURCE_RATIO[];		// these ratio's control the distribution of types among all asteroids, must add to 1.0
extern const sf::Color RESOURCE_COLOR[];

class PlayerShip : public GameObject {

	private:
		PhysicsManager*	   physics;
		Planet*			   homePlanet;

		sf::Sprite		   ship;					
		sf::Texture		   shipTexture;

		// harpoons
		Harpoon harpoon[2];			// the two harpoons (left button and right button)
		bool leftLetUp,				// these track the harpoon keys being let go of before being hit again
			 rightLetUp;

		// collector beam
		CollectorBeam collectorBeam;
		float toggleTimer;

		int	  healthCount;			// integer 0 - 100		
		int	  resourceCount[10];	// amount of resources gathered (10 is arbitrary number larger than expected amount)
		float damageTimer;			// time remaining until the ship can be damaged again. 
		float fuelCount;			// amount of remaining fuel
		float mass;					// mass of ship (mass unloaded + mass of fuel + mass of resources gathered)
		float area;					// calculated area of ship' shape (used to compute density)
		int	  damageLastFrame;		// used to track when a change in health occurs

		int  dockStage;				// denotes what (if any) stage of docking procedure with planet is happening
		int  dockDirection;			// 1 = counter-clockwise, -1 = clockwise (this stores which direction ship was heading upon docking start)
		double dockSpeed,			// stores speed of ship while docking (does not use box2d)
			   dockDist,			// distance from closest points of player ship and planet
			   dockTime;			// how long ship has been docked

		// these are updated each frame and are set if player is using either or both
		bool leftThrustOn, rightThrustOn;
		bool reverseThrust;					// true if currently applying a reverse thrust

		// these points define the points in local coordinates where forcd is applied to move the ship
		b2Vec2 leftThrustPoint, midThrustPoint, rightThrustPoint;

		// these track letting go of the keys that convert resources into fuel
		bool oneLetUp, twoLetUp, threeLetUp;	

		sf::Vector2f mouseWorldCoordinates;

		Notification notification;

		double getGravityMagnitude();
		void   startHarpoon(int i);
		void   updateMass();			// call when fuel or resource count is changed
		void   updateThrusters(float delta);
		void   addThrusterParticles(float delta, bool left, bool right);
		void   updateThrusterSounds(bool left, bool right);
		int    calculateDamage(float degreeAngle, float speedMagnitude);
		void   checkForResourceToFuel();
		void   convertResourceToFuel(int index);
		void   checkForDockingStart();	
		void   updateDocking(float delta);

	public:

		PlayerShip();
		~PlayerShip();

		void Load(PhysicsManager *physics, Planet* homePlanet, sf::Vector2f position);
		void Update(float delta);
		void Draw(sf::RenderWindow* Window);

		// getters
		float getFuelCount()			  { return fuelCount; }
		int	  getResourceCountAll();
		int	  getResourceCount(int index) { return resourceCount[index]; }
		int   getResourceCapacity();
		float getMass()					  { return mass; }

		float getHealthCountRatio();	  // returns a number between 0 and 1 denoting health count / maximum health
		float getFuelCountRatio();		  // returns a number between 0 and 1 denoting fuel count / fuel capacity
		float getResourceCapacityRatio(); // returns the capacity of the ship to carry resources

		int	  getDamageLastFrame()	{ return damageLastFrame; }
		bool  isReverseThrustOn()	{ return reverseThrust; }
		bool  isWithinPlanetFieldRadius();

		// setters
		void setFuelCount(float count)				 { fuelCount = count; }
		void setResourceCount(int index, int amount) { resourceCount[index] = amount; }
		void updateHealth(int damage);

		// adders
		int addToResourceCount(int index, int amount);

		// collision
		void handleCollision(GameObject* objectHit, const sf::Vector2f& contactPoint);
};