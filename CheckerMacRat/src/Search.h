#pragma once

#include "Evaluator.h"
#include "MoveGenerator.h"
#include <algorithm>
#include <limits.h>


class Search
{
private:
	template<bool maximize>
	static int minmax(const GameState& state, int depth, int alpha, int beta)
	{
		if(depth == 0)
			return Evaluator::evaluateState(state);

		int bestValue = maximize ? INT_MIN + 50 - depth : INT_MAX - 50 + depth;
		// ^^^^ if some moves lead to a win, we give an advantage to moves that lead to an early win

		std::vector<Move> moves = MoveGenerator::getPseudoLegalMoves(state);

		for(Move& m : moves)
			m.setValue(Evaluator::evaluateMove(state, m));

		std::vector<Move>::iterator moveIt = moves.end();
		while(moveIt != moves.begin())
		{
			moveIt = popMax(moves.begin(), moveIt);
			GameState next = state.performMove(*moveIt);
			if(!next.isLegal()) continue;

			if constexpr(maximize)
			{
				bestValue = std::max(bestValue, minmax<false>(next, depth-1, alpha, beta));
				alpha = std::max(alpha, bestValue);
			}
			else
			{
				bestValue = std::min(bestValue, minmax<true>(next, depth-1, alpha, beta));
				beta = std::min(beta, bestValue);
			}

			if(alpha >= beta) break;
		}

		return bestValue;
	}

	template<bool maximize>
	static Move searchArgminmax(const GameState& state, int depth)
	{
		Move foundArg;
		int bestValue = maximize ? INT_MIN : INT_MAX;
		std::vector<Move> moves = MoveGenerator::getPseudoLegalMoves(state);

		for(Move& m : moves)
			m.setValue(Evaluator::evaluateMove(state, m));

		std::vector<Move>::iterator moveIt = moves.end();
		while(moveIt != moves.begin())
		{
			moveIt = popMax(moves.begin(), moveIt);
			GameState next = state.performMove(*moveIt);
			if(!next.isLegal()) continue;

			if constexpr(maximize)
			{
				int test = minmax<false>(next, depth-1, INT_MIN, INT_MAX);
				if(test > bestValue)
				{
					foundArg = *moveIt;
					bestValue = test;
				}
			}
			else
			{
				int test = minmax<true>(next, depth-1, INT_MIN, INT_MAX);
				if(test < bestValue)
				{
					foundArg = *moveIt;
					bestValue = test;
				}
			}
		}

		return foundArg;
	}

	static std::vector<Move>::iterator popMax(std::vector<Move>::iterator begin, std::vector<Move>::iterator end);

public:
	static Move search(const GameState& state, int depth);
};