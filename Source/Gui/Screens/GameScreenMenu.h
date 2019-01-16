#pragma once
#include "IScreen.h"
#include "Gui/elements/Btn.h"
#include "Game/GameWorld.h"
class GameScreenMenu : public IScreen
{
public:
	GameScreenMenu(GameWorld *GameWorld);
	~GameScreenMenu(void);
	virtual void Load(ScreenManager* ScreenManager);
	virtual void Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT);
	virtual void Draw(sf::RenderWindow* Window);
	virtual bool Opaque(){return false;}
	virtual bool BlockUpdate(){return true;}


private:
	bool mEscapeButtonState;
	
	sf::Sprite mPauseSprite;
	Btn mContinue;
	Btn mRestart;
	Btn mMainMenu;
	GameWorld* mGameWorld;
	sf::RectangleShape mBackgroundBlock;

};

