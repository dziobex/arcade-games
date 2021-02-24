#include "GameHandler.h"

// looking for and removing completed rows //
void GameHandler::completeRows() {
	for (int i = 19; i >= 0; --i) {
		int counter = 0;
		for (int j = 0; j < 10; ++j)
			counter += board[i][j] != -1;
		if (counter == 10) {
			rows.push_back(i);
			for (int j = 0; j < 10; ++j)
				board[i][j] = -1;
		}
	}
}

GameHandler::GameHandler(float speed, float passedMin, float passedMinRows, sf::Texture& tetrominoTexture) {
	srand(time(NULL));
	this->speed = speed;
	this->passedMin = passedMin;
	this->tetrominoTexture = tetrominoTexture;
	this->passedMinRows = passedMinRows;

	heldShape = heldColor = -1;

	passed = passedRows = points = level = 0;
	gameOver = rowAnimation = false;
	currentTetromino = Tetromino(rand() % 7, rand() % 7, tetrominoTexture);
	nextTetromino = Tetromino(rand() % 7, rand() % 7, tetrominoTexture);

	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 10; ++j)
			board[i][j] = -1; // making empty board (-1 is an empty field)
}

void GameHandler::Move(float deltaTime, bool keyPressed) {
	if (rows.size() > 0) {
		passedRows += deltaTime * 350;
		 if (passedRows >= 80) { // full blocks go down (they're replaced with empty ones: -1)
			 for (int i = rows.size() - 1; i >= 0; --i)
				 for (int r = rows[i] - 1; r >= 0; --r)
					 for (int c = 0; c < 10; ++c) {
						 board[r + 1][c] = board[r][c];
						 board[r][c] = -1;
					 }
			points += 10 * rows.size() * (level + 1);
			rows.clear();
			passedRows = 0;
		}
	}
	passed += deltaTime * speed;
	if (currentTetromino.movable) {
		completeRows();
		int moveX = 0;
		float change = 1;

		// key handler //
		if (keyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			moveX = -25; // go left
		else if (keyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			moveX = 25; // go right
		else if (keyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			change = 0.01; // soft, fast drop
		else if (keyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			currentTetromino.Rotate(0, board); // rotate to the left side
		else if (keyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			currentTetromino.Rotate(1, board); // rotate to the right side
		else if (keyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			do {
				currentTetromino.Move(0, 25, board);
			} while (currentTetromino.movable); // hard drop
		else if (keyPressed && (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) && !currentTetromino.heldClicked) {
			if (heldShape == -1) {
				heldShape = currentTetromino.shape;
				heldColor = currentTetromino.color;
				currentTetromino = nextTetromino;
				nextTetromino = Tetromino(rand() % 7, rand() % 7, tetrominoTexture);
			}
			else {
				currentTetromino = Tetromino(heldShape, heldColor, tetrominoTexture);
				heldShape = heldColor = -1;
			}
			currentTetromino.heldClicked = true; // hold shape
		}

		currentTetromino.Move(moveX, 0, board);
		if (passed >= passedMin * change) {
			currentTetromino.Move(0, 25, board);
			passed = 0;
		}
	}
	if (!currentTetromino.movable) { // current tetromino's fallen
		for (int i = 0; i < 10; ++i)
			if (board[1][i] != -1) {
				gameOver = true;
				return; // checking for game over; if the 2nd row is filled - it means that no tetromino will fit
			}
		currentTetromino = nextTetromino; // the next tetromino becomes the current one
		nextTetromino = Tetromino(rand() % 7, rand() % 7, tetrominoTexture);
	}
}

void GameHandler::Render(sf::RenderWindow& window) {
	sf::RectangleShape boardShape(sf::Vector2f(10 * 25, 20 * 25));
	boardShape.setPosition(50, 50);
	boardShape.setFillColor(sf::Color::Black);
	boardShape.setOutlineColor(sf::Color::White);
	boardShape.setOutlineThickness(2);
	window.draw(boardShape);

	// preparing grid //
	sf::RectangleShape lines(sf::Vector2f(0.6, 20 * 25));
	lines.setFillColor(sf::Color::Color(128, 128, 128, 110));
	for (int i = 1; i <= 9; ++i) {
		lines.setPosition(50 + 25 * i, 50);
		window.draw(lines);
	}
	lines.setSize(sf::Vector2f(10 * 25, 0.6));
	for (int i = 1; i <= 19; ++i) {
		lines.setPosition(50, 50 + 25 * i);
		window.draw(lines);
	}

	sf::Font font;
	font.loadFromFile("resources/vcr_osd_mono.ttf");

	int addLevel = points / 200;
	if (addLevel > 0) { // increasing level and speed
		points -= addLevel * 200;
		level += addLevel;
		speed += 10 * addLevel;
	}

	// rendering text infomation //
	std::string textes[4]{ "LEVEL: " + std::to_string(level), "SCORE: " + std::to_string(points), "NEXT:", "HELD:" };
	int positionsX[4]{ 70, 100, 140, 325 };
	for (int i = 0; i < 4; ++i) {
		sf::Text levelText(textes[i], font, 20);
		levelText.setFillColor(sf::Color::White);
		levelText.setPosition(325, positionsX[i]);
		window.draw(levelText);
	}

	// rendeing the next shape & hold shape and their rectangles //
	for (int i = 0; i < 2; ++i) {
		sf::RectangleShape rightBorder(sf::Vector2f(125, 125));
		rightBorder.setPosition(325, 180 * (i + 1));
		rightBorder.setFillColor(sf::Color::Black);
		rightBorder.setOutlineColor(sf::Color::White);
		rightBorder.setOutlineThickness(2);
		window.draw(rightBorder);
	}

	Tetromino shapeRight(nextTetromino.shape, nextTetromino.color, tetrominoTexture, sf::Vector2f(275, 170));
	shapeRight.Render(window);
	if (heldShape != -1) {
		shapeRight = Tetromino(heldShape, heldColor, tetrominoTexture, sf::Vector2f(275, 360));
		shapeRight.Render(window);
	}

	// drawing tetrominoes blocks from the board //
	for (int i = 19; i >= 0; --i)
		for (int j = 9; j >= 0; --j)
			if (board[i][j] != -1) {
				sf::RectangleShape block(sf::Vector2f(25, 25));
				block.setTexture(&tetrominoTexture);
				block.setTextureRect(sf::IntRect(100 * board[i][j], 0, 100, 100));
				block.setPosition(50 + 25 * j, 25 + 25 * (i + 1));
				window.draw(block);
			}

	// animated removing rows //
	for (int row : rows)
		for (int j = 0; j < 10; ++j) {
			sf::RectangleShape block(sf::Vector2f(25, 25));
			block.setFillColor(sf::Color::White);
			block.setPosition(50 + 25 * j, 25 + 25 * (row + 1));
			window.draw(block);
		}

	if (rows.size() == 0) { // drawing tetromino's shade if no row is full
		Tetromino shade = currentTetromino;
		do {
			shade.Move(0, 25, board, true);
		} while (shade.movable);
		shade.Render(window, true);
		currentTetromino.Render(window);
	}
}
