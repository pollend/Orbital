#include "HelpScreen.h"
#include "ScreenManager.h"
#include "MainMenuBackground.h"
#include "StartScreen.h"

HelpScreen::HelpScreen(void)
{
}


HelpScreen::~HelpScreen(void)
{
}


void HelpScreen::Load(ScreenManager* ScreenManager)
{
	mPage = new Btn[3];
	mIndex = 0;
	mPage[1] = Btn("Assets//controls.png");
	mPage[2] = Btn("Assets//docking.png");
	mPage[0] = Btn("Assets//objective.png");

}

void HelpScreen::Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT)
{
	if(mPage[mIndex].IsButtonPressedAndReleased(Window))
	{
		mIndex++;
		if(mIndex >= 3)
		{
			ScreenManager->PopAllScreens();
			ScreenManager->AddScreen(new MainMenuBackground());
			ScreenManager->AddScreen(new StartScreen());

		}
	}
}

void HelpScreen::Draw(sf::RenderWindow* Window)
{
	mPage[mIndex].Draw(Window);
}
