#include "Cursor.h"
#include "TextureManager.h"

using namespace sf;

const float CURSOR_SIZE = 30.0f;

Cursor::Cursor() {
	Texture* texture = TextureManager::LoadAndRetrieveTexture("Assets\\cursor.png");
	cursorSprite.setTexture(*texture);
	cursorSprite.setOrigin(texture->getSize().x / 2.0f, texture->getSize().y / 2.0f);
	float scale = CURSOR_SIZE / texture->getSize().x;
	cursorSprite.setScale(scale, scale);
}

Cursor::~Cursor() {
}

void Cursor::draw(RenderWindow* window) {
	Vector2f pos = window->convertCoords(Mouse::getPosition(*window));
	cursorSprite.setPosition(pos);
	window->draw(cursorSprite);
}