#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Fruit.h"
#include "TextHandler.h"
#include "Constants.h"

int main() {
	/* rendering window, setting icon */
	sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Snakey", sf::Style::Titlebar | sf::Style::Close);
	sf::Image icon;
	icon.loadFromFile("resources/snake.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	sf::RectangleShape background(sf::Vector2f(SEGMENT_SIZE, SEGMENT_SIZE));

	/* getting textures and font from files */
	sf::Texture bckTexture;
	bckTexture.loadFromFile("resources/white_block.png");
	background.setTexture(&bckTexture);
	sf::Texture snakeHeadTexture;
	snakeHeadTexture.loadFromFile("resources/dark_green_block.png");
	sf::Texture snakeBodyTexture;
	snakeBodyTexture.loadFromFile("resources/green_block.png");
	sf::Font textFont;
	textFont.loadFromFile("resources/font.ttf");

	/* creating instances of Snake, Fruit and TextHandler classes */
	Snake snake(sf::Vector2f(100, 100), 100, 10, snakeHeadTexture, snakeBodyTexture);
	Fruit fruit(snake);
	TextHandler handler(textFont);
	sf::Clock clock;

	/* creating fps counter in order to show player how fast is snakey */
	sf::Text fpsCounter("0 FPS", textFont, 20);
	fpsCounter.setFillColor(sf::Color::Black);
	fpsCounter.setPosition(WIN_WIDTH - 120, WIN_HEIGHT - 50);

	float deltaTime;
	bool pause = false, gameOver = false, started = false;

	printf("\033[32;5m*Snake woke up*\n\033[36;5mCONTROLS:\nESC - pause\nMOVING - arrows\n\n\033[31;5mTYPES OF FRUIT:\n\t1 seg: apple, banana, cherry\n\t2 seg: strawberry, kiwi\n\t3 seg: coffee\n\t0 seg (trololo): DIO\033[0m");

	while (window.isOpen()) {
		deltaTime = clock.getElapsedTime().asSeconds();
		clock.restart();
		sf::Event e;
		while (window.pollEvent(e)) {
			/* event handling if occurred */
			switch (e.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					pause = !pause;
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && (!started || gameOver)) {
					gameOver = false;
					started = true;
					snake = Snake(sf::Vector2f(100, 100), 100, 10, snakeHeadTexture, snakeBodyTexture);
				}
				break;
			default:
				break;
			}
		}

		/* displaying text screen or game screen */
		if (!started)
			handler.display(window, TextHandler::Events::MENU);
		else if (gameOver)
			handler.display(window, TextHandler::Events::GAME_OVER, snake.GetBodyPositions().size() + 1);
		else if (pause)
			handler.display(window, TextHandler::Events::PAUSE);
		else {
			window.clear();

			for (int i = 0; i < WIN_WIDTH / SEGMENT_SIZE; ++i)
				for (int j = 0; j < WIN_HEIGHT / SEGMENT_SIZE; ++j) {
					background.setPosition(sf::Vector2f(i * SEGMENT_SIZE, j * SEGMENT_SIZE));
					window.draw(background);
				}

			snake.update(deltaTime, gameOver, fpsCounter);
			snake.draw(window);
			fruit.draw(window);
			fruit.collision(snake);

			window.draw(fpsCounter);

			window.display();
		}
	}

	return 0;
}