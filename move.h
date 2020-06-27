#ifndef MOVE_H
#define MOVE_H

#include <string>
#include "square.h"
#include "piece.h"

namespace Chess {

	struct Move {
		Square dest;
		Piece moved;

		Move() = default;
		Move(const Piece& mov, const Square& destination)
			: dest(destination), moved(mov) {}
		Move(const Piece& mov, const unsigned int& dx, const unsigned int& dy)
			: moved(mov), dest(Square::Square(dx, dy)) {}

		std::string asBasicNotation() const;

		bool operator==(const Move& other);

	};

}

#endif