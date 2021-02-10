#include "TextHandler.h"

TextHandler::TextHandler(sf::Font font) {
	this->font = font;
}

/* checking which option should be displayed and display proper textes */
void TextHandler::display(sf::RenderWindow& window, Events e, int segments) {
	window.clear(sf::Color::Black);
	sf::Text txt;
	sf::String drawText, addictionalText;

	switch (e) {
	case Events::MENU:
		drawText = "Snakey";
		addictionalText = "Press ENTER to play...";
		break;
	case Events::GAME_OVER:
		drawText = "GAME OVER";
		addictionalText = "Press ENTER to play again...";

		txt = sf::Text("You earned " + std::to_string(segments) + " points.", font, 10);
		txt.setPosition(10, 550);
		txt.setFillColor(sf::Color::Red);
		window.draw(txt);

		break;
	case Events::PAUSE:
		drawText = "PAUSED";
		addictionalText = "Press ESC to resume...";
		break;
	default:
		break;
	}

	txt = sf::Text(drawText, font, 50);
	txt.setPosition(WIN_WIDTH / 2 - (drawText.getSize() * 20), 300 - 80);
	txt.setFillColor(sf::Color::Red);
	window.draw(txt);

	txt = sf::Text(addictionalText, font, 20);
	txt.setPosition(WIN_WIDTH / 2 - (addictionalText.getSize() * 10), WIN_HEIGHT / 2);
	txt.setFillColor(sf::Color::Red);
	window.draw(txt);

	window.display();
}
