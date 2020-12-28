#pragma once

#include "Enums.h"
#include "Move.h"
#include "MoveGenerator.h"
#include "GameState.h"
#include "Search.h"
#include <stack>
#include <unordered_map>
#include <iostream>
#include <string>

class ConsoleChess
{
public:
	//Renvoie une chaine de caractère représentant l'échiquier sous forme de tableau
	static std::string boardToString(const GameState& board);

	//Démarre une partie d'échecs dans la console
	static bool play(std::string fen = "");
};
