#pragma once

#include "Enums.h"
#include "GameState.h"

class Evaluator
{
public:
	//Fonction d'évaluation de la position.
	//Positif -> en faveur des blancs, Négatif -> en faveur des noirs
	static int evaluate(const GameState& state)
	{
		int score = 0;
		score += (countSquares(state.getPieces<WHITE, KING>()) - countSquares(state.getPieces<BLACK, KING>())) * 20000;
		score += (countSquares(state.getPieces<WHITE, QUEEN>()) - countSquares(state.getPieces<BLACK, QUEEN>())) * 900;
		score += (countSquares(state.getPieces<WHITE, ROOK>()) - countSquares(state.getPieces<BLACK, ROOK>())) * 500;
		score += (countSquares(state.getPieces<WHITE, BISHOP>()) - countSquares(state.getPieces<BLACK, BISHOP>())) * 330;
		score += (countSquares(state.getPieces<WHITE, KNIGHT>()) - countSquares(state.getPieces<BLACK, KNIGHT>())) * 320;
		score += (countSquares(state.getPieces<WHITE, PAWN>()) - countSquares(state.getPieces<BLACK, PAWN>())) * 100;

		return score;
	}
};