#include <SFML/Graphics.hpp>
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "Ball.h"
#include "TextHandler.h"
#include "Constants.h"

void game(sf::RenderWindow& window);

int main() {
	/* rendering window, hiding cursor and setting icon */
	sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Pong", sf::Style::Titlebar | sf::Style::Close);
	window.setMouseCursorVisible(false);
	sf::Image icon;
	icon.loadFromFile("resources/icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	game(window); // calling main game loop

	return 0;
}

void game(sf::RenderWindow& window) {
	sf::RectangleShape line(sf::Vector2f(10, 50));
	HumanPlayer* human = new HumanPlayer(sf::Vector2f(BALL_CURSOR_SIZE, 100), sf::Vector2f(50, 50), WIN_HEIGHT / 2, false);
	Player* opponent = new HumanPlayer(sf::Vector2f(BALL_CURSOR_SIZE, WIN_WIDTH - 70), sf::Vector2f(50, 50), WIN_HEIGHT / 2, false);
	Ball ball(human, opponent, sf::Vector2f(BALL_CURSOR_SIZE, BALL_CURSOR_SIZE), sf::Vector2f(WIN_WIDTH / 2, 100), -200);

	/* getting font from file */
	sf::Font font;
	font.loadFromFile("resources/atari-font.ttf");

	/* setting new cursor */
	sf::Vector2i mousePosition = sf::Mouse::getPosition();
	sf::Mouse::setPosition(sf::Vector2i(WIN_WIDTH, WIN_HEIGHT), window);
	TextHandler textHandler(font);
	sf::Texture cursorTexture;
	cursorTexture.loadFromFile("resources/cursor.png");
	sf::RectangleShape mouseCursor(sf::Vector2f(BALL_CURSOR_SIZE, BALL_CURSOR_SIZE));
	mouseCursor.setTexture(&cursorTexture);

	bool pause = false, gameOver = false, menu = true;
	int menuIndex = 0;
	sf::Clock clock;
	float deltaTime;
	bool pvc = false;

	while (window.isOpen()) {
		bool mousePressed = false;
		deltaTime = clock.getElapsedTime().asSeconds();
		clock.restart();

		sf::Event e;
		while (window.pollEvent(e)) {
			switch (e.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					pause = !pause; // pausing and resuming game, after pressing ENTER key
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !menu && gameOver) { // go back to the menu
					gameOver = false;
					menu = true;
					menuIndex = 0;
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					mousePressed = true;
				break;
			default:
				break;
			}
		}

		if (menu) {
			/* calling menu handler */
			window.clear(sf::Color::Black);
			textHandler.DisplayMenu(window, menuIndex, mousePressed, menu);
			mouseCursor.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
			window.draw(mouseCursor);
			window.display();
			if (!menu) { // menu is not longer called, setup for new round
				pvc = textHandler.OptionalDifficulty != ComputerPlayer::Difficulty::NONE;
				human = new HumanPlayer(sf::Vector2f(BALL_CURSOR_SIZE, 100), sf::Vector2f(50, 50), WIN_HEIGHT / 2, false);
				if (pvc)
					opponent = new ComputerPlayer(sf::Vector2f(BALL_CURSOR_SIZE, 100), sf::Vector2f(WIN_WIDTH - 70, 50), WIN_HEIGHT / 2, textHandler.OptionalDifficulty);
				else
					opponent = new HumanPlayer(sf::Vector2f(BALL_CURSOR_SIZE, 100), sf::Vector2f(WIN_WIDTH - 70, 50), WIN_HEIGHT / 2, true);
				ball = Ball(human, opponent, sf::Vector2f(BALL_CURSOR_SIZE, BALL_CURSOR_SIZE), sf::Vector2f(WIN_WIDTH / 2, 100), -200);
			}
		}
		else if (human->GetPoints() >= 11 || opponent->GetPoints() >= 11) { // checking for game over
			textHandler.Show(window, TextHandler::ShowOptions::GAMEOVER, human->GetPoints(), opponent->GetPoints());
			gameOver = true;
		}
		else if (pause) // pausing game
			textHandler.Show(window, TextHandler::ShowOptions::PAUSE);
		else {
			/* setting new ball position after fail */
			if (ball.GameOver) {
				int side = -1;
				if (abs(human->GetPosition().x - ball.GetPosition().x) > abs(opponent->GetPosition().x - ball.GetPosition().x)) {
					human->IncreasePoints();
					side = 1;
				}
				else
					opponent->IncreasePoints();
				ball = Ball(human, opponent, sf::Vector2f(BALL_CURSOR_SIZE, BALL_CURSOR_SIZE), sf::Vector2f(WIN_WIDTH / 2, 100), side * 200);
			}

			/* moving players */
			human->move(deltaTime);
			if (pvc)
				dynamic_cast<ComputerPlayer*>(opponent)->move(deltaTime, ball);
			else
				dynamic_cast<HumanPlayer*>(opponent)->move(deltaTime);
			ball.move(deltaTime);

			window.clear();

			/* rendering players and points info */
			human->draw(window);
			opponent->draw(window);
			ball.draw(window);
			textHandler.Show(window, TextHandler::ShowOptions::POINTS, human->GetPoints(), opponent->GetPoints());

			/* rendering center board line */
			for (int i = 0; i < WIN_HEIGHT / 50 - 1; ++i) {
				line.setPosition(sf::Vector2f(WIN_WIDTH / 2 - 10 / 2, (50 + BALL_CURSOR_SIZE) * i + 10));
				window.draw(line);
			}

			window.display();
		}

	}
}