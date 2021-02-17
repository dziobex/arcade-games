#pragma once
#include <SFML/Graphics.hpp>

class Evolution
{
	sf::Texture blockTexture;
	int aliveCells;
	int generation;
	float speed;
	float nextMovementBarrier;
	float nextMovementCounter;

	bool grid[50][50];

	int aliveFriends(int row, int col);
	bool dead(int row, int col);
	
public:
	bool board[50][50];

	Evolution(float speed, float nextMovementBarrier, sf::Texture blockTexture);
	void evolve(float deltaTime);
	void clear();
	void Render(sf::RenderWindow& window);

	int GetGeneration() { return generation; }
	int GetAliveCells() { return aliveCells; };
	void SetSpeed(float newSpeed) { speed = newSpeed; }
	float GetSpeed() { return speed; }
};

