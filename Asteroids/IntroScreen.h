#pragma once

#include <list>
#include <iostream>
#include "Game.h"
#include "LineText.h"
#include "HighScore.h"
#include "Entity.h"

class IntroScreen
{
public:
	IntroScreen(LineText &ls, HighScore &hs);
	int Run(sf::RenderWindow &window);

private:
	LineText* lineText;
	HighScore* highScore;
};

