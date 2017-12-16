#pragma once
#include "stdafx.h"
#include <string>
#include <iostream>

class HighScore
{
public:
	HighScore();
	~HighScore();
	std::string getName(int i);
	int getHigh();
	int getLow();
	int getScore(int i);
	void setScore(std::string name, int score);

private:
	void moveDown(int i);
	void saveScores();
	std::string filename = "highscores.txt";
	std::pair<std::string, int> score[10] = { {"AAA", 1000}, {"BBB", 900}, {"CCC", 800}, {"DDD", 700}, {"EEE", 600},
											  {"FFF", 500}, {"GGG", 400}, {"HHH", 300}, {"III", 200}, {"JJJ", 100} };

};

