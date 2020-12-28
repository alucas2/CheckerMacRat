#pragma once

#include "Enums.h"
#include "GameState.h"

class Evaluator
{
private:
	static int s_numEvaluations;

public:
	static const int s_pieceValues[NUM_PIECE];

	//Evaluates a positions with an heuristic.
	//Positive -> in favor of the white
	//Negative -> in favor of the black
	static int evaluateState(const GameState& state);

	//Evaluates a move with an heuristic
	//Positive -> good move
	//Negative -> bad move
	static int evaluateMove(const GameState& state, Move m);

	//Get the number of evaluations and resets the counter
	static int resetNumEvaluations();
};