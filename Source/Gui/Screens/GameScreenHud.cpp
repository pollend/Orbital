#include "GameScreenHud.h"
#include <iostream>
#include "Gui/ScreenManager.h"
#include <sstream>  
#include "Game/TextureManager.h"
#include "Utility/OrbitsMath.h"

//RESOURCE BAR///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BAR_SPACING 20
#define WIDTH_BAR 10
#define SIZE_BAR 300
#define X_BAR_OFFSET 20
#define Y_BAR_OFFSET 40
const sf::Color COLOR_RESOURCE[] = { sf::Color(255, 50, 50), sf::Color(50, 255, 50),sf::Color(50, 50, 255) };

#define BOTTOM_BAR 500

#define ARROW_OFFSET 100
#define ARROW_SIZE 10

GameScreenHud::GameScreenHud(GameWorld* GameWorld)
{
	mGameWorld =GameWorld;
}

void GameScreenHud::Load(ScreenManager* ScreenManager)
{
	mUIBackdrop.setTexture(*TextureManager::LoadAndRetrieveTexture("Assets//MenuObjects//GameMenuOverlay//UI_backdrop.png"));
	mUIBackdrop.setPosition(0,0);
	//RESOURCE BAR///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	mResources =new  sf::RectangleShape[NUM_RESOURCE_TYPES];
	mResourceWhiteOutline = new  sf::RectangleShape[NUM_RESOURCE_TYPES];
	mResourceText = new sf::Text[NUM_RESOURCE_TYPES];
	mResourceInfoText.setString("Ship Resources");
	mResourceInfoText.setPosition(BAR_SPACING * 4 - X_BAR_OFFSET / 2, 768 - Y_BAR_OFFSET);
	mResourceInfoText.setCharacterSize(18);
	mResourceInfoText.setRotation(-90);

	mMissionResources = new sf::RectangleShape[NUM_RESOURCE_TYPES];
	mMissionResourcesWhiteOutline = new sf::RectangleShape[NUM_RESOURCE_TYPES];
	mMissionResourcesText = new sf::Text[NUM_RESOURCE_TYPES];
	mMissionText.setString("Resources Needed");
	mMissionText.setPosition(930 + BAR_SPACING - X_BAR_OFFSET, 768 - Y_BAR_OFFSET);
	mMissionText.setCharacterSize(18);
	mMissionText.setRotation(-90);

	for(int x = 0; x < NUM_RESOURCE_TYPES; ++x)
	{
		//set up the resource bars
		mResources[x].setSize(sf::Vector2f(WIDTH_BAR, -SIZE_BAR));
		mResources[x].setFillColor(COLOR_RESOURCE[x]);
		mResources[x].setPosition((float)(BAR_SPACING * x) + X_BAR_OFFSET, 768.0f - Y_BAR_OFFSET);
		mResources[x].setOutlineColor(sf::Color::Red);

		mResourceWhiteOutline[x].setSize(sf::Vector2f(WIDTH_BAR+4,-SIZE_BAR-4));
		mResourceWhiteOutline[x].setFillColor(sf::Color(0,0,0,0));
		mResourceWhiteOutline[x].setOutlineColor(sf::Color::White);
		mResourceWhiteOutline[x].setOutlineThickness(2);
		mResourceWhiteOutline[x].setPosition(mResources[x].getPosition() +sf::Vector2f(-2,2));
	
		mResourceText[x].setPosition(mResources[x].getPosition() + sf::Vector2f((WIDTH_BAR/2),0));
		mResourceText[x].setColor(COLOR_RESOURCE[x]);
		mResourceText[x].setCharacterSize(15);

		//set up the mission resource bars
		mMissionResources[x].setSize(sf::Vector2f(WIDTH_BAR, -SIZE_BAR));
		mMissionResources[x].setFillColor(COLOR_RESOURCE[x]);
		mMissionResources[x].setPosition((934.0f + BAR_SPACING * x) + X_BAR_OFFSET, 768.0f - Y_BAR_OFFSET);
		mMissionResources[x].setOutlineColor(sf::Color::White);

		mMissionResourcesWhiteOutline[x].setSize(sf::Vector2f(WIDTH_BAR+4,-SIZE_BAR-4));
		mMissionResourcesWhiteOutline[x].setFillColor(sf::Color(0,0,0,0));
		mMissionResourcesWhiteOutline[x].setOutlineColor(sf::Color::White);
		mMissionResourcesWhiteOutline[x].setOutlineThickness(2);
		mMissionResourcesWhiteOutline[x].setPosition(mMissionResources[x].getPosition() +sf::Vector2f(-2,2));

		mMissionResourcesText[x].setPosition(mMissionResources[x].getPosition() + sf::Vector2f((WIDTH_BAR/2),0));
		mMissionResourcesText[x].setColor(COLOR_RESOURCE[x]);
		mMissionResourcesText[x].setCharacterSize(15);
	}

	//HEALTH BAR
	mHealth.setPosition(10,30);
	mHealth.setSize(sf::Vector2f(BOTTOM_BAR,10));
	mHealth.setFillColor(sf::Color::Green);
	//FUEL BAR
	mFuel.setPosition(10,60);
	mFuel.setSize(sf::Vector2f(BOTTOM_BAR*.60f,10));
	mFuel.setFillColor(sf::Color::Yellow);
	
	//DIRECTIONL ARROW
	mArrow.setSize(sf::Vector2f(ARROW_SIZE,ARROW_SIZE));
	mArrow.setOrigin(ARROW_SIZE/2,ARROW_SIZE/2);
	mArrow.setTexture(TextureManager::LoadAndRetrieveTexture("Assets//MenuObjects//GameMenuOverlay//arrow.png"));

	Notification::Load("Assets//MenuObjects//GameMenuOverlay//UI_notification.png");
}

