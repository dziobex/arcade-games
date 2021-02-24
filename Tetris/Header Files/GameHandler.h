#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "Tetromino.h"

class GameHandler
{
	std::vector<int> rows;
	int board[20][10];
	Tetromino currentTetromino, nextTetromino;
	int heldShape, heldColor;
	float speed, passed, passedMin, passedRows, passedMinRows;
	sf::Texture tetrominoTexture;
	int points, level;
	bool gameOver, rowAnimation;

	void completeRows();
public:
	GameHandler(float speed, float passedMin, float passedMinRows, sf::Texture& tetrominoTexture);
	void Move(float deltaTime, bool keyPressed);

	void Render(sf::RenderWindow& window);
	bool IsGameOver() { return gameOver; }
	int GetScore() { return level * 200 + points; }
};