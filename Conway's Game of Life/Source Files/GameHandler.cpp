#include "GameHandler.h"

void GameHandler::initialize() {
	// initialize buttons from the right side (humans left side) //
	buttons[0] = Button() = { sf::Text("> START", font, 20), Button::ButtonClicked::START_PAUSE };
	buttons[0].text.setPosition(550, 285);
	buttons[0].text.setFillColor(sf::Color::Black);
	buttons[1] = Button() = { sf::Text("> CLEAR", font, 20), Button::ButtonClicked::CLEAR };
	buttons[1].text.setPosition(550, 315);
	buttons[1].text.setFillColor(sf::Color::Black);
	buttons[2] = Button() = { sf::Text("> 5 STEPS FORWARD", font, 20), Button::ButtonClicked::STEPS_FORWARD };
	buttons[2].text.setPosition(550, 345);
	buttons[2].text.setFillColor(sf::Color::Black);
	buttons[3] = Button() = { sf::Text("GET A SAMPLE", font, 20), Button::ButtonClicked::GET };
	buttons[3].text.setPosition(550, 420);
	buttons[3].text.setFillColor(sf::Color::Black);
	buttons[4] = Button() = { sf::Text("> GPS: 0", font, 20), Button::ButtonClicked::GPS_CHANGE };
	buttons[4].text.setPosition(550, 255);
	buttons[4].text.setFillColor(sf::Color::Black);

	// initialize window //
	std::string availableOptions[5]{ "COPPERHEAD EATER", "GOSPER GLIDER GUN", "SCHICK ENGINE", "SWAN", "WEEKENDER" };
	for (int i = 0; i < 5; ++i) {
		buttonsWindow[i] = Button() = { sf::Text("> " + availableOptions[i], font, 15), Button::ButtonClicked::NONE };
		buttonsWindow[i].text.setPosition(125, 520 + 30 * i);
		buttonsWindow[i].text.setFillColor(sf::Color::Black);
	}

	info = sf::RectangleShape(sf::Vector2f(300, 400));
	info.setFillColor(sf::Color::White);
	info.setPosition(100, 501);

	closeText = sf::Text("X", font, 15);
	closeText.setFillColor(sf::Color::Black);
	closeText.setPosition(info.getPosition().x + 8, info.getPosition().y);

	sampleWindow = Window(50, info, closeText, buttonsWindow);
}

GameHandler::GameHandler(sf::Font font, int& gps) {
	this->font = font;
	this->gps = &gps;
	samplesReader = SamplesReader();
	initialize();
}

// handling game event (checking if any button was clicked) //
GameHandler::Button::ButtonClicked GameHandler::GameEvents(sf::RenderWindow& window, Evolution& evolution, bool mouseClicked, float deltaTime, bool& simulation, bool pause) {
	GameHandler::Button::ButtonClicked state = GameHandler::Button::ButtonClicked::NONE;
	buttons[0].text.setString(simulation && !pause ? "> PAUSE" : (simulation && pause ? "> RESUME" : "> START"));
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

	for (int i = 0; i < 5; ++i) {
		if (evolution.GetAliveCells() < 3 && i < 3 && !simulation) { // disable buttons if aliveCellsCounter is below 3 (no cell's going to survive)
			buttons[i].text.setFillColor(sf::Color::Color(150, 150, 150));
			buttons[i].text.setStyle(sf::Text::Style::Regular);
		}
		else if (buttons[i].function != GameHandler::Button::ButtonClicked::GPS_CHANGE) {
			buttons[i].text.setFillColor(sf::Color::Black);
			if (buttons[i].Hover(mousePos)) {
				buttons[i].text.setStyle(sf::Text::Style::Underlined);
				if (mouseClicked && buttons[i].function != Button::ButtonClicked::NONE)
					state = buttons[i].function;
				if (mouseClicked && i == 3 && !sampleWindow.active) {
					sampleWindow = Window(50, info, closeText, buttonsWindow);
					sampleWindow.SetActive();
					activated = true;
				}
			}
			else
				buttons[i].text.setStyle(sf::Text::Style::Regular);
		}

		if (activated)
			sampleWindow.Move(deltaTime);

		int index = sampleWindow.Events(mousePos, mouseClicked);
		if (index > 0) {
			evolution.clear();
			samplesReader.Read(index - 1, evolution.board);
			simulation = false;
		}
		window.draw(buttons[i].text);
		sampleWindow.Render(window);
	}

	buttons[4].text.setString("> GPS: " + std::to_string(*gps)); // update GPS

	return state;
}

