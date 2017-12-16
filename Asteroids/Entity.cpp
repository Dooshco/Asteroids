#include "stdafx.h"
#include <iostream>

#include "Game.h"
#include "Entity.h"

const std::vector< std::vector<int> > fShip = { { 0,-10 },{ 7,10 },{ 3,7 },{ -3,7 },{ -7,10 },{ 0,-10 } };
const std::vector< std::vector<int> > fFlame = { { -2,8 },{ 2,8 },{ 0,12 },{ -2,8 } };
const std::vector< std::vector<int> > fBullet = { { 0,0 },{ 1,0 } };
const std::vector< std::vector<int> > fSaucer = { { -14,0 },{ 14,0 },{ 4,5 },{ -4,5 },{ -14,0 }, { -4,-4 },{ 4,-4 },{ 14,0 }, { 4,-4 },{ 2,-8 }, {-2,-8 },{-4,-4} };
const std::vector< std::vector< std::vector<int> > > fAsteroid = {
	{ { -5,-15 },{ 3,-10 },{ 13,-13 },{ 18,-5 },{ 10,-2 },{ 18,7 },{ 9,18 },{ -5,11 },{ -9,15 },{ -18,5 },{ -11,0 },{ -15, -8 },{ -5,-15 } },
	{ { -9,-18 },{ 0,-10 },{ 9,-18 },{ 18,-9 },{ 12,0 },{ 18,11 },{ 8,18 },{ -8,18 },{ -18,1 },{ -18,-1 },{ -9,-18 } },
	{ { -7,-18 },{ 7,-18 },{ 18,-11 },{ 18,-5 },{ 5, 1 },{ 18,5 },{ 8,16 },{ 4,13 },{ -8,16 },{ -18,5 },{ -18,-10 },{ -3,-10 },{ -7,-18 } }
};
const std::vector< std::vector<int> > fExplosion = { { 0,-2 },{ 3,2 },{ -3,1 },{ 0,-5 },{ -5,-1 },{ 1,-1 }, {2,1},{-3,2},{2,4},{-4,3},{1,-4},{1,1} };
const std::vector< std::vector<int> > fShipExplosion = { {-7,-10},{-7,-5}, {-7,-5},{0,10}, {0,10},{2,5}, {2,5},{7,-10}, {-3,-7},{3,-7} };


Entity::Entity(std::vector< std::vector<int> > fSource, int scale)
{
		this->scale = scale;

		// Create Normalized model of entity
		if (fSource[0][1] == -2)			// HACK TO IDENTIFY EXPLOSION
		{
			vNormalized = sf::VertexArray(sf::Points, fSource.size());
			fillVertexArray(vNormalized, fSource, scale);
			vObject = sf::VertexArray(sf::Points, fSource.size());
			translate();

		}
		else if (fSource[0][0] == -7)			// HACK TO IDENTIFY SHIP EXPLOSION
		{
			vNormalized = sf::VertexArray(sf::Lines, fSource.size());
			fillVertexArray(vNormalized, fSource, scale);
			vObject = sf::VertexArray(sf::Lines, fSource.size());
			translate();
		}
		else
		{
			vNormalized = sf::VertexArray(sf::LineStrip, fSource.size());
			fillVertexArray(vNormalized, fSource, scale);
			vObject = sf::VertexArray(sf::LineStrip, fSource.size());
			translate();
		}
}

Entity::Entity(int scale)
{
	this->scale = scale;
	int shape = rand() % 3;

	// Create Normalized model of entity
	vNormalized = sf::VertexArray(sf::LineStrip, fAsteroid[shape].size());
	fillVertexArray(vNormalized, fAsteroid[shape], scale);
	vObject = sf::VertexArray(sf::LineStrip, fAsteroid[shape].size());
	translate();

}

Entity::Entity()
{
	int shape = rand() % 3;

	// Create Normalized model of entity
	vNormalized = sf::VertexArray(sf::LineStrip, fAsteroid[shape].size());
	fillVertexArray(vNormalized, fAsteroid[shape], scale);
	vObject = sf::VertexArray(sf::LineStrip, fAsteroid[shape].size());
	translate();

}
Entity::~Entity()
{
}

