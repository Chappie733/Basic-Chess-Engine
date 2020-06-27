#ifndef CENGINE_H
#define CENGINE_H

#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include "board.h"
#include "piece.h"
#include "square.h"
#include "move.h"

namespace Chess {
	namespace Engine {

		static Move null_move;

		bool checks(Move& move, Board board);

		std::string asNotation(Move& move, Board& board);

		std::string asNotation(Move& move, Board& board, bool& capt);

		// Algo 1 (It's important to do a copy (byt not passing by reference) so that the move can be evaluated before and after without altering the actual board)
		int MoveValue(Move& move, Board board);

		// Algo 2
		int sqDiff(Move& move, Board board);

		// Algo 3
		Move getSurfaceBest(Board& board, const bool& side);

		double evaluate(Board& board, Move& move);

		// string to move (notation to move)
		Move stom(std::string& move, Board& board, const bool& turn);

		// Algo 4
		std::vector<Move> getTopMoves(const unsigned int& amount, Board& board, const bool& side);

		static int b = 0;
		static unsigned int u; // last turn when algo 5 called non recursively

		// Algo 5
		Move getBest(Board& board, const unsigned int& w, const unsigned int& d, const bool& t = true);

		namespace Openings {

			class OpeningSequence {
			private:
				std::vector<Move> opening;
				int curr_move;
			public:
				OpeningSequence(bool&& side = true);

				Move getMove();
				bool isOver() const;
			};


			static std::vector<std::vector<Move>> w_openings;
			static std::vector<std::vector<Move>> b_openings;


			void LoadOpening(std::string&& source, Board& board);
			std::vector<Move> getRandomOpening(const bool& side);
		}
	}
}


#endif