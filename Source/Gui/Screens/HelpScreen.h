#pragma once
#include "IScreen.h"
#include "Gui/elements/Btn.h"
class HelpScreen : public IScreen
{
public:
	HelpScreen(void);
	~HelpScreen(void);

	virtual void Load(ScreenManager* ScreenManager);
	virtual void Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT);
	virtual void Draw(sf::RenderWindow* Window);
	virtual bool Opaque(){return false;}
	virtual bool BlockUpdate(){return false;}
private:
	Btn* mPage;
	int mIndex;
};

