#pragma once
#include <SFML/Graphics.hpp>
#include "TexturesStorage.h"

class Shield {
	sf::RectangleShape shieldObject;
	std::vector<sf::Sound*> sounds;
	int index;

	struct Segment {
	private:
		sf::RectangleShape segmentObject, crashObject;
		sf::Sound* tapSound;

		int strength;
	public:
		Segment() = default;
		Segment(sf::Sound* tapSound, int segmentWidth, int segmentHeight, int segmentWIndex, int segmentHIndex, sf::Vector2f position, bool upper);

		void Render(sf::RenderWindow& window);
		bool Collision(sf::RectangleShape bullet, bool destroyer);
		bool GetStrength() { return strength; }
	};

	std::vector<Segment> segments;

public:

	void Initialize(int theme, int index, sf::Vector2f position, sf::Color color, int textureIndex, std::vector<sf::Sound*>& sounds);
	void Render(sf::RenderWindow& window);
	bool Collision(sf::RectangleShape bullet, bool destroyer);
};

