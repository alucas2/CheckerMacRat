#pragma once

#include "GameState.h"
#include "MoveGenerator.h"
#include <ostream>
#include <fstream>
#include <iostream>

class Test
{
public:
	static int perft(const GameState& from, int d);
	static bool testPerft(std::string filename);
};