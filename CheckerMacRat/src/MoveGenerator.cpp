#include "MoveGenerator.h"

std::vector<Move> MoveGenerator::getPseudoLegalMoves(const GameState & from)
{
	if(from.getSideToMove() == WHITE)
		return getPseudoLegalMoves<WHITE>(from);
	else
		return getPseudoLegalMoves<BLACK>(from);
}


std::vector<Move> MoveGenerator::getLegalMoves(const GameState & from)
{
	std::vector<Move> pseudoLegal = getPseudoLegalMoves(from);

	auto illegalPred = [&](Move m){
		return !from.performMove(m).second;
	};

	return std::vector<Move>(pseudoLegal.begin(), std::remove_if(pseudoLegal.begin(), pseudoLegal.end(), illegalPred));
}
