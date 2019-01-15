#include "MainMenuBackground.h"
#include "TextureManager.h"
#include "../source/game/ParticleManager.h"
#define NUMBER_BACKGROUND_ASTEROIDS 20
#define DEFAULT_CAMERA_WIDTH 68
#define FIELD_SIZE 100
MainMenuBackground::MainMenuBackground(void)
{
	index = 0;

	mPosition = new sf::Vector2f(0,0);
	mBackground.Load(mPosition);

	//set the view to the center
	cameraView.setCenter(0.0, 0.0);
	//declares varaibles
	mPhysicsManager = new PhysicsManager();
	mAsteroids = new Asteroid[NUMBER_BACKGROUND_ASTEROIDS];

	mMainBack.setTexture(*TextureManager::LoadAndRetrieveTexture("Assets//MenuObjects//MainMenu//main_back_drop.png"));
	//get the texture from the texture manager
	sf::Texture* lAsteroidTexture = TextureManager::LoadAndRetrieveTexture("Assets//asteroidTexture.png");
	lAsteroidTexture->setSmooth(true);
	//load Asteroids
	for(int x = 0; x < NUMBER_BACKGROUND_ASTEROIDS; x++)
	{
		mAsteroids[x].Load(lAsteroidTexture,mPhysicsManager,0,0,-(FIELD_SIZE/2),-(FIELD_SIZE/2),FIELD_SIZE,FIELD_SIZE);
	}

}


MainMenuBackground::~MainMenuBackground(void)
{
	//delete asteroids
	delete[] mAsteroids;
	delete mPosition;
	delete mPhysicsManager;
}
void MainMenuBackground::Load(ScreenManager* ScreenManager)
{
}

void  MainMenuBackground::Draw(sf::RenderWindow* Window)
{
	mBackground.Draw(Window);

	float aspectRatio = (float)Window->getSize().y / Window->getSize().x;
	// set camera view to appropriate size relative to render window		
	cameraView.setSize(DEFAULT_CAMERA_WIDTH, DEFAULT_CAMERA_WIDTH * aspectRatio);	
	cameraView.zoom(1);

	//set the view
	Window->setView(cameraView);
	//draw the asteroids
	for (int i = 0; i < NUMBER_BACKGROUND_ASTEROIDS; i++) 
		mAsteroids[i].Draw(Window);
	// draw particles
	ParticleManager::draw(Window);

	
	Window->setView(Window->getDefaultView());
	Window->draw(mMainBack);


}

void  MainMenuBackground::Update(ScreenManager* ScreenManager,sf::RenderWindow* Window,float DeltaT)
{
	index+= DeltaT*.2f;
	mPosition->x += std::sin(index)*.02f;
	mBackground.Update(DeltaT);


	//update the physics
	mPhysicsManager->Update(DeltaT);
	for (int i = 0; i < NUMBER_BACKGROUND_ASTEROIDS; i++) 
	{
		//knocks the asterouid back into the filed so they don't all go flying off
		if(mAsteroids[i].getPosition().x > (FIELD_SIZE/2))
		{
			if(mAsteroids[i].getVelocity().x > 0)
				mAsteroids[i].setVelocity(-mAsteroids[i].getVelocity().x,mAsteroids[i].getVelocity().y);
		}
		else if(mAsteroids[i].getPosition().x < -(FIELD_SIZE/2))
		{
			if(mAsteroids[i].getVelocity().x < 0)
				mAsteroids[i].setVelocity(-mAsteroids[i].getVelocity().x,mAsteroids[i].getVelocity().y);
		}

		if(mAsteroids[i].getPosition().y > (FIELD_SIZE/2))
		{
			if(mAsteroids[i].getVelocity().y > 0)
				mAsteroids[i].setVelocity(mAsteroids[i].getVelocity().x,-mAsteroids[i].getVelocity().y);
		}
		else if(mAsteroids[i].getPosition().y < -(FIELD_SIZE/2))
		{
			if(mAsteroids[i].getVelocity().y < 0)
				mAsteroids[i].setVelocity(mAsteroids[i].getVelocity().x,-mAsteroids[i].getVelocity().y);
		}
		//update the asteroids
		mAsteroids[i].Update(DeltaT);
	}
	// particles
	ParticleManager::update(DeltaT);
}
