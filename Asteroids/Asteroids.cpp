#include "stdafx.h"
#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "HighScore.h"
#include "LineText.h"
#include "Entity.h"
#include "IntroScreen.h"
#include "PlayScreen.h"
#include "HighScreen.h"

// Global variables
int score;

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	//settings.antialiasingLevel = 1;
	//settings.antialiasingLevel = 0;	// No Antialiasing


	// FULL SCREEN MODE
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Asteroids", sf::Style::Fullscreen, settings);

	// WINDOWED MODE
	//sf::RenderWindow window(sf::VideoMode(1024, 900), "Asteroids", sf::Style::Default, settings);

	// Calculate aspect ratio
	const float ratio = float(WIDTH) / float(HEIGHT);
	const sf::Vector2u winSize = window.getSize();
	const float winRatio = float(winSize.x) / float(winSize.y);

	if (ratio > winRatio)  // game is wider than screen
	{
		const float height = HEIGHT*ratio;
		const float yOffset = (HEIGHT - height) / 2;
		window.setView(sf::View(sf::FloatRect(0.0f, yOffset, float(WIDTH), height)));
	}
	else					// game is taller than screen
	{
		const float width = WIDTH*ratio;
		const float xOffset = (WIDTH - width) / 2;
		window.setView(sf::View(sf::FloatRect(xOffset, 0.0f, width, float(HEIGHT))));
	}

	window.setVerticalSyncEnabled(false);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursorVisible(false);

	// Build global assets
	LineText lt;
	HighScore scores;

	// Setup screens
	IntroScreen is(lt, scores);
	PlayScreen ps(lt, scores);
	HighScreen hs(lt, scores);

	int screen = INTRO;

	//Main loop between screens
	while (screen >= 0)
	{
		switch (screen)
		{
		case INTRO:
			screen = is.Run(window);
			break;
		case PLAY:
			screen = ps.Run(window);
			break;
		case HIGH:
			screen = hs.Run(window);
			break;
		}
	}
	return 0;
}

