#include "MusicManager.h"
#include "SoundManager.h"
#include "Settings.h"
#include "OrbitsMath.h"
#include <iostream>

#define CURRENT_NUM_TRACKS 2
#define DEGREGATION_DISTANCE_VALUE 750
#define DEGREGATION_FADEOUT_AMBIENT 200

#define DEFAULT_MUSIC_VOLUME .7f

MusicManager::Track* MusicManager::mTrack[CURRENT_NUM_TRACKS];

float MusicManager::masterVolume;

void MusicManager::Load()
{
	mTrack[AMBIENT] = new Track("Assets/Music/Abyss.ogg");
	mTrack[HOMEPLANET] = new Track("Assets/Music/HomePlanetSlowed.ogg");

	if (MUTE_GAME) 
		setMasterVolume(0.0f);
	else
		setMasterVolume(DEFAULT_MUSIC_VOLUME);

	MusicManager::SetVolume(MusicManager::AMBIENT,0);
	MusicManager::SetVolume(MusicManager::HOMEPLANET,100);
}

// master volume is between 0.0 and 1.0, to calculate volume of an effect -> masterVolume * volume
void MusicManager::setMasterVolume(float volume) {
	masterVolume = volume * 100.0f;
}

void MusicManager::Update(float DeltaT)
{

	//shift audio tracks by the rate
	for(int i = 0; i <CURRENT_NUM_TRACKS; i++)
	{
		Track* lTrack = mTrack[i];
		//start shifting the pitch up or down depending on the rate
	/*	if(lTrack->mPitch > lTrack->mTrack.getPitch())
		{
			lTrack->mTrack.setPitch(lTrack->mTrack.getPitch() + lTrack->mPitchRate * DeltaT);
		}
		else
		{
			lTrack->mTrack.setPitch(lTrack->mTrack.getPitch() - lTrack->mPitchRate * DeltaT);
		}*/
		//start shifting the volume depending on the rate
		if(lTrack->mTargetVolume > lTrack->mTrack.getVolume())
		{
			if(lTrack->mCurrentVolume + lTrack->mVolumeRate * DeltaT > 100)
				lTrack->mCurrentVolume = 100;
			else
				lTrack->mCurrentVolume =lTrack->mCurrentVolume+ lTrack->mVolumeRate * DeltaT;
		}
		else
		{
			if(lTrack->mCurrentVolume - lTrack->mVolumeRate * DeltaT < 0)
				lTrack->mCurrentVolume = 0;
			else
				lTrack->mCurrentVolume  = lTrack->mCurrentVolume - lTrack->mVolumeRate * DeltaT;
		}

		//set the volume at the end and applie the scaler
		lTrack->mTrack.setVolume(lTrack->mCurrentVolume * (masterVolume/100));
	}
}


void MusicManager::ChangePitch(Tracks NameOfTrack,float pitch)
{
	mTrack[NameOfTrack]->mPitch= pitch;
}
void MusicManager::ChangePitch(Tracks NameOfTrack,float pitch, float rate)
{
	mTrack[NameOfTrack]->mPitch = pitch;
	mTrack[NameOfTrack]->mPitchRate = pitch;
}

void MusicManager::SetPitch(Tracks NameOfTrack,float pitch)
{
	mTrack[NameOfTrack]->mPitch = pitch;
	mTrack[NameOfTrack]->mTrack.setPitch(pitch);
}

void MusicManager::ChangeVolume(Tracks NameOfTrack,float volume)
{
	MusicManager::mTrack[NameOfTrack]->mTargetVolume = volume;
}
void MusicManager::ChangeVolume(Tracks NameOfTrack,float volume, float rate)
{
	mTrack[NameOfTrack]->mTargetVolume = volume;
	mTrack[NameOfTrack]->mVolumeRate = rate;
}
void MusicManager::SetVolume(Tracks NameOfTrack,float volume)
{
	mTrack[NameOfTrack]->mTargetVolume = volume;
	mTrack[NameOfTrack]->mCurrentVolume = volume;
}

void MusicManager::DistanceToHome(float distanceToHome)
{
	float homeVolume = min( (DEGREGATION_DISTANCE_VALUE - distanceToHome) / DEGREGATION_DISTANCE_VALUE, 1.0f ) * masterVolume;
	float awayVolume = min( distanceToHome / DEGREGATION_DISTANCE_VALUE, 1.0f ) * masterVolume ; 
	if(awayVolume >= masterVolume)
	{
		float fadeOut = max(min( (DEGREGATION_DISTANCE_VALUE-(distanceToHome-DEGREGATION_DISTANCE_VALUE)) /DEGREGATION_DISTANCE_VALUE, 1.0f ),.2f);
		awayVolume *= fadeOut;
	}

	
	ChangeVolume(HOMEPLANET, homeVolume, 10.0f);
	ChangeVolume(AMBIENT, awayVolume, 10.0f);
}

void MusicManager::Destroy() 
{
	delete mTrack[AMBIENT];
	delete mTrack[HOMEPLANET];
}

//--------------------TRACK----------------------------------------------
MusicManager::Track::Track(std::string file)
{
	mTrack.openFromFile(file);
	mTrack.play();
	mTrack.setLoop(true);
	mPitch = 1;
	mTargetVolume = DEFAULT_MUSIC_VOLUME * 100.0f;
	mPitchRate = .5f;
	mVolumeRate = 1.0f;

	if (MUTE_GAME)
		mTrack.stop();
}