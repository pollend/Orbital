#pragma once

#include "Game/PlayerShip.h"
#include <SFML/Graphics.hpp>


class Star {

	public:

		Star() { } 

		sf::Vector2f position;
		float		 scale;
		float		 scrollSpeed;
		sf::Color	 color;
		int			 spriteIndex;

};

class Background {

	public:

		Background();
		~Background();

		void Load(PlayerShip* playerShip);
		void Load(sf::Vector2f* Position);
		void Update(float delta);
		void Draw(sf::RenderWindow* window);

	private:

		void Load();

		Star*		stars;
		sf::Sprite  starSprite[4];		
		PlayerShip* playerShip;

		float twinkleElapsed;
		int twinkleIndex;

		bool mIsPlayer;
		sf::Vector2f* mPosition;

		void sortStars();

};