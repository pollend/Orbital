#include "GameOverScreen.h"
#include "TextureManager.h"
#include "ScreenManager.h"
GameOverScreen::GameOverScreen(GameWorld* GameWorld)
{
	mGameWorld = GameWorld;
}


GameOverScreen::~GameOverScreen(void)
{
}


void GameOverScreen::Load(ScreenManager* ScreenManager)
{
	mGameOverSprite.setTexture(*TextureManager::LoadAndRetrieveTexture("Assets//gameOverIndicator.png"));
	mGameOverSprite.setPosition(0,0);
	mGameOverSprite.setPosition((1024/2)-((float)mGameOverSprite.getTexture()->getSize().x/2),(768/2)-((float)mGameOverSprite.getTexture()->getSize().y/2));

}

void GameOverScreen::Draw(sf::RenderWindow* Window)
{
	Window->draw(mGameOverSprite);
}

void GameOverScreen::Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT)
{
	//update the game world with the given pointer
	//mGameWorld->Update(DeltaT);
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		//pops the screen and reloads the game
		ScreenManager->PopScreen();
		mGameWorld->Reload();
	}
}
