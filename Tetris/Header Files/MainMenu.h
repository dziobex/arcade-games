#pragma once
#include <SFML/Graphics.hpp>
class MainMenu // class to manage main menu (events will be handled in the game loop)
{
	sf::Text logo;
	sf::Text buttons[4];
	sf::Font font, logoFont;
public:
	MainMenu();
	int Render(sf::RenderWindow& window, bool mouseClicked);
};

