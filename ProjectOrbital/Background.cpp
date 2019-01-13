#include "Background.h"
#include "TextureManager.h"
#include "OrbitsMath.h"
#include <iostream>

using namespace sf;

const int	NUM_STARS = 175;		

const float MIN_STAR_SIZE = 4.0f,
			MAX_STAR_SIZE = 6.0f,
			MIN_SCROLL_SPEED = 0.8f,
			MAX_SCROLL_SPEED = 2.0f,
			STAR_COLOR_VAR = 0.25f,
			TWINKLE_SPEED = 0.45f;

Background::Background() {
	stars = NULL;
}

Background::~Background() { 
	delete[] stars;
}

void Background::Load(PlayerShip* playerShip) {
	mIsPlayer = true;
	this->playerShip = playerShip;
	Load();
}

void Background::Load(Vector2f* Position) {
	mIsPlayer = false;
	this->mPosition = Position;
	Load();
}

void Background::Load()
{

		// setup up star sprites

	for (int i = 0; i < 4; i++) {

		sf::Texture* starTexture;
		if (i == 0) starTexture = TextureManager::LoadAndRetrieveTexture("Assets\\star1.png");
		if (i == 1) starTexture = TextureManager::LoadAndRetrieveTexture("Assets\\star2.png");
		if (i == 2) starTexture = TextureManager::LoadAndRetrieveTexture("Assets\\star3.png");
		if (i == 3) starTexture = TextureManager::LoadAndRetrieveTexture("Assets\\star4.png");
		starSprite[i].setTexture(*starTexture);

		starSprite[i].setOrigin(starTexture->getSize().x / 2.0f, starTexture->getSize().y / 2.0f);
	}

	// create stars
	delete[] stars;
	stars = new Star[NUM_STARS];

	std::cout << "Creating (" << NUM_STARS << ") stars...\n";
	double r,g,b;
	for (int i = 0; i < NUM_STARS; i++) {

		stars[i].position.x = randFloat((float)1024.0f);
		stars[i].position.y = randFloat((float)768.0f);
		stars[i].spriteIndex = rand() % 4;

		float size = MIN_STAR_SIZE + randFloat(MAX_STAR_SIZE - MIN_STAR_SIZE);
		stars[i].scale = size / starSprite[stars[i].spriteIndex].getTexture()->getSize().x;

		stars[i].scrollSpeed = MIN_SCROLL_SPEED + randFloat(MAX_SCROLL_SPEED - MIN_SCROLL_SPEED);


		r = (1.0f - STAR_COLOR_VAR) + randFloat(STAR_COLOR_VAR);		
		g = (1.0f - STAR_COLOR_VAR) + randFloat(STAR_COLOR_VAR);	
		b = (1.0f - STAR_COLOR_VAR) + randFloat(STAR_COLOR_VAR);	
		stars[i].color = Color((Uint8)(r * 255), (Uint8)(g * 255), (Uint8)(b * 255));
	}
	sortStars();

	twinkleElapsed = 100.0f;
}

void Background::Update(float delta) {

	twinkleElapsed += delta;

	if (twinkleElapsed > 1.0f) {
		twinkleElapsed = 0.0f;
		twinkleIndex = rand() % NUM_STARS;
	}
}

void Background::Draw(RenderWindow* window) {

	//if (Keyboard::isKeyPressed(Keyboard::BackSlash))
		//return;
	Vector2f shipPos;
	if(mIsPlayer)
		 shipPos = playerShip->getPosition();
	else
		 shipPos = *mPosition;

	for (int i = 0; i < NUM_STARS; i++) {

		Vector2f sp = stars[i].position;

		sp.x -= shipPos.x * stars[i].scrollSpeed;
		sp.y -= shipPos.y * stars[i].scrollSpeed;

		sp.x = fmod(sp.x, 1024.0f);
		sp.y = fmod(sp.y, 768.0f);

		if (sp.x < 0.0f)
			sp.x = 1024.0f + sp.x;
		if (sp.y < 0.0f)
			sp.y = 768.0f + sp.y;

		starSprite[stars[i].spriteIndex].setPosition(sp);	
		starSprite[stars[i].spriteIndex].setColor(stars[i].color);
		starSprite[stars[i].spriteIndex].setScale(stars[i].scale, stars[i].scale);

		if (i == twinkleIndex && twinkleElapsed < TWINKLE_SPEED) {

			double b = 1.0 - sin(twinkleElapsed * (PI / TWINKLE_SPEED));
			Color c = stars[i].color;
			c.r = (Uint8)(c.r * b);
			c.g = (Uint8)(c.g * b);
			c.b = (Uint8)(c.b * b);
			starSprite[stars[i].spriteIndex].setColor(c);
		}

		window->draw(starSprite[stars[i].spriteIndex]);
	}
}

// sort stars so the slowest moving are first in the array, so they are drawn first
// used the insertion sort algorithm from wikipedia : http://en.wikipedia.org/wiki/Insertion_sort
void Background::sortStars() {

	float value;
	int hole;

	for (int i = 1; i < NUM_STARS; i++) {

		value = stars[i].scrollSpeed;
		hole = i;

		while (hole > 0 && value < stars[hole-1].scrollSpeed) {
			stars[hole].scrollSpeed = stars[hole-1].scrollSpeed;
			hole--;
		}
		stars[hole].scrollSpeed = value;
	}
}