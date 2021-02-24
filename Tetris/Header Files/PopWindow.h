#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class PopWindow
{
public:
	struct Button {
		sf::Text buttonText;
		sf::Color normalTextColor, hoverTextColor;
		sf::RectangleShape buttonRect;
		sf::Color normalButtonColor, hoverButtonColor;
		bool showText, showRect;

		Button() = default;
		Button(bool showText, bool showRect, sf::Text buttonText, sf::RectangleShape buttonRect, sf::Color hoverTextColor = sf::Color::White, sf::Color hoverButtonColor = sf::Color::Black);

		bool Hover(sf::RenderWindow& window);

		void Render(sf::RenderWindow& window);
		void Move(float moveY);
	};

private:
	bool animation, hide;
	sf::Text title;
	sf::Vector2f position;
	sf::Vector2f size;
	sf::Font font;
	sf::Color inColor;

	sf::RectangleShape windowRect;
	Button closeButton;

	float speed, stopPointY;
public:
	bool Active;
	sf::Text optionalText;

	PopWindow() = default;
	PopWindow(sf::Text title, sf::Font font, sf::Text optionalText, sf::Color inColor = sf::Color::White, sf::Vector2f position = sf::Vector2f(0, 0), sf::Vector2f size = sf::Vector2f(0, 0));
	void Render(sf::RenderWindow& window, bool buttonClicked);
	void AnimationSettings(float speed, float stopPointY);

	void Show();
	void Move(float deltaTime);
};