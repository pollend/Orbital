#pragma once

#include <SFML/Graphics.hpp>
class OrbitalCursor
{
	private :

		sf::Sprite cursorSprite;

	public :

		OrbitalCursor();
		~OrbitalCursor();
		void draw(sf::RenderWindow* window);

};