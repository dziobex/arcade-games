#include "Enemy.h"
#include "Constants.h"

// ENEMY CLASS

// rotating horizontally the enemy body //
void Enemy::Bounce() {
	enemyObject.setScale(toRight ? -1 : 1, 1);
	enemyHairObject.setScale(toRight ? -1 : 1, 1);
}

// prepare enemy to die (enable the animation after which the enemy will be erased)
void Enemy::Dispose() {
	enemyObject.setTextureRect(sf::IntRect(220, 0, 110, 100));
	alive = false;
	for (auto& paw : paws)
		paw.Dispose();
}

// check if the enemy collides with body passed as an argument
bool Enemy::Collision(sf::RectangleShape body) {
	return this->enemyObject.getGlobalBounds().intersects(body.getGlobalBounds());
}

// shoot bullet if possible
void Enemy::Shoot() {
	if (!isShot) {
		// change face expression to evil smile >:)
		enemyObject.setTextureRect(sf::IntRect(110, 0, 110, 100));
		bullet = EnemyBullet(reachedAimSound, theme, index, 300, enemyObject.getPosition());
		isShot = true;
		// play shot sound
		shotSound.play();
	}
}

// check if the enemy reached player's position (touches the player)
bool Enemy::IsGameOver(sf::RectangleShape player) {
	return enemyObject.getGlobalBounds().intersects(player.getGlobalBounds());
}

// cancel current bullet
void Enemy::CancelBullet() {
	isShot = false;
	enemyObject.setTextureRect(sf::IntRect(0, 0, 110, 100));
}

// set domg's face expression to evil smile
void Enemy::SetBadFace() {
	enemyObject.setTextureRect(sf::IntRect(110, 0, 110, 100));
}

void Enemy::Down(float deltaTime) {
	for (int i = 0; i < 2; ++i)
		paws[i].SetActive();
	if (enemyObject.getPosition().y < row * 100 + additionalRow) {
		enemyObject.setPosition(enemyObject.getPosition() + sf::Vector2f(0, 20 * deltaTime));
		enemyHairObject.setPosition(enemyHairObject.getPosition() + sf::Vector2f(0, 20 * deltaTime));
		if (enemyObject.getPosition().y >= row * 100 + additionalRow) {
			Bounce();
			row += 1;
			active = false;
			toRight = !toRight;
			additionalRow -= 90;
			enemyObject.setPosition(enemyObject.getPosition() + sf::Vector2f(toRight ? 25 : -25, 0));
			enemyHairObject.setPosition(enemyHairObject.getPosition() + sf::Vector2f(toRight ? 25 : -25, 0));
			for (int i = 0; i < 2; ++i)
				paws[i].Bounce();
		}
	}
}

void Enemy::Initialize(int theme, int index, sf::Vector2f position, sf::Sound& shotSound, sf::Sound& reachedAimSound) {
	this->index = index;
	this->theme = theme;

	// set references to the sounds
	this->shotSound = shotSound;
	this->reachedAimSound = reachedAimSound;

	alive = toRight = true;
	isShot = disposable = active = false;
	additionalRow = 60;

	// initializing enemy's hair
	enemyHairObject = sf::RectangleShape(sf::Vector2f(90, 100));
	enemyHairObject.setPosition(position - sf::Vector2f(0, 25));
	row = (position.y - 50) / 100;
	enemyHairObject.setTexture(&TexturesStorage::GetTexture("enemy_hair"));
	enemyHairObject.setTextureRect(sf::IntRect(index * 110, 100 * theme, 110, 100));

	// initializing enemy's head
	enemyObject = sf::RectangleShape(sf::Vector2f(80, 80));
	enemyObject.setPosition(position);
	enemyObject.setTexture(&TexturesStorage::GetTexture("enemy_faces"));
	enemyObject.setTextureRect(sf::IntRect(0, 0, 110, 100));

	// initializing paw
	for (int i = 0; i < 2; ++i)
		paws[i] = Paw(enemyObject.getPosition(), i == 0 ? PAW_ROTATION_LIMIT : -PAW_ROTATION_LIMIT);

	// set origins
	enemyObject.setOrigin(40, 0);
	enemyHairObject.setOrigin(45, 0);
}

void Enemy::Move(float deltaTime) {
	if (!alive) {
		enemyObject.rotate(deltaTime * 100);
		if (enemyObject.getSize().x > 0)
			enemyObject.setSize(enemyObject.getSize() - sf::Vector2f(deltaTime * enemyObject.getSize().x * DISPOSE_SPEED, deltaTime * enemyObject.getSize().y * DISPOSE_SPEED));
		if (enemyObject.getSize().x < 1)
			disposable = true;
		enemyHairObject.rotate(deltaTime * 100);
		if (enemyHairObject.getSize().x > 0)
			enemyHairObject.setSize(enemyHairObject.getSize() - sf::Vector2f(deltaTime * enemyHairObject.getSize().x * DISPOSE_SPEED, deltaTime * enemyHairObject.getSize().y * DISPOSE_SPEED));
		
	}
	else if (active)
		Down(deltaTime);
	else {
		enemyObject.setPosition(enemyObject.getPosition() + sf::Vector2f(20 * (toRight ? 1 : -1) * deltaTime, 0));
		enemyHairObject.setPosition(enemyHairObject.getPosition() + sf::Vector2f(20 * (toRight ? 1 : -1) * deltaTime, 0));
	}
}

