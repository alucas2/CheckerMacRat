#include "GameState.h"
#include <algorithm>

GameState::GameState(std::string fen):
	m_castleRights({NO_RIGHTS, NO_RIGHTS}), m_lastJumpedSquare(NULL_SQUARE), m_sideToMove(WHITE)
{
	std::fill(m_pieceMask.begin(), m_pieceMask.end(), 0ULL);
	std::fill(m_colorMask.begin(), m_colorMask.end(), 0ULL);

	std::string::iterator it = fen.begin();

	//Partie 1: disposition du plateau
	for(Row row = ROW_8; row >= ROW_1; row = static_cast<Row>(row - 1))
	{
		for(Col col = COL_A; col <= COL_H; col = static_cast<Col>(col + 1))
		{
			char pieceLabel = *it++;
			if('1' <= pieceLabel && pieceLabel <= '8')
			{
				col = static_cast<Col>(col + (pieceLabel - '1'));
			}
			else
			{
				Bitboard sqMask = squareMask(col, row);

				if(		'A' <= pieceLabel && pieceLabel <= 'Z')	m_colorMask[WHITE] |= sqMask;
				else if('a' <= pieceLabel && pieceLabel <= 'z')	m_colorMask[BLACK] |= sqMask;

				if(		pieceLabel == 'P' || pieceLabel == 'p')	m_pieceMask[PAWN] |= sqMask;
				else if(pieceLabel == 'N' || pieceLabel == 'n')	m_pieceMask[KNIGHT] |= sqMask;
				else if(pieceLabel == 'B' || pieceLabel == 'b')	m_pieceMask[BISHOP] |= sqMask;
				else if(pieceLabel == 'R' || pieceLabel == 'r')	m_pieceMask[ROOK] |= sqMask;
				else if(pieceLabel == 'Q' || pieceLabel == 'q')	m_pieceMask[QUEEN] |= sqMask;
				else if(pieceLabel == 'K' || pieceLabel == 'k')	m_pieceMask[KING] |= sqMask;
			}
		}
		if(*it == '/') it++;
	}

	//Partie 2: la couleur qui doit jouer le prochain tour
	while(*it++ != ' ');
	if(*it == 'w')
		m_sideToMove = WHITE;
	else if(*it == 'b')
		m_sideToMove = BLACK;

	//Partie 3: les droits de roque
	while(*it++ != ' ');
	if(*it != '-')
	{
		while(*it != ' ')
		{
			if(*it == 'K') m_castleRights[WHITE] |= RIGHT_EAST;
			if(*it == 'Q') m_castleRights[WHITE] |= RIGHT_WEST;
			if(*it == 'k') m_castleRights[BLACK] |= RIGHT_EAST;
			if(*it == 'q') m_castleRights[BLACK] |= RIGHT_WEST;
			it++;
		}
	}

	//Partie 4: la case d'arrivée en cas de prise en passant
	while(*it++ != ' ');
	if(*it != '-')
	{
		Col epCol = static_cast<Col>(*it - 'a');
		it++;
		Row epRow = static_cast<Row>(*it - '1');
		m_lastJumpedSquare = static_cast<Square>(8*epRow + epCol);
	}

	//Partie 5: halfmoves
	while(*it++ != ' ');
	m_halfMoveClock = *it - '0';
	
	//Partie 6: fullmoves
	while(*it++ != ' ');
	m_fullMoveCount = *it - '0';
}

GameState::GameState():
	GameState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
{}

std::string GameState::getFen() const
{
	std::string fen;

	//Partie 1: disposition du plateau
	for(Row row = ROW_8; row >= ROW_1; row = static_cast<Row>(row - 1))
	{
		for(Col col = COL_A; col <= COL_H; col = static_cast<Col>(col + 1))
		{
			Square sq = static_cast<Square>(8*row + col);
			Piece piece = getPieceAt(sq);
			Color color = getColorAt(sq);
			if(piece == NULL_PIECE)
			{
				char last = fen.back();
				if('1' <= last && last <= '7')
					fen.back() = last + 1;
				else
					fen.push_back('1');
			}
			else
			{
				char next;
				if(		piece == PAWN)		next = 'P';
				else if(piece == KNIGHT)	next = 'N';
				else if(piece == BISHOP)	next = 'B';
				else if(piece == ROOK)		next = 'R';
				else if(piece == QUEEN)		next = 'Q';
				else if(piece == KING)		next = 'K';

				if(color == BLACK)			next += 32;

				fen.push_back(next);
			}
		}
		if(row != ROW_1)
			fen.push_back('/');
	}

	//Partie 2: la couleur qui doit jouer le prochain tour
	fen.push_back(' ');
	if(m_sideToMove == WHITE)
		fen.push_back('w');
	else
		fen.push_back('b');

	//Partie 3: les droits de roque
	fen.push_back(' ');
	if(m_castleRights[WHITE] && m_castleRights[BLACK])
	{
		if(m_castleRights[WHITE] | RIGHT_EAST) fen.push_back('K');
		if(m_castleRights[WHITE] | RIGHT_WEST) fen.push_back('Q');
		if(m_castleRights[BLACK] | RIGHT_EAST) fen.push_back('k');
		if(m_castleRights[BLACK] | RIGHT_WEST) fen.push_back('q');
	}
	else
		fen.push_back('-');

	//Partie 4: la case d'arrivée en cas de prise en passant
	fen.push_back(' ');
	if(m_lastJumpedSquare != NULL_SQUARE)
	{
		fen.push_back(label(colOf(m_lastJumpedSquare)));
		fen.push_back(label(rowOf(m_lastJumpedSquare)));
	}
	else
		fen.push_back('-');

	//Partie 5: halfmoves
	fen.push_back(' ');
	fen.push_back(m_halfMoveClock + '0');

	//Partie 6: fullmoves
	fen.push_back(' ');
	fen.push_back(m_fullMoveCount + '0');

	return fen;
}

