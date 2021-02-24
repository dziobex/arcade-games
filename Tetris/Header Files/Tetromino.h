#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>

class Tetromino
{
	bool pattern[4][4];
	sf::RectangleShape block;
	sf::Vector2f position;
public:
	bool movable;
	int color, shape, heldClicked;

	Tetromino() = default;
	Tetromino(int index, int color, sf::Texture& tetrominoesTexture, sf::Vector2f position = sf::Vector2f(75, 0));

	void Rotate(bool rightSide, int board[20][10]);
	void Render(sf::RenderWindow& window, bool shade = false);

	void Move(float moveX, float moveY, int board[20][10], bool shade = false);
	bool AnyElement(int row);
	sf::Vector2f GetPosition() { return position; }
};

