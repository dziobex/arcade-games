#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class TextHandler
{
public:
	enum Events { NONE, MENU, PAUSE, GAME_OVER };
private:
	sf::Font font;
public:
	TextHandler(sf::Font font);
	void display(sf::RenderWindow& window, Events e, int segments = 0);
};

