#pragma once
#include <SFML/Graphics.hpp>

class Btn
{
public:
	Btn(void);
	
	//button with two drawable state;
	//mouse is hovering over the sprite
	//mouse isn't hovering over the sprite
	Btn(std::string Released, std::string Depressed);
	//button only has one texture
	Btn(std::string Released);

	~Btn(void);
	//Draws the button
	void Draw(sf::RenderWindow* Window);
	//returns wheather the button is pressed and then released
	bool IsButtonPressedAndReleased(sf::RenderWindow* Window);
	//sets the position of the button
	void SetPosition(sf::Vector2f Position);
	//returns the size of the button
	sf::Vector2u getSize();
private:
	bool mIsPressed;

	//the two textures for the button
	sf::Texture* mBtnDepresed;
	sf::Texture* mBtnReleased;
	//the buttons sprite
	sf::Sprite mBtnSprite;

	//checks to see if the mouse is intersecting the button
	bool IsMousePositionIntersectingButton(sf::RenderWindow* Window);
	
};

