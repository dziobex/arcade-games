#include "Player.h"
#include "Constants.h"

Player::Player(sf::Sound& shotSound, sf::Sound& reachedAimSound) {
	shotState = facialExpression = false;

	// set references to the sounds
	this->shotSound = shotSound;
	this->reachedAimSound = reachedAimSound;

	object = sf::RectangleShape(sf::Vector2f(75, 120));
	sizeX = 75;

	// set position and origin
	object.setOrigin(37, 0);
	object.setPosition(sf::Vector2f(WIN_WIDTH / 2, WIN_HEIGHT - object.getSize().y));

	// set texture
	object.setTexture(&TexturesStorage::GetTexture("player"));
	object.setTextureRect(sf::IntRect(0, 0, 170, 300));

	// initialize death
	deathObject = sf::RectangleShape(sf::Vector2f(180, 120));
	deathObject.setTexture(&TexturesStorage::GetTexture("player_death"));
	deathObject.setTextureRect(sf::IntRect(0, 0, 200, 150));
	deathObject.setOrigin(90, 0);
	isDying = isHiding = deathCounter = deathIndex = cooldown = 0;
}

// move player
void Player::Move(float deltaTime) {
	if (shotState)
		bullet.Move(deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && object.getPosition().x >= object.getSize().x / 2)
		object.setPosition(object.getPosition() + sf::Vector2f(-SPEED * deltaTime, 0));
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && object.getPosition().x < 900 - object.getSize().x / 2)
		object.setPosition(object.getPosition() + sf::Vector2f(SPEED * deltaTime, 0));
}

// prepare object for "dying"
// prepare object for displaying animation
void Player::Death() {
	// reset values for the death animation
	isDying = 1;
	deathCounter = deathIndex = isHiding = cooldown = 0;

	// set death object to visible state
	deathObject.setPosition(object.getPosition());
	deathObject.setFillColor(sf::Color(deathObject.getFillColor().r, deathObject.getFillColor().g, deathObject.getFillColor().b, 255));
	deathObject.setTextureRect(sf::IntRect(0, 0, 200, 150));
}

// check if the player is ready for a next round (after death)
bool Player::IsReady() {
	return !isDying && !isHiding && !cooldown;
}

void Player::Render(sf::RenderWindow& window, float deltaTime, bool spacePressed, std::vector<Enemy*> enemies, std::vector<Shield*> shields, bool move) {
	if (!isDying && !isHiding && !cooldown && move) {
		Move(deltaTime);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (!shotState || bullet.Available())) {
			// create a new bullet
			bullet = PlayerBullet(reachedAimSound, -600, object.getPosition());
			shotState = facialExpression = true;
			shotSound.play();
		}
		// set face expression
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			facialExpression = false;
		if (facialExpression)
			object.setTextureRect(sf::IntRect(170, 0, 170, 300));
		else
			object.setTextureRect(sf::IntRect(0, 0, 170, 300));
		if (shotState) {
			// check if the bullet's reached any enemy
			if (bullet.ReachedAim(enemies))
				facialExpression = shotState = true;
			// check if the bullet has reached any shield and render the bullet
			bullet.ShieldsDetector(shields, 1);
			bullet.Render(window, deltaTime);
		}
	}
	else if (isDying) {
		// display death animation (manage the "death" texture)
		deathCounter += 666 * deltaTime;
		if (deathCounter >= 40 && deathIndex < 44) {
			// next frame'll be displayed
			deathCounter = 0;
			deathObject.setTextureRect(sf::IntRect(deathIndex % 10 * 200, (int)floor(deathIndex / 10) * 150, 200, 150));
			deathIndex++;
		}
		else if (deathIndex >= 44) {
			// animation's ended
			isHiding = 1;
			isDying = deathIndex = 0;
		}
	}
	if (isHiding) {
		// player death object's getting darker
		deathObject.setFillColor(sf::Color(deathObject.getFillColor().r, deathObject.getFillColor().g, deathObject.getFillColor().b, deathObject.getFillColor().a - deltaTime * 0.1));
		if (deathObject.getFillColor().a < 1) {
			isHiding = isDying = 0;
			cooldown = 1;
		}
	}
	if (cooldown) {
		// cooldown before the next round
		deathCounter += 100 * deltaTime;
		if (deathCounter >= 50) {
			cooldown = 0;
			object.setPosition(sf::Vector2f(WIN_WIDTH / 2, WIN_HEIGHT - object.getSize().y));
		}
	}
	// player "isn't dying"
	if (!isDying && !isHiding && !cooldown)
		window.draw(object);
	else // player "is dying"
		window.draw(deathObject);
}