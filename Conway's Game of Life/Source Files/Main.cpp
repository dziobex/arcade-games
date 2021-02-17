#include <SFML/Graphics.hpp>
#include "Evolution.h"
#include "GameHandler.h"

void gameLoop(sf::RenderWindow& window);

int main() {
	// setting window properties //
	sf::RenderWindow window(sf::VideoMode(800, 500), "Conway's Game of Life", sf::Style::Titlebar | sf::Style::Close);

	// setting window icon //
	sf::Image icon;
	icon.loadFromFile("resources/icon.jpg");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	gameLoop(window); // game
	return 0;
}

void gameLoop(sf::RenderWindow& window) {
	// getting block image from file //
	sf::Texture blockTexture;
	blockTexture.loadFromFile("resources/block.png");
	sf::RectangleShape block(sf::Vector2f(10, 10));
	block.setTexture(&blockTexture);

	// getting main program font //
	sf::Font font;
	font.loadFromFile("resources/big_pixel_font.otf");

	// rendering headers, evolution status //
	sf::Text text("Laboratory:", font, 30);
	text.setPosition(540, 15);
	text.setFillColor(sf::Color::Black);

	sf::Text generationText("> Generation:\n 0", font, 20);
	generationText.setPosition(550, 60);
	generationText.setFillColor(sf::Color::Black);
	sf::Text aliveCellsText("> Alive Cells:\n 0", font, 20);
	aliveCellsText.setPosition(550, 110);
	aliveCellsText.setFillColor(sf::Color::Black);

	sf::Text textSettings("Settings:", font, 30);
	textSettings.setPosition(540, 210);
	textSettings.setFillColor(sf::Color::Black);

	int speed = 10;
	GameHandler handler(font, speed);

	bool update = false, simulation = false, movePointer = false, pause = false;
	int firstTouch = -1;

	Evolution evolution(1000, 100, blockTexture);

	sf::Clock clock;
	float deltaTime;

	// initializing GPS setters (line & circle) //
	sf::RectangleShape line(sf::Vector2f(100, 5));
	line.setPosition(sf::Vector2f(685, 265));
	line.setFillColor(sf::Color::Black);

	sf::CircleShape point(6.0);
	point.setOrigin(sf::Vector2f(2, 3));
	point.setPosition(sf::Vector2f(690, 265));
	point.setFillColor(sf::Color::Black);

	while (window.isOpen()) {
		bool mouseClicked = false;
		deltaTime = clock.getElapsedTime().asSeconds();
		clock.restart();

		sf::Event e;
		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				update = true;
				mouseClicked = true;
				if (line.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
					point.setPosition(sf::Mouse::getPosition(window).x, point.getPosition().y);
					movePointer = true;
				}
				break;
			case sf::Event::MouseButtonReleased:
				update = false;
				movePointer = false;
				firstTouch = -1;
				break;
			default:
				break;
			}
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		mousePos = sf::Vector2i((int)floor(mousePos.x / 10), (int)floor(mousePos.y / 10));
		if (update && handler.GetWindowActive()) { // drawing on the board
			if (mousePos.x >= 0 && mousePos.x < 50 && mousePos.y >= 0 && mousePos.y < 50) {
				if (firstTouch == -1)
					firstTouch = evolution.board[mousePos.x][mousePos.y]; // first place where user touched the board
				evolution.board[mousePos.x][mousePos.y] = !firstTouch;
			}
		}

		window.clear(sf::Color::White);

		if (simulation && !pause) // checking if evolution is possible (simulation's running)
			evolution.evolve(deltaTime);
		evolution.Render(window);

		GameHandler::Button::ButtonClicked state = handler.GameEvents(window, evolution, mouseClicked, deltaTime, simulation, pause);
		switch (state) {
		case GameHandler::Button::ButtonClicked::START_PAUSE:
			if (simulation && mouseClicked)
				pause = !pause;
			simulation = true;
			break;
		case GameHandler::Button::ButtonClicked::CLEAR: {
			pause = false;
			simulation = false;
			evolution.clear();
			break;
		}
		case GameHandler::Button::ButtonClicked::STEPS_FORWARD: {
			int startGenaration = evolution.GetGeneration();
			do {
				evolution.evolve(deltaTime);
			} while (evolution.GetGeneration() < startGenaration + 5); // 5 steps forward
			break;
		}
		default:
			break;
		}

		// updating textes rendering them //
		generationText.setString("> Generation:\n " + std::to_string(evolution.GetGeneration()));
		aliveCellsText.setString("> Alive Cells:\n " + std::to_string(evolution.GetAliveCells()) + " (" + std::to_string(100 * evolution.GetAliveCells() / 2500) + "%)");
		window.draw(text);
		window.draw(textSettings);
		window.draw(generationText);
		window.draw(aliveCellsText);

		// rendering slider and getting GPS //
		window.draw(line);
		if (movePointer && sf::Mouse::getPosition(window).x >= 686 && sf::Mouse::getPosition(window).x <= 785) {
			point.move(sf::Mouse::getPosition(window).x - point.getPosition().x, 0);
			speed = point.getPosition().x - 685;
			evolution.SetSpeed(speed * 100);
		}

		window.draw(point);
		window.display();
	}

}