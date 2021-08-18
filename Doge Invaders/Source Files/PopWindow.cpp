#include "PopWindow.h"

// POP WINDOW

void PopWindow::prepare() {
	float distanceX = 0;
	if (toRight)
		distanceX = box.getPosition().x - rightBorder.x;
	else
		distanceX = abs(box.getPosition().x) - box.getSize().x;

	box.setPosition(box.getPosition() + sf::Vector2f(distanceX, 0));
	closeButton.setPosition(closeButton.getPosition() + sf::Vector2f(distanceX, 0));
	closeText.setPosition(closeText.getPosition() + sf::Vector2f(distanceX, 0));
	pointerObj.setPosition(pointerObj.getPosition() + sf::Vector2f(distanceX, 0));
	title.setPosition(title.getPosition() + sf::Vector2f(distanceX, 0));


	for (auto& b : buttons)
		b.Move(distanceX, 0);
	// move texts
	for (auto& t : texts)
		t.setPosition(t.getPosition() + sf::Vector2f(distanceX, 0));
	// move images
	for (auto& i : images)
		i.setPosition(i.getPosition() + sf::Vector2f(distanceX, 0));

	animate = false;
	toRight = !toRight;
}

PopWindow::PopWindow(int width, int height) {
	// initialize window box and set default title (nothing)
	box = sf::RectangleShape(sf::Vector2f(width, height));
	box.setOutlineThickness(5);
	box.setOutlineColor(sf::Color::White);
	title = sf::Text("", TexturesStorage::GetFont(), 1);

	// initialize close button
	closeButton = sf::RectangleShape(sf::Vector2f(60, 40));
	closeButton.setFillColor(sf::Color::Red);
	closeText = sf::Text("X", TexturesStorage::GetFont(), 25);
	closeText.setFillColor(sf::Color::White);

	// initialize pointer (white dot that points hovered options)
	pointerObj = sf::CircleShape(7);

	// initialize properties for animation
	hoveredIndex = 0;
	// animate = true;

	toRight = isReady = true;
	show = hide = escapeDisabled = animate = false;
	clicked = -1;
}

// set "anchors" for box, from both of the sides
void PopWindow::SetBorders(sf::Vector2f left, sf::Vector2f right) {
	this->leftBorder = left;
	this->rightBorder = right;

	// window starts its journey from left
	// adjust box position and its properties
	box.setPosition(-box.getSize().x, WIN_HEIGHT /2 - box.getSize().y / 2);
	closeButton.setPosition(sf::Vector2f(-box.getSize().x + box.getSize().x - closeButton.getSize().x, WIN_HEIGHT / 2 - box.getSize().y / 2));
	closeText.setPosition(closeButton.getPosition() + sf::Vector2f(20, 5));
}

// set window's position (and its X button)
void PopWindow::SetPosition(int x, int y) {
	// initial position is an "anchor" - the limit position will be calculated from it
	initialPosition = sf::Vector2f(x, y);
	// the window's box is hidden "very close to the screen" (its x pos + its width == 0 x pos)
	box.setPosition(-box.getSize().x, y);
	// adjust the button's pos to the window's pos
	closeButton.setPosition(sf::Vector2f(-box.getSize().x + box.getSize().x - closeButton.getSize().x, y));
	closeText.setPosition(closeButton.getPosition() + sf::Vector2f(20, 5));
}

// set window's background color
void PopWindow::SetBackgroundColor(sf::Color color) {
	box.setFillColor(color);
}

// set window's title: its text, color, position
void PopWindow::SetTitle(std::string title, sf::Color titleColor, int titleSize, int x, int y) {
	this->title = sf::Text(title, TexturesStorage::GetFont(), titleSize);
	this->title.setFillColor(titleColor);
	this->title.setPosition(box.getPosition() + sf::Vector2f(x, y));
}

void PopWindow::DisableEscape() {
	escapeDisabled = true;
}

// set texture rect in the image from imageIndex position
void PopWindow::SetImageTextureRect(int imageIndex, sf::IntRect tRect) {
	images[imageIndex].setTextureRect(tRect);
}

// add custom text to the window
void PopWindow::AddContent(std::string text, int textSize, int x, int y, sf::Color color) {
	sf::Text t(text, TexturesStorage::GetFont(), textSize);
	t.setPosition(box.getPosition() + sf::Vector2f(x, y));
	t.setFillColor(color);
	texts.push_back(t);
}

// change content of the text in the index position
void PopWindow::ChangeContent(int index, std::string newText) {
	texts[index].setString(newText);
}

