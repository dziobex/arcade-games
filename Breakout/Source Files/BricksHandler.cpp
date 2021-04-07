#include "BricksHandler.h"

// preparing bricks //
BricksHandler::BricksHandler() {
	sf::Color colors[6]{ sf::Color::Red, sf::Color::Color(255,140,0), sf::Color::Yellow, sf::Color::Green, sf::Color::Blue, sf::Color::Magenta };
	for (int i = 0, row = 0; row < 6; ++i) {
		bricks.push_back(Brick());
		bricks[row * 8 + i].brick = sf::RectangleShape(sf::Vector2f(70, 20));
		bricks[row * 8 + i].brick.setPosition(35 + i * 80, 130 + row * 30);
		bricks[row * 8 + i].brick.setFillColor(colors[row]);
		if (i >= 7) {
			i = -1;
			row++;
		}
	}
}

// draw bricks and check whether any collision occured //
void BricksHandler::Render(sf::RenderWindow& window, Ball& ball, float deltaTime, int& points) {
	for (int i = 0; i < bricks.size(); ++i) {
		if (!bricks[i].Crushed)
			if (bricks[i].Collision(points, ball, deltaTime)) {
				bricks[i].Crushed = true;
				points += 10;
			}
		window.draw(bricks[i].brick);
	}
}

// check for collision with ball //
bool BricksHandler::Brick::Collision(int& points, Ball& ball, float deltaTime) {
	if (ball.Collision(brick, deltaTime)) {
		brick.setPosition(-100, -100);
		return true;
	}
	return false;
}