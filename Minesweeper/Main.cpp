#include <SFML/Graphics.hpp>
#include "Field.h"
#include "GameHandler.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(400, 500), "Blobsweeper", sf::Style::Titlebar | sf::Style::Close);

	sf::View view(sf::Vector2f(200, 250), sf::Vector2f(400, 500));
	window.setView(view);

	sf::Image icon;
	icon.loadFromFile("resources/icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	
	// seconds counter //
	sf::Clock clock, deltaTimeClock;
	float secondsElapsed = 0, deltaTime = 0;
	int nextScene = -1;

	GameHandler handler;
	bool game = false, countTime = false;
	while (window.isOpen()) {
		if (countTime)
			secondsElapsed += clock.getElapsedTime().asSeconds();
		clock.restart();

		deltaTime = deltaTimeClock.restart().asSeconds();

		bool mousePressed = false, mouseReleased = false, thinkingFace = false;
		int flagsCounter = 0, fields = 0;
		sf::Event e;
		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed: window.close(); break;
			case sf::Event::MouseButtonPressed: mousePressed = true; break;
			case sf::Event::MouseButtonReleased: mouseReleased = true; break;
			default: break;
			}
		}

		window.clear(sf::Color::Color(255, 235, 59));
		if (!game) {
			window.setSize(sf::Vector2u(400, 500));
			view.setSize(400, 500);
			view.setCenter(400 / 2, 500 / 2);
			window.setView(view);

			nextScene = handler.RenderMenu(window, deltaTime, game, mouseReleased);
			if (nextScene != -1)
				handler.PrepareGame(nextScene, window, view);
		}
		else {
			int nextChoice = handler.Play(window, deltaTime, mousePressed, mouseReleased, thinkingFace, secondsElapsed, countTime, game);
			if (nextChoice == 2)
				handler.PrepareGame(nextScene, window, view);
		}
		window.display();
	}

	return 0;
}