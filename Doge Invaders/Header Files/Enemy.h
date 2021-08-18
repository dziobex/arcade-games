#pragma once
#include <SFML/Graphics.hpp>
#include "EnemyBullet.h"
#include "TexturesStorage.h"

class Enemy {
	sf::RectangleShape enemyObject;
	sf::RectangleShape enemyHairObject;

	struct Paw {
	private:
		float angle;
		bool negativeAngle;
		sf::RectangleShape pawObject;
		int toRight;
		bool active, alive;

	public:
		Paw(sf::Vector2f position, float startAngle);
		Paw() = default;
		void Animate(float deltaTime);
		void Render(sf::RenderWindow& window, float deltaTime, bool move);
		void Move(float deltaTime);

		void Dispose();
		void SetActive() { active = true; }
		void Bounce();
	};

	Paw paws[2];

	bool active, toRight, alive, disposable;
	int row, additionalRow;
	int theme, index;

	EnemyBullet bullet;
	sf::Sound shotSound, reachedAimSound;
	bool isShot;

public:
	int id;

	void Initialize(int theme, int index, sf::Vector2f position, sf::Sound& shotSound, sf::Sound& reachedAimSound);
	void Move(float deltaTime);
	void Render(sf::RenderWindow& window, float deltaTime, std::vector<Shield*> shields, sf::RectangleShape player, bool& reachedPlayer, bool move);

	bool Wall();

	void Activate() { active = true; }
	int GetDirection() { return toRight; }

	bool IsActive() { return active; }
	void Down(float deltaTime);
	void Bounce();
	void Dispose();

	bool IsAlive() { return alive; }
	bool Collision(sf::RectangleShape body);
	void Shoot();
	bool HasShot() { return isShot; }
	bool IsDisposable() { return disposable; }
	bool IsGameOver(sf::RectangleShape player);

	void CancelBullet();
	void SetBadFace();
};

