#include "stdafx.h"
#include "HighScore.h"
#include <iostream>
#include <fstream>

HighScore::HighScore()
{
	// At instantiation load from plain text file
	std::ifstream InputFile(filename);
	if (InputFile.is_open())
	{
		std::string name;
		int oldScore;
		for (int i = 0; i < 10; i++)
		{
			InputFile >> name;
			InputFile >> oldScore;
			score[i].first = name;
			score[i].second = oldScore;
		}
		InputFile.close();
	}
	else
		HighScore::saveScores();
}


HighScore::~HighScore()
{
}

std::string HighScore::getName(int i)
{
	return score[i].first;
}

int HighScore::getHigh()
{
	return getScore(0);
}

int HighScore::getLow()
{
	return getScore(9);
}

int HighScore::getScore(int i)
{
	return score[i].second;
}

void HighScore::setScore(std::string name, int newScore)
{
	if (newScore < score[9].second)
		return;

	if (newScore > score[0].second)
	{
		moveDown(0);
		score[0].second = newScore;
		score[0].first = name;
		saveScores();
		return;
	}

	for (int i = 9; i>0; i--)
	{
		if (newScore > score[i].second && newScore <= score[i - 1].second)
		{
			// make space for new entry
			moveDown(i);
			score[i].second = newScore;
			score[i].first = name;
			saveScores();
			return;
		}
	}
}

void HighScore::moveDown(int i)
{
	for (int n = 8; n >= i; n--)
		score[n + 1] = score[n];
}

void HighScore::saveScores()
{
	std::ofstream OutputFile(filename); //creates it if it doesn't exist
	for (int n = 0; n < 10; n++)
	{
		OutputFile << score[n].first << " ";
		OutputFile << score[n].second << " ";
	}
	OutputFile.close();
}