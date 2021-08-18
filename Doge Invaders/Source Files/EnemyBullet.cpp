#include "EnemyBullet.h"

// check is the bullet reached player
bool EnemyBullet::ReachedAim(sf::RectangleShape player) {
	if (player.getGlobalBounds().intersects(bulletObject.getGlobalBounds())) {
		// erase the bullet
		this->Dispose();

		// play the sound
		reachedAimSound.play();
		return true;
	}
	return false;
}

// check if the bullet exists (is available to a next shot)
bool EnemyBullet::Available() {
	return bulletObject.getPosition().y <= WIN_HEIGHT && IfExists();
}

// move bullet (and rotate) if exists
void EnemyBullet::Move(float deltaTime) {
	if (exists) {
		this->bulletObject.setPosition(bulletObject.getPosition() + sf::Vector2f(0, deltaTime * speed));
		this->bulletObject.rotate(abs(speed) * deltaTime);
	}
}