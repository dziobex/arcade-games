#include "GameHandler.h"

GameHandler::GameHandler() {
	srand(time(NULL));
	titleShape = sf::RectangleShape(sf::Vector2f(350, 80));
	titleShape.setPosition(20, 70);
	titleTexture.loadFromFile("resources/logo.png");
	titleTexture.setSmooth(1);
	titleShape.setTexture(&titleTexture);

	texture.loadFromFile("resources/assets.png");
	faceRect = sf::RectangleShape(sf::Vector2f(60, 60));
	faceRect.setFillColor(sf::Color::Color(244, 244, 244));
	faceRect.setOutlineThickness(1.2);
	faceRect.setOutlineColor(sf::Color::Black);
	sf::RectangleShape pattern(sf::Vector2f(30, 30));
	for (int i = 0; i < 30; ++i)
		for (int j = 0; j < 16; ++j)
			board[i][j] = Field(sf::Vector2f(35 + (i * 35), 120 + (j * 35)), pattern, texture);

	font.loadFromFile("resources/children_sans.ttf");
	std::string optionsString[3]{ "PLAY", "ABOUT", "EXIT" };
	ballTexture.loadFromFile("resources/blob_salute.png");
	for (int i = 0; i < 3; ++i) {
		sf::RectangleShape ballShape(sf::Vector2f(50, 50));
		ballShape.setTexture(&ballTexture);

		sf::Text playBtn(optionsString[i], font, 50);
		playBtn.setFillColor(sf::Color::Black);
		playBtn.setPosition(200 - (optionsString[i].size() * 9), 160 + 50 * i);

		ballShape.setOrigin(25, 25);
		ballShape.setPosition(playBtn.getPosition().x - 40 - 180, playBtn.getPosition().y+30);
		options[i].text = playBtn;
		options[i].ball = ballShape;
	}

	goBack = sf::Text("MENU", font, 30);
	faceShape = sf::RectangleShape(sf::Vector2f(50, 50));
	faceShape.setTexture(&texture);

	minesCounterText = sf::Text("Mines left:", font, 30);
	minesCounterText.setFillColor(sf::Color::Black);

	timeLeftText = sf::Text("Time:", font, 30);
	timeLeftText.setFillColor(sf::Color::Black);

	backgroundTexture.loadFromFile("resources/bck.jpg");
	backgroundShape = sf::RectangleShape(sf::Vector2f(400, 700));
	backgroundShape.setTexture(&backgroundTexture);

	this->window = new Window(sf::Vector2f(0, 0), font);
	std::string optionsWindow[3]{ "BEGGINER", "INTERMEDIATE", "EXPERT" };
	for (int i = 0; i < 3; ++i) {
		sf::Text begginerText(optionsWindow[i], font, 30);
		begginerText.setPosition(420, 150 + i * 40);
		begginerText.setFillColor(sf::Color::Black);
		window->options.push_back(begginerText);
	}

	imageTexture.loadFromFile("resources/about.png");
	sf::RectangleShape imageWindow(sf::Vector2f(190, 280));
	imageWindow.setTexture(&imageTexture);
	imageWindow.setPosition(405, 120);
	window->options.push_back(imageWindow);
}

// rendering main menu - function returns an index of chosen option //
int GameHandler::RenderMenu(sf::RenderWindow& window, float deltaTime, bool& startGame, bool mouseReleased) {
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
	for (int i = 0; i < 3; ++i) {
		if (options[i].Hover(mousePos, deltaTime, !this->window->shown)) {
			options[i].text.setStyle(sf::Text::Style::Bold);
			if (mouseReleased && !this->window->shown)
				if (i != 2) {
					this->window->active = true;
					this->window->showImg = i == 1;
				}
				else
					window.close();
		}
		else
			options[i].text.setStyle(sf::Text::Style::Regular);
		window.draw(options[i].text);
		window.draw(options[i].ball);
	}

	window.draw(titleShape);
	return this->window->Render(window, deltaTime, startGame, mouseReleased);
}

// checking whether the option is hovered //
bool GameHandler::Option::Hover(sf::Vector2f mousePos, float deltaTime, bool moveBall) {
	if (text.getGlobalBounds().contains(mousePos) && moveBall) {
		if (ball.getPosition().x < text.getPosition().x - 40 && moveBall) {
			ball.setPosition(ball.getPosition().x + (deltaTime * 300), ball.getPosition().y);
			ball.rotate((deltaTime * 600));
		}
		return true;
	}
	else if (ball.getPosition().x + 25 > 0 && moveBall) {
		ball.setPosition(ball.getPosition().x - (deltaTime * 300), ball.getPosition().y);
		ball.rotate(-(deltaTime * 600));
	}
	return false;
}

