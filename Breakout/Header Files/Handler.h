#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "BricksHandler.h"
#include "Constants.h"
#include "FileManager.h"

class Handler
{
	FileManager fm;
	Paddle paddle;
	BricksHandler handler;
	Ball ball;

	int Menu_Handler(sf::RenderWindow& window, sf::Font& font, sf::Text optionTextes[5], sf::RectangleShape hoveredBricks[6], sf::Vector2f mousePos, bool mouseReleased, float deltaTime);
	int Menu_Custom(sf::RenderWindow& window, sf::Font font, sf::Text goBackText, sf::Color hoverColor, bool mouseReleased, sf::Vector2f mousePos, std::string file, int currentStage, bool lines);
	int Menu_Highscores(sf::RenderWindow& window, sf::Font font, sf::Text goBackText, sf::Color hoverColor, bool mouseReleased, sf::Vector2f mousePos);
	int Game(sf::RenderWindow& window);
public:
	Handler();
	void Loop(sf::RenderWindow& window);
};

