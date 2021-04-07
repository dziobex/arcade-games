#pragma once
#include "Ball.h"

// class handles bricks behavior //
class BricksHandler
{
	struct Brick {
		int level;
		bool Crushed = false;
		sf::RectangleShape brick;
		bool Collision(int& points, Ball& ball, float deltaTime);
	};

	std::vector <Brick> bricks;

public:
	// public methods, fields
	int level = 0;

	BricksHandler();
	void Render(sf::RenderWindow& window, Ball& ball, float deltaTime, int& points);
	bool NextLevel() { return std::count_if(bricks.begin(), bricks.end(), [](Brick b) { return b.Crushed; }) == bricks.size(); }
};