#pragma once
#include <SFML/Graphics.hpp>
/*Michael Pollind*/
class ScreenManager;
class IScreen
{
	public:
		IScreen(){};
		virtual ~IScreen(){};
		 virtual void Load(ScreenManager* ScreenManager)=0;
		//updates the screen
		 virtual void Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT)=0;
		//draws the screen
		 virtual void Draw(sf::RenderWindow* Window)=0;
		 //false opaque layers will drop down to the next one, update and draw
		 //true stops the draw and update at that screen layer.
		 virtual bool Opaque()=0;
		 //blocks the update loop, prevents layers below this one from updating
		  virtual bool BlockUpdate()=0;
	protected:

};

