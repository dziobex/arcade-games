#include "Handler.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

// initialize main menu items
void Handler::initializeMainMenu() {
	// initialize main menu background
	menuBackground = sf::RectangleShape(sf::Vector2f(WIN_WIDTH + 300, WIN_HEIGHT));
	menuBackground.setTexture(&TexturesStorage::GetTexture("menu_background"));

	// initialize texts
	sf::Text txt("Doge Invaders", TexturesStorage::GetFont(), 100);
	txt.setOrigin(txt.getGlobalBounds().width / 2, 0);
	txt.setPosition((WIN_WIDTH + 300) / 2, 140);
	mainMenuTexts.push_back(txt);

	// initialize options texts
	std::string options[3]{ "Play", "Highscores", "Quit" };
	for (int i = 0; i < 3; i++) {
		txt = sf::Text(options[i], TexturesStorage::GetFont(), 50);
		txt.setOrigin(txt.getGlobalBounds().width / 2, 0);
		txt.setPosition((WIN_WIDTH + 300) / 2, 360 + 100 * i);
		mainMenuTexts.push_back(txt);
	}
	
	// set the same props to all texts
	for (auto& mmt : mainMenuTexts) {
		mmt.setFillColor(sf::Color::Yellow);
		mmt.setOutlineColor(sf::Color::Black);
		mmt.setOutlineThickness(3);
	}

	mainMenuHovered = 1;

	// initialize main menu pointer (points which option is hovered)
	for (int i = 1; i < mainMenuTexts.size(); i++) {
		sf::RectangleShape pointer(sf::Vector2f(60, 50));
		pointer.setPosition(mainMenuTexts[i].getPosition() - sf::Vector2f(mainMenuTexts[i].getGlobalBounds().width / 2 + 75, 0));
		pointer.setTexture(&TexturesStorage::GetTexture("cheenaut"));
		pointer.setTextureRect(sf::IntRect(0, 0, 120, 100));
		if (i != mainMenuHovered)
			pointer.setFillColor(sf::Color(pointer.getFillColor().r, pointer.getFillColor().g, pointer.getFillColor().b, 0));
		else
			mainMenuTexts[i].setFillColor(sf::Color(255, 250, 205));
		mainMenuPointers.push_back(pointer);
	}

}

// display main menu
void Handler::mainMenu(sf::Vector2f mousePos,  float deltaTime, bool keyPressed, bool enterReleased, bool mouseReleased, bool& playWin, bool& hsWin) {
	window.draw(menuBackground);
	// draw texts from main menu
	for (auto& mmt : mainMenuTexts)
		window.draw(mmt);

	for (int i = 1; i < mainMenuTexts.size(); i++) {
		auto& pointer = mainMenuPointers[i - 1];
		// change pointer's visibility
		if (i == mainMenuHovered && pointer.getFillColor().a < 255) {
			// the pointer's option is hovered - show the pointer
			pointer.setFillColor(sf::Color(pointer.getFillColor().r, pointer.getFillColor().g, pointer.getFillColor().b, pointer.getFillColor().a + 1));
			mainMenuTexts[i].setFillColor(sf::Color(255, 250, 205));
		}
		else if (i != mainMenuHovered && pointer.getFillColor().a > 0) {
			// the pointer's option isn't hovered - hide the pointer
			pointer.setFillColor(sf::Color(pointer.getFillColor().r, pointer.getFillColor().g, pointer.getFillColor().b, pointer.getFillColor().a - 1));
			mainMenuTexts[i].setFillColor(sf::Color::Yellow);
		}
	}

	// display pointers
	for (auto& mmp : mainMenuPointers)
		window.draw(mmp);

	if (!playWin && !hsWin) {
		// change main menu hovered index, based on the mouse movements
		for (int i = 1; i < mainMenuTexts.size(); i++)
			if (mainMenuTexts[i].getGlobalBounds().contains(mousePos)) {
				mainMenuHovered = i;
				break;
			}


		// change main menu hovered index, based on keyboard
		if (keyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && mainMenuHovered < mainMenuPointers.size())
			mainMenuHovered++;
		else if (keyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && mainMenuHovered == mainMenuPointers.size())
			mainMenuHovered = 1;
		else if (keyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && mainMenuHovered > 1)
			mainMenuHovered--;
		else if (keyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && mainMenuHovered <= 1)
			mainMenuHovered = mainMenuPointers.size();
	}

	// handle options
	std::string currentHs = "";
	if (!playWin && !hsWin && enterReleased || (mainMenuTexts[mainMenuHovered].getGlobalBounds().contains(mousePos) && mouseReleased)) {
		// some option has been chosen
		switch (mainMenuHovered) {
		case 1:
			// only one window can be displayed
			playWin = 1;
			hsWin = 0;
			break;
		case 2:
			playWin = 0;
			hsWin = 1;
			// display current highscores
			currentHs = FilesManager::GetHighscores();
			hsWindow.ChangeContent(0, currentHs.substr(0, currentHs.find('\n')));
			hsWindow.ChangeContent(1, currentHs.substr(currentHs.find('\n') + 1, currentHs.size() - currentHs.find('\n') - 1));
			break;
		case 3:
			window.close();
			break;
		default: break;
		}
	}

	if (playWin) {
		playWindow.Launch();
		playWindow.Checker(mousePos, mouseReleased);
		playWindow.Display(window, mousePos, deltaTime, keyPressed, enterReleased, mouseReleased);

		if (playWindow.GetClickedOption() != -1) {
			theme = playWindow.GetClickedOption();
			// set pause window image rect, according to the current theme
			pauseWindow.SetImageTextureRect(1, sf::IntRect(theme * 300, 0, 300, 300));
			// I messed up with assets order, sorry
			if (theme == 1) theme = 2;
			else if (theme == 2) theme = 1;
			renewGame();
			stage = 1;
			playWindow.Reset();
		}

		// close button's been clicked / ESC key's been pressed -> hide the window
		if (playWindow.IsReady())
			playWin = 0;
	}
	if (hsWin) {
		hsWindow.Launch();
		hsWindow.Checker(mousePos, mouseReleased);
		hsWindow.Display(window, mousePos, deltaTime, keyPressed, enterReleased, mouseReleased);

		// close button's been clicked / ESC key's been pressed -> hide the window
		if (hsWindow.IsReady())
			hsWin = 0;
	}
}

