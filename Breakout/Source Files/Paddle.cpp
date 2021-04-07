#include "Paddle.h"

// preparing paddle - size, position and color //
Paddle::Paddle(sf::Vector2f startPosition) {
	paddle = sf::RectangleShape(sf::Vector2f(120, 20));
	paddle.setPosition(startPosition);
	paddle.setFillColor(sf::Color::Cyan);
}

// move paddle (x position) //
void Paddle::Move(int x, float deltaTime) {
	if (paddle.getPosition().x + x * deltaTime >= 2 && paddle.getPosition().x + x * deltaTime <= 700 - 2 - paddle.getSize().x)
		paddle.setPosition(paddle.getPosition() + sf::Vector2f(x * deltaTime, 0));
}

// draw paddle //
void Paddle::Render(sf::RenderWindow& window) {
	window.draw(paddle);
}