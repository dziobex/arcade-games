#include "TextHandler.h"

/* initializing relations between scenes and buttons */
void TextHandler::initializeScenes() {
	scenes[0] = Scene() = { 0, sf::Text("PONG", font, 80), std::vector<Button>() = { Button() = {1, sf::Text("PLAY", font, 40) }, Button() = {2, sf::Text("CONTROLS", font, 40) }, Button() = {3, sf::Text("ABOUT", font, 40) } }, -1 };
	scenes[1] = Scene() = { 1, sf::Text("GAME MODE", font, 80), std::vector<Button>() = { Button() = {-1, sf::Text("Player vs Player", font, 40) }, Button() = {4, sf::Text("Player vs Computer", font, 40) } }, 0 };
	scenes[2] = Scene() = { 2, sf::Text("", font, 80), std::vector<Button>(), 0 };
	scenes[3] = Scene() = { 3, sf::Text("", font, 80), std::vector<Button>(), 0 };
	scenes[4] = Scene() = { 4, sf::Text("", font, 80), std::vector<Button>() = { Button() = {-1, sf::Text("EASY", font, 40) }, Button() = {-1, sf::Text("MEDIUM", font, 40) }, Button() = {-1, sf::Text("HARD", font, 40) } }, 1 }; // difficulty, ai
}

/* reading text from files for ABOUT and CONTROLS scenes */
sf::Text TextHandler::readFromFile(std::string filename) {
	std::ifstream reader("resources/" + filename);
	sf::Text result("", font, 19);
	std::string text, line;

	while (std::getline(reader, line)) {
		for (int i = 42; i < line.size(); i += 43)
			line = line.substr(0, i) + '\n' + line.substr(i, line.size());
		line += '\n';
		text += line;
	}
	result.setString(text);
	return result;
}

TextHandler::TextHandler(sf::Font font) {
	this->font = font;
	OptionalDifficulty = ComputerPlayer::Difficulty::NONE;
	initializeScenes();
}

/* showing textes that appear during the game */
void TextHandler::Show(sf::RenderWindow& window, ShowOptions option, int p1Points, int p2Points) {
	switch (option) {
	case ShowOptions::POINTS: {
			sf::Text points("0", font, 60);
			points.setPosition(sf::Vector2f(260, 50));
			points.setString(std::to_string(p1Points));
			window.draw(points);

			points.setPosition(sf::Vector2f(490, 50));
			points.setString(std::to_string(p2Points));
			window.draw(points);
		}
		break;
	case ShowOptions::PAUSE: {
			sf::Text text("PAUSED", font, 60);
			text.setPosition(sf::Vector2f(240, 200));
			text.setFillColor(sf::Color::White);
			window.draw(text);
			window.display();
		}
		break;
	case ShowOptions::GAMEOVER: {
		window.clear();
		sf::Text text("GAME OVER!", font, 80);
		text.setPosition(sf::Vector2f(40, 100));
		text.setFillColor(sf::Color::White);
		window.draw(text);

		text.setCharacterSize(40);
		text.setString((p1Points > p2Points ? "P1" : "P2") + std::string(" WON"));
		text.setPosition(sf::Vector2f(300, 200));
		window.draw(text);

		text.setString(std::to_string(p1Points) + " : " + std::to_string(p2Points));
		text.setPosition(sf::Vector2f(325, 250));
		window.draw(text);

		text.setString("Press enter to go back to the main menu...");
		text.setCharacterSize(12);
		text.setPosition(sf::Vector2f(15, 450));
		window.draw(text);

		window.display();
	}
	default:
		break;
	}
}

/* handling menu scenes, using buttons and relations between scenes and them */
void TextHandler::DisplayMenu(sf::RenderWindow& window, int& currentScene, bool mousePressed, bool& showMenu) {
	if (scenes[currentScene].bigText.getString() != "") {
		scenes[currentScene].bigText.setPosition(WIN_WIDTH / 2 - ((scenes[currentScene].bigText.getString().getSize() * 75) / 2), 100);
		window.draw(scenes[currentScene].bigText);
	}

	for (int i = 0; i < scenes[currentScene].buttons.size(); ++i) {
		scenes[currentScene].buttons[i].buttonText.setPosition(WIN_WIDTH / 2 - (scenes[currentScene].buttons[i].buttonText.getString().getSize() / 2 * 36), 200 + 50 * i);
		if (scenes[currentScene].buttons[i].buttonText.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
			scenes[currentScene].buttons[i].buttonText.setStyle(sf::Text::Style::Underlined);
			if (mousePressed) {
				if (scenes[currentScene].buttons[i].nextScene != -1)
					currentScene = scenes[currentScene].buttons[i].nextScene;
				else {
					showMenu = false;
					if (scenes[currentScene].id == 4)
						OptionalDifficulty = static_cast<ComputerPlayer::Difficulty>(i);
				}
				return;
			}
		}
		else
			scenes[currentScene].buttons[i].buttonText.setStyle(sf::Text::Style::Regular);
		window.draw(scenes[currentScene].buttons[i].buttonText);
	}

	if (scenes[currentScene].id == 2 || scenes[currentScene].id == 3) {
		sf::Text txt = readFromFile(currentScene == 2 ? "controls.txt" : "about.txt");
		txt.setPosition(10, 10);
		window.draw(txt);
	}

	if (scenes[currentScene].previousScene != -1) {
		sf::Text text("BACK", font, 40);
		text.setPosition(sf::Vector2f(WIN_WIDTH - 200, WIN_WIDTH / 2));
		if (text.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
			text.setStyle(sf::Text::Style::Underlined);
		else
			text.setStyle(sf::Text::Style::Regular);
		if (text.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) && mousePressed)
			currentScene = scenes[currentScene].previousScene;
		window.draw(text);
	}
}
