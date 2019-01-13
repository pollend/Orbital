#pragma once
#include "IScreen.h"
#include "Slider.h"
#include "Btn.h"
#include "Settings.h"
class OptionsMenu: public IScreen
{
public:
	virtual void Load(ScreenManager* ScreenManager);
	//draws the gamecreen
	virtual void Draw(sf::RenderWindow* Window);
	//updates the gamescreen
	virtual void Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT);
	//states the opacity of the gamescreen
	virtual bool Opaque(){return false;}
	virtual bool BlockUpdate(){return false;}


	OptionsMenu(void);
	~OptionsMenu(void);

private:
		Btn mBack;

		Slider mSoundSlider;
		Slider mMusicSlider;
};

