#include "Shield.h"

// SHIELD

void Shield::Initialize(int theme, int index, sf::Vector2f position, sf::Color color, int textureIndex, std::vector<sf::Sound*>& sounds) {
	// set references to the sounds
	this->sounds = sounds;
	this->index = theme;

	// initialize shield collider (represents summed segments)
	shieldObject = sf::RectangleShape(sf::Vector2f(200, 40));
	shieldObject.setPosition(position);
	shieldObject.setTexture(&TexturesStorage::GetTexture("shields"));
	shieldObject.setTextureRect(sf::IntRect(index * 200, 120 * theme + 40 * textureIndex, 200, 40));

	// initialize all segments (the last argument informs if the segment is upper or lower)
	for (int i = 0, s = 0; i < 2; i++)
		for (int j = 0; j < 5; j++, s++)
			segments.push_back(Segment(sounds[theme * 20 + s], 40, 20, index * 200 + 40 * j, 120 * theme + textureIndex * 40 + i * 20, position + sf::Vector2f(40 * j, 0), i == 0));
}

void Shield::Render(sf::RenderWindow& window) {
	for (int i = 0; i < segments.size(); ++i) {
		if (segments[i].GetStrength() <= 0) {
			// remove useless segment (it lost its strength)
			segments.erase(segments.begin() + i);
			--i;
			// play "break" sound
			sounds[20 * index + i + 1]->play();
			continue;
		}
		segments[i].Render(window);
	}
}

// check if collision with any segment has occurred
bool Shield::Collision(sf::RectangleShape bullet, bool destroyer) {
	for (int i = 0; i < segments.size(); ++i)
		if (segments[i].Collision(bullet, destroyer))
			return true;
	return false;
}

// SEGMENT

Shield::Segment::Segment(sf::Sound* tapSound, int segmentWidth, int segmentHeight, int segmentWIndex, int segmentHIndex, sf::Vector2f position, bool upper) {
	this->tapSound = tapSound;

	// initialize segment size, texture, position
	segmentObject = sf::RectangleShape(sf::Vector2f(segmentWidth, segmentHeight));
	segmentObject.setTexture(&TexturesStorage::GetTexture("shields"));
	segmentObject.setTextureRect(sf::IntRect(segmentWIndex, segmentHIndex, segmentWidth, segmentHeight));
	segmentObject.setPosition(position + sf::Vector2f(0, upper ? 0 : 20));

	// initialize crash (post-collision) mask - it will appear after receiving the first damage
	crashObject = sf::RectangleShape(sf::Vector2f(40, 20));
	crashObject.setPosition(segmentObject.getPosition());
	crashObject.setTexture(&TexturesStorage::GetTexture("shields_crash"));

	// shield strength (the segment will disappear after 2 hits)
	strength = 3;
}

void Shield::Segment::Render(sf::RenderWindow& window) {
	// check if the segment exists (has more than 0 "lives")
	if (strength > 0)
		window.draw(segmentObject);
	// check if the segment needs a crash mask and how visible it should be
	if (strength < 3 && strength >= 1) {
		crashObject.setFillColor(sf::Color(crashObject.getFillColor().r, crashObject.getFillColor().g, crashObject.getFillColor().b, strength == 2 ? 95 : 255));
		window.draw(crashObject);
	}
}

// check if the bullet has touched the segment
// destroyer determines if bullet can affect (do damage) shield
bool Shield::Segment::Collision(sf::RectangleShape bullet, bool destroyer) {
	if (this->segmentObject.getGlobalBounds().intersects(bullet.getGlobalBounds()))
		return strength -= destroyer, tapSound->play(), true;
	return false;
}
