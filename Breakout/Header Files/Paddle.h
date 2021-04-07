#pragma once
#include <SFML/Graphics.hpp>

class Paddle
{
	sf::RectangleShape paddle;

public:
	Paddle() = default;
	Paddle(sf::Vector2f startPosition);
	void Move(int x, float deltaTime);
	void Render(sf::RenderWindow& window);

	sf::RectangleShape& GetPaddle() { return paddle; }
};