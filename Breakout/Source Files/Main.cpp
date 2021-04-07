#include "Handler.h"

int main() {
	/// program entry ///
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Breakout!", sf::Style::Titlebar | sf::Style::Close);

	// set cursor //
	sf::Image cursorImg;
	cursorImg.loadFromFile("resources/cursor.png");
	sf::Cursor cursor;
	cursor.loadFromPixels(cursorImg.getPixelsPtr(), cursorImg.getSize(), sf::Vector2u(0, 0));
	window.setMouseCursor(cursor);

	// set icon //
	sf::Image icon;
	icon.loadFromFile("resources/icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	Handler handler;
	handler.Loop(window);

	return 0;
}