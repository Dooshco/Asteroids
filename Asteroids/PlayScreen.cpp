#include "stdafx.h"
#include "PlayScreen.h"

extern int score;
extern LineText lt;
extern HighScore hs;

PlayScreen::PlayScreen(LineText &lt, HighScore &hs)
{
	lineText = &lt;
	highScore = &hs;
	se = new SoundEffects();
	livesSymbol = new Ship(SCALE);
}

int PlayScreen::Run(sf::RenderWindow &window)
{
	phase = READY;
	phaseTimer = 1.0f;

	level = 0;
	score = 0;
	lives = 3;
	asteroidCount = 0;
	heartbeat = MAX_HEARTBEAT;
	hbCount = heartbeat;
	saucerWait = SAUCER_WAIT + (heartbeat*5.0f);

	sf::Clock clock;
	bool spacePressed = false;
	bool enterPressed = false;
	float currentTime = clock.restart().asSeconds();

	while (window.isOpen())
	{

		// Check if level is cleared
		if (asteroidCount == 0)
		{
			level++;
			initLevel();
			heartbeat = MAX_HEARTBEAT;
		}

		// Process Keyboard and other events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyReleased)
				if (event.key.code == sf::Keyboard::Escape)
				{
					return(INTRO);
				}
		}

		// Turn Right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			ship->rotateRight = true;
			flame->rotateRight = true;
		}
		else
		{
			ship->rotateRight = false;
			flame->rotateRight = false;
		}
		// Turn Left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			ship->rotateLeft = true;
			flame->rotateLeft = true;
		}
		else
		{
			ship->rotateLeft = false;
			flame->rotateLeft = false;
		}
		// Thrust
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			ship->thrust = true;
			flame->visible = true;
			se->Play(SoundEffects::THRUST);
		}
		else
		{
			ship->thrust = false;
			flame->visible = false;
			se->Stop(SoundEffects::THRUST);
		}
		// Hyperspace
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) )
		{
			if (enterPressed == false)
				{
				enterPressed = true;
				bool isSafe = false;
				float newX, newY;

				do
				{
					// calculate new position safely inside the screen
					newX = float(rand() % (WIDTH-50) + 25);
					newY = float(rand() % (HEIGHT-50) + 25);
					sf::Vector2f newPosition(newX, newY);
					isSafe = true;

					for (auto a : entities)
					{
						if (a->et == Entity::ASTEROID)
						{
							if (isCollision(newPosition, a->position, a->radius))
							{
								isSafe = false;
							}
						}
					}
				} while (!isSafe);

				ship->position.x = newX;
				ship->position.y = newY;
				flame->position.x = newX;
				flame->position.y = newY;
			}
		}
		else
		{
			enterPressed = false;
		}
		// Fire
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (spacePressed == false && countPlayerBullets() < 4)
			{
				se->Play(SoundEffects::LASER);
				spacePressed = true;
				PlayerBullet *b = new PlayerBullet(10);
				b->angle = ship->angle;
				b->position.x = ship->vObject[0].position.x;
				b->position.y = ship->vObject[0].position.y;
				b->speed.x = cos((ship->angle - 90)*DTR) * BULLET_SPEED;
				b->speed.y = sin((ship->angle - 90)*DTR) * BULLET_SPEED;
				b->lifeTime = 2.0;
				entities.push_back(b);
			}
		}
		else
		{
			spacePressed = false;
		}

		// Update the game
		float newTime = clock.getElapsedTime().asSeconds();
		float dt = newTime - currentTime;
		currentTime = newTime;

		if (phase == PLAYING)
		{
			Update(dt);

			// Heartbeat
			hbCount -= dt;
			if (hbCount <= 0.0f)
			{
				se->Play(SoundEffects::BEAT);
				heartbeat -= 0.02f;
				if (heartbeat < MIN_HEARTBEAT)
					heartbeat = MIN_HEARTBEAT;
				hbCount = heartbeat;
			}
		}
		else if (phase == EXPLOSION)
		{
			Update(dt);

			phaseTimer -= dt;

			if (phaseTimer <= 0) // end of Explosion of ship
			{
				phase = PLAYING;
				ship->visible = true;
				ship->thrust = false;
				ship->position.x = WIDTH / 2;
				ship->position.y = HEIGHT / 2;
				ship->speed.x = 0;
				ship->speed.y = 0;
				ship->angle = 0;
				flame->position.x = WIDTH / 2;
				flame->position.y = HEIGHT / 2;
				flame->speed.x = 0;
				flame->speed.y = 0;
				flame->angle = 0;
			}
		}
		else if (phase == GAME_OVER)
		{
			se->Stop(SoundEffects::SAUCER); // Saucer sound might still be going if ship was destroyed

			Update(dt);

			phaseTimer -= dt;

			if (phaseTimer <= 0) // end of Explosion of ship
			{
				return(HIGH);
			}
		}
		else
		{
			phaseTimer -= dt;

			if (phaseTimer <= 0)
			{
				if (phase == READY)
				{
					phase = PLAYING;
				}
			}
		}

		// Draw
		Draw(window);

	}
	
	return 0;


}

