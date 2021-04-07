#pragma once
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>

class FileManager
{
	struct Score {
		std::string date;
		int score;
	};
public:
	std::string GetHighscores();
	void SetScore(int score);
	std::string GetFromFile(std::string filename, int separator);
};