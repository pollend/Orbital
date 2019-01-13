#include "CompletionScreen.h"
#include "TextureManager.h"
#include "ScreenManager.h"
#include "StartScreen.h"
#include "MainMenuBackground.h"

CompletionScreen::CompletionScreen(GameWorld* GameWorld)
{
	mGameWorld = GameWorld;
}


CompletionScreen::~CompletionScreen(void)
{
}


void CompletionScreen::Load(ScreenManager* ScreenManager)
{
	mCompletionSprite.setTexture(*TextureManager::LoadAndRetrieveTexture("Assets//gameCompletionIndicator.png"));
	mCompletionSprite.setPosition(0,0);
	mCompletionSprite.setPosition((1024/2)-((float)mCompletionSprite.getTexture()->getSize().x/2),(768/2)-((float)mCompletionSprite.getTexture()->getSize().y/2));

}

void CompletionScreen::Draw(sf::RenderWindow* Window)
{
	Window->draw(mCompletionSprite);
}

void CompletionScreen::Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT)
{
	//update the game world with the given pointer
	//mGameWorld->Update(DeltaT);
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		//pops the screen and reloads the game
		ScreenManager->PopScreen();
		mGameWorld->Reload();
	}

	//pop all the screen and loads the main menu
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		ScreenManager->PopAllScreens();
		ScreenManager->AddScreen(new MainMenuBackground());
		ScreenManager->AddScreen(new StartScreen());
	}
}