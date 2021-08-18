#include "Bullet.h"

// animation of the bullet's explosion
void Bullet::explode(float deltaTime) {
	if (explosionIndex == 10)
		// the animation is over --> the bullet was "officially" destroyed (won't appear anymore on the screen)
		return exists = false, void(0);

	explosionCounter += EXPLOSION_SPEED * deltaTime;
	if (explosionCounter >= EXPLOSION_LIMIT) {
		// explosion counter has reached the limit - time to change the texture's fragment
		explosionCounter = 0;
		explosionIndex++;
		explosion.setTextureRect(sf::IntRect(explosionIndex * 80, 0, 80, 80));
	}
}

Bullet::Bullet(sf::Sound& reachedAimSound, int speed, sf::Vector2f position) {
	// set technical variables: info if the bullet exists, speed for moving the bullet, explosion vars for explosion animation
	this->speed = speed;
	exists = true;
	explosionCounter = 0;
	explosionIndex = -1;

	this->reachedAimSound = reachedAimSound;
	
	// prepare the bullet object
	// bullet texture will be set in inheriting objects
	bulletObject = sf::RectangleShape(sf::Vector2f(25, 25));
	bulletObject.setOrigin(10, 10);
	bulletObject.setPosition(position);

	// set an object which will represent the explosion animation
	explosion = sf::RectangleShape(sf::Vector2f(40, 40));
	explosion.setOrigin(20, speed > 0 ? 10 : 30);
	explosion.setTexture(&TexturesStorage::GetTexture("explosion"));
}

// move bullet if exists
void Bullet::Move(float deltaTime) {
	if (exists)
		this->bulletObject.setPosition(bulletObject.getPosition() + sf::Vector2f(0, deltaTime * speed));
}

// render bullet and explosion, if the bullet has hit something (explosion index doesn't equal -1)
void Bullet::Render(sf::RenderWindow& window, float deltaTime) {
	window.draw(bulletObject);
	if (explosionIndex != -1) {
		explode(deltaTime);
		window.draw(explosion);
	}
}

// final dispose of the bullet
void Bullet::Dispose() {
	bulletObject.setSize(sf::Vector2f(0, 0));
	explosionIndex = 0;
	explosion.setTextureRect(sf::IntRect(0, 0, 80, 80));
	explosion.setPosition(bulletObject.getPosition());
}

// check if collision with any shield has occurred
// destroyer informs if the bullet was made by an enemy (destructive) or player (neutral)
void Bullet::ShieldsDetector(std::vector<Shield*> shields, bool destroyer) {
	for (auto& shield : shields)
		if (shield->Collision(this->bulletObject, destroyer)) {
			this->Dispose();
			return;
		}
}

// return the info about the bullet existence
bool Bullet::IfExists() {
	return exists;
}