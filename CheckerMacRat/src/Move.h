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

	std::string getUCI()
	{
		std::string uci;
		Square from = getFrom();
		Square to = getTo();
		MoveFlags flags = getFlags();
		uci.push_back(label(colOf(from)));
		uci.push_back(label(rowOf(from)));
		uci.push_back(label(colOf(to)));
		uci.push_back(label(rowOf(to)));

		if(flags & PROMOTION_FLAG)
		{
			if(		flags == PROMOTION_QUEEN)	uci.push_back('q');
			else if(flags == PROMOTION_ROOK)	uci.push_back('r');
			else if(flags == PROMOTION_BISHOP)	uci.push_back('b');
			else if(flags == PROMOTION_KNIGHT)	uci.push_back('n');
		}

		return uci;
	}

private:
	uint16_t m_data;
};