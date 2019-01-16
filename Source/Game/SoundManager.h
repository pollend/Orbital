#pragma once
#include <SFML/Audio.hpp>

#define MUTE_GAME false				// CHANGE THIS IF YOU WANT TO TEMPORARILY MUTE SOUND

// Currently implemented as a purely static class
class SoundManager
{
	private:

		static sf::SoundBuffer* effects;
		static sf::Sound*		sounds;

		static sf::SoundBuffer leftThrustBuffer, rightThrustBuffer;
		static sf::Sound	   leftThrustSound, rightThrustSound;

		static float masterVolume;

	public:

		enum Effect { 
			HARPOON_FIRE_1,
			HARPOON_FIRE_2,
			HARPOON_CUT,
			HARPOON_HIT,
			ASTEROID_COLLISION_1,
			ASTEROID_COLLISION_2,
			ASTEROID_COLLISION_3,
			PLAYER_COLLISION_LIGHT,
			PLAYER_COLLISION_MEDIUM,
			PLAYER_COLLISION_STRONG,
			MINING_SOUND,
			EXPLOSION_LARGE
		}; 

		static void load();
		static void destroy();			// needs to be called or sounds won't get deleted
		static void setMasterVolume(float volume);
		static float getMasterVolume() { return masterVolume; }

		static void play(Effect effect, float volume);
		static void play(Effect effect);

		static void setLeftThrust(bool on);
		static void setRightThrust(bool on);
};


