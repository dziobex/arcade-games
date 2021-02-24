#include "MainMenu.h"

// preparing buttons and logo //
MainMenu::MainMenu() {
	std::string options[4]{ "PLAY", "HIGHSCORES", "CONTROLS", "ABOUT" };
	font.loadFromFile("resources/vcr_osd_mono.ttf");
	for (int i = 0; i < 4; ++i) {
		buttons[i] = sf::Text(options[i], font, 40);
		buttons[i].setFillColor(sf::Color::White);
		buttons[i].setPosition(250 - (20 * options[i].size() / 2), 250 + 40 * i);
	}

	logoFont.loadFromFile("resources/modern-tetris.ttf");
	logo = sf::Text("TETRIS", logoFont, 70);
	logo.setPosition(65, 110);
}

int MainMenu::Render(sf::RenderWindow& window, bool mouseClicked) {
	window.draw(logo);

	sf::Texture miniShapeTexture;
	miniShapeTexture.loadFromFile("resources/mini_shapy.png");
	sf::RectangleShape miniShape(sf::Vector2f(50, 50));
	miniShape.setTexture(&miniShapeTexture); // mini-mate that shows which button (option) is hovered

	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
	int indexReturn = -1;
	for (int i = 0; i < 4; ++i) {
		window.draw(buttons[i]);
		if (buttons[i].getGlobalBounds().contains(mousePos)) {
			miniShape.rotate(90 * i);
			miniShape.setPosition(buttons[i].getPosition().x - (50 * (i == 0 || i == 3)), buttons[i].getPosition().y + (50 * (i == 2 || i == 3)));
			window.draw(miniShape);
			if (mouseClicked)
				indexReturn = i;
		}
	}
	return indexReturn; // information for game loop (-1 means no option was selected)
}
