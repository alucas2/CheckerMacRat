#include "ConsoleChess.h"
#include "../test/testPerft.h"

int main(int argc, char** argv)
{
	std::string fen;
	std::cout << "Enter a valid FEN position or type 's' to start from the initial position" << std::endl;
	std::cin >> fen;

	if (fen == "s")
	{
		ConsoleChess::play();
	}
	else
	{
		ConsoleChess::play(fen);
	}

	//testPerft();

	return 0;
}