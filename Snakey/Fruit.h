#pragma once
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <ctime>
#include "Snake.h"

class Fruit
{
	sf::Vector2f position;
	sf::Color color;
	int additionalSegments;

	sf::Texture texture;
	void setNextFruct(Snake& snake);
	bool possiblePosition(Snake& snake);
public:
	Fruit(Snake& snake);
	void draw(sf::RenderWindow& window);
	void collision(Snake& snake);
};

