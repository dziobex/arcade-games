#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(sf::Vector2f paddleSize, sf::Vector2f paddlePosition, float speed, bool moveArrows)
	: Player(paddleSize, paddlePosition, speed) {
	this->moveArrows = moveArrows;
}

/* moving the paddle with keyboard input */
void HumanPlayer::move(float deltaTime) {
	int side = 0;

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && moveArrows) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !moveArrows))
		side = -1;
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && moveArrows) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !moveArrows))
		side = 1;
	else
		return;

	if ((paddlePosition + sf::Vector2f(0, side * speed * deltaTime)).y >= 0 && (paddlePosition + sf::Vector2f(0, side * speed * deltaTime)).y <= WIN_HEIGHT - paddleSize.y)
		paddlePosition += sf::Vector2f(0, side * speed * deltaTime);
}
