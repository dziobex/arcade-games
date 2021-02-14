#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "HumanPlayer.h"
#include "Constants.h"

class Ball
{
	Player* p1;
	Player* p2;
	float speedX, speedY;

	sf::Vector2f ballPositions[2];
	sf::Vector2f ballSize;

	int collision(); // -1: collsion didn't occur; 0: collision up-down; 1: collision right-left; 2: game over (ball is out of players view)
public:
	bool GameOver;

	Ball(HumanPlayer* p1, Player* p2, sf::Vector2f ballSize, sf::Vector2f firstPosition, float speedX);
	void move(float deltaTime);
	void draw(sf::RenderWindow& window);

	sf::Vector2f GetPosition() { return ballPositions[0]; }
	float GetSpeedX() { return speedX; }
	float GetSpeedY() { return speedY; }
};

