#include "testPerft.h"

int perft(const GameState& from, int d)
{
	if(d == 0)
		return 1;

	int result = 0;
	for(Move move : MoveGenerator::getPseudoLegalMoves(from))
	{
		GameState next = from.performMove(move);
		if(next.isLegal()) result += perft(next, d-1);
	}
	return result;
}

bool testPerft()
{
	std::ifstream file("test/perft_results.txt");
	if(!file)
	{
		std::cout << "Test table not found" << std::endl;
		return false;
	}

	std::string line;
	int i = 1;
	auto t0 = std::chrono::high_resolution_clock::now();

	while(std::getline(file, line))
	{
		if(line.empty() || line.front() == '#')
			continue;

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
			std::cout << "Calculating perft(" << depth << "), expecting " << p.second << ", ";
			int result = perft(game, depth);
			std::cout << "found " << result << std::endl;
			if(result != expected)
			{
				std::cout << "TEST FAILED" << std::endl;
				return false;
			}
		}
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = t1 - t0;
	std::cout << "All perft tests passed." << std::endl;
	std::cout << "Time taken: " << elapsed.count() << " seconds" << std::endl;
	return true;
}
