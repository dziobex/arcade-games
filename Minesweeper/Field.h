#pragma once
#include <SFML/Graphics.hpp>

class Field
{
	sf::Vector2f location;
	sf::RectangleShape textureField;
	sf::RectangleShape iconEmoji;
	sf::Texture texture;
	bool hover;

public:
	sf::RectangleShape myField;
	int minesCounter;
	int State; // 0 - blank, 1 - flagged, 2 - question mark, -1 - opened
	bool IsMine; // field that contains information whether the fields is mine

	Field() = default;
	Field(sf::Vector2f location, sf::RectangleShape pattern, sf::Texture& texture);

	void Render(sf::RenderWindow& window);
	bool Open(bool& gameOver, sf::Vector2f mousePosition, bool leftMouseButton, bool mouseReleased, int lastButton, bool& thinkingFace);
	void Neighborhood(Field board[30][16], int dimX, int dimY, int indexX, int indexY);
};

