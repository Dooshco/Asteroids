#pragma once
#include "stdafx.h"
#include <SFML/Graphics.hpp>

class Entity
{
public:
	const enum entityType { SHIP, FLAME, ASTEROID, BULLET, PLAYER_BULLET, SAUCER, EXPLOSION, EXPLODING_SHIP };
	entityType et;

	bool life = true;
	float lifeTime;

	sf::Vector2f position = sf::Vector2f();
	sf::Vector2f speed = sf::Vector2f();
	sf::VertexArray vNormalized;
	sf::VertexArray vObject;
	float angle = 0;
	int scale = 1;
	float radius = 0;
	float growth = 1;
	bool visible = true;

	Entity(std::vector< std::vector<int> > fSource, int scale);
	Entity(int scale);
	Entity();
	virtual ~Entity();
	void translate();
	virtual void update(float dt);
	void draw(sf::RenderWindow& window);

private:
	void fillVertexArray(sf::VertexArray &object, std::vector< std::vector<int> > fSource, int scale);
	sf::Vertex rotate_point(float angle, sf::Vertex p);
};

class Asteroid : public Entity
{
public:
	Asteroid(int scale, float x, float y, float dx, float dy);
};


class Flame : public Entity
{
public:
	Flame(int scale);
	void update(float newAngle, float x, float y);
	void update(float dt);
	bool rotateLeft = false;
	bool rotateRight = false;
	bool thrust = false;
};

class ShipExplosion : public Entity
{
public:
	ShipExplosion(int scale);
	void update(float newAngle, float x, float y);
	void update(float dt) override;
};

class Ship : public Entity
{
public:
	Ship(int scale);
	void update(float dt) override;
	bool rotateLeft = false;
	bool rotateRight = false;
	bool thrust = false;
private:
	
};


class Bullet : public Entity
{
public:
	Bullet(int scale);
	void update(float dt);
};

class PlayerBullet : public Entity
{
public:
	PlayerBullet(int scale);
	void update(float dt);
};

class Saucer : public Entity
{
public:
	Saucer(int scale, float x, float y, float dx, float dy);
	void update(float dt);
	bool isLarge;
	float dirCount;
};

class Explosion : public Entity
{
public:
	Explosion(float x, float y, int scale);
	void update(float dt);
};
