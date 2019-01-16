#include "Btn.h"
#include "Game/TextureManager.h"
Btn::Btn(void){
}

Btn::Btn(std::string Released, std::string Depressed)
{
	
	Btn::mBtnDepresed = TextureManager::LoadAndRetrieveTexture(Depressed);
	Btn::mBtnReleased= TextureManager::LoadAndRetrieveTexture(Released);

	 Btn::mBtnSprite.setTexture(*Btn::mBtnReleased);
	 Btn::mIsPressed = false;
}

Btn::Btn(std::string Released)
{
	Btn::mBtnReleased = TextureManager::LoadAndRetrieveTexture(Released);
	Btn::mBtnDepresed = TextureManager::LoadAndRetrieveTexture(Released);
	Btn::mBtnSprite.setTexture(*Btn::mBtnReleased);
	Btn::mIsPressed = false;
}
sf::Vector2u Btn::getSize()
{
	return Btn::mBtnReleased->getSize();
}

void Btn::Draw(sf::RenderWindow* Window)
{
	if( Btn::IsMousePositionIntersectingButton(Window))
	{
		 Btn::mBtnSprite.setTexture(*Btn::mBtnDepresed,false);
	}
	else
	{
		 Btn::mBtnSprite.setTexture(*Btn::mBtnReleased,false);
	}
	Window->draw( Btn::mBtnSprite);

}

bool Btn::IsButtonPressedAndReleased(sf::RenderWindow* Window)
{
	bool lPress = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	if(!lPress && Btn::mIsPressed)
	{
		if(Btn::IsMousePositionIntersectingButton(Window))
		{
			return true;
		}
	}
	Btn::mIsPressed = lPress;
	return false;
}

void Btn::SetPosition(sf::Vector2f Position)
{
	Btn::mBtnSprite.setPosition(Position);
}

bool Btn::IsMousePositionIntersectingButton(sf::RenderWindow* Window)
{
	
	//memory varaibles
	sf::Vector2u lBtnSize =Btn::mBtnReleased->getSize();
	sf::Vector2f lBtnPosition = Btn::mBtnSprite.getPosition() ;
	sf::Vector2f lMousePosition =Window->convertCoords(sf::Mouse::getPosition((*Window)));
	

	//checks if the mouse is intersecting the button
	if(lMousePosition.x > lBtnPosition.x && lMousePosition.x < lBtnPosition.x +lBtnSize.x && lMousePosition.y > lBtnPosition.y && lMousePosition.y < lBtnPosition.y +lBtnSize.y)
	{
		return true;
	}

	return false;
}

Btn::~Btn(void)
{
}
