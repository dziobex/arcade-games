#pragma once
#include <SFML/Graphics.hpp>

class Ball
{
	sf::CircleShape ball;
	int moveX, moveY;
	sf::Vector2f previousPosition;
public:
	Ball() = default;
	Ball(int speed, int randomSide);
	void Render(sf::RenderWindow& window);
	bool Move(float deltaTime, bool game, sf::RectangleShape paddle = sf::RectangleShape(sf::Vector2f(0, 0)));

	bool Collision(sf::RectangleShape object, float deltaTime);
};