#include "FilesManager.h"

// getting 10, the highest records from file with highscores //
std::string FilesManager::GetHighscores() {
	std::ifstream reader("resources/hs.txt");
	Score scores[10];
	std::string date;
	int score;
	for (int i = 0; i < 10; ++i) {
		reader >> date >> score;
		scores[i] = Score() = { date, score };
	}
	std::sort(scores, scores + 10, [](Score& r1, Score r2) {return r1.score > r2.score; });

	std::string hs;
	for (int i = 0; i < 10; ++i)
		hs += std::to_string(i + 1) + ". " + scores[i].date + " " + std::to_string(scores[i].score) + '\n';

	return hs;
}

// setting new score if current score is higher than the last one from highscores //
void FilesManager::SetScore(int score) {
	std::ifstream reader("resources/hs.txt");
	Score scores[10];
	std::string date;
	int scoreFile;
	for (int i = 0; i < 10; ++i) {
		reader >> date >> scoreFile;
		scores[i] = Score() = { date, scoreFile };
	}

	std::sort(scores, scores + 10, [](Score& r1, Score r2) {return r1.score > r2.score; });
	if (scores[9].score < score) { // setting new record - current date and the score passed as an argument
  		std::time_t t = std::time(0);
	#pragma warning(suppress : 4996) // vs stop insulting me pls
		std::tm* now = std::localtime(&t);
		scores[9] = Score() = { std::to_string(now->tm_year + 1900) + '/' + std::to_string(now->tm_mon + 1) + '/' + std::to_string(now->tm_mday), score };
	}

	std::sort(scores, scores + 10, [](Score& r1, Score r2) {return r1.score > r2.score; });
	std::fstream rewrite("resources/hs.txt", std::ios::out);
	for (int i = 0; i < 10; ++i)
		rewrite << scores[i].date << ' ' << scores[i].score << '\n';
	rewrite.close();
}

// getting long messages from files //
std::string FilesManager::GetFromFile(std::string filename, int separator) {
	std::string text, line;
	std::ifstream reader("resources/" + filename);
	
	while (std::getline(reader, line))
		text += line + (separator == -1 ? "\n" : "");
	if (separator != -1)
		for (int i = 0; i < text.size(); i += separator)
			text = text.substr(0, i) + '\n' + text.substr(i, text.size());

	return text;
}
