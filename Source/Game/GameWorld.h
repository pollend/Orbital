#pragma once

#include "Game/entities/Asteroid.h"
#include "Game/Background.h"
#include "Game/CollectorBeam.h"
#include "Game/Entities/Mine.h"
#include "../source/game/PlayerShip.h"
#include "../source/game/Planet.h"
#include "../source/game/PhysicsManager.h"
#include <SFML/Graphics.hpp>

#define TEXT_BUFFER_SIZE 100

/* GameWorld class - I picture this class as the top-level container/controller for all game objects/logic
   GameScreen should contain an instance of this class
   Things like status displays should probably not be here, I think GameScreen should handle that
*/
class GameWorld {

	private:

		// physics
		PhysicsManager physics;					// interface to box2D

		// camera
		sf::View cameraView;					// this view makes up the current camera view
		float	 cameraZoom;					// 1.0 = default, < 1 = bigger objects, > 1 = smaller objects

		// player ship
		PlayerShip playerShip;					// game object that represents player's ship

		// asteroids
		Asteroid* asteroids;					// array of asteroids
		sf::Texture* asteroidTexture;			// same texture for all asteroids

		// mines
		Mine* mines;
		void checkMinesRespawn();

		// planet
		Planet planet;
		sf::Texture planetTexture;

		// background
		Background background;

		char textBuffer[TEXT_BUFFER_SIZE];		// used by all on-screen text as a temporary buffer

		// game over/completion
		float gameOverTimer;
		bool isGameOver;
		bool isGameComplete;

		// screen shake
		bool isShaking;
		float shakeElapsed;
		float shakeSize;
		void startScreenShake();


	public:

		GameWorld();
		~GameWorld();

		void Load();
		void Reload();
		void Destroy();
		void Update(float delta);
		void Draw(sf::RenderWindow* Window);

		// getters
		sf::View*	getView()			{ return &cameraView; }
		PlayerShip* getPlayerShip()		{ return &playerShip; }
		Planet*		getPlanet()			{ return &planet; }
		bool		getIsGameOver()		{ return isGameOver; }
		bool		getIsGameComplete() { return isGameComplete; }
};