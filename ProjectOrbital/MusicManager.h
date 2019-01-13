#pragma once
#include <SFML\Audio.hpp>
#include "Settings.h"
class MusicManager
{
	private:
		class Track
		{
		
			public:
				Track(std::string file);
				float mPitchRate;
				float mPitch;
				float mTargetVolume;
				float mCurrentVolume;
				float mVolumeRate;
				sf::Music mTrack;
		};

		static Track* mTrack[];
		
		static float masterVolume;

		enum Tracks
		{
			AMBIENT,
			HOMEPLANET
		};
		//change the tracks pitch with the current rate of pitch change
		static void ChangePitch(Tracks NameOfTrack,float pitch);
		//change the tracks pitch setting the pitch and the change rate
		static void ChangePitch(Tracks NameOfTrack,float pitch, float rate);
		//set the pitch immediately
		static void SetPitch(Tracks NameOfTrack,float pitch);

		//change the volume of the track with the current rate
		static void ChangeVolume(Tracks NameOfTrack,float volume);
		//change the volume of the track with both the volume and the rate
		static void ChangeVolume(Tracks NameOfTrack,float volume, float rate);
		//set the volume immediately
		static void SetVolume(Tracks NameOfTrack,float volume);
	 public:
		//loads the music
		static void Load();

		static void setMasterVolume(float volume);
		static float getMasterVolume() { return masterVolume / 100.0f; }

		//starts to play
		static void Update(float DeltaT);
		//distance
		static void DistanceToHome(float distanceToHome);
		//deletes each track
		static void Destroy();

};

