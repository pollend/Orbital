#include "Notification.h"
#include "Game/TextureManager.h"
#include "Utility/OrbitsMath.h"
#include <iostream>

using namespace sf;

#define NOTIFICATION_DISPLAY_TIME	5 
#define TEXT_Y_OFFSET				5


float Notification::mDrawTime;
float Notification::y;

sf::Texture* Notification::mNotificationImage;
sf::Sprite   Notification::mNotificationSprite;
sf::Text	 Notification::mNotificationText;

char textBuffer[TEXT_BUFFER_SIZE];


void Notification::Load(std::string Image)
{
	mNotificationImage = TextureManager::LoadAndRetrieveTexture(Image);
	mNotificationSprite.setTexture(*mNotificationImage);

	mDrawTime = -1;

	mNotificationText.setCharacterSize(20);
}

void Notification::Draw(sf::RenderWindow* Window)
{
	if(mDrawTime >= 0)
	{			
		Window->draw(mNotificationSprite);
		Window->draw(mNotificationText);
	}
}

void Notification::Update(float DeltaT)
{
	if(mDrawTime >= 0)
	{
		mDrawTime -= DeltaT;

		y = (float)(768.0 - sin(PI * ((NOTIFICATION_DISPLAY_TIME - mDrawTime) / NOTIFICATION_DISPLAY_TIME)) * mNotificationImage->getSize().y);

		//std::cout << "Position: " << y << std::endl;

		mNotificationSprite.setPosition(1024/2 - mNotificationSprite.getGlobalBounds().width / 2, y);
		mNotificationText.setPosition(1024/2 - mNotificationText.getGlobalBounds().width / 2, y + TEXT_Y_OFFSET);
	}
}

void Notification::StartMessage(char* text) 
{
	mNotificationText.setString(text);
	mDrawTime = NOTIFICATION_DISPLAY_TIME;
	mNotificationSprite.setPosition(0.0f, (768.0f + mNotificationImage->getSize().y));
}