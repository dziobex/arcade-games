#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class TexturesStorage {
	static std::map<std::string, sf::Texture> textures;
	static std::map<std::string, sf::SoundBuffer> sounds;
	static sf::Font font;

public:
	// textures
	static void AddTexture(std::string key, std::string path);
	static sf::Texture& GetTexture(std::string key);

	// sounds
	static void AddSound(std::string key, std::string path);
	static sf::SoundBuffer& GetSound(std::string key);

	// font
	static void SetFont(std::string path);
	static sf::Font& GetFont();
};

