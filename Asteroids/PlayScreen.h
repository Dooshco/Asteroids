#pragma once
#include "stdafx.h"
#include <list>
#include <iostream>
#include "Game.h"
#include "LineText.h"
#include "HighScore.h"
#include "Entity.h"
#include "SoundEffects.h"

class PlayScreen
{
public:
	PlayScreen(LineText &ls, HighScore &hs);
	int Run(sf::RenderWindow &window);
	void Update(float dt);
	void Draw(sf::RenderWindow& window);

private:
	bool isCollision(sf::Vector2f point, sf::Vector2f circle, float radius);				// Point & Circle
	bool isCollision(sf::Vector2f point, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c);	// Point & Triangle
	bool isCollision(sf::Vector2f point, sf::Vector2f a, sf::Vector2f b);					// Point & Rectangle
	void initLevel();
	int countPlayerBullets();
	void spawnSaucer();
	void destroyAsteroid(Entity* e);

private:
	const enum playPhase { READY, PLAYING, EXPLOSION, GAME_OVER };
	playPhase phase;
	float phaseTimer;

	int level;
	int lives;
	bool invincible = false;
	int asteroidCount;
	float saucerWait;
	float bulletWait;
	float heartbeat, hbCount;
	static SoundEffects* se;
	static Ship* ship;
	Saucer* saucer;
	static Ship* livesSymbol;
	static Flame* flame;
	std::list<Entity*> entities;
	static LineText* lineText;
	static HighScore* highScore;
};

