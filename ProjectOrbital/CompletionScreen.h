#pragma once
#include "IScreen.h"
#include "GameWorld.h"

class CompletionScreen : public IScreen
{
public:
	virtual void Load(ScreenManager* ScreenManager);
	virtual void Draw(sf::RenderWindow* Window);
	virtual void Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT);
	virtual bool Opaque(){return false;}
	virtual bool BlockUpdate(){return true;}

	CompletionScreen(GameWorld* GameWorld);
	~CompletionScreen(void);
private:
	GameWorld *mGameWorld;

	sf::Sprite mCompletionSprite;
};