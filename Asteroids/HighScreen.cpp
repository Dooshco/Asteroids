#include "stdafx.h"
#include "HighScreen.h"

extern int score;

HighScreen::HighScreen(LineText &lt, HighScore &hs)
{
	lineText = &lt;
	highScore = &hs;
}

int HighScreen::Run(sf::RenderWindow &window)
{
	bool inputMode;
	if (score > highScore->getLow())
		inputMode = true;
	else
		inputMode = false;

	bool enterPressed = false;
	int charEditing = 0;

	cInitials[0] = '_';
	cInitials[1] = '_';
	cInitials[2] = '_';
	cInitials[3] = 0;

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
			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Space)
				{
					return(1);
				}
			if (event.key.code == sf::Keyboard::Escape)
			{
				return(-1);
			}
			if (inputMode)
			{
				// Turn Right - Characters Up
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					char c = cInitials[charEditing];
					(c == '_' ? c = 'A' : c++);
					if (c > 'Z') c = '_';
					cInitials[charEditing] = c;
				}
				// Turn Left - Characters Down
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					char c = cInitials[charEditing];
					(c == '_' ? c = 'Z' : c--);
					if (c < 'A') c = '_';
					cInitials[charEditing] = c;
				}
				// Hyperspace - Enter - Confirm character
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !enterPressed)
				{
					enterPressed = true;
					charEditing++;
					if (charEditing <= 2)
					{
						cInitials[charEditing] = 'A';
					}
					else
					{
						highScore->setScore(cInitials, score);
						score = 0;
						inputMode = false;
					}
				}
				else
					enterPressed = false;
			}
		}

		float newTime = clock.getElapsedTime().asSeconds();
		float dt = newTime - currentTime;
		currentTime = newTime;

		// Draw
		window.clear(border);

		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(float(WIDTH), float(HEIGHT)));
		rectangle.setFillColor(sf::Color::Black);
		window.draw(rectangle);

		// Draw score
		std::string s = std::to_string(score);
		std::string h = std::to_string(highScore->getHigh());
		lineText->drawString(window, s, LARGE_TEXT * 8, LARGE_TEXT * 2, LARGE_TEXT);
		lineText->drawString(window, h, (WIDTH - SMALL_TEXT * 10) / 2, LARGE_TEXT * 2, SMALL_TEXT);

		lineText->drawString(window, "00", WIDTH - (LARGE_TEXT * 10), LARGE_TEXT * 2, LARGE_TEXT);

		if (inputMode)
		{
			lineText->drawString(window, "YOUR SCORE IS ONE OF THE TEN BEST", 100, 200, LARGE_TEXT);
			lineText->drawString(window, "PLEASE ENTER YOUR INITIALS", 100, 230, LARGE_TEXT);
			lineText->drawString(window, "PUSH ROTATE TO SELECT LETTER", 100, 260, LARGE_TEXT);
			lineText->drawString(window, "PUSH HYPERSPACE WHEN LETTER IS CORRECT", 100, 290, LARGE_TEXT);

			lineText->drawLetter(window, &cInitials[0], 540, 600, VERY_LARGE_TEXT);
			lineText->drawLetter(window, &cInitials[1], 540+lineText->getLetterWidth(VERY_LARGE_TEXT), 600, VERY_LARGE_TEXT);
			lineText->drawLetter(window, &cInitials[2], 540 + lineText->getLetterWidth(VERY_LARGE_TEXT)*2, 600, VERY_LARGE_TEXT);
		}
		else
		{
			displayTime -= dt;
			if (displayTime <= 0)
				return(INTRO);

			lineText->drawString(window, "PUSH START", 500, 120, LARGE_TEXT);
			lineText->drawString(window, "HIGH SCORES", 500, 180, LARGE_TEXT);

			for (int i = 0; i < 10; i++)
			{
				char buf[15];
				//std::sprintf(buf, "%2d %5d %s", i + 1, highScore->getScore(i), highScore->getName(i).c_str());
				sprintf_s(buf, "%2d %5d %s", i + 1, highScore->getScore(i), highScore->getName(i).c_str());

				lineText->drawString(window, buf , 475, 250+i*int(LARGE_TEXT*1.5f), LARGE_TEXT);
			}

		}

		lineText->drawString(window, "2017 DOOSHCO INC", (WIDTH - (16 * SMALL_TEXT)) / 2, HEIGHT - (SMALL_TEXT * 2), SMALL_TEXT);
		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}