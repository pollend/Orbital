
#include <iostream>
#include <SFML\Audio.hpp>
#include <SFML/Graphics.hpp>
#include "PhysicsManager.h"
#include "ScreenManager.h"
#include "ParticleManager.h"
#include "StartScreen.h"
#include "SoundManager.h"
#include "MusicManager.h"
#include "GameScreen.h"
#include "MainMenuBackground.h"
#include "Settings.h"
#include "Cursor.h"
#define SCREEN_WIDTH	1024
#define SCREEN_HEIGHT	768
#define BITS_PER_PIXEL	32

void DisplayFPS(int& numFrames, sf::Time& elapsedTime, sf::Time& fpsTime);

int main()
{
	Settings::LoadSettings();

	sf::RenderWindow* Window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BITS_PER_PIXEL), "Momentus");
	Window->setMouseCursorVisible(false);
	Cursor mouseCursor;
	bool   lostFocus = false;

	//loads the music manager and sets up the tracks
	MusicManager::Load();
	//loads the sound manager
	SoundManager::load();
	//load the particle manager
	ParticleManager::load();
	
	// used to compute fps
	sf::Clock clock;
	sf::Time lElapsedTime = clock.getElapsedTime();
	sf::Time fpsTime = clock.getElapsedTime();				
	int numFrames = 0;

	// seed the random-number generator
	srand((unsigned int)time(NULL));	

	ScreenManager*screenManager = new ScreenManager();
	screenManager->AddScreen(new MainMenuBackground());
	screenManager->AddScreen(new StartScreen());

	while (Window->isOpen())
	{
		sf::Event Event;
		sf::Vector2u ws;
		sf::Vector2i pos;

		while (Window->pollEvent(Event))
		{
			switch (Event.type)
			{
			case sf::Event::Closed:
				Window->close();
				break;
			case sf::Event::Resized:

				ws.y = Window->getSize().y;
				ws.x = (sf::Uint16)(ws.y * ((double)SCREEN_WIDTH / SCREEN_HEIGHT));
				Window->setSize(ws);
				pos.x = (sf::VideoMode::getDesktopMode().width - ws.x) / 2;
				pos.y = (sf::VideoMode::getDesktopMode().height - ws.y) / 2;

				if (pos.y < 20)
					pos.y = 0;

				Window->setPosition(pos);
				break;

			case sf::Event::LostFocus:
				lostFocus = true;
				break;
			case sf::Event::GainedFocus:
				lostFocus = false;
				break;
			default:
				break;
			}
		}

		if (lostFocus)
			continue;

		float lFrameTime = clock.getElapsedTime().asSeconds() - lElapsedTime.asSeconds();
		lElapsedTime = clock.getElapsedTime();

		DisplayFPS(numFrames, lElapsedTime, fpsTime);
		if(lFrameTime >1)
		lFrameTime =0;

		screenManager->Update(Window, lFrameTime);

		//update the sounds
		SoundManager::setMasterVolume(Settings::getSoundScale());
		MusicManager::setMasterVolume(Settings::getMusicManagerScale());
		MusicManager::Update(lFrameTime);

		//draws the screen manager
		Window->clear();
		screenManager->Draw(Window);
		mouseCursor.draw(Window);
		Window->display();
		
	}

	delete screenManager;
	delete Window;
	ParticleManager::destroy();
	SoundManager::destroy();
	MusicManager::Destroy();
	
	screenManager = 0;
	Window = 0;

	return 0;
}

void DisplayFPS(int& numFrames, sf::Time& elapsedTime, sf::Time& fpsTime)
{
	// Calculate/Display FPS
	numFrames++;
	if (elapsedTime.asSeconds() >= fpsTime.asSeconds() + 1.0f) 
	{
		std::cout << "fps = " << numFrames << "\n";
		fpsTime = elapsedTime;
		numFrames = 0;
	}
}