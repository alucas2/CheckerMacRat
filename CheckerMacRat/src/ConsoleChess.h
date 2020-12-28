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
	//Renvoie une chaine de caract�re repr�sentant l'�chiquier sous forme de tableau
	static std::string boardToString(const GameState& board);

	//D�marre une partie d'�checs dans la console
	static bool play(std::string fen = "");
};
