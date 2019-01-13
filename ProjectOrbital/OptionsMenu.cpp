#include "OptionsMenu.h"
#include <iostream>
#include "StartScreen.h"
#include "ScreenManager.h"
#include "SoundManager.h"
#include "MusicManager.h"
void OptionsMenu::Load(ScreenManager* ScreenManager)
{
	mBack = Btn("Assets//MenuObjects//Options//Back.png","Assets//MenuObjects//Options//Back_Selected.png");
	mBack.SetPosition(sf::Vector2f((1024/2) - (((float)mBack.getSize().x)/2),600));

	mSoundSlider = Slider((1024/2)-(200/2),200,200,"Sound");
	mMusicSlider = Slider((1024/2)-(200/2),300,200,"Music");

	mSoundSlider.setPercent(SoundManager::getMasterVolume());
	mMusicSlider.setPercent(MusicManager::getMasterVolume());
}

void OptionsMenu::Draw(sf::RenderWindow* Window)
{
	mSoundSlider.Draw(Window);
	mMusicSlider.Draw(Window);
	mBack.Draw(Window);
}
void OptionsMenu::Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT)
{
	Settings::setMusicManagerScale(mMusicSlider.getPercent());
	Settings::setSoundScale(mSoundSlider.getPercent());

	mSoundSlider.Update(Window);
	mMusicSlider.Update(Window);



	if(mBack.IsButtonPressedAndReleased(Window))
	{
		ScreenManager->PopScreen();
		ScreenManager->AddScreen(new StartScreen());
	}
}


OptionsMenu::OptionsMenu(void)
{
}


OptionsMenu::~OptionsMenu(void)
{
}
