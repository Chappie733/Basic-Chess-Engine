#include <iostream>
#include "piece.h"
#include "cengine.h"

void LoadAllOpenings(Chess::Board& b);

int main() {
	srand(time(NULL));

	Chess::Board board;
	LoadAllOpenings(board);
	unsigned int turn = 1; // odd -> white, even -> black
	std::string moves = "";

	Chess::Engine::Openings::OpeningSequence opening(false);

	while (true) {
		std::cout << "Chess Engine developed by Nicola Trombini starting from 02/06/2020\n" << "Insert -1 -1 to exit\n" <<
			"The input format is as follows: a b (example \"5 5\")\n\n";
		int px, py, dx, dy;
		std::cout << moves << "\n\n";

		std::cout << board.asString() << "\n\n";

		Chess::Move move;

		if (turn % 2 != 0) { // player's turn (white's)
			std::cout << "Move piece at position: ";
			std::cin >> px >> py;

			if (px == -1 && py == -1)
				break;

			std::cout << "To position: ";
			std::cin >> dx >> dy;
			move = Chess::Move::Move(board.getPiece(px, py), dx, dy);
		}
		else {
			std::cout << "Computer's move (Processing...): ";
			if (opening.isOver()) move = Chess::Engine::getBest(board, turn, 3);
			else move = opening.getMove();
		}

		if (board.getPiece(move.moved.loc).white_side != (turn % 2 != 0)) { // the piece moved is not on the side moving
			std::cout << "It is " << ((turn % 2 == 0) ? "black" : "white") << "'s turn!\n";
			std::cin.get();
			system("cls");
			continue;
		}

		bool captured = board.getPiece(move.dest).type != Chess::NONE;
		std::cout << Chess::Engine::asNotation(move, board, captured) << "\n";
		if (!board.processMove(move))
			std::cout << "\nThe move is invalid!";
		else {
			moves.append(Chess::Engine::asNotation(move, board, captured) + std::string("\n"));
			++turn;
		}

		system("cls");
	}

	return 0;
}

void LoadAllOpenings(Chess::Board& b) {
	Chess::Engine::Openings::LoadOpening("openings/scotch.ops", b);
	Chess::Engine::Openings::LoadOpening("openings/Biel.ops", b);
	Chess::Engine::Openings::LoadOpening("openings/kIndian.ops", b);
}