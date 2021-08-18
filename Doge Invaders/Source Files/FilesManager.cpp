#include "FilesManager.h"

std::vector<std::string> FilesManager::pauseWindowQuotes;

void FilesManager::AddScore(int score) {
	// get all records from the current highscore
	std::ifstream reader("resources/data/highscores.txt");

	std::vector<std::pair<std::string, int>> hs;
	std::string date; int fileScore;

	for (int i = 0; i < 10; i++) {
		reader >> date >> fileScore;
		hs.push_back(std::make_pair(date, fileScore));
	}
	reader.close();

	// sort highscores from the lowest to the greatest num
	std::sort(hs.begin(), hs.end(), [](std::pair<std::string, int>& sc1, std::pair<std::string, int>& sc2) { return sc1.second > sc2.second; });

	// check if the score can be placed in the hs
	if (score > hs[9].second) {
#pragma warning(disable : 4996)
		auto time = std::time(0);
		auto now = std::localtime(&time);

		std::string currentDate = std::to_string(now->tm_year + 1900) + '/' + std::to_string(now->tm_mon + 1) + '/' + std::to_string(now->tm_mday);
		hs[9] = std::make_pair(currentDate, score);
	}

	// sort again
	std::sort(hs.begin(), hs.end(), [](std::pair<std::string, int>& sc1, std::pair<std::string, int>& sc2) { return sc1.second > sc2.second; });

	// save new highscore list
	std::ofstream writer("resources/data/highscores.txt");
	for (int i = 0; i < 10; i++)
		writer << hs[i].first << ' ' << hs[i].second << '\n';
	writer.close();
}

std::string FilesManager::GetHighscores() {
	FilesManager::AddScore(0);
	std::string hs = "", date = "";
	int score;

	// get the currnt highscores
	std::ifstream reader("resources/data/highscores.txt");
	for (int i = 0; i < 10; i++) {
		reader >> date >> score;
		hs += std::to_string(i + 1) + ". " + date + ' ' + std::to_string(score) + '\n';
	}
	reader.close();

	return hs;
}

// initialize quotes
// those quotes will be displayed in the pause window
void FilesManager::InitializeQuotes(int space) {
	std::ifstream reader("resources/data/deep_quotes.txt");
	std::string quote = "";

	while (std::getline(reader, quote)) {
		for (int i = 0; i < quote.size(); i += space)
			if (i != 0 && i != quote.size() - 1)
			quote = quote.substr(0, i + 1) + '\n' + quote.substr(i + 1, quote.size() - i - 1);
		pauseWindowQuotes.push_back("\"" + quote + "\"");
	}

	reader.close();
}

// get random quote from the quotes
std::string FilesManager::GetRandomQuote() {
	return pauseWindowQuotes[rand() % pauseWindowQuotes.size()];
}
