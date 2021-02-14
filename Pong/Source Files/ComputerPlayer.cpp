#include "ComputerPlayer.h"
#include "Player.h"

ComputerPlayer::ComputerPlayer(sf::Vector2f paddleSize, sf::Vector2f paddlePosition, float speed, Difficulty difficulty)
	: Player(paddleSize, paddlePosition, speed) {
	this->difficulty = difficulty;
	newSpeed = false;
}

void ComputerPlayer::move(float deltaTime, Ball& ball) {
	// moving depends on difficulty AND ball position
	if (!newSpeed) {
		if (difficulty == Difficulty::EASY)
			speed *= 0.8;
		if (difficulty == Difficulty::MODERATE)
			speed *= 1.4;
		else if (difficulty == Difficulty::HARD)
			speed *= 2;
		newSpeed = true;

	}
	// following ball
	if (ball.GetSpeedX() > 0) {
		float d = ball.GetPosition().y;
		if (paddlePosition.y > d)
			paddlePosition += sf::Vector2f(0, -abs(speed * deltaTime));
		else if (paddlePosition.y + 100 < d)
			paddlePosition += sf::Vector2f(0, abs(speed * deltaTime));
	}
}
