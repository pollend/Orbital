#pragma once

#include "Game/entities/GameObject.h"
#include "../source/game/PhysicsManager.h"
#include "../source/game/PlayerShip.h"
#include <SFML/Graphics.hpp>

class Mine : public GameObject {

	private:

		enum MineState {
			MINE_NEEDS_TO_SPAWN,
			MINE_NORMAL,
			MINE_EXPLODING
		};

		sf::CircleShape mineShape;
		MineState		mineState;
		float			explodeCount;

		PlayerShip*		playerShip;
		PhysicsManager* physics;

		bool isVisible(sf::RenderWindow* Window);

	public:

		Mine();
		~Mine();

		void Load(PhysicsManager* physics, PlayerShip* playerShip, const sf::Vector2f& position);
		void Update(float delta);
		void Draw(sf::RenderWindow* Window);
		void handleCollision(GameObject* objectHit, const sf::Vector2f& contactPoint);

		bool needsRespawn() { return (mineState == MINE_NEEDS_TO_SPAWN); }

};