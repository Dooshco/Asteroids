#pragma once
#include "stdafx.h"
#include <SFML/Graphics.hpp>

class LineText
{
public:
	LineText();
	~LineText();
	void drawLetter(sf::RenderWindow & window, char *Letter, int x, int y, int s);
	void drawString(sf::RenderWindow & window, std::string t, int x, int y, int s);
	int getLetterWidth(int s);

private:

};

