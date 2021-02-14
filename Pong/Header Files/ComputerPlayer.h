#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Ball.h"

class ComputerPlayer : public Player
{
public:
	enum class Difficulty { EASY = 0, MODERATE = 1, HARD = 2, NONE = -1 };
private:
	Difficulty difficulty;
	bool newSpeed;
public:
	ComputerPlayer(sf::Vector2f paddleSize, sf::Vector2f paddlePosition, float speed, Difficulty difficulty);
	void move(float deltaTime, Ball& ball);
};

