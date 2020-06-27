#include "move.h"
using namespace Chess;

std::string Move::asBasicNotation() const {
	std::string letter = ((moved.getType() != "P") ? moved.getType() : std::string(""));
	return (letter + dest.asCoordinate());
}

bool Move::operator==(const Move& other) {
	return (dest == other.dest && moved == other.moved);
}