#pragma once
#include "iscreen.h"
#include "../../../ProjectOrbital/GameWorld.h"
class GameOverScreen :public IScreen
{
public:
	virtual void Load(ScreenManager* ScreenManager);
	virtual void Draw(sf::RenderWindow* Window);
	virtual void Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT);
	virtual bool Opaque(){return false;}
	virtual bool BlockUpdate(){return true;}


	GameOverScreen(GameWorld* GameWorld);
	~GameOverScreen(void);
private:
	GameWorld *mGameWorld;

	sf::Sprite mGameOverSprite;
};

