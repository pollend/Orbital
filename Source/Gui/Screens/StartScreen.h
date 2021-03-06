#pragma once
#include "Gui/Screens/IScreen.h"
#include "Gui/Elements/Slider.h"
#include "Gui/Elements/Btn.h"
class StartScreen : public IScreen
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


		StartScreen(void);
		~StartScreen(void);
	private:

		sf::Sprite mLogo;
		sf::View mGameView;

		Btn mStartButton;
		Btn mHelpButton;
		Btn mExitButton;
		Btn mOptionButton;
};

