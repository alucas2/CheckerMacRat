#include "ConsoleChess.h"

std::string ConsoleChess::boardToString(const GameState& board)
{
	std::string resultat;

	for(Row row = ROW_8; row >= ROW_1; row = static_cast<Row>(row - 1))
	{
		for(Col col = COL_A; col <= COL_H; col = static_cast<Col>(col + 1))
		{
			Square sq = static_cast<Square>(8*row + col);
			Piece piece = board.getPieceAt(sq);
			if(piece != NULL_PIECE)
			{
				char label;
				if(		piece == PAWN)		label = 'P';
				else if(piece == KNIGHT)	label = 'N';
				else if(piece == BISHOP)	label = 'B';
				else if(piece == ROOK)		label = 'R';
				else if(piece == QUEEN)		label = 'Q';
				else if(piece == KING)		label = 'K';
				if(board.getColorAt(sq) == BLACK)	label += 32;
				resultat.push_back(label);
			}
			else
			{
				resultat.push_back(' ');
			}
			resultat.push_back('.');
		}
		if(row != ROW_1)
			resultat.push_back('\n');
	}

	return resultat;
}

bool ConsoleChess::play(std::string fen)
{
	std::stack<GameState> gameStack;
	if(fen == "")
		gameStack.push(GameState());
	else
		gameStack.push(GameState(fen));

	while(true)
	{
		std::cout << "----------------" << std::endl;
		std::cout << boardToString(gameStack.top()) << std::endl;
		std::cout << "----------------" << std::endl;
		
		const GameState& top = gameStack.top();

		std::unordered_map<std::string, Move> legalMoves;
		for(Move m : MoveGenerator::getPseudoLegalMoves(top))
		{
			if(top.performMove(m).second)
				legalMoves.emplace(m.getNotation(), m);
		}

		if(top.getSideToMove() == WHITE)
			std::cout << top.getFullMoveCount() << ". WHITE turn." << std::endl;
		else
			std::cout << top.getFullMoveCount() << ". BLACK turn." << std::endl;

		std::cout << legalMoves.size() << " moves generated" << std::endl;
		for(auto p : legalMoves)
			std::cout << p.first << ",";
		std::cout << std::endl;

		std::string input;
		std::cin >> input;

		if(legalMoves.find(input) != legalMoves.end())
		{
			gameStack.push(top.performMove(legalMoves[input]).first);
		}
		else
		{
			if(input == "exit")
				return false;
			else if(input == "0000")
				gameStack.push(top.performNullMove());
			else if(input == "undo" && gameStack.size() > 1)
				gameStack.pop();
			else
				std::cout << "Invalid move" << std::endl;
		}
	}

	return true;
}