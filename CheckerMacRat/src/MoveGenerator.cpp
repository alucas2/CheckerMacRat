#include "MoveGenerator.h"

std::vector<Move> MoveGenerator::getPseudoLegalMoves(const GameState& from)
{
	if(from.getSideToMove() == WHITE)
		return getPseudoLegalMoves<WHITE>(from);
	else
		return getPseudoLegalMoves<BLACK>(from);
}
