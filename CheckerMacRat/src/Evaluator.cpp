#include "Evaluator.h"
#include "MoveGenerator.h"
#include <iostream>

int Evaluator::s_numEvaluations = 0;

const int Evaluator::s_pieceValues[NUM_PIECE] ={
	10,
	32,
	33,
	50,
	90,
	0//we don't care about the king because there is always one on each side
};

int Evaluator::evaluateState(const GameState& state)
{
	s_numEvaluations++;
	
	int score = 0;
	score += (countSquares(state.getPieces<WHITE, QUEEN>())		- countSquares(state.getPieces<BLACK, QUEEN>()))	* s_pieceValues[QUEEN];
	score += (countSquares(state.getPieces<WHITE, ROOK>())		- countSquares(state.getPieces<BLACK, ROOK>()))		* s_pieceValues[ROOK];
	score += (countSquares(state.getPieces<WHITE, BISHOP>())	- countSquares(state.getPieces<BLACK, BISHOP>()))	* s_pieceValues[BISHOP];
	score += (countSquares(state.getPieces<WHITE, KNIGHT>())	- countSquares(state.getPieces<BLACK, KNIGHT>()))	* s_pieceValues[KNIGHT];
	score += (countSquares(state.getPieces<WHITE, PAWN>())		- countSquares(state.getPieces<BLACK, PAWN>()))		* s_pieceValues[PAWN];

	return score;
}

int Evaluator::evaluateMove(const GameState& state, Move m)
{
	Square from = m.getFrom();
	Square to = m.getTo();

	//Most Valuable Victim - Least Valuable Aggressor
	if(state.getPieceAt(to) != NULL_PIECE)
		return s_pieceValues[state.getPieceAt(to)] - s_pieceValues[state.getPieceAt(from)];

	return 0;
}

int Evaluator::resetNumEvaluations()
{
	int temp = s_numEvaluations;
	s_numEvaluations = 0;
	return temp;
}
