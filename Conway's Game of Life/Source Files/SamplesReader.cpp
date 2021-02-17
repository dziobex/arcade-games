#include "SamplesReader.h"

// setting available filenames //
SamplesReader::SamplesReader() {
	files[0] = "copperhead_eater.txt";
	files[1] = "gosper_glider_gun.txt";
	files[2] = "schick_engine.txt";
	files[3] = "swan.txt";
	files[4] = "weekender.txt";
}

// getting pattern from file and print it on the board //
void SamplesReader::Read(int index, bool board[50][50]) {
	std::ifstream reader("samples/" + files[index]);
	std::string line;
	int row = 0;
	while (std::getline(reader, line)) {
		for (int col = 0; col < 50; ++col)
			board[col][row] = line[col] == 'X';
		++row;
	}
}
