#pragma once

#include "Enums.h"
#include "Bitboard.h"
#include "Table.h"

//Foncteur qui génère les attaques du roi
struct KingAttacksGen
{
	static constexpr Bitboard makeEntry(Square from)
	{
		Bitboard fromBB = squareMask(from);
		return shift<NORTH>(fromBB) | shift<SOUTH>(fromBB)
			| shift<EAST>(fromBB) | shift<WEST>(fromBB)
			| shift<NORTHEAST>(fromBB) | shift<NORTHWEST>(fromBB)
			| shift<SOUTHEAST>(fromBB) | shift<SOUTHWEST>(fromBB);
	}
};

//Foncteur qui génère les attaques du cavalier
struct KnightAttacksGen
{
	static constexpr Bitboard makeEntry(Square from)
	{
		Bitboard fromBB = squareMask(from);
		return shift<NORTHEAST>(shift<NORTH>(fromBB)) | shift<NORTHWEST>(shift<NORTH>(fromBB))
			| shift<SOUTHEAST>(shift<SOUTH>(fromBB)) | shift<SOUTHWEST>(shift<SOUTH>(fromBB))
			| shift<NORTHEAST>(shift<EAST>(fromBB)) | shift<SOUTHEAST>(shift<EAST>(fromBB))
			| shift<NORTHWEST>(shift<WEST>(fromBB)) | shift<SOUTHWEST>(shift<WEST>(fromBB));
	}
};

//Modèle de foncteurs qui génèrent un rayon dans une direction
template<Direction direction>
struct RayGen
{
	static constexpr Bitboard rec_makeRay(Bitboard from)
	{
		if(!from)
			return 0ULL;

		Bitboard shifted = shift<direction>(from);
		return shifted | rec_makeRay(shifted);
	}

	static constexpr Bitboard makeEntry(Square from)
	{
		return rec_makeRay(squareMask(from));
	}
};

//Modèle de foncteurs qui génèrent les captures des pions d'une couleur
template<Color color>
struct PawnCapturesGen
{
	static constexpr Bitboard makeEntry(Square from)
	{
		Bitboard fromBB = squareMask(from);

		if constexpr(color == WHITE)
		{
			return shift<NORTHEAST>(fromBB) | shift<NORTHWEST>(fromBB);
		}
		else
		{
			return shift<SOUTHEAST>(fromBB) | shift<SOUTHWEST>(fromBB);
		}
	}
};

//Renvoie le bitboard des attaques du roi
inline Bitboard kingAttacks(Square from)
{
	return Table<KingAttacksGen>::get(from);
}

//Renvoie le bitboard des attaques du cavalier
inline Bitboard knightAttacks(Square from)
{
	return Table<KnightAttacksGen>::get(from);
}

//Opération slide: lance un rayon dans une direction et s'arrête au premier obstacle
template<Direction direction>
Bitboard slide(Square from, Bitboard obstacles)
{
	Bitboard ray = Table<RayGen<direction>>::get(from);

	if constexpr(direction > 0)
	{
		return ray ^ Table<RayGen<direction>>::get(firstSquare(ray & obstacles | 0x8000000000000000ULL));
	}
	else
	{
		return ray ^ Table<RayGen<direction>>::get(lastSquare(ray & obstacles | 1ULL));
	}
}

//Renvoie le bitboard des attaques de la tour
inline Bitboard rookAttacks(Square from, Bitboard obstacles)
{
	return slide<NORTH>(from, obstacles)
		| slide<SOUTH>(from, obstacles)
		| slide<EAST>(from, obstacles)
		| slide<WEST>(from, obstacles);
}

//Renvoie le bitboard des attaques du fou
inline Bitboard bishopAttacks(Square from, Bitboard obstacles)
{
	return slide<NORTHEAST>(from, obstacles)
		| slide<NORTHWEST>(from, obstacles)
		| slide<SOUTHEAST>(from, obstacles)
		| slide<SOUTHWEST>(from, obstacles);
}

//Renvoie le bitboard des attaques du pion hors prise en passant
template<Color color>
Bitboard pawnAttacks(Square from, Bitboard opponents)
{
	return Table<PawnCapturesGen<color>>::get(from) & opponents;
}

//Renvoie le bitboard des avancées du pion. Suppose que le pion n'est pas sur la dernière rangée
template<Color color>
Bitboard pawnPushes(Square from, Bitboard obstacles)
{
	constexpr Direction forward = color == WHITE ? NORTH : SOUTH;
	constexpr Rank startRow = color == WHITE ? RANK_2 : RANK_7;

	Rank fromRow = rankOf(from);

	Bitboard result = shift<forward>(squareMask(from)) & ~obstacles;
	if(fromRow == startRow && result)
	{
		result |= shift<forward>(result) & ~obstacles;
	}
	return result;
}