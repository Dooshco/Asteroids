#include "stdafx.h"
#include <iostream>
#include <string>
#include "LineText.h"


static std::vector< std::vector< std::vector<__int8> > > numbers = {
	{ { 8, 0 },{ 8,10 },{ 0,10 },{ 0, 0 },{ 8, 0 } },								// 0
	{ { 4, 0 },{ 4,10 } },															// 1
	{ { 0, 0 },{ 8, 0 },{ 8, 5 },{ 0, 5 },{ 0,10 },{ 8,10 } },						// 2
	{ { 0, 0 },{ 8, 0 },{ 8, 5 },{ 0, 5 },{ 8, 5 },{ 8,10 },{ 0,10 } },				// 3
	{ { 0, 0 },{ 0, 5 },{ 8, 5 },{ 8, 0 },{ 8,10 } },								// 4
	{ { 8, 0 },{ 0, 0 },{ 0, 5 },{ 8, 5 },{ 8,10 },{ 0,10 } },						// 5
	{ { 0, 0 },{ 0,10 },{ 8,10 },{ 8, 5 },{ 0, 5 } },								// 6
	{ { 0, 0 },{ 8, 0 },{ 8,10 } },													// 7
	{ { 0, 0 },{ 8, 0 },{ 8,10 },{ 0,10 },{ 0, 0 },{ 0, 5 },{ 8, 5 } },				// 8
	{ { 8,10 },{ 8, 0 },{ 0, 0 },{ 0, 5 },{ 8, 5 } }								// 9
};

static std::vector< std::vector< std::vector<__int8> > > letters = {
	{ { 0,10 },{ 0, 3 },{ 4, 0 },{ 8, 3 },{ 8,10 },{ 8, 6 },{ 0, 6 } },												// A
	{ { 0, 0 },{ 6, 0 },{ 8, 2 },{ 8, 3 },{ 6, 5 },{ 8, 7 },{ 8, 8 }, { 6,10 }, { 0,10}, {0,0}, {0,5}, {6,5}},		// B
	{ { 8, 0 },{ 0, 0 },{ 0,10 },{ 8,10 }},																			// C
	{ { 0, 0 },{ 6, 0 },{ 8, 3 },{ 8, 7 },{ 6,10 },{ 0,10 },{ 0, 0 } },												// D
	{ { 8, 0 },{ 0, 0 },{ 0, 5 },{ 6, 5 },{ 0, 5 },{ 0,10 },{ 8,10 } },												// E
	{ { 8, 0 },{ 0, 0 },{ 0, 5 },{ 6, 5 },{ 0, 5 },{ 0,10 } },														// F
	{ { 8, 3 },{ 8, 0 },{ 0, 0 },{ 0,10 },{ 8,10 },{ 8, 7 },{ 3, 7 } },												// G
	{ { 0, 0 },{ 0,10 },{ 0, 5 },{ 8, 5 },{ 8, 0 },{ 8,10 } },														// H
	{ { 0, 0 },{ 8, 0 },{ 4, 0 },{ 4,10 },{ 0,10 },{ 8,10 } },														// I
	{ { 8, 0 },{ 8,10 },{ 4,10 },{ 0, 7 } },																		// J
	{ { 0, 0 },{ 0,10 },{ 0, 5 },{ 8, 0 },{ 0, 5 }, { 8,10} },														// K
	{ { 0, 0 },{ 0,10 },{ 8,10 } },																					// L
	{ { 0,10 },{ 0, 0 },{ 4, 3 },{ 8, 0 },{ 8,10} },																// M
	{ { 0,10 },{ 0, 0 },{ 8,10 },{ 8, 0 } },																		// N
	{ { 8, 0 },{ 8,10 },{ 0,10 },{ 0, 0 },{ 8, 0 } },																// O
	{ { 0,10 },{ 0, 0 },{ 8, 0 },{ 8, 5 },{ 0, 5 } },																// P
	{ { 8, 6 },{ 8, 0 },{ 0, 0 },{ 0,10 },{ 4,10 },{ 8, 6 },{ 6, 8 }, { 4, 6 }, { 8,10 } },							// Q
	{ { 0,10 },{ 0, 0 },{ 8, 0 },{ 8, 5 },{ 0, 5 },{ 3, 5 },{ 8,10 } },												// R
	{ { 8, 0 },{ 0, 0 },{ 0, 5 },{ 8, 5 },{ 8,10 },{ 0,10 } },														// S
	{ { 0, 0 },{ 8, 0 },{ 4, 0 },{ 4,10 } },																		// T
	{ { 0, 0 },{ 0,10 },{ 8,10 },{ 8, 0 } },																		// U
	{ { 0, 0 },{ 4,10 },{ 8, 0 } },																					// V
	{ { 0, 0 },{ 0,10 },{ 4, 7 },{ 8,10 },{ 8, 0 } },																// W
	{ { 0, 0 },{ 8,10 },{ 4, 5 },{ 8, 0 },{ 0,10 } },																// X
	{ { 0, 0 },{ 4, 3 },{ 8, 0 },{ 4, 3 },{ 4,10 } },																// Y
	{ { 0, 0 },{ 8, 0 },{ 0,10 },{ 8,10 } },																		// Z
	{ { 0,10 },{10,10} },																							// [	TBD
	{ { 0,10 },{ 10,10 } },																							// \	TBD
	{ { 0,10 },{ 10,10 } },																							// ]	TBD
	{ { 0,10 },{ 10,10 } },																							// ^	TBD
	{ { 0,10 },{ 10,10 } }																							// _
};


sf::VertexArray vNumbers[10];


LineText::LineText()
{

}


LineText::~LineText()
{
}

void LineText::drawLetter(sf::RenderWindow & window, char *Letter, int x, int y, int s)
{
	if (*Letter < 48 || (*Letter > 57 && *Letter < 65) || *Letter > 95)
		return;

	unsigned int num;
	if (*Letter <= 57)
	{
		num = unsigned int(*Letter) - 48;
		sf::VertexArray vertexLetter(sf::LineStrip, numbers[num].size());

		for (unsigned int i = 0; i < numbers[num].size(); i++)
		{
			vertexLetter[i].position.x = (float)x + (float)(numbers[num][i][0] * s) / 10;
			vertexLetter[i].position.y = (float)y + (float)(numbers[num][i][1] * s) / 10;
			vertexLetter[i].color = sf::Color::White;
		}

		window.draw(vertexLetter);
	}
	else
	{
		num = int(*Letter) - 65;

		sf::VertexArray vertexLetter(sf::LineStrip, letters[num].size());

		for (unsigned int i = 0; i < letters[num].size(); i++)
		{
			vertexLetter[i].position.x = (float)x + (float)(letters[num][i][0] * s) / 10;
			vertexLetter[i].position.y = (float)y + (float)(letters[num][i][1] * s) / 10;
			vertexLetter[i].color = sf::Color::White;
		}

		window.draw(vertexLetter);
	}
}

void LineText::drawString(sf::RenderWindow & window, std::string t, int x, int y, int s)
{
	int xPos = x;
	int yPos = y;
	for (unsigned int i = 0; i < t.size(); i++)
	{
		drawLetter(window, &t[i] , xPos, yPos, s);
		xPos += int(s*1.2f);
	}
}

int LineText::getLetterWidth(int s)
{
	return int(s*1.2f);
}

