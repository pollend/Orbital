#pragma once
#include "IScreen.h"
#include "GameWorld.h"
#include <SFML\System\Vector2.hpp>
/*Michael Pollind*/
class GameScreen :public IScreen
{
public:
	virtual void Load(ScreenManager* ScreenManager);
	virtual void Draw(sf::RenderWindow* Window);
	virtual void Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT);
	virtual bool Opaque(){return true;}
	virtual bool BlockUpdate(){return false;}


	GameScreen();
	~GameScreen(void);
private:
	GameWorld gameWorld;
	bool mEscapeButtonState;
};