GameScreenHud::~GameScreenHud(void)
{
	delete[] mResources;
	delete[] mResourceWhiteOutline;
	delete[] mResourceText;

	delete[] mMissionResources;
	delete[] mMissionResourcesWhiteOutline;
	delete[] mMissionResourcesText;
}


void GameScreenHud::Draw(sf::RenderWindow* Window)
{
	Window->draw(mUIBackdrop);
	//HELATH BAR
	Window->draw(mHealth);
	//FUEL BAR
	Window->draw(mFuel);

	//RESOURCE BAR
	//draws the resource bars
	for(int x = 0; x < NUM_RESOURCE_TYPES; ++x)
	{
		Window->draw(mResourceText[x]);
		Window->draw(mResourceWhiteOutline[x]);
		Window->draw(mResources[x]);

		Window->draw(mMissionResourcesText[x]);
		Window->draw(mMissionResourcesWhiteOutline[x]);
		Window->draw(mMissionResources[x]);
	}
	Window->draw(mResourceInfoText);
	Window->draw(mMissionText);

	// PLANET LOCATOR
	if ( !mGameWorld->getPlayerShip()->isWithinPlanetFieldRadius() )
		Window->draw(mArrow);

	Notification::Draw(Window);
}

void GameScreenHud::Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT){

	//RESOURCE BARS
	int lMaxCapacity = mGameWorld->getPlayerShip()->getResourceCapacity();
	int lAmountLeft = mGameWorld->getPlayerShip()->getResourceCapacity() - mGameWorld->getPlayerShip()->getResourceCountAll();
	//the amount left is 0
	if(lAmountLeft == 0)
	{
		for(int x = 0; x < NUM_RESOURCE_TYPES; ++x)
		{
			int amount=	mGameWorld->getPlayerShip()->getResourceCount(x);
			mResources[x].setScale(sf::Vector2f(1,((float)amount/(float)lMaxCapacity)));
			mResourceWhiteOutline[x].setSize(sf::Vector2f(WIDTH_BAR+4,(-SIZE_BAR*((float)amount/(float)lMaxCapacity))-4));
			//ship is full, set color to red
			mResourceWhiteOutline[x].setOutlineColor(sf::Color::Red);
		}
	}
	else
	{
		for(int x = 0; x < NUM_RESOURCE_TYPES; ++x)
		{
			int amount=	mGameWorld->getPlayerShip()->getResourceCount(x);
			//Set the text of the characters
			std::ostringstream lOStringStream;
			lOStringStream<<amount;
			mResourceText[x].setString(lOStringStream.str());
			mResourceText[x].setOrigin(((float)(9*mResourceText[x].getString().getSize()))/2,0);

			//set the size of the resource box's
			mResources[x].setScale(sf::Vector2f(1,((float)amount/(float)lMaxCapacity)));
			mResourceWhiteOutline[x].setSize(sf::Vector2f(WIDTH_BAR+4,(-SIZE_BAR*((float)(lAmountLeft +amount )/(float)(lMaxCapacity))-4)));
			//set the color to red if it isnt
			mResourceWhiteOutline[x].setOutlineColor(sf::Color::White);

		}
	}

	// Mission objectives
	for(int x = 0; x < NUM_RESOURCE_TYPES; ++x)
	{
		int amount = max(0, mGameWorld->getPlanet()->getResourcesNeeded(x) - mGameWorld->getPlanet()->getResourceCount(x));
		std::ostringstream lOStringStream;
		lOStringStream<<amount;
		mMissionResourcesText[x].setString(lOStringStream.str());
		mMissionResourcesText[x].setOrigin(((float)(9*mMissionResourcesText[x].getString().getSize()))/2,0);

		mMissionResources[x].setScale(1, (float)mGameWorld->getPlanet()->getResourceCount(x) / (float)mGameWorld->getPlanet()->getResourcesNeeded(x));
	}

	//HEALTH BAR
	mFuel.setScale(sf::Vector2f(mGameWorld->getPlayerShip()->getFuelCountRatio(),1));
	mHealth.setScale(sf::Vector2f(mGameWorld->getPlayerShip()->getHealthCountRatio(),1));

	//Directional Arrow
	float rotation  =getAngleOfVectorDegrees(mGameWorld->getPlayerShip()->getPosition(),mGameWorld->getPlanet()->getPosition());
	float XLocation = (1024/2)+(std::cos(getRadians(rotation))*ARROW_OFFSET);
	float YLocation = (768/2)+(std::sin(getRadians(rotation))*ARROW_OFFSET);
	
	mArrow.setRotation(rotation);
	mArrow.setPosition(XLocation,YLocation);

	Notification::Update(DeltaT);
}

