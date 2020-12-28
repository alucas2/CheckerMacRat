#pragma once

#include <stdint.h>
#include <type_traits>

enum File
{
	FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};

enum Rank
{
	RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8
};

enum Square
{
	SQUARE_A1, SQUARE_B1, SQUARE_C1, SQUARE_D1, SQUARE_E1, SQUARE_F1, SQUARE_G1, SQUARE_H1,
	SQUARE_A2, SQUARE_B2, SQUARE_C2, SQUARE_D2, SQUARE_E2, SQUARE_F2, SQUARE_G2, SQUARE_H2,
	SQUARE_A3, SQUARE_B3, SQUARE_C3, SQUARE_D3, SQUARE_E3, SQUARE_F3, SQUARE_G3, SQUARE_H3,
	SQUARE_A4, SQUARE_B4, SQUARE_C4, SQUARE_D4, SQUARE_E4, SQUARE_F4, SQUARE_G4, SQUARE_H4,
	SQUARE_A5, SQUARE_B5, SQUARE_C5, SQUARE_D5, SQUARE_E5, SQUARE_F5, SQUARE_G5, SQUARE_H5,
	SQUARE_A6, SQUARE_B6, SQUARE_C6, SQUARE_D6, SQUARE_E6, SQUARE_F6, SQUARE_G6, SQUARE_H6,
	SQUARE_A7, SQUARE_B7, SQUARE_C7, SQUARE_D7, SQUARE_E7, SQUARE_F7, SQUARE_G7, SQUARE_H7,
	SQUARE_A8, SQUARE_B8, SQUARE_C8, SQUARE_D8, SQUARE_E8, SQUARE_F8, SQUARE_G8, SQUARE_H8,
	NUM_SQUARE, NULL_SQUARE
};

inline Rank rankOf(Square square) { return static_cast<Rank>(square / 8); }
inline File fileOf(Square square) { return static_cast<File>(square % 8); }
inline char label(Rank row) { return '1' + row; }
inline char label(File col) { return 'a' + col; }

enum Direction
{
	NORTH = 8,
	SOUTH = -8,
	EAST = 1,
	WEST = -1,
	NORTHEAST = NORTH + EAST,
	NORTHWEST = NORTH + WEST,
	SOUTHEAST = SOUTH + EAST,
	SOUTHWEST = SOUTH + WEST
};

constexpr Square operator+(Square sq, Direction d) { return static_cast<Square>(static_cast<int>(sq) + static_cast<int>(d)); }
constexpr Direction operator+(Direction d1, Direction d2) { return static_cast<Direction>(static_cast<int>(d1) + static_cast<int>(d2)); }

enum Piece
{
	PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NUM_PIECE, NULL_PIECE
};

enum Color
{
	WHITE, BLACK, NUM_COLOR, NULL_COLOR
};

constexpr Color operator!(Color color) { return color == WHITE ? BLACK : WHITE; }

enum MoveFlags
{
	EN_PASSANT			= 0b1100,
	CASTLE_FLAG			= 0b0010,
	CASTLE_WEST			= 0b0110,
	CASTLE_EAST			= 0b1010,
	PROMOTION_FLAG		= 0b0001,
	PROMOTION_QUEEN		= 0b0001,
	PROMOTION_ROOK		= 0b0101,
	PROMOTION_BISHOP	= 0b1001,
	PROMOTION_KNIGHT	= 0b1101
};

enum CastleFlags
{
	NO_RIGHTS			= 0,
	RIGHT_EAST			= 0b01,
	RIGHT_WEST			= 0b10
};

template<typename E>
E operator|(E a, E b)
{
	static_assert(std::is_enum_v<E>);
	using UT = typename std::underlying_type_t<E>;
	return static_cast<E>(static_cast<UT>(a) | static_cast<UT>(b));
}

template<typename E>
E& operator|=(E& a, E b)
{
	static_assert(std::is_enum_v<E>);
	using UT = typename std::underlying_type_t<E>;
	a = static_cast<E>(static_cast<UT>(a) | static_cast<UT>(b));
	return a;
}

template<typename E>
E operator&(E a, E b)
{
	static_assert(std::is_enum_v<E>);
	using UT = typename std::underlying_type_t<E>;
	return static_cast<E>(static_cast<UT>(a) & static_cast<UT>(b));
}

template<typename E>
E& operator&=(E& a, E b)
{
	static_assert(std::is_enum_v<E>);
	using UT = typename std::underlying_type_t<E>;
	a = static_cast<E>(static_cast<UT>(a) & static_cast<UT>(b));
	return a;
}

template<typename E>
E operator~(E a)
{
	static_assert(std::is_enum_v<E>);
	using UT = typename std::underlying_type_t<E>;
	return static_cast<E>(~static_cast<UT>(a));
}