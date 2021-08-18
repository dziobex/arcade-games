#include "TexturesStorage.h"

// initialize static maps
std::map<std::string, sf::Texture> TexturesStorage::textures;
std::map<std::string, sf::SoundBuffer> TexturesStorage::sounds;

// initialize static font
sf::Font TexturesStorage::font;

// add texture from path to the map
void TexturesStorage::AddTexture(std::string key, std::string path) {
	sf::Texture texture;
	texture.loadFromFile(path);
	textures.insert(std::make_pair(key, texture));
}

// get a reference to the texture from the "key" place 
sf::Texture& TexturesStorage::GetTexture(std::string key) {
	return textures[key];
}

// add sound from path to the map
void TexturesStorage::AddSound(std::string key, std::string path) {
	sf::SoundBuffer sb;
	sb.loadFromFile(path);
	sounds.insert(std::make_pair(key, sb));
}

// get a reference to the sound from the "key" place
sf::SoundBuffer& TexturesStorage::GetSound(std::string key) {
	return sounds[key];
}

// set font from path
void TexturesStorage::SetFont(std::string path) {
	font.loadFromFile(path);
}

// get a reference to the font
sf::Font& TexturesStorage::GetFont() {
	return font;
}
