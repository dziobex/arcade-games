#include "Ball.h"

/* checking type of collision (0 - collision didn't occur) */
int Ball::collision() {
	if (ballPositions[0].y <= 0)
		return 9; // collison with the top wall
	if (ballPositions[0].y >= WIN_HEIGHT - ballSize.y)
		return 10; // collison with the bottom wall
	else if (ballPositions[0].x >= p1->GetPosition().x && p1->GetPosition().x + BALL_CURSOR_SIZE >= ballPositions[0].x && ballPositions[0].y + BALL_CURSOR_SIZE >= p1->GetPosition().y && p1->GetPosition().y + p1->GetSize().y >= ballPositions[0].y)
		return 19; // collison with left paddle
	else if (ballPositions[0].x + BALL_CURSOR_SIZE / 2 >= p2->GetPosition().x && p2->GetPosition().x + BALL_CURSOR_SIZE >= ballPositions[0].x + BALL_CURSOR_SIZE / 2 && ballPositions[0].y + BALL_CURSOR_SIZE >= p2->GetPosition().y && p2->GetPosition().y + p2->GetSize().y >= ballPositions[0].y)
		return 20; // collision with right paddle
	else if (ballPositions[0].x < 0 || ballPositions[0].x > WIN_WIDTH - ballSize.x)
		return 2; // game over
	return -1;
}

Ball::Ball(HumanPlayer* p1, Player* p2, sf::Vector2f ballSize, sf::Vector2f firstPosition, float speedX) {
	this->p1 = p1;
	this->p2 = p2;
	this->ballSize = ballSize;
	this->speedX = speedX;
	this->speedY = speedX;

	ballPositions[0] = firstPosition;
	GameOver = false;
}

void Ball::move(float deltaTime) {
	int collisionType = collision();
	// handling collision (and changing speedX, speedY if necessary)
	switch (collisionType) {
	case 9:
		speedY = abs(speedY);
		break;
	case 10:
		speedY = -abs(speedY);
		break;
	case 19:
		speedX = abs(speedX);
		speedY = 2 * ballPositions[0].y - ballPositions[1].y;
		break;
	case 20:
		speedX = -abs(speedX);
		speedY = 2 * ballPositions[0].y - ballPositions[1].y;
		break;
	case 2:
		GameOver = true;
		break;
	default:
		break;
	}
	if (abs(speedY) <= 30)
		speedY = (speedY / abs(speedY)) * abs(speedY) + 70;
	ballPositions[1] = ballPositions[0];
	ballPositions[0] += sf::Vector2f(speedX * deltaTime, speedY * deltaTime);
}

/* rendering ball */
void Ball::draw(sf::RenderWindow& window) {
	sf::RectangleShape ball(ballSize);
	ball.setPosition(ballPositions[0]);
	window.draw(ball);
}
