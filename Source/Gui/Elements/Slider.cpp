#include "Slider.h"


Slider::Slider(float X, float Y, float Width, std::string DisplayText)
{
	mInnerRectangle.setPosition(X,Y);
	mInnerRectangle.setSize(sf::Vector2f(Width,10));
	mInnerRectangle.setOrigin(0,5);

	mSlideBar.setOrigin(sf::Vector2f(5,10));
	mSlideBar.setPosition(X,Y);
	mSlideBar.setSize(sf::Vector2f(10,20));

	mLeft = X;
	mRight = X + Width;

	mInfoText.setPosition(X,Y-30);
	mInfoText.setString(DisplayText);
	mInfoText.setCharacterSize(15);


}

float Slider::getPercent()
{
	return (mSlideBar.getPosition().x-mLeft)/(mRight-mLeft);
}

void Slider::setPercent(float percent) {

	float x = mLeft + (mRight - mLeft) * percent;
	float y = mSlideBar.getPosition().y;

	mSlideBar.setPosition(x,y);
}

void Slider::Update(sf::RenderWindow* Window)
{
	//mouse is in the box
	if(mSlideBar.getGlobalBounds().contains(Window->mapPixelToCoords(sf::Mouse::getPosition((*Window)))) || mLockMouse)
	{
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			mLockMouse = true;
			mSlideBar.setPosition((float)sf::Mouse::getPosition((*Window)).x,mSlideBar.getPosition().y);
			if(mSlideBar.getPosition().x >mRight)
			{
				mSlideBar.setPosition(mRight,mSlideBar.getPosition().y);
			}
			else if(mSlideBar.getPosition().x <mLeft)
			{
				mSlideBar.setPosition(mLeft,mSlideBar.getPosition().y);
			}
		}
		else
		{
			mLockMouse = false;
		}
	}
}

void Slider::Draw(sf::RenderWindow* Window)
{
	Window->draw(mInnerRectangle);
	Window->draw(mSlideBar);
	Window->draw(mInfoText);
}

Slider::Slider(void)
{
}


Slider::~Slider(void)
{
}
