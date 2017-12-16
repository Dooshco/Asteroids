#include "stdafx.h"
#include "IntroScreen.h"


IntroScreen::IntroScreen(LineText &lt, HighScore &hs)
{
	lineText = &lt;
	highScore = &hs;
}

int IntroScreen::Run(sf::RenderWindow &window)
{
	//create a list structure for all entities
	std::list<Entity*> entities;
	
	for (int i = 0; i<10; i++)
	{
		float xSpeed = float(ASTEROID_MAX_SPEED - (rand() % int(ASTEROID_MAX_SPEED)) * 2);
		float ySpeed = float(ASTEROID_MAX_SPEED - (rand() % int(ASTEROID_MAX_SPEED)) * 2);

		Asteroid *a = new Asteroid(ASTEROID_LARGE, float(rand() % WIDTH), float(rand() % HEIGHT), xSpeed, ySpeed);
		entities.push_back(a);
	}

	sf::Clock clock;
	bool spacePressed = false;
	float currentTime = clock.restart().asSeconds();

	float displayTime = 10.0f;

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyReleased)
				if (event.key.code == sf::Keyboard::Space)
				{
					return(PLAY);
				}
			if (event.key.code == sf::Keyboard::Escape)
			{
				return(-1);
			}

		}

		float newTime = clock.getElapsedTime().asSeconds();
		float dt = newTime - currentTime;
		currentTime = newTime;

		displayTime -= dt;
		if (displayTime <= 0)
			return(HIGH);

		for (auto e : entities)
		{
			e->update(dt);
		}


		// Draw
		window.clear(border);

		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(float(WIDTH), float(HEIGHT)));
		rectangle.setFillColor(sf::Color::Black);
		window.draw(rectangle);

		for (auto e : entities)
			e->draw(window);

		// Draw score
		std::string h = std::to_string(highScore->getHigh());
		lineText->drawString(window, h, (WIDTH - SMALL_TEXT * 10) / 2, LARGE_TEXT * 2, SMALL_TEXT);

		lineText->drawString(window, "00", LARGE_TEXT * 8, LARGE_TEXT * 2, LARGE_TEXT);
		lineText->drawString(window, "00", WIDTH - (LARGE_TEXT * 10), LARGE_TEXT * 2, LARGE_TEXT);

		lineText->drawString(window, "1 COIN 1 PLAY", (WIDTH - (13 * LARGE_TEXT)) / 2, HEIGHT - (LARGE_TEXT * 6), LARGE_TEXT);
		lineText->drawString(window, "2017 DOOSHCO INC", (WIDTH - (16 * SMALL_TEXT)) / 2, HEIGHT - (SMALL_TEXT * 2), SMALL_TEXT);
		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}