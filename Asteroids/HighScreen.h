#pragma once

#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "LineText.h"
#include "HighScore.h"
#include "Entity.h"

class HighScreen
{
public:
	HighScreen(LineText &ls, HighScore &hs);
	int Run(sf::RenderWindow &window);

private:
	LineText* lineText;
	HighScore* highScore;
	char cInitials[4];
};


