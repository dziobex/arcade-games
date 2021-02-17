#include "Evolution.h"

int Evolution::aliveFriends(int row, int col) {
	int aliveCells = 0;

	// checking how many friends that surrounding cell from row, col are alive //
	for (int i = -1; i <= 1; i += 2) { // 1 iteration = 1 group of friends (4)
		aliveCells += (board[row + i < 0 ? 49 : (row + i >= 50 ? 0 : row + i)][col] ? 1 : 0);
		aliveCells += (board[row][col + i < 0 ? 49 : (col + i >= 50 ? 0 : col + i)] ? 1 : 0);
		aliveCells += (board[row + i < 0 ? 49 : (row + i >= 50 ? 0 : row + i)][col + i < 0 ? 49 : (col + i >= 50 ? 0 : col + i)] ? 1 : 0);
		aliveCells += (board[row - i < 0 ? 49 : (row - i >= 50 ? 0 : row - i)][col + i < 0 ? 49 : (col + i >= 50 ? 0 : col + i)] ? 1 : 0);
	}

	return aliveCells;
}

// checking whether cell from row, coll is going to be dead in the next generation //
bool Evolution::dead(int row, int col) {
	int alive = aliveFriends(row, col);
	return (board[row][col] && alive == 2) || (board[row][col] && alive == 3) || (!board[row][col] && alive == 3);
}

Evolution::Evolution(float speed, float nextMovementBarrier, sf::Texture blockTexture) {
	this->speed = speed;
	this->nextMovementBarrier = nextMovementBarrier;
	this->blockTexture = blockTexture;
	clear();
}

// evolution process (using grid 2D array as a helper) //
void Evolution::evolve(float deltaTime) {
	nextMovementCounter += (speed * deltaTime);
	if (nextMovementCounter >= nextMovementBarrier) {
		++generation;
		for (int i = 0; i < 50; ++i)
			for (int j = 0; j < 50; ++j)
				grid[i][j] = dead(i, j); // grid represents the next generation, board array is a "base" (parents) that allow to get info about the next gen (children)
		for (int i = 0; i < 50; ++i)
			for (int j = 0; j < 50; ++j)
				board[i][j] = grid[i][j];
		nextMovementCounter = 0;
	}
}

// clear the whole board and reset values for the previous one //
void Evolution::clear() {
	for (int i = 0; i < 50; ++i)
		for (int j = 0; j < 50; ++j)
			board[i][j] = grid[i][j] = false;
	generation = aliveCells = 0;
	nextMovementCounter = 0;
}

// rendering board //
void Evolution::Render(sf::RenderWindow& window) {
	sf::RectangleShape block(sf::Vector2f(10, 10));
	block.setTexture(&blockTexture); // setting block texture to the block object

	aliveCells = 0;
	for (int i = 0; i < 50; ++i)
		for (int j = 0; j < 50; ++j) {
			aliveCells += board[i][j];
			if (board[i][j])
				block.setFillColor(sf::Color::Black);
			else
				block.setFillColor(sf::Color::White);
			block.setPosition(i * 10, j * 10);
			window.draw(block);
		}
}