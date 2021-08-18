#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerBullet.h"
#include "TexturesStorage.h"

class Player {
	sf::Vector2f position;
	sf::RectangleShape object, deathObject;

	sf::Sound shotSound, reachedAimSound;

	bool shotState, facialExpression;
	PlayerBullet bullet;

	float sizeX;

	bool isDying, isHiding, cooldown;
	float deathCounter;
	int deathIndex;

public:
	Player() = default;
	Player(sf::Sound& shotSound, sf::Sound& reachedAimSound);
	void Render(sf::RenderWindow& window, float deltaTime, bool spacePressed, std::vector<Enemy*> enemies, std::vector<Shield*> shields, bool move);
	void Move(float deltaTime);

	sf::RectangleShape GetObject() { return object; }

	void Death();
	bool IsReady();
};