#pragma once

#include "Enums.h"
#include <stdint.h>
#include <string>

//La classe Move code un mouvement sur 16 bits
//bits 0 à 5: case de départ
//bits 5 à 11: case d'arrivée
//bits 12 à 15: drapeaux, voir la définition de MoveFlags
class Move
{
public:
	Move(Square from = SQUARE_A1, Square to = SQUARE_A1, MoveFlags flags = static_cast<MoveFlags>(0)):
		m_data(from | to << 6 | flags << 12)
	{}

	Square getFrom() const
	{
		return static_cast<Square>(m_data & 63);
	}

	Square getTo() const
	{
		return static_cast<Square>(m_data >> 6 & 63);
	}

	MoveFlags getFlags() const
	{
		return static_cast<MoveFlags>(m_data >> 12 & 15);
	}

	std::string getNotation()
	{
		std::string notation;
		Square from = getFrom();
		Square to = getTo();
		MoveFlags flags = getFlags();
		notation.push_back(label(colOf(from)));
		notation.push_back(label(rowOf(from)));
		notation.push_back(label(colOf(to)));
		notation.push_back(label(rowOf(to)));

		if(flags & PROMOTION_FLAG)
		{
			if(		flags == PROMOTION_QUEEN)	notation.push_back('q');
			else if(flags == PROMOTION_ROOK)	notation.push_back('r');
			else if(flags == PROMOTION_BISHOP)	notation.push_back('b');
			else if(flags == PROMOTION_KNIGHT)	notation.push_back('n');
		}

		return notation;
	}

private:
	uint16_t m_data;
};