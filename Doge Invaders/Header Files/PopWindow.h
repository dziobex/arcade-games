#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "TexturesStorage.h"

class PopWindow {
	sf::CircleShape pointerObj;

	sf::RectangleShape box, closeButton;
	sf::Text closeText;
	sf::Vector2f initialPosition;
	bool visible, animate, escapeDisabled;

	bool show, hide;

	bool isReady;
	bool toRight;

	sf::Text title;

	sf::Vector2f leftBorder, rightBorder;

	struct Button {
	private:
		sf::RectangleShape buttonBody;
		sf::Text buttonText, hoveredText;
		sf::Color bodyOutColor, bodyInColor, textOutColor, textInColor;
		int index;
		float x, y;
		float limitX, limitY;
		float pathLength;
		bool horizontally;

	public:
		Button(std::string text, int textSize, int x0, int x, int y, int width, int height);
		void SetIndex(int index);
		void AddSubtext(std::string text, int textSize, int x0, int x, int y, float pathLength, bool horizontally);
		void SetColors(sf::Color bodyOutColor, sf::Color textOutColor);
		bool Hover(sf::Vector2f mousePos);
		void Move(float x, float y);
		void Display(sf::RenderWindow& window, sf::Vector2f mousePos, float deltaTime, int& hoveredIndex, bool animate);

		sf::Vector2f GetTextPosition() { return buttonText.getPosition(); }
	};

	std::vector<Button> buttons;
	std::vector<sf::Texture> textures;
	std::vector<sf::RectangleShape> images;
	std::vector<sf::Text> texts;

	int hoveredIndex;
	int clicked;

	void prepare();

public:
	PopWindow() = default;
	PopWindow(int width, int height);

	void SetBorders(sf::Vector2f left, sf::Vector2f right);
	void SetPosition(int x, int y);
	void SetBackgroundColor(sf::Color color);
	void SetTitle(std::string title, sf::Color titleColor, int titleSize, int x, int y);
	void DisableEscape();
	void SetImageTextureRect(int imageIndex, sf::IntRect tRect);

	void AddContent(std::string text, int textSize, int x, int y, sf::Color color=sf::Color::White);
	void ChangeContent(int index, std::string newText);
	void AddImage(std::string key, int width, int height, int x, int y);
	void AddButton(std::string text, int textSize, sf::Color textOutColor, sf::Color backgroundColorOut, int x, int y, int width, int height);
	void AddSubtext(std::string text, int textSize, int x, int y, int buttonIndex, int pathLength, bool horizontally=true);

	void Animate(float deltaTime);
	void Display(sf::RenderWindow& window, sf::Vector2f mousePos, float deltaTime, bool keyPressed, bool enterReleased, bool mouseReleased);
	void Checker(sf::Vector2f mousePos, bool mouseReleased);

	bool IsReady();
	void Launch();

	int GetClickedOption();
	void Reset();
};

