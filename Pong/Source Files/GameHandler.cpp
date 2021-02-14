#include "GameHandler.h"

void GameHandler::Game(sf::RenderWindow& window) {
	sf::RectangleShape line(sf::Vector2f(10, 50));
	HumanPlayer* human = new HumanPlayer(sf::Vector2f(20, 100), sf::Vector2f(50, 50), 250, false);
	Player* opponent = new HumanPlayer(sf::Vector2f(20, 900), sf::Vector2f(50, 50), 250, false);
	Ball ball(human, opponent, sf::Vector2f(20, 20), sf::Vector2f(400, 100), -200);

	sf::Font font;
	font.loadFromFile("resources/atari-font.ttf");

	sf::Vector2i mousePosition = sf::Mouse::getPosition();
	sf::Mouse::setPosition(sf::Vector2i(800, 500), window);
	TextHandler textHandler(font);
	sf::Texture cursorTexture;
	cursorTexture.loadFromFile("resources/cursor.png");
	sf::RectangleShape mouseCursor(sf::Vector2f(20, 20));
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
					pause = !pause;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !menu && gameOver) {
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
			window.clear(sf::Color::Black);
			textHandler.DisplayMenu(window, menuIndex, mousePressed, menu);
			mouseCursor.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
			window.draw(mouseCursor);
			window.display();
			if (!menu) {
				pvc = textHandler.OptionalDifficulty != ComputerPlayer::Difficulty::NONE;
				if (pvc)
					opponent = new ComputerPlayer(sf::Vector2f(20, 100), sf::Vector2f(730, 50), 250, textHandler.OptionalDifficulty);
				else
					opponent = new HumanPlayer(sf::Vector2f(20, 100), sf::Vector2f(730, 50), 250, true);
			}
		}
		else if (human->GetPoints() >= 11 || opponent->GetPoints() >= 11) {
			textHandler.Show(window, TextHandler::ShowOptions::GAMEOVER, human->GetPoints(), opponent->GetPoints());
			gameOver = true;
		}
		else if (pause)
			textHandler.Show(window, TextHandler::ShowOptions::PAUSE);
		else {
			if (ball.GameOver) {
				int side = -1;
				if (abs(human->GetPosition().x - ball.GetPosition().x) > abs(opponent->GetPosition().x - ball.GetPosition().x)) {
					human->IncreasePoints();
					side = 1;
				}
				else
					opponent->IncreasePoints();
				ball = Ball(human, opponent, sf::Vector2f(20, 20), sf::Vector2f(400, 100), side * 200);
			}

			human->move(deltaTime);
			if (pvc)
				dynamic_cast<ComputerPlayer*>(opponent)->move(deltaTime, ball);
			else
				dynamic_cast<HumanPlayer*>(opponent)->move(deltaTime);
			ball.move(deltaTime);

			window.clear();

			human->draw(window);
			opponent->draw(window);
			ball.draw(window);
			textHandler.Show(window, TextHandler::ShowOptions::POINTS, human->GetPoints(), opponent->GetPoints());

			for (int i = 0; i < 500 / 50 - 1; ++i) {
				line.setPosition(sf::Vector2f(800 / 2 - 10 / 2, (50 + 20) * i + 10));
				window.draw(line);
			}

			window.display();
		}

	}
}