void PlayScreen::Update(float dt)
{
	// Randomly spawn Saucer
	saucerWait -= dt;
	bulletWait -= dt;
	if (saucer == nullptr)
	{
		// If time is right
		if (saucerWait <= 0.0f)
		{
			spawnSaucer();
			se->Play(SoundEffects::SAUCER);
		}
	}
	else  // saucer is alive so it can shoot
	{
		if (bulletWait <= 0.0f)
		{
			Bullet *b = new Bullet(10);
			// Aim the bullet
			if (saucer->isLarge)
			{
				b->angle = float(rand() % 360);
			}
			else
			{
				// formula for direct aim is angle = atan2(y2 - y1, x2 - x1) * 180 / PI;
				b->angle = atan2f(ship->position.y - saucer->position.y, ship->position.x - saucer->position.x) / DTR;
			}
			b->position.x = saucer->position.x;
			b->position.y = saucer->position.y;
			b->speed.x = cos(b->angle*DTR) * BULLET_SPEED;
			b->speed.y = sin(b->angle*DTR) * BULLET_SPEED;
			b->lifeTime = 2.0;
			entities.push_back(b);

			bulletWait = 1.5f - float(level) / 20.0f;
			//bulletWait = 0.05;

		}

	}
		
	// Update all entities
	for (auto e : entities)
	{
		e->update(dt);
	}

	// If not in PLAYING phase dont do collision
	if (phase != PLAYING)
		return;

	// Check for collisions
	for (auto a : entities)
		for (auto b : entities)
		{
			// Asteroid and Bullet
			if (a->et == Entity::ASTEROID && (b->et == Entity::PLAYER_BULLET || b->et == Entity::BULLET))
			{
				if (isCollision(b->position, a->position, a->radius) && b->life)
				{
					se->Play(SoundEffects::EXPLOSION);
					a->life = false;
					b->life = false;
					destroyAsteroid(a);
				}
			}
			// Asteroid and Ship
			if (a->et == Entity::ASTEROID && b->et == Entity::SHIP && !invincible)
			{
				// Compare with three outside points of ship
				sf::Vector2f x(b->vObject[0].position.x, b->vObject[0].position.y);
				sf::Vector2f y(b->vObject[1].position.x, b->vObject[1].position.y);
				sf::Vector2f z(b->vObject[2].position.x, b->vObject[2].position.y);
				if (isCollision(x, a->position, a->radius) || isCollision(y, a->position, a->radius) || isCollision(z, a->position, a->radius))
				{
					ShipExplosion *shipEx;
					shipEx = new ShipExplosion(SCALE);
					shipEx->position.x = b->position.x;
					shipEx->position.y = b->position.y;
					shipEx->angle = b->angle + 180;
					shipEx->lifeTime = 2.0f;
					entities.push_back(shipEx);

					se->Play(SoundEffects::EXPLOSION);

					a->life = false;
					destroyAsteroid(a);

					b->visible = false;
					lives--;
					phase = EXPLOSION;
					phaseTimer = 2.0f;
					if (lives <= 0)
					{
						phase = GAME_OVER;
					}
				}
			}
			// Ship and Bullet
			if (a->et == Entity::SHIP && (b->et == Entity::BULLET || b->et == Entity::PLAYER_BULLET) && !invincible)
			{
				sf::Vector2f x(a->vObject[0].position.x, a->vObject[0].position.y);
				sf::Vector2f y(a->vObject[1].position.x, a->vObject[1].position.y);
				sf::Vector2f z(a->vObject[2].position.x, a->vObject[2].position.y);

				if (isCollision(b->position, x, y, z))
				{
					ShipExplosion *shipEx;
					shipEx = new ShipExplosion(SCALE);
					shipEx->position.x = a->position.x;
					shipEx->position.y = a->position.y;
					shipEx->angle = a->angle + 180;
					shipEx->lifeTime = 2.0f;
					entities.push_back(shipEx);

					se->Play(SoundEffects::EXPLOSION);

					b->life = false;
					a->visible = false;
					lives--;
					phase = EXPLOSION;
					phaseTimer = 2.0f;
					if (lives <= 0)
					{
						phase = GAME_OVER;
					}
				}
			}
			// Saucer and Bullet
			if (a->et == Entity::SAUCER && b->et == Entity::PLAYER_BULLET)
				{
				sf::Vector2f p1(a->vObject[0].position.x, a->vObject[9].position.y);
				sf::Vector2f p2(a->vObject[1].position.x, a->vObject[2].position.y);

				if (isCollision(b->position, p1, p2))
				{
					if (saucer->isLarge == true)
						score += 200;
					else
						score += 1000;
					se->Play(SoundEffects::EXPLOSION);
					a->life = false;
					saucer = nullptr;
					b->life = false;
					Explosion *boom = new Explosion(a->position.x, a->position.y, 10);
					entities.push_back(boom);

				}
			}
			// Saucer and Asteroid
			if (a->et == Entity::SAUCER && b->et == Entity::ASTEROID)
			{
				// Compare all four corners of rectangle (saucer) with radius from asteroid
				const sf::Vector2f p1(a->vObject[0].position.x, a->vObject[9].position.y);
				const sf::Vector2f p2(a->vObject[1].position.x, a->vObject[2].position.y);
				const sf::Vector2f p3(a->vObject[0].position.x, a->vObject[2].position.y);
				const sf::Vector2f p4(a->vObject[1].position.x, a->vObject[9].position.y);
				
				if (isCollision(p1, b->position, b->radius) ||
					isCollision(p2, b->position, b->radius) ||
					isCollision(p3, b->position, b->radius) ||
					isCollision(p4, b->position, b->radius))
				{
					se->Play(SoundEffects::EXPLOSION);
					a->life = false;
					saucer = nullptr;
					b->life = false;
					destroyAsteroid(b);
					Explosion *boom = new Explosion(a->position.x, a->position.y, 10);
					entities.push_back(boom);
				}
			}

			// Ship and Saucer
			if (a->et == Entity::SAUCER && b->et == Entity::SHIP)
			{
				// Compare all three corners of triangle (saucer) with rectangle of saucer
				sf::Vector2f p1(a->vObject[0].position.x, a->vObject[9].position.y);
				sf::Vector2f p2(a->vObject[1].position.x, a->vObject[2].position.y);

				if (isCollision(b->position, p1, p2) ||
					isCollision(b->position, p1, p2) ||
					isCollision(b->position, p1, p2))
				{
					a->life = false;
					saucer = nullptr;
					Explosion *boom = new Explosion(a->position.x, a->position.y, 10);
					entities.push_back(boom);

					ShipExplosion *shipEx;
					shipEx = new ShipExplosion(SCALE);
					shipEx->position.x = b->position.x;
					shipEx->position.y = b->position.y;
					shipEx->angle = b->angle + 180;
					shipEx->lifeTime = 2.0f;
					entities.push_back(shipEx);

					se->Play(SoundEffects::EXPLOSION);

					b->visible = false;
					lives--;
					phase = EXPLOSION;
					phaseTimer = 2.0f;
					if (lives <= 0)
					{
						phase = GAME_OVER;
					}
				}
			}

			// Player Bullet and Saucer Bullet
			if (a->et == Entity::PLAYER_BULLET && b->et == Entity::BULLET)
			{
				if (a->position == b->position)
				{
					a->life = false;
					b->life = false;
				}
			}
		}


	// Remove "dead" entities
	for (auto it = entities.begin(); it != entities.end(); /*increment in body*/)
	{
		if (!(*it)->life)
		{
			if ((*it)->et == Entity::SAUCER)	// very ugly, clean up later
			{
				it = entities.erase(it);
				saucer = nullptr;
				saucerWait = SAUCER_WAIT + (heartbeat*5.0f);
				se->Stop(SoundEffects::SAUCER);

			}
			else
			{
				it = entities.erase(it);
			}
		}
		else
		{
			++it;
		}
	}
}

