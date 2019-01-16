#include "SoundManager.h"
#include <iostream>

using namespace sf;

const int NUM_EFFECTS = 12;				// number of different sound effects
const int MAX_EFFECTS = 50;				// maximum number of sounds played at once (can have multiple of same sound)

const float DEFAULT_MASTER_VOLUME = 0.50f;
const float THRUSTER_VOLUME		  = 0.6f;

SoundBuffer* SoundManager::effects;
Sound*		 SoundManager::sounds;

SoundBuffer SoundManager::leftThrustBuffer, SoundManager::rightThrustBuffer;
Sound	    SoundManager::leftThrustSound, SoundManager::rightThrustSound;

float		SoundManager::masterVolume;

void SoundManager::load() {

	effects = new SoundBuffer[NUM_EFFECTS];
	sounds  = new Sound[MAX_EFFECTS];

	// load sounds here
	effects[HARPOON_FIRE_1].loadFromFile("Assets//Sounds//_Harpoon 1 Fire.wav");
	effects[HARPOON_FIRE_2].loadFromFile("Assets//Sounds//_Harpoon 2 Fire.wav");
	effects[HARPOON_CUT].loadFromFile("Assets//Sounds//_Harpoon Cut.wav");
	effects[HARPOON_HIT].loadFromFile("Assets//Sounds//_Harpoon Hit.wav");
	effects[ASTEROID_COLLISION_1].loadFromFile("Assets//Sounds//_Asteroid Collision 1.wav");
	effects[ASTEROID_COLLISION_2].loadFromFile("Assets//Sounds//_Asteroid Collision 2.wav");
	effects[ASTEROID_COLLISION_3].loadFromFile("Assets//Sounds//_Asteroid Collision 3.wav");
	effects[PLAYER_COLLISION_LIGHT].loadFromFile("Assets//Sounds//_Spaceship Collision Light.wav");
	effects[PLAYER_COLLISION_MEDIUM].loadFromFile("Assets//Sounds//_Spaceship Collision Medium.wav");
	effects[PLAYER_COLLISION_STRONG].loadFromFile("Assets//Sounds//_Spaceship Collision Strong.wav");
	effects[MINING_SOUND].loadFromFile("Assets//Sounds//_Mining Sound.wav");
	effects[EXPLOSION_LARGE].loadFromFile("Assets//Sounds//_Explosion Large.wav");

	leftThrustBuffer.loadFromFile("Assets//Sounds//_Thruster 1.wav");
	rightThrustBuffer.loadFromFile("Assets//Sounds//_Thruster 1.wav");
	leftThrustSound.setBuffer(leftThrustBuffer);
	rightThrustSound.setBuffer(rightThrustBuffer);
	leftThrustSound.setVolume(THRUSTER_VOLUME * 100.0f);
	rightThrustSound.setVolume(THRUSTER_VOLUME * 100.0f);
	leftThrustSound.setLoop(true);
	rightThrustSound.setLoop(true);

	if (MUTE_GAME) 
		setMasterVolume(0.0f);
	else
		setMasterVolume(DEFAULT_MASTER_VOLUME);
}

// needs to be called or sounds won't get deleted
void SoundManager::destroy() {			
	delete[] effects;
	delete[] sounds;
}

// master volume is between 0.0 and 1.0, to calculate volume of an effect -> masterVolume * volume
void SoundManager::setMasterVolume(float volume) {
	masterVolume = volume;
	leftThrustSound.setVolume(THRUSTER_VOLUME * masterVolume * 100.0f);
	rightThrustSound.setVolume(THRUSTER_VOLUME * masterVolume * 100.0f);
}

// volume is a number between 0.0 and 1.0 inclusive where 0.0 = muted and 1.0 = full volume
void SoundManager::play(Effect effect, float volume) {
	if (MUTE_GAME) return;

	// search for an open sound slot
	for (int i = 0; i < MAX_EFFECTS; i++) {
		if (sounds[i].getStatus() == Sound::Stopped) {

			sounds[i].setBuffer(effects[effect]);
			sounds[i].setVolume(masterVolume * volume * 100.0f);	// sfml takes volume between 0 and 100.0
			sounds[i].play();
			break;
		}
	}
}

void SoundManager::play(Effect effect) {

	play(effect, 1.0f);
}

void SoundManager::setLeftThrust(bool on) {

	if (on && (leftThrustSound.getStatus() == Sound::Paused || leftThrustSound.getStatus() == Sound::Stopped))
		leftThrustSound.play();
	else if (!on && leftThrustSound.getStatus() == Sound::Playing)
		leftThrustSound.pause();
}

void SoundManager::setRightThrust(bool on) {

	if (on && (rightThrustSound.getStatus() == Sound::Paused || rightThrustSound.getStatus() == Sound::Stopped))
		rightThrustSound.play();
	else if (!on && rightThrustSound.getStatus() == Sound::Playing)
		rightThrustSound.pause();
}