#include "Ball.h"

// preparing the ball - setting radius, color, position, and direction values //
Ball::Ball(int speed, int randomSide) {
	ball = sf::CircleShape(10);
	ball.setFillColor(sf::Color::White);
	ball.setPosition(350, 300);
	moveX = randomSide == 1 ? 100 : -100; // start direction: right or left
	moveY = speed;
}

// draw ball //
void Ball::Render(sf::RenderWindow& window) {
	window.draw(ball);
}

// move the ball in its current direction (moveX & moveY) and bounce the ball if touches any wall //
bool Ball::Move(float deltaTime, bool game, sf::RectangleShape paddle) {
	if (!game) {
		ball.setPosition(paddle.getPosition() + sf::Vector2f(paddle.getSize().x / 2 - 10, -paddle.getSize().y));
		return true;
	}
	previousPosition = ball.getPosition();
	if (ball.getPosition().x + moveX * deltaTime < 0 || ball.getPosition().x + moveX * deltaTime > 690)
		moveX = -moveX;
	if (ball.getPosition().y + moveY * deltaTime < 60)
		moveY = -moveY;
	if (ball.getPosition().y + moveY * deltaTime > 560)
		return false;
	ball.setPosition(ball.getPosition() + sf::Vector2f(moveX * deltaTime, moveY * deltaTime));
	return true;
}

// detect collision with bricks and set the next direction based on the collision side //
bool Ball::Collision(sf::RectangleShape object, float deltaTime) {
	if (ball.getGlobalBounds().intersects(object.getGlobalBounds())) {
		sf::CircleShape helper = ball;
		helper.setPosition(helper.getPosition() + sf::Vector2f(-moveX * 2* deltaTime, 0));
		if (helper.getGlobalBounds().intersects(object.getGlobalBounds())) {
			if (object.getPosition().y > ball.getPosition().y)
				moveY = -abs(moveY);
			else
				moveY = abs(moveY);
		}
		else {
			moveX = -moveX;
		}
		return true;
	}
	return false;
}
