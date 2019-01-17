#include "StartScreen.h"
#include "Gui/ScreenManager.h"
#include "Gui/Screens/GameScreen.h"
#include <iostream>
#include "Game/Settings.h"
#include "Game/TextureManager.h"
#include "Gui/Screens/OptionsMenu.h"
#include "Gui/Screens/HelpScreen.h"

void StartScreen::Load(ScreenManager* ScreenManager)
{

	StartScreen::mStartButton = Btn("Assets//MenuObjects//MainMenu//start.png","Assets//MenuObjects//MainMenu//start_hover.png");
	StartScreen::mStartButton.SetPosition(sf::Vector2f((1024/2) - (((float)StartScreen::mStartButton.getSize().x)/2),200));

	StartScreen::mHelpButton = Btn("Assets//MenuObjects//MainMenu//help.png","Assets//MenuObjects//MainMenu//help_hover.png");
	StartScreen::mHelpButton.SetPosition(sf::Vector2f((1024/2) - (((float)StartScreen::mHelpButton.getSize().x)/2),300));

	StartScreen::mOptionButton = Btn("Assets//MenuObjects//MainMenu//options.png","Assets//MenuObjects//MainMenu//options_hover.png");
	StartScreen::mOptionButton.SetPosition(sf::Vector2f((1024/2) - (((float)StartScreen::mOptionButton.getSize().x)/2),400));

	mExitButton =  Btn("Assets//MenuObjects//MainMenu//quit.png","Assets//MenuObjects//MainMenu//quit_hover.png");
	mExitButton.SetPosition(sf::Vector2f((1024/2) - (((float)StartScreen::mExitButton.getSize().x)/2),500));

	mLogo.setTexture(*TextureManager::LoadAndRetrieveTexture("Assets//MenuObjects//MainMenu//Logo.png"));
	mLogo.setPosition(sf::Vector2f((1024/2) - (((float)mLogo.getTexture()->getSize().x)/2),0));

	std::cout << "Start Screen" << "\n";
}

void StartScreen::Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT)
{


	if(mStartButton.IsButtonPressedAndReleased(Window))
	{
		ScreenManager->PopAllScreens();
		ScreenManager->AddScreen(new GameScreen());
	}
	if(mHelpButton.IsButtonPressedAndReleased(Window))
	{
		ScreenManager->PopAllScreens();
		ScreenManager->AddScreen(new HelpScreen());
	}
	if(mExitButton.IsButtonPressedAndReleased(Window))
	{
		Window->close();
	}
	if(mOptionButton.IsButtonPressedAndReleased(Window))
	{
		ScreenManager->PopScreen();
		ScreenManager->AddScreen(new OptionsMenu());
	}


}


void StartScreen::Draw(sf::RenderWindow* Window)
{
	StartScreen::mStartButton.Draw(Window);
	StartScreen::mHelpButton.Draw(Window);
	StartScreen::mExitButton.Draw(Window);
	StartScreen::mOptionButton.Draw(Window);
}
StartScreen::StartScreen(void)
{
}
StartScreen::~StartScreen(void)
{
}
