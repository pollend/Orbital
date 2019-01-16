#include "GameWorld.h"
#include "Utility/OrbitsMath.h"
#include "MusicManager.h"
#include "../source/game/ParticleManager.h"
#include "TextureManager.h"
#include "Gui/elements/Notification.h"

#include <iostream>
#include <stdio.h>

using namespace sf;

// camera
const int DEFAULT_CAMERA_WIDTH  = 68;			// width in meters of camera view, this would be the default zoom
const float CAMERA_SPEED_SCALE	= 0.1f,			// controls ratio of player speed to camera zoom
			CAMERA_THRESHOLD	= 0.1f,			// prevents camera from zooming back & forth around ideal zoom
			CAMERA_INCREMENT	= 0.20f,		// controls speed at which camera zooms in & out
			CAMERA_MAX_ZOOM		= 2.0f;			// maximum zoomed out setting (relative to 1.0 as default zoom)

// planet
const float PLANET_RADIUS		= 25.0f,		// radius of planet
			PLANET_DOCK_RADIUS	= 45.0f,		// radius of docking range around planet
			PLANET_FIELD_RADIUS = 125.0f;		// radius of the protective force field around planet

// asteroids		
const double ASTEROID_FIELD_DENSITY = 0.0004;	// this equals # asteroids / (width*height), so asteroids per meters squared
const int	 ASTEROID_FIELD_WIDTH   = 3000,		// width in meters of asteroid field
			 ASTEROID_FIELD_HEIGHT  = 3000;		// height in meters of asteroid field
const int	 NUM_ASTEROIDS = (int)((ASTEROID_FIELD_WIDTH * ASTEROID_FIELD_HEIGHT) * ASTEROID_FIELD_DENSITY);			

// mines
const double MINE_DENSITY = 0.00005;
const int	 NUM_MINES	  = (int)((ASTEROID_FIELD_WIDTH * ASTEROID_FIELD_HEIGHT) * MINE_DENSITY);	

// screen shake
const float	 SHAKE_TIME       = 1.0f,	// length in seconds that screen shaking lasts
			 SHAKE_X_MAX_AMP = 14.0f,	// maximum horizontal displacement of screen shake
			 SHAKE_X_CYCLES  = 20,		// controls how many times the screen goes back & forth during shaking (not exact, varies depending on shake time)
			 SHAKE_Y_MAX_AMP = 10.0f,	// maximum vertical displacement of screen shake
			 SHAKE_Y_CYCLES  = 15;		// controls how many times the screen goes back & forth during shaking (not exact, varies depending on shake time)
const int	 SHAKE_MAX_DAMAGE = 39;		// if damage dealth is greater than this, then maximum shaking will occur

// text size
const int UI_TEXT_SIZE = 20;

// timers
const float GAME_OVER_TIME = 1.0f;

GameWorld::GameWorld() {
	asteroids = NULL;
	mines = NULL;

	planetTexture.loadFromFile("Assets//planetTexture.png");
	asteroidTexture = TextureManager::LoadAndRetrieveTexture("Assets//asteroidTexture.png");
}

GameWorld::~GameWorld() {
	Destroy();
}

void GameWorld::Load() {
	gameOverTimer = GAME_OVER_TIME;
	isGameOver = false;
	isGameComplete = false;

	// initialize camera		
	cameraView.setCenter(0.0, 0.0);
	cameraZoom = CAMERA_MAX_ZOOM;

	// player
	playerShip.Load(&physics, &planet, Vector2f(0,-PLANET_RADIUS-1.5f));
	ParticleManager::setPlayerShip(&playerShip);

	// planet
	planetTexture.setSmooth(true);
	planet.Load(&planetTexture, &physics, Vector2f(0,0), PLANET_RADIUS, PLANET_DOCK_RADIUS, PLANET_FIELD_RADIUS);

	// background
	background.Load(&playerShip);

	// asteroids
	asteroidTexture->setSmooth(true);
	asteroids = new Asteroid[NUM_ASTEROIDS];
	int fieldX = -(ASTEROID_FIELD_WIDTH / 2);
	int fieldY = -(ASTEROID_FIELD_HEIGHT / 2);

	std::cout << "Creating (" << NUM_ASTEROIDS << ") asteroids - ";
	for (int i = 0; i < NUM_ASTEROIDS; i++) {
		asteroids[i].Load(asteroidTexture, &physics, &playerShip, &planet, fieldX, fieldY, ASTEROID_FIELD_WIDTH, ASTEROID_FIELD_HEIGHT);
		if (i % 30 == 0) std::cout << "*";
	}
	std::cout << "\n";

	// mines
	mines = new Mine[NUM_MINES];
	std::cout << "Creating (" << NUM_MINES << ") mines\n";
	checkMinesRespawn();

	isShaking = false;

	Notification::StartMessage("Collect resources for your home planet");
}

