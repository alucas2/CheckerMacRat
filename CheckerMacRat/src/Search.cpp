#include "Search.h"


std::vector<Move>::iterator Search::popMax(std::vector<Move>::iterator begin, std::vector<Move>::iterator end)
{
	for(auto it = begin; it != end-1; ++it)
	{
		if(*(it+1) < *it)
			std::iter_swap(it+1, it);
	}
	return end-1;
}

Move Search::search(const GameState& state, int depth)
{
	if(state.getSideToMove() == WHITE)
		return searchArgminmax<true>(state, depth);
	else
		return searchArgminmax<false>(state, depth);
}