void PlayScreen::Draw(sf::RenderWindow& window)
{
	window.clear(border);

	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(float(WIDTH), float(HEIGHT)));
	rectangle.setFillColor(sf::Color::Black);
	window.draw(rectangle);

	switch(phase)
	{
		case READY:
			lineText->drawString(window, "PLAYER 1", 500, 250, LARGE_TEXT);
			break;
		case EXPLOSION:
		case PLAYING:
			for (auto e : entities)
				e->draw(window);
			break;
		case GAME_OVER:
			lineText->drawString(window, "GAME OVER", 500, 250, LARGE_TEXT);
			break;
	}

	// Draw lives
	for (int i = 0; i < lives; i++)
	{
		livesSymbol->position.x = 130.0f + i*SCALE*1.5f;
		livesSymbol->position.y = 80.0f;
		livesSymbol->update(0.0f);
		livesSymbol->draw(window);
	}

	// Draw score
	std::string s = std::to_string(score);
	std::string h = std::to_string(highScore->getHigh());
	lineText->drawString(window, s, LARGE_TEXT * 8, LARGE_TEXT * 2, LARGE_TEXT);
	lineText->drawString(window, h, (WIDTH - SMALL_TEXT * 10) / 2, LARGE_TEXT * 2, SMALL_TEXT);

	lineText->drawString(window, "2017 DOOSHCO INC", (WIDTH - (16 * SMALL_TEXT)) / 2, HEIGHT - (SMALL_TEXT * 2), SMALL_TEXT);
	window.display();

}


