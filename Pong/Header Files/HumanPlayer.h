#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Constants.h"

class HumanPlayer : public Player
{
	bool moveArrows;
public:
	HumanPlayer(sf::Vector2f paddleSize, sf::Vector2f paddlePosition, float speed, bool moveArrows);
	void move(float deltaTime);
};