void Handler::initializeBoard() {
	boardTexts.clear();

	// board background
	backgroundBoardObject = sf::RectangleShape(sf::Vector2f(300, WIN_HEIGHT));
	backgroundBoardObject.setFillColor(sf::Color::Black);
	backgroundBoardObject.setPosition(WIN_WIDTH, 0);

	// texts
	sf::Text helperText("Space", TexturesStorage::GetFont(), 60);
	helperText.setFillColor(sf::Color::White);
	helperText.setPosition(WIN_WIDTH + 30, 10);
	boardTexts.push_back(helperText);

	helperText = sf::Text("board", TexturesStorage::GetFont(), 55);
	helperText.setFillColor(sf::Color::White);
	helperText.setPosition(WIN_WIDTH + 100, 75);
	boardTexts.push_back(helperText);

	// the last text is text with scores
	lifeObject = sf::RectangleShape(sf::Vector2f(50, 50));
	lifeObject.setTexture(&TexturesStorage::GetTexture("life"));

	std::string labels[4]{ "Score", "Highscore", "Wave", "Lives" };
	for (int i = 0; i < 4; ++i) {
		std::string value = i == 0 ? std::to_string(score) : i == 1 ? "0" : i == 2 ? std::to_string(wave) : "";
		helperText = sf::Text("* " + labels[i] + ":\n" + value, TexturesStorage::GetFont(), 30);
		helperText.setFillColor(sf::Color::White);
		helperText.setPosition(WIN_WIDTH + 50, 185+ 110 * i);
		boardTexts.push_back(helperText);
	}

	popWindowsInitializer();
}

// display SPACE BOARD (the right side of the game window)
void Handler::displayBoard() {
	window.draw(backgroundBoardObject);

	for (auto& txt : boardTexts)
		window.draw(txt);

	// display lives
	for (int i = 0; i < 3; ++i) {
		lifeObject.setPosition(WIN_WIDTH + 50 + 60 * i, 185 + 110 * 3 + 50);
		if (i >= lives)
			lifeObject.setFillColor(sf::Color(lifeObject.getFillColor().r, lifeObject.getFillColor().g, lifeObject.getFillColor().b, 40));
		else
			lifeObject.setFillColor(sf::Color(lifeObject.getFillColor().r, lifeObject.getFillColor().g, lifeObject.getFillColor().b, 254));
		window.draw(lifeObject);
	}
}

