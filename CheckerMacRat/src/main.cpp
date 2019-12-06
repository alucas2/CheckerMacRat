#include "ConsoleChess.h"
#include "Tests.h"

int main(int argc, char** argv)
{
	ConsoleChess::play();

	Test::testPerft("perft/perft_results.txt");

	system("PAUSE");
	return 0;
}