void GameWorld::Reload() {

	Destroy();			// delete old asteroids and stuff
	physics.Reload();	// delete old physics world
	Load();				// load up everything again
}

void GameWorld::Destroy() {
	delete[] asteroids;
	asteroids = NULL;
	delete[] mines;
	mines = NULL;
}

void GameWorld::Update(float delta) {

	// update physics
	physics.Update(delta);
	
	// check for player damage to start screen shake
	if (!isShaking) {
		if (playerShip.getDamageLastFrame() != 0)
			startScreenShake();
	}
	else {
		shakeElapsed += delta;
		if (shakeElapsed > SHAKE_TIME)
			isShaking = false;
	}

	// background
	background.Update(delta);

	// update asteroids
	for (int i = 0; i < NUM_ASTEROIDS; i++)
		asteroids[i].Update(delta);

	// update mines
	checkMinesRespawn();
	for (int i = 0; i < NUM_MINES; i++) 
		mines[i].Update(delta);
		
	// update player
	playerShip.Update(delta);

	if(playerShip.getIsDead())
		gameOverTimer -= delta;
	isGameOver = gameOverTimer <= 0;
	isGameComplete = planet.getIsMissionComplete();

	cameraView.setCenter(playerShip.getPosition());

	// set camera zoom based on ship speed
	float ideal = (float)getLength(playerShip.getVelocity()) * CAMERA_SPEED_SCALE;		// target zoom ratio

	// if player is within planet field radius, we should zoom out so they can try to dock
	if (playerShip.isWithinPlanetFieldRadius())
		ideal = 100.0f;		// max zoom

	if (ideal > cameraZoom + CAMERA_THRESHOLD)
		cameraZoom += CAMERA_INCREMENT * delta;
	else if (ideal < cameraZoom - CAMERA_THRESHOLD)
		cameraZoom -= CAMERA_INCREMENT * delta;
	cameraZoom = clamp(cameraZoom, 1.0f, CAMERA_MAX_ZOOM);

	//update the music for the distance of the planet
	MusicManager::DistanceToHome(getDistance(playerShip.getPosition(), planet.getPosition()));

	// update particle effects
	ParticleManager::update(delta);
}

void GameWorld::Draw(RenderWindow* Window) {

	// draw background first (uses its own view)
	background.Draw(Window);

	// set camera view to appropriate size relative to render window		
	float aspectRatio = (float)Window->getSize().y / Window->getSize().x;
	cameraView.setSize((float)DEFAULT_CAMERA_WIDTH, DEFAULT_CAMERA_WIDTH * aspectRatio);	
	cameraView.zoom(cameraZoom);

	if (isShaking) {
		double x = sin(SHAKE_X_CYCLES * shakeElapsed) * ((SHAKE_TIME - shakeElapsed) / SHAKE_TIME) * SHAKE_X_MAX_AMP * shakeSize;
		double y = sin(SHAKE_Y_CYCLES * shakeElapsed) * ((SHAKE_TIME - shakeElapsed) / SHAKE_TIME) * SHAKE_Y_MAX_AMP * shakeSize;
		cameraView.setCenter((float)(playerShip.getPosition().x + x), (float)(playerShip.getPosition().y + y));
	}

	Window->setView(cameraView);

	// draw planet
	planet.Draw(Window);

	// draw asteroids
	for (int i = 0; i < NUM_ASTEROIDS; i++) 
		asteroids[i].Draw(Window);

	// draw mines
	for (int i = 0; i < NUM_MINES; i++) 
		mines[i].Draw(Window);

	// draw players ship
	playerShip.Draw(Window);

	// draw particles
	ParticleManager::draw(Window);

	// ui stuff
	Window->setView(Window->getDefaultView());
}

void GameWorld::checkMinesRespawn() {

	Vector2f position;
	int fieldX = -(ASTEROID_FIELD_WIDTH / 2);
	int fieldY = -(ASTEROID_FIELD_HEIGHT / 2);
	
	for (int i = 0; i < NUM_MINES; i++) {
		if (mines[i].needsRespawn()) {

			bool done = false;

			while (!done) {

				position.x = (float)(rand() % ASTEROID_FIELD_WIDTH  + fieldX);
				position.y = (float)(rand() % ASTEROID_FIELD_HEIGHT + fieldY);

				// make sure not on-screen and not within planet field radius
				done = true;

				if (getDistance(position, planet.getPosition()) < PLANET_FIELD_RADIUS)
					done = false;
				else if (getDistance(position, cameraView.getCenter()) < DEFAULT_CAMERA_WIDTH * CAMERA_MAX_ZOOM)
					done = false;
			}

			mines[i].Load(&physics, &playerShip, position);
		}
	}
}

void GameWorld::startScreenShake() 
{
	isShaking = true;
	shakeElapsed = 0.0f;
	shakeSize = min((float)playerShip.getDamageLastFrame() / SHAKE_MAX_DAMAGE, 1.0f);
}