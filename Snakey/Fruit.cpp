#include "Fruit.h"

/* setting random fruct with random additional segments (how many segments will fruct give snake after eating) */
void Fruit::setNextFruct(Snake& snake) {
	short option = rand() % 7;

	additionalSegments = 1;
	std::string chosenFile;
	switch (option) {
	case 0: chosenFile = "apple.png"; break;
	case 1: chosenFile = "banana.png"; break;
	case 2: chosenFile = "cherry.png"; break;
	case 3: chosenFile = "strawberry.png"; additionalSegments = 2; break;
	case 4: chosenFile = "kiwi.png"; additionalSegments = 2;  break;
	case 5: chosenFile = "coffee.png"; additionalSegments = 3; break;
	default: chosenFile = "dio.png"; additionalSegments = 0; break;
	}

	texture.loadFromFile("resources/" + chosenFile);
	do {
		position = sf::Vector2f(rand() % 39 * SEGMENT_SIZE, rand() % 29 * SEGMENT_SIZE);
	} while (!possiblePosition(snake));
}

/* checking if new fruct position's not taken by some of snake's body part */
bool Fruit::possiblePosition(Snake& snake) {
	if (snake.GetHeadPosition() == position)
		return false;
	for (sf::Vector2f part : snake.GetBodyPositions())
		if (part == position)
			return false;
	return true;
}

Fruit::Fruit(Snake& snake) {
	srand(time(NULL));
	setNextFruct(snake);
}

void Fruit::draw(sf::RenderWindow& window) {
	sf::RectangleShape fruit(sf::Vector2f(SEGMENT_SIZE, SEGMENT_SIZE));
	fruit.setPosition(position);
	fruit.setTexture(&texture);

	window.draw(fruit);
}

/* checking if collision with snake's head occurred, if yes, prepare next fruct */
void Fruit::collision(Snake& snake) {
	if ((snake.GetHeadPosition().x <= position.x + SEGMENT_SIZE / 2 && snake.GetHeadPosition().x + SEGMENT_SIZE / 2 >= position.x)
		&& (snake.GetHeadPosition().y <= position.y + SEGMENT_SIZE / 2 && snake.GetHeadPosition().y + SEGMENT_SIZE / 2 >= position.y)) {
		snake.addNewPart(additionalSegments, Snake::Direction::DOWN);
		setNextFruct(snake);
	}
}
