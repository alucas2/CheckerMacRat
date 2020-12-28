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

	//Renvoie le masque de toutes les pi�ces d'une couleur
	template<Color color>
	Bitboard getPieces() const
	{
		return m_colorMask[color];
	}

	//Renvoie le masque de toutes les pi�ce d'un type et d'une couleur
	template<Color color, Piece piece>
	Bitboard getPieces() const
	{
		return m_colorMask[color] & m_pieceMask[piece];
	}

	//Renvoie le masque de toutes les pi�ces
	Bitboard getPieces() const
	{
		return m_colorMask[WHITE] | m_colorMask[BLACK];
	}

	//Renvoie le type de la pi�ce qui occupe cette case
	//ou NULL_PIECE si cette case est vide
	Piece getPieceAt(Square square) const;

	//Renvoie la couleur de la pi�ce qui occupe cette case
	//ou NULL_COLOR si cette case est vide
	Color getColorAt(Square square) const;

	//Renvoie le camp qui doit jouer le prochain tour
	Color getSideToMove() const { return m_sideToMove; }

	//Renvoie le derni�re case qui a �t� saut�e par un pion au tour pr�c�dent
	//ou NULL_SQUARE si aucun pion n'a saut� au tour pr�c�dent
	Square getLastJumpedSquare() const { return m_lastJumpedSquare; }

	//Renvoie le nombre de tour r�alis�s sans aucune capture ou mouvement de pion
	//Sert � d�terminer les �galit�s par la r�gle des 50 coups
	int getHalfMoveClock() const { return m_halfMoveClock; }

	//Renvoie le nombre de coups des noirs, commence � 1
	int getFullMoveCount() const { return m_fullMoveCount; }

	//Renvoie le masque des droits de roque pour un camp
	template<Color color>
	CastleFlags getCaslteRights() const
	{
		return m_castleRights[color];
	}

	//Renvoie l'�tat du jeu apr�s avoir effectu� un mouvement
	GameState performMove(Move move) const;

	//Renvoie l'�tat du jeu apr�s avoir pass� un tour
	GameState performNullMove() const;

	//Renvoie true ssi cette position est l�gale
	bool isLegal() const;

	//Renvoie true ssi cette case est attaqu�e par une pi�ce de la couleur indiqu�e
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