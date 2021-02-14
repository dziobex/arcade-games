#pragma once
#include <SFML/Graphics.hpp>

class Player
{
protected:
	sf::Vector2f paddleSize;
	sf::Vector2f paddlePosition;
	int points;
	float speed;

public:
	Player();
	Player(sf::Vector2f paddleSize, sf::Vector2f paddlePosition, float speed);
	void draw(sf::RenderWindow& window);
	virtual void move();

	sf::Vector2f GetPosition() { return paddlePosition; }
	sf::Vector2f GetSize() { return paddleSize; }
	int GetPoints() { return points; }

	void IncreasePoints() { points++; }
};