// preparing window //
Window::Window(sf::Vector2f startPosition, sf::Font& font) {
	active = false;
	this->font = font;
	this->startPosition = startPosition;
	mainWindow = sf::RectangleShape(sf::Vector2f(200, 300));
	mainWindow.setFillColor(sf::Color::Color(211, 211, 211));
	mainWindow.setOutlineThickness(2);
	mainWindow.setOutlineColor(sf::Color::Black);

	closeShape = sf::RectangleShape(sf::Vector2f(30, 20));
	closeShape.setFillColor(sf::Color::Red);
	closeShape.setOutlineThickness(2);
	closeShape.setOutlineColor(sf::Color::Black);

	mainWindow.setPosition(400, 100);
	closeShape.setPosition(570, 100);
	closeText = sf::Text("X", font, 25);
	closeText.setPosition(580, 95);
}

// rendering pop-window - function returns an index of chosen option //
int Window::Render(sf::RenderWindow& window, float deltaTime, bool& startGame, bool mouseReleased) {
	if (active && mainWindow.getPosition().x > 100) {
		shown = true;
		mainWindow.setPosition(mainWindow.getPosition() + sf::Vector2f(-(deltaTime * 500), 0));
		closeShape.setPosition(closeShape.getPosition() + sf::Vector2f(-(deltaTime * 500), 0));
		closeText.setPosition(closeText.getPosition() + sf::Vector2f(-(deltaTime * 500), 0));
		for (int i = 0; i < options.size(); ++i)
			options[i].Move(-(deltaTime * 500));
	}
	else if (!active && mainWindow.getPosition().x < 410) {
		mainWindow.setPosition(mainWindow.getPosition() + sf::Vector2f((deltaTime * 500), 0));
		closeShape.setPosition(closeShape.getPosition() + sf::Vector2f((deltaTime * 500), 0));
		closeText.setPosition(closeText.getPosition() + sf::Vector2f((deltaTime * 500), 0));
		for (int i = 0; i < options.size(); ++i)
			options[i].Move((deltaTime * 500));
		if (mainWindow.getPosition().x >= 410)
			shown = false;
	}

	window.draw(mainWindow);
	window.draw(closeShape);

	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

	if (closeShape.getGlobalBounds().contains(mousePos)) {
		closeShape.setFillColor(sf::Color::Black);
		closeText.setFillColor(sf::Color::White);
		if (mouseReleased)
			active = false;
	}
	else {
		closeShape.setFillColor(sf::Color::Red);
		closeText.setFillColor(sf::Color::Black);
	}
	window.draw(closeText);

	for (int i = 0; i < options.size(); ++i) {
		if (options[i].Clicked(mousePos, mouseReleased) && !showImg) {
			startGame = true;
			return i;
		}
		if ((options[i].showImage && this->showImg) || (options[i].showText && !this->showImg))
		options[i].Render(window);
	}
	return -1;
}

// setting board for a new game //
void GameHandler::PrepareGame(int level, sf::RenderWindow& window, sf::View& view) {
	showMenu = victory = gameOver = false;
	flagsCounter = fields = 0;
	mines = sizeX = sizeY = 0;
	lastButton = -1;
	int sizeWindowX = 0;
	switch (level) {
	case 0: mines = BEGINNER_MINES; sizeX = BEGINNER_DIM_X; sizeY = BEGINNER_DIM_Y;
		window.setSize(sf::Vector2u(BEGINNER_WIN_X, BEGINNER_WIN_Y));
		view.setSize(BEGINNER_WIN_X, BEGINNER_WIN_Y);
		view.setCenter(BEGINNER_WIN_X / 2, BEGINNER_WIN_Y / 2);
		window.setView(view);
		sizeWindowX = BEGINNER_WIN_X;

		backgroundTexture.loadFromFile("resources/bck.jpg");
		backgroundShape = sf::RectangleShape(sf::Vector2f(INTERMEDIATE_WIN_X, INTERMEDIATE_WIN_Y));
		backgroundShape.setTexture(&backgroundTexture);
		break;
	case 1: mines = INTERMEDIATE_MINES; sizeX = INTERMEDIATE_DIM_X; sizeY = INTERMEDIATE_DIM_Y;
		window.setSize(sf::Vector2u(INTERMEDIATE_WIN_X, INTERMEDIATE_WIN_Y));
		view.setSize(INTERMEDIATE_WIN_X, INTERMEDIATE_WIN_Y);
		view.setCenter(INTERMEDIATE_WIN_X / 2, INTERMEDIATE_WIN_Y / 2);
		window.setView(view);
		sizeWindowX = INTERMEDIATE_WIN_X;

		backgroundTexture.loadFromFile("resources/bck1.jpg");
		backgroundShape = sf::RectangleShape(sf::Vector2f(INTERMEDIATE_WIN_X, INTERMEDIATE_WIN_Y));
		backgroundShape.setTexture(&backgroundTexture);
		break;
	case 2: mines = EXPERT_MINES; sizeX = EXPERT_DIM_X; sizeY = EXPERT_DIM_Y;
		window.setSize(sf::Vector2u(EXPERT_WIN_X, EXPERT_WIN_Y));
		view.setSize(EXPERT_WIN_X, EXPERT_WIN_Y);
		view.setCenter(EXPERT_WIN_X / 2, EXPERT_WIN_Y / 2);
		window.setView(view);
		sizeWindowX = EXPERT_WIN_X;

		backgroundTexture.loadFromFile("resources/bck2.jpg");
		backgroundShape = sf::RectangleShape(sf::Vector2f(EXPERT_WIN_X, EXPERT_WIN_Y));
		backgroundShape.setTexture(&backgroundTexture);
		break;
	default: break;
	}

	faceRect.setPosition(sizeWindowX / 2 - 30, 20);
	faceShape.setPosition(sizeWindowX / 2 - 25, 25);
	timeLeftText.setPosition(10, 20);
	minesCounterText.setPosition(sizeWindowX - 80, 20);
	faceShape.setTextureRect(sf::IntRect(0, 100, 50, 50));
	goBack.setPosition(faceShape.getPosition().x, faceShape.getPosition().y + 55);

	for (int i = 0; i < sizeX; ++i)
		for (int j = 0; j < sizeY; ++j) {
			board[i][j].IsMine = false;
			board[i][j].State = 0;
			board[i][j].minesCounter = -2;
		}

	int counter = 0;
	do {
		int x = rand() % sizeX, y = rand() % sizeY;
		if (!board[x][y].IsMine) {
			board[x][y].IsMine = true;
			counter++;
		}
	} while (counter < mines);
}

