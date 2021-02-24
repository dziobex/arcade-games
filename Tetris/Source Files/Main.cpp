#include <SFML/Graphics.hpp>
#include "GameHandler.h"
#include "MainMenu.h"
#include "PopWindow.h"
#include "FilesManager.h"

void gameLoop(sf::RenderWindow& window);

int main() {
	sf::RenderWindow window(sf::VideoMode(500, 600), "Tetris", sf::Style::Titlebar | sf::Style::Close);
	sf::Image icon;
	icon.loadFromFile("resources/icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	gameLoop(window);

	return 0;
}

void gameLoop(sf::RenderWindow& window) {
	sf::Texture tetrominoesTexture;
	tetrominoesTexture.loadFromFile("resources/blocks.png");

	GameHandler handler(100, 70, 60, tetrominoesTexture);

	sf::Clock clock;
	float deltaTime;
	bool pause = false, game = false;

	sf::Font font;
	font.loadFromFile("resources/vcr_osd_mono.ttf");
	sf::Text pausedText("PAUSED", font, 30);
	pausedText.setFillColor(sf::Color::White);
	pausedText.setPosition(125, 250);

	MainMenu menu;
	FilesManager manager;

	std::string titles[3]{ "Highscores", "Controls", "About" };
	PopWindow popWindows[3];
	std::string text = "";

	// menu pop-windows //
	for (int i = 0; i < 3; ++i) {
		sf::Text title(titles[i], font, 20);
		title.setOutlineThickness(2);
		title.setOutlineColor(sf::Color::Black);
		popWindows[i] = PopWindow(title, font, sf::Text("", font, 20), sf::Color::Color(220, 220, 220), sf::Vector2f(85, 50), sf::Vector2f(300, (i == 1 ? -100 : (i == 2 ? 50 : 0)) + 300));
		popWindows[i].optionalText.setFillColor(sf::Color::Black);
		popWindows[i].AnimationSettings(400, 200);
		popWindows[i].Active = false;
	}

	text = manager.GetHighscores();
	popWindows[0].optionalText.setString(text);
	text = manager.GetFromFile("controls.txt", -1);
	popWindows[1].optionalText.setString(text);
	text = manager.GetFromFile("about.txt", 25);
	popWindows[2].optionalText.setString(text);

	// buttons during the game //
	sf::Text buttons[2];
	for (int i = 0; i < 2; ++i) {
		buttons[i] = sf::Text(i == 0 ? "RESTART" : "MAIN MENU", font, 20);
		buttons[i].setPosition(340, 520 + 30 * i);
	}

	bool savedScore = false;
	while (window.isOpen()) {
		bool keyPressed = false, mouseClicked = false;
		deltaTime = clock.restart().asSeconds();
		sf::Event e;

		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed: {
				keyPressed = true;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !handler.IsGameOver())
					pause = !pause;
				break;
			}
			case sf::Event::MouseButtonPressed: {
				if (e.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
					mouseClicked = true;
				break;
			}
			default: break;
			}
		}

		window.clear();
		if (!game) { // menu events
			int menuResult = menu.Render(window, mouseClicked); // getting number for buttons handling
			if (menuResult == 0)
				game = true;
			else if (menuResult != -1) {
				int count = 0;
				for (int i = 0; i < 3; ++i)
					count += popWindows[i].Active;
				if (count == 0) {
					if (menuResult == 1)
						popWindows[menuResult - 1].optionalText.setString(manager.GetHighscores());
					popWindows[menuResult - 1].Show(); // showing pop windows
				}
			}
		}
		else { // game events
			handler.Render(window);
			if (pause || handler.IsGameOver()) {
				if (pause) {
					pausedText.setString("PAUSED");
					pausedText.setPosition(125, 250);
				}
				window.draw(pausedText);
			}
			if (!pause && !handler.IsGameOver())
				handler.Move(deltaTime, keyPressed);
			if (handler.IsGameOver() && pausedText.getString() != "GAME OVER") {
				pausedText.setString("GAME OVER");
				pausedText.setPosition(100, 250);
				if (!savedScore) // checking whether score saving is possible
					manager.SetScore(handler.GetScore());
				savedScore = true;
			}

			sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
			for (int i = 0; i < 2; ++i) {
				if (buttons[i].getGlobalBounds().contains(mousePos)) {
					buttons[i].setStyle(sf::Text::Style::Underlined);
					if (mouseClicked) {
						handler = GameHandler(100, 70, 60, tetrominoesTexture);
						if (i != 0)
							game = false;
						savedScore = false;
						pause = false;
					}
				}
				else
					buttons[i].setStyle(sf::Text::Style::Regular);
				window.draw(buttons[i]);
			}
		}

		for (int i = 0; i < 3; ++i)
			if (popWindows[i].Active) { // rendering windows if possible
				popWindows[i].Move(deltaTime);
				popWindows[i].Render(window, mouseClicked);
			}
		window.display();
	}
}