// moving window //
void GameHandler::Window::Move(float deltaTime) {
	if (!active && GameHandler::Window::window.getPosition().y <= 500) { // hide window
		GameHandler::Window::window.move(0, abs(speed) * deltaTime);
		closeShape.move(0, abs(speed) * deltaTime);
		closeText.move(0, abs(speed) * deltaTime);
		// if (aboutText.getString().getSize() > 0)
		for (int i = 0; i < 5; ++i)
			GameHandler::Window::buttons[i].text.move(0, abs(speed) * deltaTime);
	}
	if (animation) { // windows animation
		GameHandler::Window::window.move(0, speed * deltaTime);
		closeShape.move(0, speed * deltaTime);
		closeText.move(0, speed * deltaTime);

		for (int i = 0; i < 5; ++i) // loop for moving buttons
			GameHandler::Window::buttons[i].text.move(0, speed * deltaTime);

		if ((window.getPosition().y > 100 && !bounce) || (window.getPosition().y >= 70 && bounce))
			speed = -abs(speed);
		else if (window.getPosition().y <= 30 && !bounce) {
			bounce = true;
			speed = abs(speed);
		}
		else if (window.getPosition().y <= 30 && bounce)
			animation = false;
	}
}

// rendering window's window //
void GameHandler::Window::Render(sf::RenderWindow& win) {
	win.draw(GameHandler::Window::window);
	win.draw(closeShape);
	win.draw(closeText);
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(win));
	for (int i = 0; i < 5; ++i) {
		if (GameHandler::Window::buttons[i].text.getGlobalBounds().contains(mousePos))
			GameHandler::Window::buttons[i].text.setStyle(sf::Text::Style::Underlined);
		else
			GameHandler::Window::buttons[i].text.setStyle(sf::Text::Style::Regular);
		win.draw(GameHandler::Window::buttons[i].text);
	}
}

// handling windows events (if any option button or X (closing) button was clicked) //
int GameHandler::Window::Events(sf::Vector2f mousePos, bool mouseClicked) {
	if (closeShape.getGlobalBounds().contains(mousePos)) {
		closeShape.setFillColor(sf::Color::Black);
		closeText.setFillColor(sf::Color::White);
		if (mouseClicked && active && !animation) {
			active = false;
			return 0;
		}
	}
	else {
		closeShape.setFillColor(sf::Color::Red);
		closeText.setFillColor(sf::Color::Black);
	}
	for (int i = 0; i < 5; ++i)
		if (Window::buttons[i].Hover(mousePos) && mouseClicked && active && !animation) {
			active = false;
			return i + 1;
		}
	return -1;
}

// initializing pop-window //
GameHandler::Window::Window(float speed, sf::RectangleShape window, sf::Text closeText, GameHandler::Button buttons[5]) {
	this->speed = speed;
	this->window = window;
	for (int i = 0; i < 5; ++i)
		this->buttons[i] = buttons[i];

	this->closeText = closeText;
	closeShape = sf::RectangleShape(sf::Vector2f(window.getSize().x * 0.1, window.getSize().y * 0.05));
	closeShape.setFillColor(sf::Color::Red);
	closeShape.setPosition(window.getPosition().x + window.getSize().x - closeShape.getSize().x, window.getPosition().y);
	this->closeText.setPosition(window.getPosition().x + window.getSize().x - closeShape.getSize().x + 8, window.getPosition().y);

	animation = active = false;
	bounce = false;
}