// add image to the window (under the "key", the texture for the image is stored)
void PopWindow::AddImage(std::string key, int width, int height, int x, int y) {
	sf::RectangleShape img(sf::Vector2f(width, height));
	img.setPosition(box.getPosition().x + x, box.getPosition().y + y);
	img.setTexture(&TexturesStorage::GetTexture(key));
	images.push_back(img);
}

void PopWindow::AddButton(std::string text, int textSize, sf::Color textOutColor, sf::Color backgroundColorOut, int x, int y, int width, int height) {
	// initialize new button
	Button btn(text, textSize, box.getPosition().x, box.getPosition().x + x, box.getPosition().y + y, width, height);
	btn.SetColors(backgroundColorOut, textOutColor);
	btn.SetIndex(buttons.size());
	buttons.push_back(btn);

	// the first button (if exists) is by default hovered
	if (buttons.size() == 1)
		pointerObj.setPosition(buttons[0].GetTextPosition() + sf::Vector2f(-25, 11));
}

// add subtext (text that will be shown after button from buttonIndex is hovered)
void PopWindow::AddSubtext(std::string text, int textSize, int x, int y, int buttonIndex, int pathLength, bool horizontally) {
	buttons[buttonIndex].AddSubtext(text, textSize, box.getPosition().x + x, initialPosition.x + x, box.getPosition().y + y, pathLength, horizontally);
}

void PopWindow::Animate(float deltaTime) {
	if (show || hide) {
		// showing animations -> elements're going to reach half of the screen
		float change = deltaTime * POP_WINDOW_SPEED * (toRight ? 1 : -1);
		// move box
		this->box.setPosition(box.getPosition() + sf::Vector2f(change, 0));
		// move close button
		this->closeButton.setPosition(closeButton.getPosition() + sf::Vector2f(change, 0));
		this->closeText.setPosition(closeText.getPosition() + sf::Vector2f(change, 0));
		// move pointer
		this->pointerObj.setPosition(pointerObj.getPosition() + sf::Vector2f(change, 0));
		// move title
		title.setPosition(title.getPosition() + sf::Vector2f(change, 0));
		// move buttons
		for (auto& b : buttons)
			b.Move(change, 0);
		// move texts
		for (auto& t : texts)
			t.setPosition(t.getPosition() + sf::Vector2f(change, 0));
		// move images
		for (auto& i : images)
			i.setPosition(i.getPosition() + sf::Vector2f(change, 0));

		// stop mechanism if possible
		if (show) {
			if ((toRight && box.getPosition().x >= (rightBorder.x - leftBorder.x) / 2 - box.getSize().x / 2)
			|| (!toRight && box.getPosition().x <= (rightBorder.x - leftBorder.x) / 2 - box.getSize().x / 2))
				show = false;
		}
		else if (hide) {
			if ((toRight && box.getPosition().x >= rightBorder.x + 180)
			|| (!toRight && box.getPosition().x <= -rightBorder.x - 180)) {
				hide = false;
				isReady = true;
				prepare();
			}
		}
		animate = true;
	}
	else
		animate = false;
}

void PopWindow::Display(sf::RenderWindow& window, sf::Vector2f mousePos, float deltaTime, bool keyPressed, bool enterReleased, bool mouseReleased) {
	this->Animate(deltaTime);
	window.draw(box);
	window.draw(title);

	// close button appearance -> change if the hover state's changed
	if (!escapeDisabled) {
		if (closeButton.getGlobalBounds().contains(mousePos) && closeButton.getFillColor() != sf::Color::White) {
			closeButton.setFillColor(sf::Color::White);
			closeText.setFillColor(sf::Color::Black);
		}
		else if (!closeButton.getGlobalBounds().contains(mousePos) && closeButton.getFillColor() != sf::Color::Red) {
			closeButton.setFillColor(sf::Color::Red);
			closeText.setFillColor(sf::Color::White);
		}
		// displaying close button
		window.draw(closeButton);
		window.draw(closeText);
	}

	// displaying images
	for (auto i : images)
		window.draw(i);

	// displaying additional texts
	for (auto t : texts)
		window.draw(t);
	
	if (!animate && keyPressed) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			hoveredIndex = hoveredIndex == buttons.size() - 1 ? 0 : hoveredIndex + 1;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			hoveredIndex = hoveredIndex == 0 ? buttons.size() - 1 : hoveredIndex - 1;
	}
	// displaying buttons
	for (auto& b : buttons) {
		b.Display(window, mousePos, deltaTime, hoveredIndex, animate);
	}

	for (int i = 0; i < buttons.size(); i++) {
		buttons[i].Display(window, mousePos, deltaTime, hoveredIndex, animate);;
		if (!animate && ((buttons[i].Hover(mousePos) && mouseReleased) || (enterReleased && i == hoveredIndex))) {
			clicked = i;
		}
	}

	if (!animate && buttons.size() > 0)
		if (pointerObj.getPosition().y - 11 > buttons[hoveredIndex].GetTextPosition().y && pointerObj.getPosition().y - 10 > buttons[hoveredIndex].GetTextPosition().y)
			pointerObj.setPosition(pointerObj.getPosition() - sf::Vector2f(0, deltaTime * 200));
		else if (pointerObj.getPosition().y - 11 < buttons[hoveredIndex].GetTextPosition().y && pointerObj.getPosition().y - 10 < buttons[hoveredIndex].GetTextPosition().y)
			pointerObj.setPosition(pointerObj.getPosition() + sf::Vector2f(0, deltaTime * 200));

	if (buttons.size() > 0)
	window.draw(pointerObj);
}