// Collison between point and circle
bool PlayScreen::isCollision(sf::Vector2f point, sf::Vector2f circle, float radius)
{
	float distanceSquare = abs(circle.x - point.x) * abs(circle.x - point.x) +
		abs(circle.y - point.y) * abs(circle.y - point.y);
	float radiusSquare = radius * radius;
	if (distanceSquare <= radiusSquare)
		return true;
	else
		return false;
}
// Collision between point and triangle
bool PlayScreen::isCollision(sf::Vector2f point, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c)
{
	float as_x = point.x - a.x;
	float as_y = point.y - a.y;

	bool s_ab = (b.x - a.x)*as_y - (b.y - a.y)*as_x > 0;

	if ((c.x - a.x)*as_y - (c.y - a.y)*as_x > 0 == s_ab) return false;

	if ((c.x - b.x)*(point.y - b.y) - (c.y - b.y)*(point.x - b.x) > 0 != s_ab) return false;

	return true;

}

// Collision between point and rectangle
bool PlayScreen::isCollision(sf::Vector2f point, sf::Vector2f a, sf::Vector2f b)
{
	if (point.x >= a.x && point.x <= b.x && point.y >= a.y && point.y <= b.y)
		return true;
	else
		return false;
}




void PlayScreen::initLevel()
{
	entities.clear();

	phase = READY;
	phaseTimer = 1.0f;

	asteroidCount = 0;
	heartbeat = MAX_HEARTBEAT;
	hbCount = heartbeat;


	// Create a ship with thruster flame
	ship = new Ship(SCALE);
	entities.push_back(ship);
	flame = new Flame(SCALE);
	entities.push_back(flame);

	// Create Saucer
	//saucer = new Saucer(SAUCER_LARGE, float(rand() % WIDTH), float(rand() % HEIGHT), 100.0f, 0.0f);
	//entities.push_back(saucer);
	saucer = nullptr;
	saucerWait = SAUCER_WAIT + (heartbeat*5.0f);

	// Create Asteroids: 2, 4, 6, 8, 10, 11, 11...
	int num = 2 + level * 2;
	if (num > 10) num = 11;
	//num = 10;  // FOR TESTING
	for (int i = 0; i<num; i++)
	{
		int SPEED = int(ASTEROID_MIN_SPEED) + level * 10;
		if (SPEED > ASTEROID_MAX_SPEED)
			SPEED = int(ASTEROID_MAX_SPEED);
		float angle = float(rand() % 360);
		float xSpeed = SPEED * sin(angle*DTR);
		float ySpeed = SPEED * cos(angle*DTR);
		bool inBorder = true;
		float xPosition;
		float yPosition;
		// Make sure the position of asteroids is in 1/5 of edge of screen
		do
		{
			xPosition = float(rand() % WIDTH);
			yPosition = float(rand() % HEIGHT);
			if ((xPosition > WIDTH / 5 || xPosition < WIDTH / 5 * 4) && (yPosition < HEIGHT / 5 || yPosition > HEIGHT / 5 * 4))
				inBorder = false;
			else
				inBorder = true;
		} while (inBorder);

		Asteroid *a = new Asteroid(ASTEROID_LARGE, xPosition, yPosition, xSpeed, ySpeed);
		a->angle = angle;

		entities.push_back(a);
		asteroidCount++;
	}


}

