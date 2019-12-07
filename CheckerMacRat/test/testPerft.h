#pragma once

#include "../src/GameState.h"
#include "../src/MoveGenerator.h"
#include <fstream>
#include <iostream>


int perft(const GameState& from, int d);
bool testPerft();
