#pragma once

#include <SFML\Graphics.hpp>

class Cursor
{
	private :

		sf::Sprite cursorSprite;

	public :

		Cursor();
		~Cursor();
		void draw(sf::RenderWindow* window);

};