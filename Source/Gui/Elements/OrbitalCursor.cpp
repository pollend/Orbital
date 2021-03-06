#include "OrbitalCursor.h"
#include "Game/TextureManager.h"

using namespace sf;

const float CURSOR_SIZE = 30.0f;

OrbitalCursor::OrbitalCursor() {
	Texture* texture = TextureManager::LoadAndRetrieveTexture("Assets\\cursor.png");
	cursorSprite.setTexture(*texture);
	cursorSprite.setOrigin(texture->getSize().x / 2.0f, texture->getSize().y / 2.0f);
	float scale = CURSOR_SIZE / texture->getSize().x;
	cursorSprite.setScale(scale, scale);
}

OrbitalCursor::~OrbitalCursor() {
}

void OrbitalCursor::draw(RenderWindow* window) {
	Vector2f pos = window->mapPixelToCoords(Mouse::getPosition(*window));
	cursorSprite.setPosition(pos);
	window->draw(cursorSprite);
}