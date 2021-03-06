#include "Field.h"

Field::Field(sf::Vector2f location, sf::RectangleShape pattern, sf::Texture& texture) {
	this->location = location;
	this->textureField = pattern;
	this->texture = texture;

	IsMine = false;
	hover = false;
	State = 0;
	minesCounter = -2;

	myField = sf::RectangleShape(sf::Vector2f(30, 30));
	iconEmoji = sf::RectangleShape(sf::Vector2f(30, 30));

	iconEmoji.setTexture(&texture);
	myField.setPosition(location);
	textureField.setFillColor(sf::Color::Color(244, 244, 244));
}

// rendering field //
void Field::Render(sf::RenderWindow& window) {
	int x = -1, y = -1;
	if (State == 0)
		if (hover)
			textureField.setFillColor(sf::Color::Color(192, 192, 192));
		else
			textureField.setFillColor(sf::Color::White);
	if (State == -1) {
		if (IsMine) {
			x = 100;
			y = 50;
			textureField.setFillColor(sf::Color::Red);
		}
		else {
			if (minesCounter > 0) {
				x = (minesCounter - 1) * 50;
				y = 0;
			}
			textureField.setFillColor(sf::Color::Color(192, 192, 192));
		}
	}
	else if (State == 1 || State == 2) {
		x = State == 2 ? 50 : 0;
		y = 50;
	}

	textureField.setPosition(location);
	textureField.setOutlineThickness(1.2);
	textureField.setOutlineColor(sf::Color::Black);
	window.draw(textureField);

	if (x != -1) {
		iconEmoji.setTextureRect(sf::IntRect(x, y, 50, 50));
		iconEmoji.setPosition(location);
		window.draw(iconEmoji);
	}
}

// checking if the field can be opened - and setting the current field appearance and game end if the selected field is a mine //
bool Field::Open(bool& gameOver, sf::Vector2f mousePosition, bool leftMouseButton, bool mouseReleased, int lastButton, bool& thinkingFace) {
	if (gameOver) {
		State = -1;
		return false;
	}
	if (myField.getGlobalBounds().contains(mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mouseReleased && State != -1) {
		thinkingFace = true;
		hover = true;
		return false;
	}
	else
		hover = false;
	if (!myField.getGlobalBounds().contains(mousePosition))
		return false;

	if (State != -1 && mouseReleased) {
		if (lastButton == 0 && State < 1) {
			if (IsMine)
				gameOver = true;
			State = -1;
			return true;
		}
		else if (lastButton == 1)
			if (State < 2)
				State++;
			else
				State = 0;
	}
	return false;
}

void Field::Neighborhood(Field board[30][16], int dimX, int dimY, int indexX, int indexY) {
	if (IsMine) { // mines cannot have any friends
		minesCounter = -1;
		return;
	}
	// checking for mines in the neighborhood //
	if (minesCounter == -2) {
		minesCounter = 0;
		for (int x = indexX - 1 < 0 ? 0 : indexX - 1; (indexX + 1 < dimX && x < indexX + 2) || (indexX + 1 >= dimX && x < dimX); ++x)
			for (int y = indexY - 1 < 0 ? 0 : indexY - 1; (indexY + 1 < dimY && y < indexY + 2) || (indexY + 1 >= dimY && y < dimY); ++y)
				minesCounter += board[y][x].IsMine;
	}
	// opening blank fields in the neighborhood //
	for (int x = indexX - 1 < 0 ? 0 : indexX - 1; (indexX + 1 < dimX && x < indexX + 2) || (indexX + 1 >= dimX && x < dimX); ++x)
		for (int y = indexY - 1 < 0 ? 0 : indexY - 1; (indexY + 1 < dimY && y < indexY + 2) || (indexY + 1 >= dimY && y < dimY); ++y)
			if ((x == indexX || y == indexY) && board[y][x].State == -1 && !board[y][x].IsMine && board[y][x].minesCounter == 0) {
				State = -1;
				return;
			}
}
