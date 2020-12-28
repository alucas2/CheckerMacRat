#pragma once

#include "Enums.h"
#include "Bitboard.h"
#include "Move.h"
#include "Attacks.h"
#include <array>
#include <string>
#include <utility>

//La classe GameState code une position du plateau de jeu
class GameState
{
public:
	GameState(std::string fen);
	GameState();

	//Renvoie la position en notation FEN
	std::string getNotation() const;

	//Renvoie le masque de toutes les pièces d'une couleur
	template<Color color>
	Bitboard getPieces() const
	{
		return m_colorMask[color];
	}

	//Renvoie le masque de toutes les pièce d'un type et d'une couleur
	template<Color color, Piece piece>
	Bitboard getPieces() const
	{
		return m_colorMask[color] & m_pieceMask[piece];
	}

	//Renvoie le masque de toutes les pièces
	Bitboard getPieces() const
	{
		return m_colorMask[WHITE] | m_colorMask[BLACK];
	}

	//Renvoie le type de la pièce qui occupe cette case
	//ou NULL_PIECE si cette case est vide
	Piece getPieceAt(Square square) const;

	//Renvoie la couleur de la pièce qui occupe cette case
	//ou NULL_COLOR si cette case est vide
	Color getColorAt(Square square) const;

	//Renvoie le camp qui doit jouer le prochain tour
	Color getSideToMove() const { return m_sideToMove; }

	//Renvoie le dernière case qui a été sautée par un pion au tour précédent
	//ou NULL_SQUARE si aucun pion n'a sauté au tour précédent
	Square getLastJumpedSquare() const { return m_lastJumpedSquare; }

	//Renvoie le nombre de tour réalisés sans aucune capture ou mouvement de pion
	//Sert à déterminer les égalités par la règle des 50 coups
	int getHalfMoveClock() const { return m_halfMoveClock; }

	//Renvoie le nombre de coups des noirs, commence à 1
	int getFullMoveCount() const { return m_fullMoveCount; }

	//Renvoie le masque des droits de roque pour un camp
	template<Color color>
	CastleFlags getCaslteRights() const
	{
		return m_castleRights[color];
	}

	//Renvoie l'état du jeu après avoir effectué un mouvement
	GameState performMove(Move move) const;

	//Renvoie l'état du jeu après avoir passé un tour
	GameState performNullMove() const;

	//Renvoie true ssi cette position est légale
	bool isLegal() const;

	//Renvoie true ssi cette case est attaquée par une pièce de la couleur indiquée
	template<Color color>
	bool isAttacked(Square square) const
	{
		return pawnAttacks<!color>(square, ~0ULL) & getPieces<color, PAWN>()
			|| knightAttacks(square) & getPieces<color, KNIGHT>()
			|| bishopAttacks(square, getPieces()) & (getPieces<color, BISHOP>() | getPieces<color, QUEEN>())
			|| rookAttacks(square, getPieces()) & (getPieces<color, ROOK>() | getPieces<color, QUEEN>())
			|| kingAttacks(square) & getPieces<color, KING>();
	}

private:
	std::array<Bitboard, NUM_PIECE> m_pieceMask;
	std::array<Bitboard, NUM_COLOR> m_colorMask;
	std::array<CastleFlags, NUM_COLOR> m_castleRights;
	Color m_sideToMove;
	Square m_lastJumpedSquare;
	int m_halfMoveClock;
	int m_fullMoveCount;
};