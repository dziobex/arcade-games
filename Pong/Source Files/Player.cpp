#include "Player.h"

Player::Player() {}

Player::Player(sf::Vector2f paddleSize, sf::Vector2f paddlePosition, float speed) {
	this->paddlePosition = paddlePosition;
	this->paddleSize = paddleSize;
	this->speed = speed;

	points = 0;
}

/* rendering players paddle */
void Player::draw(sf::RenderWindow& window) {
	sf::RectangleShape paddle(paddleSize);
	paddle.setPosition(paddlePosition);
	window.draw(paddle);
}

void Player::move() {}