int PlayScreen::countPlayerBullets()
{
	int count = 0;
	for (auto a : entities)
	{
		if (a->et == Entity::PLAYER_BULLET)
			count++;
	}
	return count;
}

void PlayScreen::spawnSaucer()
{
	// Randomly decide if it is small or large saucer to be spawned,
	// but higher the level more likely it will be small one
	int saucerSize;
	float saucerSpeed;
	if (rand() % (level+1) == 1)
	{
		saucerSize = SAUCER_LARGE;
		saucerSpeed = LARGE_SAUCER_SPEED;
	}
	else
	{
		saucerSize = SAUCER_SMALL;
		saucerSpeed = SMALL_SAUCER_SPEED;
	}
	// decide whether spawn left or right
	float x, y, speedx;
	if (rand() % 2 == 1)
	{
		x = 0.0f;
		speedx = saucerSpeed;
	}
	else
	{
		x = WIDTH;
		speedx = -saucerSpeed;
	}

	// decide Y for spawn
	y = float(rand() % (HEIGHT - 50) + 25);

	saucer = new Saucer(saucerSize, x, y, speedx, 0.0f);
	saucer->lifeTime = 10.0f;
	saucer->isLarge = (saucerSize == SAUCER_LARGE);
	entities.push_back(saucer);
}

void PlayScreen::destroyAsteroid(Entity* a)
{
	int oldscore = score;
	if (a->scale == ASTEROID_LARGE)
	{
		score += 20;
		asteroidCount--;
	}
	else if (a->scale == ASTEROID_MEDIUM)
	{
		score += 50;
		asteroidCount--;
	}
	else
	{
		score += 100;
		asteroidCount--;
	}
	if (score / 10000 != oldscore / 10000)
	{
		lives++;
		se->Play(SoundEffects::NEWLIFE);
	}

	Explosion *boom = new Explosion(a->position.x, a->position.y, 10);
	entities.push_back(boom);

	if (a->scale != ASTEROID_SMALL)
	{
		for (int i = 0; i < 2; i++)
		{

			int SPEED = int(ASTEROID_MIN_SPEED) + level * 10;
			if (SPEED > ASTEROID_MAX_SPEED)
				SPEED = int(ASTEROID_MAX_SPEED);

			float newAngle = a->angle + (15 - i * 30);
			float xSpeed = SPEED * sin(newAngle*DTR);
			float ySpeed = SPEED * cos(newAngle*DTR);

			Asteroid *newA;
			if (a->scale == ASTEROID_LARGE)
				newA = new Asteroid(ASTEROID_MEDIUM, a->position.x, a->position.y, xSpeed, ySpeed);
			else
				newA = new Asteroid(ASTEROID_SMALL, a->position.x, a->position.y, xSpeed, ySpeed);

			newA->angle = newAngle;

			entities.push_back(newA);
			asteroidCount++;
		}
	}
}


SoundEffects* PlayScreen::se;
Ship* PlayScreen::ship;
//Saucer* PlayScreen::saucer;
Ship* PlayScreen::livesSymbol;
Flame* PlayScreen::flame;
LineText* PlayScreen::lineText;
HighScore* PlayScreen::highScore;