void Handler::prepareGame(int theme) {
	// clear containers from the previous theme

	for (auto& e : enemies)
		delete e;

	for (auto& de : deadEnemies)
		// delete myGoodMood;
		delete de;


	enemies.clear();
	aliveEnemies.clear();
	deadEnemies.clear();
	
	// full them with values that fit to the current theme

	// prepare "shelves" for enemies objects
	for (int i = 0; i < 32; ++i) {
		enemies.push_back(new Enemy());
	}

	// initialize enemies
	for (int i = 0, c = 0; i < 4; ++i)
		for (int j = 0; j < 8; ++j, ++c) {
			int randomSkin = rand() % 13;
			enemies[c]->Initialize(theme, randomSkin, sf::Vector2f(50 + 100 * j, 50 + 100 * i), sounds[c], sounds[33]);
			enemies[c]->id = i;
			aliveEnemies.push_back(enemies[c]);
		}

	// initialize background
	int backgroundIndex = rand() % 4;
	gameBackground.setTextureRect(sf::IntRect(450 * backgroundIndex, theme * 400, 450, 400));
	gameBackground.setFillColor(sf::Color(100, 100, 100));
}

// prepare objects for a new game
void Handler::renewGame() {
	for (auto& s : shields) // memory leaks sumcks
		delete s;

	// refresh the shields
	shields.clear();

	// prepare shields
	for (int i = 0; i < 3; ++i)
		shields.push_back(new Shield());

	int shieldIndex = rand() % 4; // select which texture for the current theme will be choosen (4 options)
	for (int i = 0; i < 3; ++i)
		shields[i]->Initialize(theme, shieldIndex, sf::Vector2f(50 + 300 * i, 610), sf::Color::Green, i, shieldSounds);

	// refresh player
	player = Player(sounds[32], sounds[34]);

	score = wave = 0;
	lives = 3;
	enemiesCount = 32;
	bulletCooldown = SHOT_COOLDOWN;
	scoreSaved = false;
	prepareGame(theme);
}

// initialize all textures that will be used
void Handler::initializeTextures() {
	TexturesStorage::SetFont("resources/data/ComicSansMS3.ttf");
	TexturesStorage::AddTexture("menu_background", "resources/backgrounds/mb0.jpg");
	TexturesStorage::AddTexture("explosion", "resources/bullets/explosion.png");
	TexturesStorage::AddTexture("life", "resources/data/life.png");
	TexturesStorage::AddTexture("shields_crash", "resources/shields/crash.png");
	TexturesStorage::AddTexture("pause_doge", "resources/data/dog.png");
	TexturesStorage::AddTexture("pause_doge_hair", "resources/data/hair.png");
	TexturesStorage::AddTexture("dead_doge", "resources/data/dead.png");
	TexturesStorage::AddTexture("cheenaut", "resources/enemies/cheenauts.png");
	TexturesStorage::AddTexture("flames", "resources/enemies/flames.png");
	TexturesStorage::AddTexture("player_death", "resources/enemies/death.png");
	TexturesStorage::AddTexture("backgrounds", "resources/backgrounds/backgrounds.jpg");
	TexturesStorage::AddTexture("enemy_faces", "resources/enemies/enemy_faces.png");
	TexturesStorage::AddTexture("enemy_hair", "resources/enemies/hair.png");
	TexturesStorage::AddTexture("enemy_bullets", "resources/bullets/bullets.png");
	TexturesStorage::AddTexture("enemy_paw", "resources/enemies/paw.png");
	TexturesStorage::AddTexture("player", "resources/player/player.png");
	TexturesStorage::AddTexture("player_bullet", "resources/bullets/p_b0.png");
	TexturesStorage::AddTexture("shields", "resources/shields/shields.jpg");
	TexturesStorage::AddTexture("crown", "resources/data/crown.png");

	// initialize sounds
	TexturesStorage::AddSound("enemy_shot", "resources/music/enemy_shot.wav");
	TexturesStorage::AddSound("player_shot", "resources/music/player_shot.wav");
	TexturesStorage::AddSound("score", "resources/music/score.wav");
	TexturesStorage::AddSound("death", "resources/music/enemy_hit.wav");
	TexturesStorage::AddSound("player_hit", "resources/music/player_hit.wav");
	TexturesStorage::AddSound("break", "resources/music/brick_break.wav");
	TexturesStorage::AddSound("tap", "resources/music/tap.wav");
}

