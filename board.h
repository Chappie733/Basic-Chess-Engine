#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include "move.h"

#define abs(x) ((x>0)?x:-x)
#define norm(x) ((x>=0)?1:-1)

namespace Chess {

	class Board {
	private:
		Piece board[8][8];
	public:
		Board();

		bool isLegal(const Move& move) const;
		bool processMove(Move& move); // returns if the move is valid
		[[noreturn]] void reset();
		std::vector<Move> getPossibleMoves(const Piece& piece, bool&& c_legals = true) const;
		std::vector<Move> getAllPossibleMoves(const bool& side) const; // true -> white, false -> black

		std::string asString();
		bool isChecked(const bool& side) const;
		bool capturesPiece(const Move& move) const;
		[[noreturn]] void castle(bool& side, bool&& kside);
		bool canCastle(bool& side, bool& kside) const;

		Piece getPiece(const Square& location) const;
		Piece getPiece(const unsigned int& x, const unsigned int& y) const;
		void setPiece(const Square& location, const Piece& piece);

		// Applies move even if it's not considered valid, it is used for castling and optimization
		void applyMove(Move& move);
		bool isThreatened(unsigned int& x, unsigned int& y, const bool& side) const;
		bool isThreatened(Square& square, const bool& side) const;
		bool isThreatened(Square& square, const bool& side, Piece& ignored) const;
	};

}

#endif