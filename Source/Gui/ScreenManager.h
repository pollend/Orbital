#pragma once
#include <stack>
#include <SFML/Graphics.hpp>
#include "Game/Settings.h"

class IScreen;
/*Michael Pollind*/
class ScreenManager
{
private:
		//stack of screens
	std::vector<IScreen*> mScreens;
	std::vector<IScreen*> mDeadScreens;

	Settings* mSettings;
public:
	ScreenManager(void);
	~ScreenManager(void);

	//updates the screen
	void Update(sf::RenderWindow* Window,float DeltaT);

	//draws the screens
	void Draw(sf::RenderWindow* Window);

	void PopAllScreens();
	//pops the screen off the stack
	void PopScreen();
	//removes the screen by its pointer
	void RemoveScreen(IScreen* Screen);
	
	//adds a screen to the stack
	void AddScreen(IScreen* Screen);

};

