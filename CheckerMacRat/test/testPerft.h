#pragma once

#include "../src/GameState.h"
#include "../src/MoveGenerator.h"
#include <fstream>
#include <iostream>
#include <chrono>

int perft(const GameState& from, int d);
bool testPerft();