// initialize sounds that will be used
void Handler::initializeSounds() {
	// enemies shot sounds
	for (int i = 0; i < 32; i++) {
		sf::Sound s;
		s.setBuffer(TexturesStorage::GetSound("enemy_shot"));
		sounds.push_back(s);
	}

	{
		// player shot sound
		sf::Sound s;
		s.setBuffer(TexturesStorage::GetSound("player_shot"));
		sounds.push_back(s);

		// "reached aim..." sounds
		s.setBuffer(TexturesStorage::GetSound("death"));
		sounds.push_back(s);

		s.setBuffer(TexturesStorage::GetSound("player_hit"));
		sounds.push_back(s);

		// additional score sound
		s.setBuffer(TexturesStorage::GetSound("score"));
		sounds.push_back(s);
	}

	// shields sounds
	// player has 3 shields, each shield has 10 segments (2 rows, each row consists of 5 pieces)
	for (int i = 0; i < 30; i++) {
		sf::Sound* s1 = new sf::Sound(), * s2 = new sf::Sound();
		s1->setBuffer(TexturesStorage::GetSound("tap"));
		shieldSounds.push_back(s1);
		s2->setBuffer(TexturesStorage::GetSound("break"));
		shieldSounds.push_back(s2);

	}
}

void Handler::popWindowsInitializer() {
	// play window
	playWindow = PopWindow(700, 650);
	playWindow.SetBackgroundColor(sf::Color::Black);
	playWindow.SetPosition(140, 50);
	playWindow.SetBorders(sf::Vector2f(0, 0), sf::Vector2f(WIN_WIDTH + 300, 0));
	playWindow.SetTitle("Select theme:", sf::Color::White, 80, 100, 40);
	playWindow.AddButton("Genshin", 40, sf::Color::White, sf::Color::Black, 80, 210, 145, 40);
	playWindow.AddSubtext("(ehe)", 15, 170, 230, 0, 165);
	playWindow.AddButton("Pokemon", 40, sf::Color::White, sf::Color::Black, 80, 280, 160, 40);
	playWindow.AddSubtext("(Gotta Kill 'Em All!)", 15, 105, 300, 1, 250);
	playWindow.AddButton("Jojo", 40, sf::Color::White, sf::Color::Black, 80, 350, 350, 40);
	playWindow.AddSubtext("(I'll snap your fingers in half like matchsticks!)", 15, 85, 370, 2, 35, 0);

	// highscores window
	hsWindow = PopWindow(650, 650);
	hsWindow.SetBackgroundColor(sf::Color::Black);
	hsWindow.SetPosition(140, 50);
	hsWindow.SetBorders(sf::Vector2f(0, 0), sf::Vector2f(WIN_WIDTH + 300, 0));
	hsWindow.SetTitle("Highscores:", sf::Color::White, 80, 100, 40);
	hsWindow.AddContent("1.", 30, 90, 200, sf::Color::Yellow);
	hsWindow.AddContent("2., 3., 4...", 30, 90, 240);
	hsWindow.AddImage("crown", 40, 40, 40, 200);

	// pause window
	pauseWindow = PopWindow(600, 650);
	pauseWindow.SetBackgroundColor(sf::Color::Black);
	pauseWindow.SetPosition(140, 50);
	pauseWindow.SetBorders(sf::Vector2f(0, 0), sf::Vector2f(WIN_WIDTH, 0));
	pauseWindow.SetTitle("Paused.", sf::Color::White, 80, 160, 40);
	pauseWindow.AddButton("Continue", 30, sf::Color::White, sf::Color::Black, 80, 300, 120, 40);
	pauseWindow.AddSubtext("(I'm chad)", 15, 122, 315, 0, 90);
	pauseWindow.AddButton("Renew", 30, sf::Color::White, sf::Color::Black, 80, 350, 90, 40);
	pauseWindow.AddSubtext("(I fu- messed up)", 15, 42, 365, 1, 140);
	pauseWindow.AddButton("Quimt the game", 30, sf::Color::White, sf::Color::Black, 80, 400, 232, 40);
	pauseWindow.AddSubtext("(I'm virgin)", 15, 85, 415, 2, 30, 0);
	pauseWindow.AddImage("pause_doge", 300, 300, 280, 340);
	pauseWindow.AddImage("pause_doge_hair", 300, 300, 280, 340);
	pauseWindow.SetImageTextureRect(1, sf::IntRect(0, 0, 300, 300));
	pauseWindow.AddContent("\"\"", 20, 50, 150);
	FilesManager::InitializeQuotes(50);

	// game over window
	gameOverWindow = PopWindow(600, 650);
	gameOverWindow.SetBackgroundColor(sf::Color::Black);
	gameOverWindow.SetPosition(140, 50);
	gameOverWindow.SetBorders(sf::Vector2f(0, 0), sf::Vector2f(WIN_WIDTH, 0));
	gameOverWindow.SetTitle("Game Over.", sf::Color::White, 80, 100, 40);
	gameOverWindow.AddButton("Renew", 30, sf::Color::White, sf::Color::Black, 80, 210, 90, 40);
	gameOverWindow.AddSubtext("(This time I'll win)", 15, 42, 225, 0, 140);
	gameOverWindow.AddButton("Quimt the game", 30, sf::Color::White, sf::Color::Black, 80, 260, 232, 40);
	gameOverWindow.AddSubtext("(This game sumcks.)", 15, 85, 275, 1, 30, 0);
	gameOverWindow.AddImage("dead_doge", 500, 300, 60, 340);
	gameOverWindow.DisableEscape();
}

