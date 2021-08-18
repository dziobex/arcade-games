#pragma once
#include "Constants.h"
#include "TexturesStorage.h"
#include "Shield.h"

class Bullet {
public:
	sf::RectangleShape bulletObject;
	sf::RectangleShape explosion;

	sf::Sound reachedAimSound;

	int speed, explosionIndex;
	bool isDestroyed;
	bool exists;
	float explosionCounter;

	void explode(float deltaTime);
public:
	Bullet() = default;
	Bullet(sf::Sound& reachedAimSound, int speed, sf::Vector2f position);

	virtual void Move(float deltaTime);

	void Render(sf::RenderWindow& window, float deltaTime);
	void Dispose();
	void ShieldsDetector(std::vector<Shield*> shields, bool destroyer = false);

	bool IfExists();
};