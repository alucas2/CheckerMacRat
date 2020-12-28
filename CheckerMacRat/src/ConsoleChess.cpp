#include "ConsoleChess.h"

std::string ConsoleChess::boardToString(const GameState& board)
{
	std::string resultat;
	
	resultat += "    a b c d e f g h\n    ---------------\n";
	for(Rank row = RANK_8; row >= RANK_1; row = static_cast<Rank>(row - 1))
	{
		resultat += label(row);
		resultat += " | ";
		for(File col = FILE_A; col <= FILE_H; col = static_cast<File>(col + 1))
		{
			Square sq = static_cast<Square>(8*row + col);
			Piece piece = board.getPieceAt(sq);
			if(piece != NULL_PIECE)
			{
				if (board.getColorAt(sq) == WHITE)
				{
					if(		piece == PAWN)		resultat += "\x1b[36mP\x1b[0m";
					else if(piece == KNIGHT)	resultat += "\x1b[36mN\x1b[0m";
					else if(piece == BISHOP)	resultat += "\x1b[36mB\x1b[0m";
					else if(piece == ROOK)		resultat += "\x1b[36mR\x1b[0m";
					else if(piece == QUEEN)		resultat += "\x1b[36mQ\x1b[0m";
					else if(piece == KING)		resultat += "\x1b[36mK\x1b[0m";
				}
				else
				{
					if (piece == PAWN)			resultat += "\x1b[35mP\x1b[0m";
					else if (piece == KNIGHT)	resultat += "\x1b[35mN\x1b[0m";
					else if (piece == BISHOP)	resultat += "\x1b[35mB\x1b[0m";
					else if (piece == ROOK)		resultat += "\x1b[35mR\x1b[0m";
					else if (piece == QUEEN)	resultat += "\x1b[35mQ\x1b[0m";
					else if (piece == KING)		resultat += "\x1b[35mK\x1b[0m";
				}
			}
			else
			{
				resultat.push_back('.');
			}
			resultat.push_back(' ');
		}
		resultat += "| ";
		resultat += label(row);
		resultat += "\n";
	}
	resultat += "    ---------------\n    a b c d e f g h\n";

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
		std::cout << boardToString(gameStack.top()) << std::endl;
		
		const GameState& top = gameStack.top();

		std::unordered_map<std::string, Move> legalMoves;
		for(Move move : MoveGenerator::getPseudoLegalMoves(top))
		{
			if(top.performMove(move).isLegal()) 
				legalMoves.emplace(move.getNotation(), move);
		}

		if(legalMoves.size() == 0) {
			if (top.getSideToMove() == WHITE)
				std::cout << "(#" << top.getFullMoveCount() << "). Checkmate! \x1b[35mBLACK\x1b[0m won." << std::endl;
			else
				std::cout << "(#" << top.getFullMoveCount() << "). Checkmate! \x1b[36mWHITE\x1b[0m won." << std::endl;

			std::cout << "Enter a command. Commands are 's', 'list', 'pass', 'undo', 'exit', 'fen'" << std::endl;
		}
		else
		{
			if(top.getSideToMove() == WHITE)
				std::cout << "(#" << top.getFullMoveCount() << "). It is \x1b[36mWHITE\x1b[0m's turn." << std::endl;
			else
				std::cout << "(#" << top.getFullMoveCount() << "). It is \x1b[35mBLACK\x1b[0m's turn." << std::endl;

			std::cout << "Enter your move or a command. Commands are 's', 'list', 'pass', 'undo', 'exit', 'fen'" << std::endl;
		}


		while(true) {
			std::string input;
			std::cin >> input;
			if(legalMoves.find(input) != legalMoves.end())
			{
				gameStack.push(top.performMove(legalMoves[input]));
				break;
			}
			else
			{
				if(input == "s")
				{
					if (legalMoves.size() != 0)
					{
						Move bestMove = Search::search(top, 5);//hardcode the search depth here
						gameStack.push(top.performMove(bestMove));
						std::cout << "The computer plays " << bestMove.getNotation() << std::endl;
						std::cout << "(" << Evaluator::resetNumEvaluations() << " nodes evaluated)" << std::endl;
						break;
					}
					else
					{
						std::cout << "The game is done, the computer cannot help you here." << std::endl;
					}
				}
				else if (input == "list")
				{
					std::cout << "You can do " << legalMoves.size() << " moves :" << std::endl;
					for (auto p : legalMoves)
						std::cout << p.first << ",";
					std::cout << std::endl;
				}
				else if (input == "exit")
				{
					return false;
				}
				else if (input == "pass")
				{
					if (top.performNullMove().isLegal()) {
						gameStack.push(top.performNullMove());
						break;
					}
					else
					{
						std::cout << "You cannot pass here" << std::endl;
					}
				}
				else if (input == "undo")
				{
					if (gameStack.size() > 1)
					{
						gameStack.pop();
						break;
					}
					else
					{
						std::cout << "You cannot undo here" << std::endl;
					}
				}
				else if (input == "fen")
				{
					std::cout << "FEN: " << top.getNotation() << std::endl;
				}
				else
				{
					std::cout << input << " is not a valid move or command" << std::endl;
				}
			}
		}
	}

	return true;
}