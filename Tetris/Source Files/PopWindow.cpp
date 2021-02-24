#include "PopWindow.h"

PopWindow::PopWindow(sf::Text title, sf::Font font, sf::Text optionalText, sf::Color inColor, sf::Vector2f position, sf::Vector2f size) {
	this->title = title;
	this->font = font;
	this->position = position;
	this->size = size;
	this->inColor = inColor;
	this->optionalText = optionalText;

	stopPointY = 100;
}

// rendering window //
void PopWindow::Render(sf::RenderWindow& window, bool buttonClicked) {
	windowRect.setPosition(position);
	title.setPosition(position.x + 8, position.y + 3);
	optionalText.setPosition(position.x + 5, position.y + 35);
	window.draw(windowRect);
	window.draw(title);
	closeButton.Hover(window);
	if (buttonClicked && closeButton.Hover(window) && !hide && !animation)
		hide = true;
	closeButton.Render(window);
	window.draw(optionalText);
}

// animation settings //
void PopWindow::AnimationSettings(float speed, float stopPointY) {
	this->speed = -speed; // windows speed
	this->stopPointY = stopPointY; // point at which the window should stop
}

//  preparation for showing the window //
void PopWindow::Show() {
	position = sf::Vector2f(100, 600);
	Active = animation = true;
	hide = false;
	windowRect = sf::RectangleShape(size);
	windowRect.setOutlineThickness(2);
	windowRect.setOutlineColor(sf::Color::Black);
	windowRect.setFillColor(inColor);

	sf::RectangleShape closeRect(sf::Vector2f(40, 20));
	closeRect.setPosition(size.x + position.x - 40, position.y);
	closeRect.setOutlineThickness(2);
	closeRect.setFillColor(sf::Color::Red);
	closeRect.setOutlineColor(sf::Color::Black);

	sf::Text closeText("X", font, 18);
	closeText.setPosition(size.x + position.x - 25, position.y + 1);
	closeText.setOutlineThickness(2);
	closeText.setFillColor(sf::Color::White);
	closeText.setOutlineColor(sf::Color::Black);

	closeButton = Button(1, 1, closeText, closeRect);
	optionalText.setPosition(position.x + 5, position.y + 35);
}

// animating pop window //
void PopWindow::Move(float deltaTime) {
	if (hide) {
		position += sf::Vector2f(0, deltaTime * abs(speed));
		closeButton.Move(deltaTime * abs(speed));
		if (position.y >= 610)
			Active = hide = false;
	}
	if (animation) {
		position += sf::Vector2f(0, deltaTime * speed);
		closeButton.Move(deltaTime * speed);
		if (position.y > stopPointY)
			speed = -abs(speed);
		else if (position.y <= stopPointY)
			animation = false;
	}
}

PopWindow::Button::Button(bool showText, bool showRect, sf::Text buttonText, sf::RectangleShape buttonRect, sf::Color hoverTextColor, sf::Color hoverButtonColor) {
	this->showText = showText;
	this->showRect = showRect;
	this->buttonRect = buttonRect;
	this->buttonText = buttonText;
	this->hoverTextColor = hoverTextColor;
	this->hoverButtonColor = hoverButtonColor;
}

// checking whether button is hovered //
bool PopWindow::Button::Hover(sf::RenderWindow& window) {
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
	if (showRect)
		buttonRect.setFillColor(buttonRect.getGlobalBounds().contains(mousePos) ? sf::Color::Black : sf::Color::Red);
	if (showText)
		buttonText.setFillColor(buttonText.getGlobalBounds().contains(mousePos) ? sf::Color::White : sf::Color::White);
	return (showText && buttonText.getGlobalBounds().contains(mousePos)) || (showRect && buttonRect.getGlobalBounds().contains(mousePos));
}

// drawing button on the screen //
void PopWindow::Button::Render(sf::RenderWindow& window) {
	if (showRect)
		window.draw(buttonRect);
	if (showText)
		window.draw(buttonText);
}

// moving button //
void PopWindow::Button::Move(float moveY) {
	if (showRect)
		buttonRect.move(0, moveY);
	if (showText)
		buttonText.move(0, moveY);
}
