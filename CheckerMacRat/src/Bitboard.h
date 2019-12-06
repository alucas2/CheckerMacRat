#pragma once

#include "Enums.h"

#ifndef _WIN64
#error "Only 64-bit systems are supported"
#endif

typedef uint64_t Bitboard;

//Renvoie le masque d'une ligne
constexpr Bitboard rowMask(Row row)
{
	return 0xffULL << (8*row);
}

//Renvoie le masque d'une colonne
constexpr Bitboard colMask(Col col)
{
	return 0x101010101010101ULL << col;
}

//Renvoie le masque d'une case
constexpr Bitboard squareMask(Square square)
{
	return 1ULL << square;
}

constexpr Bitboard squareMask(Col col, Row row)
{
	return squareMask(static_cast<Square>(8*row + col));
}

#ifdef _MSC_VER
#include <intrin.h>

//Renvoie la premiere case non nulle
inline Square firstSquare(Bitboard b)
{
	unsigned long tz;
	_BitScanForward64(&tz, b);
	return static_cast<Square>(tz);
}

//Renvoie la dernière case non nulle
inline Square lastSquare(Bitboard b)
{
	unsigned long lz;
	_BitScanReverse64(&lz, b);
	return static_cast<Square>(lz);
}

#else
#error "No implementation of trailing zeros counting has been found"
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
constexpr Bitboard shift<EAST>(Bitboard b) { return (b & ~colMask(COL_H)) << 1; }

template<>
constexpr Bitboard shift<WEST>(Bitboard b) { return (b & ~colMask(COL_A)) >> 1; }

template<>
constexpr Bitboard shift<NORTHEAST>(Bitboard b) { return (b & ~colMask(COL_H)) << 9; }

template<>
constexpr Bitboard shift<NORTHWEST>(Bitboard b) { return (b & ~colMask(COL_A)) << 7; }

template<>
constexpr Bitboard shift<SOUTHEAST>(Bitboard b) { return (b & ~colMask(COL_H)) >> 7; }

template<>
constexpr Bitboard shift<SOUTHWEST>(Bitboard b) { return (b & ~colMask(COL_A)) >> 9; }
