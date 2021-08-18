#pragma once
#include "Bullet.h"
#include "Enemy.h"

class PlayerBullet : public Bullet {
public:
	PlayerBullet() = default;
	PlayerBullet(sf::Sound& shotSound, int speed, sf::Vector2f position) : Bullet(shotSound, speed, position) {
		// set bullet's texture
		this->bulletObject.setTexture(&TexturesStorage::GetTexture("player_bullet"));
	}

	bool ReachedAim(std::vector <Enemy*> enemies);
	bool Available();

	~PlayerBullet() {
		isDestroyed = 1;
	}
};