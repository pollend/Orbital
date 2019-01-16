#include "GameScreen.h"
#include "Gui/ScreenManager.h"
#include "Gui/screens/GameScreenMenu.h"
#include "GameScreenHud.h"
#include "StartScreen.h"
#include "Gui/screens/CompletionScreen.h"
#include "../source/gui/screens/GameOverScreen.h"
#include "../source/game/ParticleManager.h"
#include "Game/Settings.h"
#include <iostream>

void GameScreen::Load(ScreenManager* ScreenManager)
{
	ScreenManager->AddScreen(new GameScreenHud(&gameWorld));

	// load the game world (load art, sounds, generate asteroids, etc...)
	gameWorld.Load();
	ParticleManager::clearAllParticles();

	std::cout << "GameScreen" << "\n";

}
 void GameScreen::Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT)
{
	//Trigger for the pause menu
	if(mEscapeButtonState && !sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		//adds the gamescreen menu overlay
		ScreenManager->AddScreen(new GameScreenMenu(&gameWorld));
	}
	mEscapeButtonState = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

	gameWorld.Update(DeltaT);

	//check if the game is over and then trigger the game over screen overlay
	if(gameWorld.getIsGameOver()) 
		ScreenManager->AddScreen(new GameOverScreen(&gameWorld));	
	else if(gameWorld.getIsGameComplete())
		ScreenManager->AddScreen(new CompletionScreen(&gameWorld));
}

void GameScreen::Draw(sf::RenderWindow* Window)
{
	// draws player, asteroids, background, etc...
	gameWorld.Draw(Window);


}

GameScreen::GameScreen()
{
	//set the state to false to prevent the pause menu from just popping
	mEscapeButtonState= false;
}

GameScreen::~GameScreen(void)
{
}
