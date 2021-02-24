#pragma once
#include <fstream>
#include <algorithm>
#include <ctime>
#include <string>

class FilesManager
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

