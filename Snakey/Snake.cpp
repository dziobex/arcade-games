#include "Snake.h"

/* checking if snake is in front of the player's view, */
void Snake::moveSegments() {
	if (headPosition.x >= WIN_WIDTH)
		headPosition.x = 0;
	else if (headPosition.x < 0)
		headPosition.x = WIN_WIDTH - SEGMENT_SIZE;
	if (headPosition.y >= WIN_HEIGHT)
		headPosition.y = 0;
	else if (headPosition.y < 0)
		headPosition.y = WIN_HEIGHT - SEGMENT_SIZE;

	for (int i = bodyPositions.size() - 1; i >= 1; --i)
		bodyPositions[i] = bodyPositions[i - 1];
	if (bodyPositions.size() > 0)
		bodyPositions[0] = headPosition;
}

Snake::Snake(sf::Vector2f headPosition, float speed, float pause, sf::Texture headTexture, sf::Texture bodyTexture) {
	this->headPosition = headPosition;
	this->speed = speed;
	this->pause = pause;
	this->headTexture = headTexture;
	this->bodyTexture = bodyTexture;
	nextMove = 0;
	previousDirection = Direction::LEFT;

	movements = sf::Vector2f(speed / 5, 0);
}

void Snake::draw(sf::RenderWindow& window) {
	sf::RectangleShape part(sf::Vector2f(SEGMENT_SIZE, SEGMENT_SIZE));
	part.setPosition(headPosition);
	part.setTexture(&headTexture);
	window.draw(part);

	part.setTexture(&bodyTexture);
	for (int i = 0; i < bodyPositions.size(); ++i) {
		part.setPosition(bodyPositions[i]);
		window.draw(part);
	}
}

/* setting next direction, moving snake and displaying deltatime if has proper value */
void Snake::update(float deltaTime, bool& gameOver, sf::Text& fpsCounter) {
	nextMove += speed * deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && previousDirection != Direction::LEFT && (bodyPositions.size() < 1 || (bodyPositions.size() > 0 && headPosition.y != bodyPositions[0].y))) {
		movements = sf::Vector2f(speed / 5, 0);
		previousDirection = Direction::RIGHT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && previousDirection != Direction::RIGHT && (bodyPositions.size() < 1 || (bodyPositions.size() > 0 && headPosition.y != bodyPositions[0].y))) {
		movements = sf::Vector2f(-speed / 5, 0);
		previousDirection = Direction::LEFT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && previousDirection != Direction::DOWN && (bodyPositions.size() < 1 || (bodyPositions.size() > 0 && headPosition.x != bodyPositions[0].x))) {
		movements = sf::Vector2f(0, -speed / 5);
		previousDirection = Direction::UP;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && previousDirection != Direction::UP && (bodyPositions.size() < 1 || (bodyPositions.size() > 0 && headPosition.x != bodyPositions[0].x))) {
		movements = sf::Vector2f(0, speed / 5);
		previousDirection = Direction::DOWN;
	}

	if (nextMove >= pause) {
		fpsCounter.setString(std::to_string(int(round(1.0f / deltaTime))) + " FPS");
		nextMove = 0;
		moveSegments();
		headPosition += movements;

		if (collision())
			gameOver = true;
	}
}

/* adding next part (segment) to snake's body */
void Snake::addNewPart(int numberOfParts, Direction dir) {
	for (int i = 0; i < numberOfParts; ++i)
		bodyPositions.push_back(sf::Vector2f(-SEGMENT_SIZE, -SEGMENT_SIZE));
}

/* checking if collision occurred */
bool Snake::collision() {
	for (int i = 1; i < bodyPositions.size(); ++i)
		if ((headPosition.x <= bodyPositions[i].x + SEGMENT_SIZE / 2 && headPosition.x + SEGMENT_SIZE / 2 >= bodyPositions[i].x)
			&& (headPosition.y <= bodyPositions[i].y + SEGMENT_SIZE / 2 && headPosition.y + SEGMENT_SIZE / 2 >= bodyPositions[i].y))
			return true;

	return false;
}
