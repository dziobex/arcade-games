#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include "ComputerPlayer.h"
#include "Constants.h"

class TextHandler
{
public:
	enum class ShowOptions { POINTS, PAUSE, GAMEOVER };
private:
	struct Button {
		int nextScene; // go to the next scene (scene with nextScene id), if equals -1 then start game
		sf::Text buttonText; // properties of the button, eg text, position, size of the text
	};

	struct Scene {
		int id;
		sf::Text bigText; // show big text like "PONG" in front of the screen; can be "" (then don't show)
		std::vector<Button> buttons; // buttons in that scene; include "BACK" button if possible
		int previousScene;
	};

	sf::RenderWindow window;
	sf::Font font;
	Scene scenes[5];

	void initializeScenes();
	sf::Text readFromFile(std::string filename);

public:
	ComputerPlayer::Difficulty OptionalDifficulty; // default value is NONE
	TextHandler(sf::Font font);

	void Show(sf::RenderWindow& window, ShowOptions option, int p1Points = 0, int p2Points = 0);
	void DisplayMenu(sf::RenderWindow& window, int& currentScene, bool mousePressed, bool& showMenu);
};

