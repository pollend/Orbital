#pragma once
#include "IScreen.h"
#include "Game/GameWorld.h"
#include "Gui/Elements/Notification.h"

class GameScreenHud :public IScreen
{
public:
	virtual void Load(ScreenManager* ScreenManager);
	virtual void Draw(sf::RenderWindow* Window);
	virtual void Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT);
	virtual bool Opaque(){return false;}
	virtual bool BlockUpdate(){return false;}


	GameScreenHud(GameWorld* GameWorld);
	~GameScreenHud(void);
private:
	sf::Sprite mUIBackdrop;

	sf::RectangleShape* mResourceWhiteOutline;
	sf::RectangleShape* mResources;
	sf::Text* mResourceText;
	sf::Text  mResourceInfoText;

	sf::RectangleShape* mMissionResourcesWhiteOutline;
	sf::RectangleShape* mMissionResources;
	sf::Text* mMissionResourcesText;
	sf::Text  mMissionText;

	sf::RectangleShape mFuel;
	sf::RectangleShape mHealth;

	sf::RectangleShape mArrow;

	GameWorld* mGameWorld;

};