void Entity::translate()
{
	for (unsigned int i = 0; i < vNormalized.getVertexCount(); i++)
	{
		vObject[i] = rotate_point(angle, vNormalized[i]);
		vObject[i].position.x = position.x + vObject[i].position.x;
		vObject[i].position.y = position.y + vObject[i].position.y;
	}
}


void Entity::update(float dt)
{
	if (angle >= 360) angle = angle - 360;

	position.x += speed.x * dt;
	position.y += speed.y * dt;

	if (position.x>WIDTH) position.x = 0;
	if (position.x<0) position.x = WIDTH;
	if (position.y>HEIGHT) position.y = 0;
	if (position.y<0) position.y = HEIGHT;

	translate();
}

void Entity::draw(sf::RenderWindow& window)
{
	if (visible && life)
		window.draw(vObject);
}

void Entity::fillVertexArray(sf::VertexArray &object, std::vector< std::vector<int> > fSource, int scale)
{
	for (unsigned int i = 0; i < fSource.size(); i++)
	{
		object[i].position.x = float(fSource[i][0] * scale) / 10.0f;
		object[i].position.y = float(fSource[i][1] * scale) / 10.0f;
		object[i].color = sf::Color::White;
	}
}

sf::Vertex Entity::rotate_point(float angle, sf::Vertex p)
{
	float s = sin(angle*DTR);
	float c = cos(angle*DTR);
	sf::Vertex vReturn;

	vReturn.position.x = p.position.x * c - p.position.y * s;
	vReturn.position.y = p.position.x * s + p.position.y * c;

	return vReturn;
}


Flame::Flame(int scale) : Entity(fFlame, scale)
{
	et = FLAME;
	position.x = WIDTH / 2;
	position.y = HEIGHT / 2;
}

void Flame::update(float newAngle, float x, float y)
{
	position.x = x;
	position.y = y;
	angle = newAngle;
	Entity::translate();
}

void Flame::update(float dt)
{
	if (rotateLeft)
	{
		angle = angle - SHIP_ROTATE*dt;
	}
	if (rotateRight)
	{
		angle = angle + SHIP_ROTATE*dt;
	}
	if (visible)
	{
		speed.x += cos((angle - 90)*DTR)*SHIP_ACCELERATION*dt;
		speed.y += sin((angle - 90)*DTR)*SHIP_ACCELERATION*dt;
		if ((speed.x*speed.x + speed.y*speed.y) >= SHIP_MAX_SPEED*SHIP_MAX_SPEED)
		{
			speed.x = cos((angle - 90)*DTR)*SHIP_MAX_SPEED;
			speed.y = sin((angle - 90)*DTR)*SHIP_MAX_SPEED;
		}

	}

	Entity::update(dt);
}


ShipExplosion::ShipExplosion(int scale) : Entity(fShipExplosion, scale)
{
	et = EXPLODING_SHIP;
	angle = 180;
	position.x = WIDTH / 2;
	position.y = HEIGHT / 2;
}

void ShipExplosion::update(float newAngle, float x, float y)
{
	position.x = x;
	position.y = y;
	angle = newAngle;
	Entity::translate();
}
void ShipExplosion::update(float dt)
{
	lifeTime -= dt;
	if (lifeTime <= 0.0f)
	{
		life = false;
		return;
	}

	for (unsigned int i = 0; i < vObject.getVertexCount(); i=i+2)
	{
		float dx = vNormalized[i].position.x * dt * 0.5f;
		float dy = vNormalized[i].position.y * dt * 0.5f;

		vNormalized[i].position.x = vNormalized[i].position.x + dx;
		vNormalized[i].position.y = vNormalized[i].position.y + dy;
		vNormalized[i+1].position.x = vNormalized[i+1].position.x + dx;
		vNormalized[i+1].position.y = vNormalized[i+1].position.y + dy;
	}

	Entity::update(dt);
}