Handler::Handler() {
	srand(time(NULL));

	// initialize window's basic appearance
	this->window.create(sf::VideoMode(WIN_WIDTH + 300, WIN_HEIGHT), "Doge Invaders!", sf::Style::Titlebar | sf::Style::Close);

	// initialize icon
	sf::Image icon;
	icon.loadFromFile("resources/data/icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	theme = stage = scoreSaved = 0;

	// initalizers
	initializeTextures();
	initializeMainMenu();
	initializeSounds();

	// initialize background
	gameBackground = sf::RectangleShape(sf::Vector2f(WIN_WIDTH, WIN_HEIGHT));
	gameBackground.setTexture(&TexturesStorage::GetTexture("backgrounds"));

	bulletCooldown = SHOT_COOLDOWN;
	renewGame(); // prepare new game
}

// main game loop
void Handler::Loop() {
	// prepare delta time counter
	float deltaTime;
	sf::Clock clock;

	int shooter = -1;
	float cooldownCounter = 0;
	bool animate = false, reachedAim = false, paused = false, death = false, gameOver = false;

	// prepare space board
	initializeBoard();

	// border between game window and space board
	sf::RectangleShape border(sf::Vector2f(5, 40));
	border.setFillColor(sf::Color::White);

	sf::Text sign("@dziobex", TexturesStorage::GetFont(), 25);
	sign.setFillColor(sf::Color(255, 255, 255, 69));
	sign.setPosition(WIN_WIDTH + 170, WIN_HEIGHT - 35);

	// for main menu
	bool playWin = false, hsWin = false;

	while (window.isOpen()) {
		// values that can change in every frame
		bool spacePressed = false, keyPressed = 0, keyReleased = 0, mouseReleased = 0, enterReleased = 0;
		deltaTime = clock.restart().asSeconds();

		// handle events triggered by user
		sf::Event e;
		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				spacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
				keyPressed = 1;
				break;
			case sf::Event::KeyReleased: {
				keyReleased = 0;
				if (e.key.code == sf::Keyboard::Enter)
					enterReleased = 1;
				break;
			}
			case sf::Event::MouseButtonReleased:
				mouseReleased = 1;
				break;
			default:
				break;
			}
		}

		window.clear(sf::Color::White);

		sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
		switch (stage) {
		case 0:
			this->mainMenu(mousePos, deltaTime, keyPressed, enterReleased, mouseReleased, playWin, hsWin);
			break;
		case 1:
			if (!gameOver && keyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && pauseWindow.IsReady()) {
				// display pause window if possible (game over isn't turned on, ESC key's been pressed and pause window isn't displaying any animation)
				paused = true;
				pauseWindow.ChangeContent(0, FilesManager::GetRandomQuote());
			}

			if (!death && !gameOver && !paused) {
				cooldownCounter += deltaTime * SHOT_SPEED;
				// shoot bullet (from the opponent side) if possible
				if (cooldownCounter > bulletCooldown) {
					aliveEnemies.clear();
					// prepare list of enemies who are alive and hasn't shot yet
					std::copy_if(enemies.begin(), enemies.end(), std::back_inserter(aliveEnemies), [](Enemy* e) { return e->IsAlive() && !e->HasShot(); });
					if ((aliveEnemies.size() == 1 && !aliveEnemies[0]->HasShot()) || ((aliveEnemies.size() > 1))) {
						// get a random shooter from choosen entities
						do {
							shooter = rand() % aliveEnemies.size();
						} while (aliveEnemies[shooter]->HasShot());
						aliveEnemies[shooter]->Shoot();
					}
					// refresh cooldown
					cooldownCounter = 0;
				}

				// one of the enemies's been defeated
				while (enemiesCount > std::count_if(enemies.begin(), enemies.end(), [](Enemy* e) { return e->IsAlive(); })) {
					score += (wave + 1) * 5;
					enemiesCount--;
					sounds[35].play();
					initializeBoard();
				}
			}

			// check if all the enemies've been defeated - if yes, prepare game for a new wave
			if (std::count_if(enemies.begin(), enemies.end(), [](Enemy* e) { return e->IsAlive(); }) == 0) {
				wave++;
				bulletCooldown -= 100;
				enemiesCount = 32;
				prepareGame(theme);
				initializeBoard();
			}

			// display background
			window.draw(gameBackground);

			// display shields
			for (int i = 0; i < 3; i++)
				shields[i]->Render(window);

			
			for (int c = 0; c < enemies.size(); c++) {
				if (!death && !gameOver && !paused) {
					if (enemies[c]->IsDisposable()) {
						// check if the enemy can be erased (its animation has ended and the enemy's useless)
						deadEnemies.push_back(enemies[c]);
						enemies.erase(enemies.begin() + c--);
						continue;
					}
					if (!animate && !enemies[c]->IsActive())
						// check if the current enemy's touched the wall (this info's stored in animate variable)
						animate = enemies[c]->Wall();
				}
				// check if the enemy's touched the player (=game over)
				if (enemies[c]->IsGameOver(player.GetObject())) {
					lives = 0;
					gameOver = 1;
				}
				// render enemy
				enemies[c]->Render(window, deltaTime, shields, player.GetObject(), reachedAim, !gameOver && !paused && !death);
			}

			if (!death && animate && !gameOver && !paused) {
				// change all enemies' direction if at least one of them's touched the wall
				for (int c = 0; c < enemies.size(); c++)
					enemies[c]->Activate();
				animate = false;
			}

			if (death && player.IsReady()) {
				death = false;

				// cancel bullets
				for (auto& e : enemies)
					if (e->IsAlive())
					e->CancelBullet();
			}

			if (!death && !gameOver && !paused && reachedAim && lives >= 0) {
				// the player was shot
				lives--;
				reachedAim = false;
				death = true;
				// set all enemies face expressions to evil smile
				for (auto& e : enemies)
					if (e->IsAlive())
						e->SetBadFace();
				// display player's death animation
				player.Death();

				if (lives == -1) gameOver = true;
			}
			// render player
			player.Render(window, deltaTime, spacePressed, enemies, shields, !gameOver && !paused);

			if (paused) {
				// "launch" pause window (start animation) if possible
				pauseWindow.Launch();
				// check if the close button X's been clicked
				pauseWindow.Checker(mousePos, mouseReleased);
				// display pause window
				pauseWindow.Display(window, mousePos, deltaTime, keyPressed, enterReleased, mouseReleased);

				// pause window's been hidden
				if (pauseWindow.IsReady())
					paused = false;

				// -1 means "no option's been clicked, continue"
				if (pauseWindow.GetClickedOption() != -1) {
					if (pauseWindow.GetClickedOption() != 0) {
						if (!scoreSaved) {
							// save score if possible
							FilesManager::AddScore(score);
							scoreSaved = true;
						}
						renewGame();
					}
					// read it as: "Quimt the game." has been clicked
					if (pauseWindow.GetClickedOption() == 2) stage = 0;
					pauseWindow.Reset();
					initializeBoard();
					paused = false;
				}
			}

			if (gameOver) {
				gameOverWindow.Launch();
				gameOverWindow.Display(window, mousePos, deltaTime, keyPressed, enterReleased, mouseReleased);

				if (!scoreSaved) {
					// save score if possible
					FilesManager::AddScore(score);
					scoreSaved = true;
				}

				if (gameOverWindow.GetClickedOption() != -1) {
					// go back to the main menu
					if (gameOverWindow.GetClickedOption() == 1)
						stage = 0;
					// 0 option is "renew game" - in both situations those properties should be refreshed
					renewGame();
					gameOverWindow.Reset();
					gameOver = false;
					initializeBoard();
				}

			}

			// display space board
			displayBoard();

			// display board border
			for (int i = 0; i < 11; i++) {
				border.setPosition(WIN_WIDTH, 75 * i + 7);
				window.draw(border);
			}

			window.draw(sign);
			break;
		default: break;
		}

		window.display();
	}
}