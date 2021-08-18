#include "PlayerBullet.h"

// check if the bullet has hit any enemy
bool PlayerBullet::ReachedAim(std::vector<Enemy*> enemies) {
	for (auto& enemy : enemies)
		if (enemy->Collision(bulletObject) && exists && enemy->IsAlive()) {
			// destroy the hit enemy and the bullet
			enemy->Dispose();
			this->Dispose();
			// play the sound
			reachedAimSound.play();
			isDestroyed = 1;
			return true;
		}
	return false;
}

// check if the bullet exists (is available for a next shot)
bool PlayerBullet::Available() {
	return !exists || bulletObject.getPosition().y - 20 <= 0;
}