void Enemy::Render(sf::RenderWindow& window, float deltaTime, std::vector<Shield*> shields, sf::RectangleShape player, bool& reachedPlayer, bool move) {
	// move body if possible
	if (move)
		Move(deltaTime);

	// render in order: back paw, enemy's head, enemy's hair, front paw
	paws[0].Render(window, deltaTime, move);
	window.draw(enemyObject);
	window.draw(enemyHairObject);
	paws[1].Render(window, deltaTime, move);

	if (isShot) {
		// move bullet if possible
		if (move) {
			bullet.Move(deltaTime);
			// detect colliders (shields, player)
			bullet.ShieldsDetector(shields, true);
			if (bullet.ReachedAim(player))
				reachedPlayer = true;
			// erase bullet if isn't available (out of the screen)
			if (!bullet.Available())
				CancelBullet();
		}
		bullet.Render(window, deltaTime);
	}
}

// trigger when the enemy object meets wall
bool Enemy::Wall() {
	return (enemyObject.getPosition().x > WIN_WIDTH - enemyHairObject.getSize().x * 0.55) || (enemyObject.getPosition().x - enemyObject.getSize().x / 2 < 0);
}

// PAW STRUCTURE

Enemy::Paw::Paw(sf::Vector2f position, float startAngle) {
	// prepare paw object
	pawObject = sf::RectangleShape(sf::Vector2f(70, 20));
	pawObject.setPosition(position + sf::Vector2f(-35, 70));
	pawObject.setScale(-1, 1);
	pawObject.setOrigin(70, 15);
	pawObject.setTexture(&TexturesStorage::GetTexture("enemy_paw"));

	// set properties for paw animation
	this->angle = startAngle;
	negativeAngle = 0;
	toRight = alive = true;
	active = false;
}

// paw animation: fast, small rotations around the leg start point
void Enemy::Paw::Animate(float deltaTime) {
	if (!alive) return;
	else if (angle <= -PAW_ROTATION_LIMIT) {
		// paw has touched its upper bound => now, the rotation direction in lower place
		negativeAngle = 0;
		angle = -PAW_ROTATION_LIMIT;
	}
	else if (angle >= PAW_ROTATION_LIMIT) {
		// paw has touched its lower bound => now, the rotation direction in upper place
		negativeAngle = 1;
		angle = PAW_ROTATION_LIMIT;
	}
	// rotate paw
	angle += PAW_ROTATION_SPEED * deltaTime * (negativeAngle ? -1 : 1);
	pawObject.setRotation(angle);
}

void Enemy::Paw::Render(sf::RenderWindow& window, float deltaTime, bool move)  {
	if (move) {
		// rotate paw
		Animate(deltaTime);
		// move paw forward (or not if the enemy's going down)
		Move(deltaTime);
	}
	// draw paw
	window.draw(pawObject);
}

void Enemy::Paw::Move(float deltaTime) {
	if (!alive) {
		// paw's death animation: rotate and made smaller until the paw's width is > 0
		pawObject.rotate(deltaTime * 100);
		if (pawObject.getSize().x > 0)
			pawObject.setSize(pawObject.getSize() - sf::Vector2f(deltaTime * pawObject.getSize().x * DISPOSE_SPEED, deltaTime * pawObject.getSize().y * DISPOSE_SPEED));
	}
	// enemy's moving forward
	else if (!active)
		pawObject.setPosition(pawObject.getPosition() + sf::Vector2f(20 * (toRight ? 1 : -1) * deltaTime, 0));
	// enemy's moving down
	else
		pawObject.setPosition(pawObject.getPosition() + sf::Vector2f(0, 20 * deltaTime));
}

// prepare paw for its erasure
void Enemy::Paw::Dispose() {
	alive = false;
}

// rotating horizontally paw
void Enemy::Paw::Bounce() {
	// adjusting the position for the rotation
	pawObject.setPosition(pawObject.getPosition() + sf::Vector2f((toRight ? 1 : -1) * 45, 0));
	// horizontal rotation
	pawObject.setScale(toRight ? 1 : -1, 1);
	// the direction's been changed - paw adjusts to enemy's body in a new direction
	toRight = !toRight;
	// turning off active trigger - paw starts to follow the rest of the enemy's body
	active = false;
}
