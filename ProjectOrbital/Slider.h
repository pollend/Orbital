#pragma once
#include <SFML/Graphics.hpp>
class Slider
{
public:
	Slider(float X, float Y, float Width, std::string DisplayText);
	Slider(void);
	~Slider(void);

	void Update(sf::RenderWindow* Window);
	void Draw(sf::RenderWindow* Window);
	//get the percent of the bar
	float getPercent();
	void  setPercent(float percent);
private:
	sf::RectangleShape mInnerRectangle;
	sf::RectangleShape mSlideBar;
	float mLeft;
	float mRight;
	bool mLockMouse;
	sf::Text mInfoText;
};

