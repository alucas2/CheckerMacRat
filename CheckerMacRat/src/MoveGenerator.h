#pragma once

#include "GameState.h"
#include "Bitboard.h"
#include "Move.h"
#include "Attacks.h"
#include <vector>
#include <algorithm>

class MoveGenerator
{
private:

	//Génère tous les mouvements de tous les cavaliers
	template<Color color>
	static void appendKnightMoves(std::vector<Move>& moves, const GameState& position)
	{
		Bitboard departures = position.getPieces<color, KNIGHT>();
		while(departures)
		{
			Square from = popFirstSquare(departures);
			Bitboard arrivals = knightAttacks(from) & ~position.getPieces<color>();
			while(arrivals)
				moves.emplace_back(from, popFirstSquare(arrivals));
		}
	}

	//Génère tous les mouvements de tous les fous
	template<Color color>
	static void appendBishopMoves(std::vector<Move>& moves, const GameState& position)
	{
		Bitboard departures = position.getPieces<color, BISHOP>() | position.getPieces<color, QUEEN>();
		while(departures)
		{
			Square from = popFirstSquare(departures);
			Bitboard arrivals = bishopAttacks(from, position.getPieces()) & ~position.getPieces<color>();
			while(arrivals)
				moves.emplace_back(from, popFirstSquare(arrivals));
		}
	}

	//Génère tous les mouvements de toutes les tours
	template<Color color>
	static void appendRookMoves(std::vector<Move>& moves, const GameState& position)
	{
		Bitboard departures = position.getPieces<color, ROOK>() | position.getPieces<color, QUEEN>();
		while(departures)
		{
			Square from = popFirstSquare(departures);
			Bitboard arrivals = rookAttacks(from, position.getPieces()) & ~position.getPieces<color>();
			while(arrivals)
				moves.emplace_back(from, popFirstSquare(arrivals));
		}
	}

	//Génère tous les mouvements du roi sauf les roques
	template<Color color>
	static void appendKingMoves(std::vector<Move>& moves, const GameState& position)
	{
		Bitboard departures = position.getPieces<color, KING>();
		while(departures)
		{
			Square from = popFirstSquare(departures);
			Bitboard arrivals = kingAttacks(from) & ~position.getPieces<color>();
			while(arrivals)
				moves.emplace_back(from, popFirstSquare(arrivals));
		}
	}

	//Génère tous les mouvements de tous les pions, y compris les prises en passant et les promotions
	template<Color color>
	static void appendPawnMoves(std::vector<Move>& moves, const GameState& position)
	{
		Bitboard departures = position.getPieces<color, PAWN>();
		constexpr Row lastRow = color == WHITE ? ROW_7 : ROW_2;
		while(departures)
		{
			Square from = popFirstSquare(departures);

			Bitboard arrivals =
				pawnAttacks<color>(from, position.getPieces<!color>()) | 
				pawnPushes<color>(from, position.getPieces()) & ~position.getPieces<color>();

			if(rowOf(from) != lastRow)
				while(arrivals)
					moves.emplace_back(from, popFirstSquare(arrivals));
			else
				while(arrivals)
				{
					Square to = popFirstSquare(arrivals);
					moves.emplace_back(from, to, PROMOTION_QUEEN);
					moves.emplace_back(from, to, PROMOTION_ROOK);
					moves.emplace_back(from, to, PROMOTION_BISHOP);
					moves.emplace_back(from, to, PROMOTION_KNIGHT);
				}
		}

		Square epDest = position.getLastJumpedSquare();
		if(epDest != NULL_SQUARE)
		{
			Bitboard canEp = pawnAttacks<!color>(epDest, position.getPieces<color, PAWN>());
			while(canEp)
				moves.emplace_back(popFirstSquare(canEp), epDest, EN_PASSANT);
		}
	}

	//Génère les roques
	template<Color color>
	static void appendCastles(std::vector<Move>& moves, const GameState& position)
	{
		Bitboard obstacles = position.getPieces();
		CastleFlags castleRights = position.getCaslteRights<color>();
		
		if constexpr(color == WHITE)
		{
			if(castleRights & RIGHT_WEST  
				&& slide<WEST>(SQUARE_E1, obstacles) & squareMask(SQUARE_A1) & position.getPieces<color, ROOK>()
				&& !position.isAttacked<!color>(SQUARE_E1)
				&& !position.isAttacked<!color>(SQUARE_D1)
				&& !position.isAttacked<!color>(SQUARE_C1))
			{
				moves.emplace_back(SQUARE_E1, SQUARE_C1, CASTLE_WEST);
			}
			if(castleRights & RIGHT_EAST
				&& slide<EAST>(SQUARE_E1, obstacles) & squareMask(SQUARE_H1) & position.getPieces<color, ROOK>()
				&& !position.isAttacked<!color>(SQUARE_E1)
				&& !position.isAttacked<!color>(SQUARE_F1)
				&& !position.isAttacked<!color>(SQUARE_G1))
			{
				moves.emplace_back(SQUARE_E1, SQUARE_G1, CASTLE_EAST);
			}
		}
		else
		{
			if(castleRights & RIGHT_WEST
				&& slide<WEST>(SQUARE_E8, obstacles) & squareMask(SQUARE_A8) & position.getPieces<color, ROOK>()
				&& !position.isAttacked<!color>(SQUARE_E8)
				&& !position.isAttacked<!color>(SQUARE_D8)
				&& !position.isAttacked<!color>(SQUARE_C8))
			{
				moves.emplace_back(SQUARE_E8, SQUARE_C8, CASTLE_WEST);
			}
			if(castleRights & RIGHT_EAST
				&& slide<EAST>(SQUARE_E8, obstacles) & squareMask(SQUARE_H8) & position.getPieces<color, ROOK>()
				&& !position.isAttacked<!color>(SQUARE_E8)
				&& !position.isAttacked<!color>(SQUARE_F8)
				&& !position.isAttacked<!color>(SQUARE_G8))
			{
				moves.emplace_back(SQUARE_E8, SQUARE_G8, CASTLE_EAST);
			}
		}
	}

	//Renvoie la liste des mouvements pseudo-légaux
	template<Color color>
	static std::vector<Move> getPseudoLegalMoves(const GameState& position)
	{
		std::vector<Move> moves;
		
		appendPawnMoves<color>(moves, position);
		appendKnightMoves<color>(moves, position);
		appendBishopMoves<color>(moves, position);
		appendRookMoves<color>(moves, position);
		appendKingMoves<color>(moves, position);
		appendCastles<color>(moves, position);

		return moves;
	}

public:
	//Renvoie la liste des mouvements pseudo-légaux
	static std::vector<Move> getPseudoLegalMoves(const GameState& from);

	//Renvoie la liste des mouvements légaux qui est un sous-ensemble des mouvements pseudo-légaux.
	//Beacoup plus lent que de générer les mouvements pseudo-légaux car cela nécessite d'effectuer le mouvement
	static std::vector<Move> getLegalMoves(const GameState& from);
};