#pragma once
#include "Bullet.h"

class EnemyBullet : public Bullet {
public:
	EnemyBullet() = default;
	EnemyBullet(sf::Sound& shotSound, int theme, int index, int speed, sf::Vector2f position) : Bullet(shotSound, speed, position) {
		this->bulletObject.setTexture(&TexturesStorage::GetTexture("enemy_bullets"));
		this->bulletObject.setTextureRect(sf::IntRect(20 * index, 20 * theme, 20, 20));
	}

	void Move(float deltaTime);
	bool ReachedAim(sf::RectangleShape player);
	bool Available();
};