#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Constants.h"

class Snake
{
public:
	enum Direction { UP, DOWN, LEFT, RIGHT };
private:
	Direction previousDirection;

	std::vector<sf::Vector2f> bodyPositions;
	sf::Vector2f headPosition;
	sf::Vector2f movements;
	sf::Texture headTexture;
	sf::Texture bodyTexture;

	float speed;
	float pause;
	float nextMove;

	void moveSegments();

public:
	Snake(sf::Vector2f headPosition, float speed, float pause, sf::Texture headTexture, sf::Texture bodyTexture);
	void draw(sf::RenderWindow& window);
	void update(float delaTime, bool& gameOver, sf::Text& fpsCounter);
	void addNewPart(int numberOfParts, Direction dir);

	bool collision();

	sf::Vector2f GetHeadPosition() { return headPosition; }
	std::vector<sf::Vector2f> GetBodyPositions() { return bodyPositions; }
};

