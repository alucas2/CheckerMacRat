#pragma once

#include "Evaluator.h"
#include "MoveGenerator.h"
#include <algorithm>
#include <limits.h>

class Search
{
private:
	static int minmax(const GameState& state, int depth)
	{
		if(depth == 0)
			return Evaluator::evaluate(state);

		int minValue = INT_MAX;
		for(Move m : MoveGenerator::getPseudoLegalMoves(state))
		{
			std::pair<GameState, bool> p = state.performMove(m);
			if(p.second) minValue = std::min(minValue, maxmin(p.first, depth-1));
		}
		return minValue;
	}

	static int maxmin(const GameState& state, int depth)
	{
		if(depth == 0)
			return Evaluator::evaluate(state);

		int maxValue = INT_MIN;
		for(Move m : MoveGenerator::getPseudoLegalMoves(state))
		{
			std::pair<GameState, bool> p = state.performMove(m);
			if(p.second) maxValue = std::max(maxValue, minmax(p.first, depth-1));
		}
		return maxValue;
	}

public:
	static Move search(const GameState& state, int depth)
	{
		Color movingSide = state.getSideToMove();
		Move bestMove;
		int bestValue = movingSide == WHITE ? INT_MIN : INT_MAX;
		for(Move m : MoveGenerator::getPseudoLegalMoves(state))
		{
			std::pair<GameState, bool> p = state.performMove(m);
			if(p.second)
			{
				if(movingSide == WHITE)
				{
					int test = minmax(p.first, depth);
					if(test > bestValue)
					{
						bestValue = test;
						bestMove = m;
					}
				}
				else
				{
					int test = maxmin(p.first, depth);
					if(test < bestValue)
					{
						bestValue = test;
						bestMove = m;
					}
				}
			}
		}
		return bestMove;
	}
};