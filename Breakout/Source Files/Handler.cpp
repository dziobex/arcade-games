#include "Handler.h"

// initialize handler: prepare the paddle, the brick manager and the ball //
Handler::Handler() {
	srand(time(NULL));
	paddle = Paddle(sf::Vector2f(300, 500));
	handler = BricksHandler();
	ball = Ball(150, rand () % 2);
}

// handle menu //
int Handler::Menu_Handler(sf::RenderWindow& window, sf::Font& font, sf::Text optionTextes[5], sf::RectangleShape hoveredBricks[6], sf::Vector2f mousePos, bool mouseReleased, float deltaTime) {
	int hovered = -1;
	sf::Text title("Breakout!", font, 110);
	title.setPosition(150, 90);

	sf::Color colors[6]{ sf::Color::Red, sf::Color(255,140,0), sf::Color::Yellow, sf::Color::Green, sf::Color::Blue, sf::Color::Magenta };

	for (int i = 0; i < 5; ++i) {
		window.draw(optionTextes[i]);
		if (optionTextes[i].getGlobalBounds().contains(mousePos)) {
			hovered = i;
			optionTextes[i].setFillColor(colors[i]);
			if (mouseReleased)
				return i + 1;
		}
		else optionTextes[i].setFillColor(sf::Color::White);
		window.draw(optionTextes[i]);
	}

	for (int i = 0; i < 6; ++i) {
		if (i == hovered && hoveredBricks[i].getPosition().y < 50)
			hoveredBricks[i].setPosition(hoveredBricks[i].getPosition() + sf::Vector2f(0, 200 * deltaTime));
		else if (i != hovered && hoveredBricks[i].getPosition().y > 10)
			hoveredBricks[i].setPosition(hoveredBricks[i].getPosition() - sf::Vector2f(0, 200 * deltaTime));
		window.draw(hoveredBricks[i]);
	}

	window.draw(title);
	return 0;
}

// set custom menu window with custom text from path, passed as an argument //
int Handler::Menu_Custom(sf::RenderWindow& window, sf::Font font, sf::Text goBackText, sf::Color hoverColor, bool mouseReleased, sf::Vector2f mousePos, std::string file, int currentStage, bool lines) {
	sf::Text text(fm.GetFromFile(file, !lines ? 40 : -1), font, 40);

	text.setPosition(10, 40);
	window.draw(text);
	if (goBackText.getGlobalBounds().contains(mousePos)) {
		goBackText.setFillColor(hoverColor);
		if (mouseReleased)
			return 0;
	}
	else
		goBackText.setFillColor(sf::Color::White);
	window.draw(goBackText);
	return currentStage;
}

// get 10 records and show them on the screen //
int Handler::Menu_Highscores(sf::RenderWindow& window, sf::Font font, sf::Text goBackText, sf::Color hoverColor, bool mouseReleased, sf::Vector2f mousePos) {
	std::string highscores = fm.GetHighscores();
	sf::Text highest(highscores.substr(0, highscores.find('\n')), font, 40);
	sf::Text text(highscores.erase(0, highscores.find('\n')), font, 40);
	highest.setFillColor(sf::Color::Yellow);
	highest.setPosition(50, 30);
	text.setPosition(50, 30);
	window.draw(highest);
	window.draw(text);
	if (goBackText.getGlobalBounds().contains(mousePos)) {
		goBackText.setFillColor(hoverColor);
		if (mouseReleased)
			return 0;
	}
	else
		goBackText.setFillColor(sf::Color::White);
	window.draw(goBackText);
	return 2;
}

