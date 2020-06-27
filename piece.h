#ifndef PIECE_H
#define PIECE_H

#include "square.h"

namespace Chess {

	enum PieceType {
		NONE = 0,
		PAWN = 1,
		BISHOP = 2,
		KNIGHT = 3,
		ROOK = 4,
		QUEEN = 6,
		KING = 7
	};

	struct Piece {
		Square loc;
		PieceType type;
		bool white_side;

		Piece() = default;
		Piece(const PieceType& pt, const bool& w_side) : type(pt), white_side(w_side) {}
		Piece(const Square& location, const bool& w_side) : loc(location), white_side(w_side) {}
		Piece(const Square& location, const PieceType& tp, const bool& w_side) : loc(location), type(tp), white_side(w_side) {}

		std::string getType() const;

		std::string asString() const;

		unsigned int getValue() const;

		bool operator==(const Piece& other);

		bool operator!=(const Piece& other);
	};

	PieceType ctot(const char& type); // char to type

}

#endif