Ship::Ship(int scale) : Entity(fShip, scale)
{
	et = SHIP;
	thrust = false;
	position.x = WIDTH / 2;
	position.y = HEIGHT / 2;

}

void Ship::update(float dt)
{
	if (rotateLeft)
	{
		angle = angle - SHIP_ROTATE*dt;
	}
	if (rotateRight)
	{
		angle = angle + SHIP_ROTATE*dt;
	}
	if (thrust)
	{
		speed.x += cos((angle - 90)*DTR)*SHIP_ACCELERATION*dt;
		speed.y += sin((angle - 90)*DTR)*SHIP_ACCELERATION*dt;
		if ((speed.x*speed.x + speed.y*speed.y) >= SHIP_MAX_SPEED*SHIP_MAX_SPEED)
		{
			speed.x = cos((angle - 90)*DTR)*SHIP_MAX_SPEED;
			speed.y = sin((angle - 90)*DTR)*SHIP_MAX_SPEED;
		}
	}

	Entity::update(dt);
}


Asteroid::Asteroid(int scale, float x, float y, float dx, float dy) : Entity(scale)
{
	et = ASTEROID;
	position.x = x;
	position.y = y;
	speed.x = dx;
	speed.y = dy;
	radius = 1.8f * float(scale);
}


Bullet::Bullet(int scale) : Entity(fBullet, scale)
{
	et = BULLET;
}

void Bullet::update(float dt)
{
	lifeTime -= dt;
	if (lifeTime <= 0.0f)
	{
		life = false;
		return;
	}

	position.x += speed.x * dt;
	position.y += speed.y * dt;

	if (position.x > WIDTH) position.x = 0;
	if (position.x < 0) position.x = WIDTH;
	if (position.y > HEIGHT) position.y = 0;
	if (position.y < 0) position.y = HEIGHT;

	Entity::translate();
}

PlayerBullet::PlayerBullet(int scale) : Entity(fBullet, scale)
{
	et = PLAYER_BULLET;
}

void PlayerBullet::update(float dt)
{
	lifeTime -= dt;
	if (lifeTime <= 0.0f)
	{
		life = false;
		return;
	}

	position.x += speed.x * dt;
	position.y += speed.y * dt;

	if (position.x > WIDTH) position.x = 0;
	if (position.x < 0) position.x = WIDTH;
	if (position.y > HEIGHT) position.y = 0;
	if (position.y < 0) position.y = HEIGHT;

	Entity::translate();
}

Saucer::Saucer(int scale, float x, float y, float dx, float dy) : Entity(fSaucer, scale)
{
	et = SAUCER;
	position.x = x;
	position.y = y;
	speed.x = dx;
	speed.y = dy;
	dirCount = float(rand() % 3 + 1);
}


void Saucer::update(float dt)
{
	lifeTime -= dt;
	if (lifeTime <= 0.0f)
	{
		life = false;
		return;
	}
	// Change Direction
	dirCount -= dt;
	if (dirCount <= 0.0f)
	{
		dirCount = float(rand() % 3 + 1);
		int newDir = rand() % 3;
		if (newDir == 0)
			speed.y = 0;
		else if (newDir == 1)
			speed.y = 30.0f;
		else
			speed.y = -30.0f;
	}

	position.x += speed.x * dt;
	position.y += speed.y * dt;

	Entity::update(dt);
}

Explosion::Explosion(float x, float y, int scale) : Entity(fExplosion, scale)
{
	et = EXPLOSION;
	position.x = x;
	position.y = y;
	growth = 1.001f;
}

void Explosion::update(float dt)
{

	growth = 1.0f + 3.0f*dt;

	for (unsigned int i = 0; i < vObject.getVertexCount(); i++)
	{
		vNormalized[i].position.x = vNormalized[i].position.x * growth;
		vNormalized[i].position.y = vNormalized[i].position.y * growth;
		
		if (vNormalized[i].position.x > 20.0f)
			life = false;
	}

	Entity::update(dt);
}