void Handler::Loop(sf::RenderWindow& window) {
	sf::Clock clock;
	float deltaTime = 0;
	bool game = false, paused = false, scoreSaved = false;
	int points = 0, livesCounter = 3, level = 0, currentStage = 0;
	sf::Font font;
	font.loadFromFile("resources/aAnggota.ttf");

	// game items //
	sf::Text levelText("Level: 000", font, 25);
	sf::Text scoreText("Score: 000", font, 25);
	sf::Text livesText("Lives:", font, 25);
	levelText.setPosition(70, 10);
	scoreText.setPosition(270, 10);
	livesText.setPosition(470, 10);

	// messages displayed during the game //
	std::string gameItemsTextes[4]{ "GAME OVER!", "PRESS 'SPACE' TO START", "*PAUSED*", "PLAY AGAIN?" };
	sf::Text gameItems[4];
	for (int i = 0; i < 4; ++i) {
		gameItems[i] = sf::Text(gameItemsTextes[i], font, 50);
		gameItems[i].setPosition(WINDOW_WIDTH / 2 - gameItemsTextes[i].size() * 10, i != 3 ? 180 : 350);
		gameItems[i].setOutlineThickness(3);
		gameItems[i].setOutlineColor(sf::Color::Black);
	}

	// lives displayed in the forms of circles //
	sf::CircleShape lives[3];
	for (int i = 0; i < 3; ++i) {
		lives[i] = sf::CircleShape(10);
		lives[i].setFillColor(sf::Color::White);
		lives[i].setPosition(550 + 30 * i, 20);
	}

	sf::RectangleShape border(sf::Vector2f(40, 5));
	border.setFillColor(sf::Color::White);

	// menu handler //
	sf::Text title("Breakout!", font, 110);
	title.setPosition(150, 90);
	std::string options[5]{ "PLAY", "HIGHSCORES", "ABOUT", "SHORTCUTS", "QUIT" };
	sf::Color colors[6]{ sf::Color::Red, sf::Color(255, 140, 0), sf::Color::Yellow, sf::Color::Green, sf::Color::Blue, sf::Color::Magenta };

	sf::Text optionTextes[5];
	for (int i = 0; i < 5; ++i) {
		optionTextes[i] = sf::Text(options[i], font, 50);
		optionTextes[i].setPosition(245 + 10 * (10 - options[i].size()), 230 + i * 60);
	}

	// hover text animation (with moving bricks): main menu //
	sf::RectangleShape hoveredBricks[6];
	for (int i = 0; i < 6; ++i) {
		hoveredBricks[i] = sf::RectangleShape(sf::Vector2f(100, 30));
		hoveredBricks[i].setFillColor(colors[i]);
		hoveredBricks[i].setPosition(25 + i * 110, 10);
	}

	// go back button //
	sf::Text goBack("< BACK", font, 50);
	goBack.setPosition(20, 500);

	// void function (lambda) for restart game: reseting counters, points //
	auto restartGame = [&]() {
		if (!scoreSaved)
			fm.SetScore(points);
		paused = game = scoreSaved = false;
		livesCounter = 3;
		points = level = 0;
		paddle.GetPaddle().setPosition(sf::Vector2f(WINDOW_WIDTH / 2- paddle.GetPaddle().getSize().x / 2, 500));
		handler = BricksHandler();
		ball = Ball(150 + level * 10, rand() % 2);
		for (int i = 0; i < 3; ++i)
			lives[i].setFillColor(sf::Color::White);
	};

	while (window.isOpen()) {
		int move = 0;
		bool mouseReleased = false;
		deltaTime = clock.restart().asSeconds();
		sf::Event e;
		sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed: window.close(); break;
			case sf::Event::KeyPressed: {if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !game && currentStage == 1) game = true; if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && game) paused = !paused; }
			case sf::Event::MouseButtonReleased: mouseReleased = true;
			default: break;
			}
		}

		if (livesCounter > 0 && (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))) move = -200;
		else if (livesCounter > 0 && (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))) move = 200;

		window.clear();
		// stages handling: stages can be changed in the main menu (stage 0) //
		switch (currentStage) {
		case 0: currentStage = Menu_Handler(window, font, optionTextes, hoveredBricks, mousePos, mouseReleased, deltaTime); break; // main menu
		case 2: currentStage = Menu_Highscores(window, font, goBack, sf::Color(255, 140, 0), mouseReleased, mousePos); break; // highscores
		case 3: currentStage = Menu_Custom(window, font, goBack, sf::Color::Yellow, mouseReleased, mousePos, "about.txt", currentStage, 1); break; // 'about' option (from the menu)
		case 4: currentStage = Menu_Custom(window, font, goBack, sf::Color::Green, mouseReleased, mousePos, "shortcuts.txt", currentStage, 1); break; // available 'shortcuts' option (from the menu)
		case 5: window.close(); break;
		default: {
				paddle.Render(window);
				window.draw(goBack);
				// handle go back button events //
				if (goBack.getGlobalBounds().contains(mousePos)) {
					goBack.setFillColor(sf::Color::Red);
					if (mouseReleased) {
						restartGame();
						currentStage = 0;
						continue;
					}
				}
				else goBack.setFillColor(sf::Color::White);
				handler.Render(window, ball, deltaTime, points);
				// all bricks were destroyed, preparing the next level //
				if (handler.NextLevel()) {
					paddle.GetPaddle().setPosition(WINDOW_WIDTH / 2- paddle.GetPaddle().getSize().x / 2, 500);
					game = false;
					handler = BricksHandler();
					level++;
					levelText.setString("Level: " + std::string(3 - std::to_string(level).size(), '0') + std::to_string(level));
				}
				if (livesCounter > 0 && !paused)
					paddle.Move(move, deltaTime);

				ball.Collision(paddle.GetPaddle(), deltaTime);
				// life loss //
				if (!paused && livesCounter > 0 && !ball.Move(deltaTime, game, paddle.GetPaddle())) {
					livesCounter--;
					game = false;
					paddle.GetPaddle().setPosition(WINDOW_WIDTH / 2- paddle.GetPaddle().getSize().x / 2, 500);
					ball = Ball(150 + level * 10, rand() % 2);
					if (livesCounter >= 0)
						lives[livesCounter].setFillColor(sf::Color(255, 255, 255, 50));
				}
				ball.Render(window);

				// game over, restart the game after clicking 'restart' button //
				if (livesCounter <= 0) {
					window.draw(gameItems[0]);
					if (gameItems[3].getGlobalBounds().contains(mousePos)) {
						gameItems[3].setFillColor(sf::Color::Red);
						if (mouseReleased) {
							restartGame();
							continue;
						}
					}
					else gameItems[3].setFillColor(sf::Color::White);
					window.draw(gameItems[3]);
					if (!scoreSaved) {
						fm.SetScore(points);
						scoreSaved = true;
					}
				}

				if (!game && livesCounter > 0) window.draw(gameItems[1]);
				if (paused) window.draw(gameItems[2]);

				// draw texts (texts with level, score and lives) at the top of the screen //
				window.draw(levelText);
				scoreText.setString("Score: " + std::string(3 - std::to_string(points).size(), '0') + std::to_string(points));
				window.draw(scoreText);
				window.draw(livesText);
				for (int i = 0; i < 3; ++i)
					window.draw(lives[i]);

				// draw upper border //
				for (int i = 0; i < 14; ++i) {
					border.setPosition(50 * i + 5, UP_BORDER);
					window.draw(border);
				}
			}
		}
		window.display();
	}
}