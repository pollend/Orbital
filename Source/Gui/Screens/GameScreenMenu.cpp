#include "GameScreenMenu.h"
#include "Gui/ScreenManager.h"
#include "Game/TextureManager.h"
#include <iostream>
#include "GameScreen.h"
#include "StartScreen.h"
#include "GameScreenHud.h"
#include "MainMenuBackground.h"

GameScreenMenu::GameScreenMenu(GameWorld *GameWorld)
{
	mGameWorld = GameWorld;
}

GameScreenMenu::~GameScreenMenu(void)
{
	GameScreenMenu::mEscapeButtonState = false;
}

 void GameScreenMenu::Load(ScreenManager* ScreenManager)
 {
	 mEscapeButtonState = false;
	 std::cout<<"GameScreenMenu" << "\n";

	 mPauseSprite = sf::Sprite();
	 mPauseSprite.setTexture(*TextureManager::LoadAndRetrieveTexture("Assets//MenuObjects//Pause//pause_menu_title.png"));
	 mPauseSprite.setPosition(sf::Vector2f((1024/2) - (((float)mPauseSprite.getTexture()->getSize().x)/2),0));

	 mContinue = Btn("Assets//MenuObjects//Pause//continue.png","Assets//MenuObjects//Pause//continue_selected.png");
	 mContinue.SetPosition(sf::Vector2f((1024/2) - (((float)mContinue.getSize().x)/2),200));

	 mRestart = Btn("Assets//MenuObjects//Pause//restart.png","Assets//MenuObjects//Pause//restart_selected.png");
	  mRestart.SetPosition(sf::Vector2f((1024/2) - (((float)mRestart.getSize().x)/2)-200,500));

	mMainMenu=Btn("Assets//MenuObjects//Pause//mainmenu.png","Assets//MenuObjects//Pause//mainmenu_selected.png");
	mMainMenu.SetPosition(sf::Vector2f((1024/2) - (((float)mMainMenu.getSize().x)/2)+200,500));
 
	mBackgroundBlock.setPosition(0,0);
	mBackgroundBlock.setSize(sf::Vector2f(1024,768));
	mBackgroundBlock.setFillColor(sf::Color(0,0,0,200));

 }
 void GameScreenMenu::Draw(sf::RenderWindow* Window)
 {
	 	 Window->draw(mBackgroundBlock);
	 Window->draw(mPauseSprite);
	 mContinue.Draw(Window);
	 mRestart.Draw(Window);
	 mMainMenu.Draw(Window);
 }
 void GameScreenMenu::Update(ScreenManager *ScreenManager,sf::RenderWindow* Window,float DeltaT)
 {
	//just pops the screen off the returns to the game
	if((GameScreenMenu::mEscapeButtonState && !sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) ||mContinue.IsButtonPressedAndReleased(Window))
	{
		ScreenManager->PopScreen();
	}
	//pop all the screens and reload the game
	if(mRestart.IsButtonPressedAndReleased(Window))
	{
		ScreenManager->PopScreen();
		ScreenManager->PopScreen();
		mGameWorld->Reload();
		ScreenManager->AddScreen(new GameScreenHud(mGameWorld));
	}
	//pop all the screen and loads the main menu
	if(mMainMenu.IsButtonPressedAndReleased(Window))
	{
		ScreenManager->PopAllScreens();
		ScreenManager->AddScreen(new MainMenuBackground());
		ScreenManager->AddScreen(new StartScreen());
	}
	GameScreenMenu::mEscapeButtonState = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
 }

