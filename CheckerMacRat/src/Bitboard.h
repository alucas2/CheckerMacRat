#pragma once

#include "Enums.h"

typedef uint64_t Bitboard;

//Renvoie le masque d'une ligne
constexpr Bitboard rowMask(Rank row)
{
	return 0xffULL << (8*row);
}

//Renvoie le masque d'une colonne
constexpr Bitboard colMask(File col)
{
	return 0x101010101010101ULL << col;
}

//Renvoie le masque d'une case
constexpr Bitboard squareMask(Square square)
{
	return 1ULL << square;
}

constexpr Bitboard squareMask(File col, Rank row)
{
	return squareMask(static_cast<Square>(8*row + col));
}

#if defined(_MSC_VER) && defined(_M_X64)
#include <intrin.h>

//Renvoie la premiere case non nulle
inline Square firstSquare(Bitboard b)
{
	return static_cast<Square>(_tzcnt_u64(b));
}

//Renvoie la dernière case non nulle
inline Square lastSquare(Bitboard b)
{
	return static_cast<Square>(63-_lzcnt_u64(b));
}

//Renvoie le nombre de cases non nulles
inline int countSquares(Bitboard b)
{
	return static_cast<int>(__popcnt64(b));
}

#else
#error "Compiles only on x64 platforms at the moment"
#endif

//Remet à zéro la première case non nulle et la renvoie
inline Square popFirstSquare(Bitboard& b)
{
	Square square = firstSquare(b);
	b &= b-1;
	return square;
}

//Opération shift: décale le bitboard dans une direction
template<Direction direction>
Bitboard shift(Bitboard b) { static_assert(false, "Must provide a direction"); }

template<>
constexpr Bitboard shift<NORTH>(Bitboard b) { return b << 8; }

template<>
constexpr Bitboard shift<SOUTH>(Bitboard b) { return b >> 8; }

template<>
constexpr Bitboard shift<EAST>(Bitboard b) { return (b & ~colMask(FILE_H)) << 1; }

template<>
constexpr Bitboard shift<WEST>(Bitboard b) { return (b & ~colMask(FILE_A)) >> 1; }

template<>
constexpr Bitboard shift<NORTHEAST>(Bitboard b) { return (b & ~colMask(FILE_H)) << 9; }

template<>
constexpr Bitboard shift<NORTHWEST>(Bitboard b) { return (b & ~colMask(FILE_A)) << 7; }

template<>
constexpr Bitboard shift<SOUTHEAST>(Bitboard b) { return (b & ~colMask(FILE_H)) >> 7; }

template<>
constexpr Bitboard shift<SOUTHWEST>(Bitboard b) { return (b & ~colMask(FILE_A)) >> 9; }