// rendering game stuff //
int GameHandler::Play(sf::RenderWindow& window, float deltaTime, bool mousePressed, bool mouseReleased, bool& thinkingFace, float& secondsElapsed, bool& countTime, bool& game) {
	int gameStarted = 0;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		lastButton = 0;
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		lastButton = 1;
	window.draw(backgroundShape);

	flagsCounter = 0;
	fields = 0;
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
	for (int i = 0; i < sizeX; ++i)
		for (int j = 0; j < sizeY; ++j) {
			board[i][j].Neighborhood(board, sizeY, sizeX, j, i);
			flagsCounter += board[i][j].State == 1;
			if (!victory && board[i][j].Open(gameOver, mousePos, mousePressed, mouseReleased, lastButton, thinkingFace)) {
				gameStarted = 1;
				countTime = true;
			}
			if (!gameOver && board[i][j].State == -1)
				fields++;
			board[i][j].Render(window);
		}

	// set face //
	if (!victory) {
		if (thinkingFace)
			faceShape.setTextureRect(sf::IntRect(50, 100, 50, 50));
		else
			faceShape.setTextureRect(sf::IntRect(0, 100, 50, 50));
		if (gameOver) {
			faceShape.setTextureRect(sf::IntRect(150, 100, 50, 50));
			countTime = false;
		}
	}
	if (faceShape.getGlobalBounds().contains(mousePos) && mouseReleased) {
		gameStarted = 2;
		secondsElapsed = 0;
		countTime = false;
	}

	minesCounterText.setString("Mines:\n" + std::to_string(mines - flagsCounter));
	window.draw(minesCounterText);
	window.draw(faceRect);
	window.draw(faceShape);

	timeLeftText.setString("Time:\n" + std::to_string((int)floor(secondsElapsed)));
	window.draw(timeLeftText);
	if (goBack.getGlobalBounds().contains(mousePos)) {
		goBack.setStyle(sf::Text::Style::Bold);
		if (mouseReleased) {
			game = false;
			secondsElapsed = 0;
			countTime = false;
		}
	} else goBack.setStyle(sf::Text::Style::Regular);
	window.draw(goBack);

	if (fields == sizeX * sizeY - mines) {
		victory = true;
		countTime = false;
		faceShape.setTextureRect(sf::IntRect(100, 100, 50, 50));
	}
	return gameStarted;
}

// checking whether pop-window option was clicked //
bool Window::Option::Clicked(sf::Vector2f mousePos, bool mouseReleased) {
	if (text.getGlobalBounds().contains(mousePos)) {
		text.setStyle(sf::Text::Style::Bold);
		if (mouseReleased)
			return true;
	} else
		text.setStyle(sf::Text::Style::Regular);
	return false;
}

// moving pop-window option //
void Window::Option::Move(float moveX) {
	if (showText)
		text.setPosition(text.getPosition() + sf::Vector2f(moveX, 0));
	if (showImage)
		image.setPosition(image.getPosition() + sf::Vector2f(moveX, 0));
}

// rendering pop-window option //
void Window::Option::Render(sf::RenderWindow& window) {
	if (showText)
		window.draw(text);
	if (showImage)
		window.draw(image);
}