Piece GameState::getPieceAt(Square square) const
{
	Bitboard sqMask = squareMask(square);

	for(Piece p = PAWN; p < NUM_PIECE; p = static_cast<Piece>(p + 1))
		if(sqMask & m_pieceMask[p])
			return p;

	return NULL_PIECE;
}

Color GameState::getColorAt(Square square) const
{
	Bitboard sqMask = squareMask(square);

	if(sqMask & m_colorMask[WHITE])
		return WHITE;
	else if(sqMask & m_colorMask[BLACK])
		return BLACK;
	
	return NULL_COLOR;
}

std::pair<GameState, bool> GameState::performMove(Move move) const
{
	GameState result(*this);
	Color movingColor = m_sideToMove;
	Square from = move.getFrom();
	Square to = move.getTo();
	MoveFlags flags = move.getFlags();

	//Effectue la capture d'abord
	if(flags != EN_PASSANT)
	{
		Piece capturedPiece = getPieceAt(to);
		if(capturedPiece != NULL_PIECE)
		{
			Bitboard captureMask = squareMask(to);
			result.m_pieceMask[capturedPiece] ^= captureMask;
			result.m_colorMask[!movingColor] ^= captureMask;
			result.m_halfMoveClock = -1;
		}
	}
	else
	{
		Bitboard captureMask = squareMask(m_lastJumpedSquare + (movingColor == WHITE ? SOUTH : NORTH));
		result.m_pieceMask[PAWN] ^= captureMask;
		result.m_colorMask[!movingColor] ^= captureMask;
		result.m_halfMoveClock = -1;
	}

	//Effectue le mouvement ensuite
	Piece movingPiece = getPieceAt(from);
	Bitboard moveMask = squareMask(from) | squareMask(to);
	result.m_pieceMask[movingPiece] ^= moveMask;
	result.m_colorMask[movingColor] ^= moveMask;

	//Effectue la promotion
	if(flags & PROMOTION_FLAG)
	{
		Bitboard promotionPositionMask = squareMask(to);
		if(flags == PROMOTION_QUEEN)
			result.m_pieceMask[QUEEN] |= promotionPositionMask;
		else if(flags == PROMOTION_ROOK)
			result.m_pieceMask[ROOK] |= promotionPositionMask;
		else if(flags == PROMOTION_BISHOP)
			result.m_pieceMask[BISHOP] |= promotionPositionMask;
		else if(flags == PROMOTION_KNIGHT)
			result.m_pieceMask[KNIGHT] |= promotionPositionMask;
		result.m_pieceMask[PAWN] ^= promotionPositionMask;
	}
	//Effectue le mouvement de la tour du roque
	else if(flags & CASTLE_FLAG)
	{
		Bitboard rookMoveMask;
		if(flags == CASTLE_EAST)
		{
			rookMoveMask = movingColor == WHITE ?
				squareMask(SQUARE_H1) | squareMask(SQUARE_F1) :
				squareMask(SQUARE_H8) | squareMask(SQUARE_F8);
		}
		else if(flags == CASTLE_WEST)
		{
			rookMoveMask = movingColor == WHITE ?
				squareMask(SQUARE_A1) | squareMask(SQUARE_D1) :
				squareMask(SQUARE_A8) | squareMask(SQUARE_D8);
		}
		result.m_pieceMask[ROOK] ^= rookMoveMask;
		result.m_colorMask[movingColor] ^= rookMoveMask;
	}

	//Met à jour la possibilité de prise en passant
	if(movingPiece == PAWN && (to - from == NORTH+NORTH || to - from == SOUTH+SOUTH))
		result.m_lastJumpedSquare = to + (movingColor == WHITE ? SOUTH : NORTH);
	else
		result.m_lastJumpedSquare = NULL_SQUARE;

	//Met à jour les droits de roque
	if(movingPiece == KING)
		result.m_castleRights[movingColor] = NO_RIGHTS;
	else if(movingPiece == ROOK)
	{
		Col col = colOf(from);
		if(col == COL_A)
			result.m_castleRights[movingColor] &= ~RIGHT_WEST;
		else if(col == COL_H)
			result.m_castleRights[movingColor] &= ~RIGHT_EAST;
	}

	//Teste si le mouvement ne laisse pas le roi en échec
	bool isLegal;
	if(movingColor == WHITE)
		isLegal = !result.isAttacked<BLACK>(firstSquare(result.getPieces<WHITE, KING>()));
	else
		isLegal = !result.isAttacked<WHITE>(firstSquare(result.getPieces<BLACK, KING>()));

	//Met à jour le tour
	result.m_sideToMove = !result.m_sideToMove;

	//Met à jour les compteurs
	if(movingPiece == PAWN) result.m_halfMoveClock = -1;
	if(movingColor == BLACK) result.m_fullMoveCount++;
	result.m_halfMoveClock++;

	return std::make_pair(result, isLegal);
}

GameState GameState::performNullMove() const
{
	GameState result(*this);

	result.m_sideToMove = !result.m_sideToMove;

	return result;
}
