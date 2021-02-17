#pragma once
#include <SFML/Graphics.hpp>
#include "SamplesReader.h"
#include "Evolution.h"

class GameHandler
{
public:
	struct Button {
		enum class ButtonClicked { START_PAUSE, CLEAR, STEPS_FORWARD, GET, GPS_CHANGE, NONE };

		sf::Text text;
		ButtonClicked function;

		bool Hover(sf::Vector2f mousePos) { return text.getGlobalBounds().contains(mousePos); }
	};

	class Window {
	public:
		sf::RectangleShape window;
		Button buttons[5];
		sf::RectangleShape closeShape;
		sf::Text closeText;
		
		int speed;
		bool active;
		bool animation;
		bool bounce;
	public:
		Window() {};
		Window(float speed, sf::RectangleShape window, sf::Text closeText, GameHandler::Button buttons[5]);
		void Move(float deltaTime);
		void Render(sf::RenderWindow& win);
		int Events(sf::Vector2f mousePos, bool mouseClicked);

		void SetActive() { animation = active = true; }
	};

private:
	int speedy = 100;
	bool activated;

	SamplesReader samplesReader;
	Button buttonsWindow[5];
	sf::Text closeText;
	sf::Text aboutText;
	sf::Font font;
	Button buttons[6];
	int* gps;
	sf::RectangleShape info, about;
	Window sampleWindow, aboutWindow;

	void initialize();

public:
	GameHandler(sf::Font font, int& gps);

	Button::ButtonClicked GameEvents(sf::RenderWindow& window, Evolution& evolution, bool mouseClicked, float deltaTime, bool& simulation, bool pause);
	bool GetWindowActive() { return sampleWindow.window.getPosition().y > 500 || aboutWindow.window.getPosition().y > 500; }
};

