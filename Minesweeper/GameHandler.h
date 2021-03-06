#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "Field.h"
#include "Constants.h"

class Window {
public:
	struct Option {
		sf::RectangleShape image; // represents single, big img
		sf::Text text;

		bool showText, showImage;
		Option() = default;
		Option(sf::Text text, bool showImage = false) { this->text = text; showText = true; this->showImage = showImage; }
		Option(sf::RectangleShape image, bool showText = false) { this->image = image; showImage = true; this->showText = showText; }

		bool Clicked(sf::Vector2f mousePos, bool mouseReleased);
		void Move(float moveX);
		void Render(sf::RenderWindow& window);
	};
private:
	sf::Vector2f startPosition;
	sf::RectangleShape mainWindow;
	sf::RectangleShape closeShape;
	sf::Text closeText;
	sf::Font font;

public:
	std::vector<Option> options;
	bool active, shown;
	bool showImg;

	// Window() = default;
	Window(sf::Vector2f startPosition, sf::Font& font);
	int Render(sf::RenderWindow& window, float deltaTime, bool& startGame, bool mouseReleased);
};

class GameHandler {
	// menu items //
	sf::Texture imageTexture;
	sf::Texture titleTexture;
	sf::RectangleShape titleShape;
	sf::Texture ballTexture;
	sf::Font font;

	// game items //
	sf::Texture texture, backgroundTexture;
	sf::RectangleShape backgroundShape, faceRect, faceShape;
	sf::Text minesCounterText, timeLeftText;
	sf::Text goBack;

	bool showMenu, victory, gameOver;
	int flagsCounter, fields, lastButton;
	int mines, sizeX, sizeY;

	struct Option {
		sf::Text text;
		sf::RectangleShape ball;

		bool Hover(sf::Vector2f mousePos, float deltaTime, bool moveBall);
	};

	Window* window;
	Option options[3];
	Field board[30][16];
public:

	GameHandler();
	int RenderMenu(sf::RenderWindow& window, float deltaTime, bool& startGame, bool mouseReleased);

	// game methods //
	void PrepareGame(int level, sf::RenderWindow& window, sf::View& view);
	int Play(sf::RenderWindow& window, float deltaTime, bool mousePressed, bool mouseReleased, bool& thinkingFace, float& secondsElapsed, bool& countTime, bool& game);
};