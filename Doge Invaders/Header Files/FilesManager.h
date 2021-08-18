#pragma once
#include <fstream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <string>

class FilesManager {
	static std::vector<std::string> pauseWindowQuotes;

public:
	static void AddScore(int score);
	static std::string GetHighscores();

	static void InitializeQuotes(int space);
	static std::string GetRandomQuote();
};

