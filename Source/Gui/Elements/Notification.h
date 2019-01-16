#pragma once
#include <SFML/Graphics.hpp>

#define TEXT_BUFFER_SIZE 100

class Notification
{
public:

	static void Load(std::string Image);
	static void Update(float DeltaT);
	static void Draw(sf::RenderWindow* Window);

	static void StartMessage(char* text);


private:
	static float mDrawTime;
	static float y;

	static sf::Texture*	mNotificationImage;
	static sf::Sprite	mNotificationSprite;
	static sf::Text		mNotificationText;

};