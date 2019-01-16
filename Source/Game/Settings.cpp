#include "Settings.h"
#include <iostream>
#include <fstream>
#include <string>

double Settings::mSoundScale;
double Settings::mMusicManagerScale;

void Settings::LoadSettings()
{
	//create a stream
	std::ifstream lSettingsFile;
	lSettingsFile.open(FILE_SETTINGS);
	//attempts the read the settings file
	if (lSettingsFile.is_open())
	{
		std::string line;
		int index = 0;
		while ( lSettingsFile.good() )
		{
		  std::getline(lSettingsFile,line);
			switch(index)
			{
				case SOUND_VOLUME:
					mSoundScale = std::atof(line.c_str());
					break;
				case MUSIC_VOLUME:
					mMusicManagerScale = std::atof(line.c_str());
					break;

			}

		  index++;
		}
		lSettingsFile.close();
	}
	else
	{
		Settings::mMusicManagerScale = 1;
		Settings::mSoundScale = 1;
		//create new save
		lSettingsFile.close();
		Settings::SaveSettings();
	}

}

void Settings::SaveSettings()
{
	std::ofstream lSettingFile;
	lSettingFile.open(FILE_SETTINGS);
	lSettingFile.clear();
	lSettingFile << mSoundScale << "\n";
	lSettingFile << mMusicManagerScale << "\n";
	lSettingFile.close();
}

void Settings::setMusicManagerScale(float scale)
{
	//scale can't be larger then 1
	if(scale > 1)
	{
		scale = 1;
	}
	Settings::mMusicManagerScale = scale;
	//save the settings to the file
	Settings::SaveSettings();

}
void Settings::setSoundScale(float scale)
{
	//scale can't be larger then 1
	if(scale > 1)
	{
		scale = 1;
	}
	Settings::mSoundScale = scale;
	//save the settings to the file
	Settings::SaveSettings();
}

