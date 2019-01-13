#pragma once
#include "PhysicsManager.h"
#include "IScreen.h"
#include "Asteroid.h"
#include "Background.h"
class MainMenuBackground: public IScreen
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

		MainMenuBackground(void);
		~MainMenuBackground(void);
	private:
		PhysicsManager* mPhysicsManager;
		Asteroid* mAsteroids;
		sf::View cameraView;	

		//position for the background stars
		float index;
		sf::Vector2f* mPosition;
		Background mBackground;

		sf::Sprite mMainBack;
		float test;


};

