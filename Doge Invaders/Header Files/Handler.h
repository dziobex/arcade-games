#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "FilesManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Shield.h"
#include "TexturesStorage.h"
#include "PopWindow.h"

class Handler {
	int stage;
	sf::RenderWindow window;
	bool scoreSaved;

	Player player;
	
	// main menu
	void initializeMainMenu();
	void mainMenu(sf::Vector2f mousePos, float deltaTime, bool keyPressed, bool enterReleased, bool mouseReleased,
					bool& playWin, bool &hsWin);

	int mainMenuHovered;

	std::vector<sf::Text> mainMenuTexts;
	std::vector<sf::RectangleShape> mainMenuPointers;
	sf::RectangleShape menuBackground;

	std::vector<sf::Sound> sounds;
	std::vector<sf::Sound*> shieldSounds;

	// pop windows

	/// main-menu ones
	PopWindow playWindow, hsWindow;

	/// in-game ones
	PopWindow pauseWindow, gameOverWindow;

	// board
	sf::RectangleShape lifeObject;

	int theme, themeIndex, score, wave, lives, enemiesCount;
	float bulletSpeed, bulletCooldown;

	sf::RectangleShape backgroundBoardObject;
	std::vector<sf::Text> boardTexts;

	void displayBoard();

	// preparations for a new game

	sf::RectangleShape gameBackground;

	std::vector<Enemy*> enemies;
	std::vector<Enemy*> aliveEnemies;
	std::vector<Enemy*> deadEnemies;
	std::vector<Shield*> shields;

	void prepareGame();
	void renewGame();

	// initializers
	void initializeBoard();
	void initializeTextures();
	void initializeSounds();
	void popWindowsInitializer();

public:
	Handler();
	void Loop();
};
