#include "piece.h"
using namespace Chess;

std::string Piece::getType() const {
	switch (type) {
	case NONE: return std::string(1, '-');
	case PAWN: return std::string(1, 'P');
	case BISHOP: return std::string(1, 'B');
	case KNIGHT: return std::string(1, 'N');
	case ROOK: return std::string(1, 'R');
	case QUEEN: return std::string(1, 'Q');
	case KING: return std::string(1, 'K');
	default: return std::string("Unknown piece");
	}
}

std::string Piece::asString() const {
	return (std::string("Piece:\n") + std::string("location: ") + loc.asCoordinate() + std::string("\ntype: ") + getType() +
		"\nside: " + (white_side ? "White" : "Black") + "\n\n");
}

unsigned int Piece::getValue() const {
	switch (type) {
	case NONE: return 0;
	case PAWN: return 1;
	case BISHOP: return 3;
	case KNIGHT: return 3;
	case ROOK: return 5;
	case QUEEN: return 7;
	case KING: return 1000;
	default: return 0;
	}
}

bool Piece::operator==(const Piece& other) {
	return (type == other.type && loc == other.loc && white_side == other.white_side);
}

bool Piece::operator!=(const Piece& other) {
	return !(type == other.type && loc == other.loc && white_side == other.white_side);
}

PieceType Chess::ctot(const char& type) {
	switch (type) {
	case '-': return NONE;
	case 'P': return PAWN;
	case 'B': return BISHOP;
	case 'N': return KNIGHT;
	case 'R': return ROOK;
	case 'Q': return QUEEN;
	default: return NONE;
	}
}