void PopWindow::Checker(sf::Vector2f mousePos, bool mouseReleased) {
	if (!escapeDisabled && (!show && !isReady && !hide && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) || (closeButton.getGlobalBounds().contains(mousePos) && mouseReleased)) {
		show = false;
		hide = true;
	}
}

// return info whether window is ready to move
bool PopWindow::IsReady() { return isReady; }

// set window to "movable" state if possible
void PopWindow::Launch() {
	if (isReady) {
		isReady = false;
		show = true;
	}
}

// get index of the clicked option
int PopWindow::GetClickedOption() {
	return clicked;
}

void PopWindow::Reset() {
	clicked = -1;
	prepare();
	if (!toRight)
		prepare();
	isReady = true;
}

// BUTTON

PopWindow::Button::Button(std::string text, int textSize, int x0, int x, int y, int width, int height) {
	// initialize button's body (background)
	this->buttonBody = sf::RectangleShape(sf::Vector2f(width + (x - x0), height));
	this->buttonBody.setPosition(x0, y);
	// initialize button's text
	this->buttonText = sf::Text(text, TexturesStorage::GetFont(), textSize);
	this->buttonText.setPosition(x, y);
}

// set button's index (necessary for animating him when he's hovered)
void PopWindow::Button::SetIndex(int index) {
	this->index = index;
}

void PopWindow::Button::AddSubtext(std::string text, int textSize, int x0, int x, int y, float pathLength, bool horizontally) {
	this->x = x0;
	this->y = y;
	limitX = x;
	limitY = y;

	this->horizontally = horizontally;
	this->pathLength = pathLength;
	this->hoveredText = sf::Text(text, TexturesStorage::GetFont(), textSize);
	this->hoveredText.setPosition(this->x, this->y);
}

void PopWindow::Button::SetColors(sf::Color bodyOutColor, sf::Color textOutColor) {
	this->bodyOutColor = bodyOutColor;
	this->textOutColor = textOutColor;

	// starter colors: body's color and not-full-bright text
	this->buttonBody.setFillColor(bodyOutColor);
	this->buttonText.setFillColor(sf::Color(textOutColor.r, textOutColor.g, textOutColor.b, 200));
}

// checking if mouse's over the button 
bool PopWindow::Button::Hover(sf::Vector2f mousePos) {
	return this->buttonBody.getGlobalBounds().contains(mousePos);
}

// move button and its elements
void PopWindow::Button::Move(float x, float y) {
	this->x += x;
	this->y += y;
	this->buttonBody.setPosition(buttonBody.getPosition() + sf::Vector2f(x, y));
	this->buttonText.setPosition(buttonText.getPosition() + sf::Vector2f(x, y));
	this->hoveredText.setPosition(hoveredText.getPosition() + sf::Vector2f(x, y));
}

void PopWindow::Button::Display(sf::RenderWindow& window, sf::Vector2f mousePos, float deltaTime, int& hoveredIndex, bool animate) {
	if (!animate) {
		if (Hover(mousePos))
			hoveredIndex = index;
		if (hoveredIndex == index) {
			buttonText.setFillColor(sf::Color(buttonText.getFillColor().r, buttonText.getFillColor().g, buttonText.getFillColor().b, 255));
			buttonText.setStyle(sf::Text::Style::Underlined);
			if (horizontally && limitX + pathLength > x)
				x += 150 * deltaTime;
			else if (!horizontally && limitY + pathLength > y)
				y += 100 * deltaTime;
		}
		else {
			buttonText.setFillColor(sf::Color(buttonText.getFillColor().r, buttonText.getFillColor().g, buttonText.getFillColor().b, 200));
			buttonText.setStyle(sf::Text::Style::Regular);
			if (horizontally && x > limitX)
				x -= 150 * deltaTime;
			else if (!horizontally && y > limitY)
				y -= 100 * deltaTime;
		}
	}
	hoveredText.setPosition(x, y);
	window.draw(hoveredText);
	window.draw(buttonBody);
	window.draw(buttonText);
}