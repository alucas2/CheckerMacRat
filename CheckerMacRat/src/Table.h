#pragma once

#include "Enums.h"
#include "Bitboard.h"
#include <array>

//	Une table de Bitboards index�e par une case
//	La table est g�n�r�e � la compilation � l'aide de l'argument de mod�le Generator
//	La table g�n�r�e fait une taille de NUM_SQUARE*sizeof(Bitboard) = 512 octets
//	Le foncteur Generator doit poss�der une m�thode d�finie comme suit: static constexpr Bitboard makeEntry(Square);
template<class Generator>
class Table
{
private:
	static constexpr std::array<Bitboard, NUM_SQUARE> makeTable()
	{
		std::array<Bitboard, NUM_SQUARE> result{};

		for(Square sq = SQUARE_A1; sq < NUM_SQUARE; sq = static_cast<Square>(sq + 1))
		{
			result[sq] = Generator::makeEntry(sq);
		}

		return result;
	}

public:
	static Bitboard get(Square from)
	{
		static constexpr std::array<Bitboard, NUM_SQUARE> table = makeTable();

		return table[from];
	}
};