#include "Tests.h"

int Test::perft(const GameState& from, int d)
{
	if(d == 0)
		return 1;

	int result = 0;

	for(Move m : MoveGenerator::getPseudoLegalMoves(from))
	{
		std::pair<GameState, bool> next = from.performMove(m);
		if(next.second)
			result += perft(next.first, d-1);
	}

	return result;
}

bool Test::testPerft(std::string filename)
{
	std::ifstream file(filename);
	if(!file)
	{
		std::cout << "Results not found" << std::endl;
		return false;
	}

	std::string line;
	int i = 1;

	while(std::getline(file, line))
	{
		//Parse la ligne du fichier
		size_t split = line.find(';');
		std::string fen = line.substr(0, split);
		line.erase(0, split+1);
		std::vector<std::pair<int, int>> perftTable;
		while(!line.empty())
		{
			split = line.find(';');
			if(split == std::string::npos) split = line.size();
			std::string sample = line.substr(0, split);
			line.erase(0, split+1);
			perftTable.emplace_back(
				std::stoi(sample.substr(1, sample.find(' ')-1)),
				std::stoi(sample.substr(sample.find(' ')))
			);
		}

		//Réalise le test
		GameState game(fen);
		std::cout << "Testing position " << "#" << i++ << ": " << fen << std::endl;
		for(std::pair<int, int> p : perftTable)
		{
			int depth = p.first;
			int expected = p.second;
			std::cout << "Calculating perft(" << depth << ")... ";
			int result = perft(game, depth);
			std::cout << "Found " << result;
			if(result == expected)
			{
				std::cout << ", as expected." << std::endl;
			}
			else
			{
				std::cout << ", expected " << expected << ", TEST FAILED" << std::endl;
				return false;
			}
		}
	}
	
	std::cout << "All perft tests passed." << std::endl;
	return true;
}
