#pragma once
#include <iostream>
#include <string>
#include <fstream>

class SamplesReader
{
	std::string files[5]{ "copperhead_eater.txt", "gosper_glider_gun.txt", "schick_engine.txt", "swan.txt", "weekender.txt" };
public:
	SamplesReader();

	void Read(int index, bool board[50][50]);
};

