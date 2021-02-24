#include "Tetromino.h"

Tetromino::Tetromino(int index, int color, sf::Texture& tetrominoesTexture, sf::Vector2f position) {
	this->color = color;
	this->shape = index;
	std::ifstream reader("resources/tetrominoes.txt");
	std::string line;
	for (int i = 0; i < index * 4; ++i)
		std::getline(reader, line);

	// preparing tetrominos shape //
	for (int row = 0; row < 4; ++row) {
		std::getline(reader, line);
		for (int col = 0; col < 4; ++col)
			pattern[row][col] = line[col] == 'X'; // information about the tetrominoes appearance is in the file
	}
	
	heldClicked = false;
	this->position = position;
	block = sf::RectangleShape(sf::Vector2f(25, 25));
	block.setTexture(&tetrominoesTexture);
	block.setTextureRect(sf::IntRect(color * 100, 0, 100, 100));
}

void Tetromino::Rotate(bool rightSide, int board[20][10]) {
	bool helper[4][4];
	for (int i = 0; i < 4; ++i) // rotating tetrimino using helper array
		for (int j = 0; j < 4; ++j) {
			if (!rightSide)
				helper[i][j] = pattern[j][3 - i];
			else
				helper[i][j] = pattern[3 - j][i];
		}

	for (int i = 0; i < 4; ++i) // checking whether rotation is possible (if not, end function)
		for (int j = 0; j < 4; ++j) {
			if (position.x < 0 && helper[j][(int)abs(position.x / 25) - 1])
				return;
			if (11 - abs(position.x / 25) <= 4 && helper[j][10 - (int)abs(position.x / 25)])
				return;
			if (helper[i][j] && board[(int)(position.y + 25 * (i + 1)) / 25][(int)(position.x + 25 * j) / 25] != -1)
				return;
		}

	for (int i = 0; i < 4; ++i) // rotating tetromino
		for (int j = 0; j < 4; ++j)
			pattern[i][j] = helper[i][j];
}

// rendering tetromino, setting its color to gray when it's shade //
void Tetromino::Render(sf::RenderWindow& window, bool shade) {
	if (shade)
		block.setFillColor(sf::Color(169, 169, 169, 169));
	block.setTextureRect(sf::IntRect(color * 100, 0, 100, 100));
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (pattern[i][j]) {
				block.setPosition(position.x + 50 + 25 * j, position.y + 25 + 25 * i);
				window.draw(block);
			}
}

void Tetromino::Move(float moveX, float moveY, int board[20][10], bool shade) {
	if (position.y / 25 > 16 && AnyElement(20 - (position.y / 25))) // checking if tetromino's touching the bottom of the board
		movable = false;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			if (pattern[i][j] && board[(int)(position.y + 25 * (i)) / 25][(int)(position.x + 25 * j) / 25] != -1) {
				movable = false; // tetromino's touched another shape
				break;
			}
			// checking for bounds (from board walls or the previous tetrominoes) //
			if ((moveX < 0 && position.x <= 0 && pattern[j][(int)abs(position.x / 25)]) || (moveX > 0 && 9 - abs(position.x / 25) < 4 && pattern[j][9 - (int)abs(position.x / 25)]))
				moveX = 0;
			else if (moveX < 0 && pattern[i][j] && board[(int)(position.y + 25 * (i)) / 25 - 1][(int)(position.x + 25 * j) / 25 - 1] != -1)
				moveX = 0;
			else if (moveX > 0 && pattern[i][j] && board[(int)(position.y + 25 * (i)) / 25 - 1][(int)(position.x + 25 * j) / 25 + 1] != -1)
				moveX = 0;
		}
	if (!movable) {
		moveX = 0;
		moveY = 0;
	}
	position += sf::Vector2f(moveX, moveY);

	// putting pattern on the board //
	if (!movable && !shade)
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				if (pattern[i][j])
					board[(int)(position.y / 25) + i- 1][(int)(position.x / 25) + j] = color;
}

// checking if any element exists in passed row (in tetromino's pattern) //
bool Tetromino::AnyElement(int row) {
	for (int i = 0; i < 4; ++i)
		if (pattern[row][i])
			return true;
	return false;
}
