#pragma once


#define FILE_SETTINGS "settings.bin"

class Settings
{
public:
	 static enum TypeOfSettings{
		SOUND_VOLUME,
		MUSIC_VOLUME
	};

	 	 static void LoadSettings();

	 //gets the scaling value for the sound
	 static float getSoundScale(){return (float)Settings::mSoundScale;}
	  //gets the scaling value for the MusicManager
	  static float getMusicManagerScale(){	return (float)Settings::mMusicManagerScale;}
	 //sets the scaling value for the sounds 0.0-1.0
	  static void setSoundScale(float scale);
	  //sets the scaling value for the MusicManager 0.0-1.0
	  static void setMusicManagerScale(float scale);

private:

	 static double mSoundScale;
	 static double mMusicManagerScale;

	//save settings to a file
	static void SaveSettings();

};

