#include "cengine.h"
using namespace Chess;

bool Engine::checks(Move& move, Board board) {
	board.processMove(move);
	return board.isChecked(!move.moved.white_side);
}

std::string Engine::asNotation(Move& move, Board& board) {
	bool capt = board.getPiece(move.dest).type != NONE;
	std::string captured = (capt) ? std::string("x") : std::string("");
	std::string check = (Engine::checks(move, board)) ? std::string("+") : std::string("");
	std::string letter = (move.moved.getType() != "P") ? move.moved.getType() : ((capt) ? std::string(1, (char)(move.moved.loc.x + 96)) : std::string(""));
	return (letter + captured + move.dest.asCoordinate() + check); // have to add mate
}

std::string Engine::asNotation(Move& move, Board& board, bool& capt) {
	std::string captured = (capt) ? std::string("x") : std::string("");
	std::string check = (Engine::checks(move, board)) ? std::string("+") : std::string("");
	std::string letter = (move.moved.getType() != "P") ? move.moved.getType() : ((capt) ? std::string(1, (char)(move.moved.loc.x + 96)) : std::string(""));
	return (letter + captured + move.dest.asCoordinate() + check); // have to add mate
}

int Engine::MoveValue(Move& move, Board board) {
	int evaluation = 0;

	for (Move& m : board.getAllPossibleMoves(!move.moved.white_side)) {
		if (!board.capturesPiece(m))
			continue;

		evaluation -= board.getPiece(m.dest).getValue();
	}

	board.applyMove(move);

	for (Move& m : board.getAllPossibleMoves(!move.moved.white_side)) {
		if (!board.capturesPiece(m))
			continue;

		evaluation += board.getPiece(m.dest).getValue();
	}

	return evaluation;
}

int Engine::sqDiff(Move& move, Board board) {
	int m = 0; // squares gained (>0) or lost (<0)
	for (Move& mov : board.getPossibleMoves(move.moved)) { // for each move the piece could have done before the move
		if (!board.isThreatened(mov.dest, mov.moved.white_side) && board.getPiece(mov.dest).type == NONE) // if the square it was threatening isn't already threatened by some other piece and the square is clear
			--m;
	}

	board.applyMove(move);

	for (Move& mov : board.getPossibleMoves(move.moved)) { // for each possible move after the move
		if (!board.isThreatened(mov.dest, mov.moved.white_side, mov.moved) && board.getPiece(mov.dest).type == NONE) // if the square the piece is moving to is empty and no other piece is threatening that square
			++m; // this square is newly gained control under
	}

	return m;
}

Move Engine::getSurfaceBest(Board& board, const bool& side) {
	Move best;
	int n = -5000;
	for (Move& m : board.getAllPossibleMoves(side)) {
		int v = MoveValue(m, board),
			k = sqDiff(m, board),
			r = board.getPiece(m.dest).getValue();

		int s = k + r - v; // points gained + squares gained - points exposed
		if (s > n) {
			n = s;
			best = m;
		}
	}
	return best;
}

double Engine::evaluate(Board& board, Move& move) {
	if (move == null_move)
		return -5000;
	int v = MoveValue(move, board),
		r = board.getPiece(move.dest).getValue();
	double k = sqDiff(move, board) * 0.3;

	return (double)(k + r - v);
}

bool isnumeric(const char& a) {
	return (!isalpha(a) && isalnum(a));
}

Move Engine::stom(std::string& move, Board& board, const bool& turn) {
	Square dest;
	Piece moved;
	unsigned int x, y;
	PieceType type;
	switch (move.size()) {
	case 2: {
	pawn_stom: // any pawn move, for example e4, or e7+, or d6#
		// it's certainly a pawn
		x = (move[0] - 96); y = move[1] - '0';
		dest = Square::Square(x, y);
		if (turn) { // if the move is on white's turn
			if (board.getPiece(x, y - 1).type == PAWN) // if in the move the pawn moves one square up
				moved = board.getPiece(x, y - 1);
			else moved = board.getPiece(x, y - 2); // else if it moves 2 squares up
		}
		else { // else if it's on black turn
			if (board.getPiece(x, y + 1).type == PAWN)
				moved = board.getPiece(x, y + 1);
			else moved = board.getPiece(x, y + 2);
		}
		break;
	} break;
	case 3: {
		if (move[2] == '+' || move[2] == '+') // if the move is actually just a pawn move
			goto pawn_stom; // run the code accordingly

	standard_move: // any move with this format: {Piece that is not pawn}{coordinate}, for example Be5
		type = Chess::ctot(move[0]);
		x = (move[1] - 96); y = (int)(move[2] - '0');
		bool found = false;
		for (int yt = 1; yt <= 8; yt++) { // for every piece
			for (int xt = 1; xt <= 8; xt++) { // on the board
				// if it's on the same side and of the same type as the piece in the move
				if (board.getPiece(xt, yt).type == type && board.getPiece(xt, yt).white_side == turn) {
					for (Move& m : board.getPossibleMoves(board.getPiece(xt, yt), false)) { // for each move it has availabl
						if (m.dest.x == x && m.dest.y == y) { // if one gets exactly where the move we have to find goes
							moved = board.getPiece(xt, yt); // this must be the piece
							dest = m.dest;
							found = true;
							break;
						}
					}
				}

				if (found) break;
			}
			if (found) break;
		}
		break;
	} break;
	case 4: {
		// There are different possible cases 
		// Nf5+ or Bf2# are examples of the first case
		// exf5 is a second case (Pawn captures something)
		// Bxa3 is a third case (any piece but pawn captures something)
		// Rde1 is a fourth case (a Piece moves to a position, a coord of its original position is specified)

		if (move[3] == '+' || move[3] == '#')
			goto standard_move;

		if (move[1] == 'x') { // if the move includes a capturing
			move.erase(move.begin() + 1);
			if (toupper(move[0]) == move[0]) // if it's not a pawn that's moving
				goto standard_move;
			else
				goto pawn_stom;
		}
		else { // there's no capturing and it is not a "simple" move
		specific_piece_move:
			type = Chess::ctot(move[0]);
			x = (move[2] - 96); y = move[3] - '0';
			int ox = ((isalpha(move[1])) ? (move[1] - 96) : -1),
				oy = ((isnumeric(move[1])) ? (move[1] - '0') : -1); // original x;
			bool found = false; // just for optimization purposes
			for (int yt = 1; yt <= 8; yt++) { // for every piece
				for (int xt = 1; xt <= 8; xt++) { // on the board
					// if it's on the same side and of the same type as the piece in the move
					if (board.getPiece(xt, yt).type == type && board.getPiece(xt, yt).white_side == turn) {
						if ((xt == ox || ox == -1) && (yt == oy || oy == -1)) {
							for (Move& m : board.getPossibleMoves(board.getPiece(xt, yt), false)) { // for each move it has available
								if (m.dest.x == x && m.dest.y == y) { // if one gets exactly where the move we have to find goes
									moved = board.getPiece(xt, yt); // this must be the piece
									dest = m.dest;
									found = true;
									break;
								}
							}
						}
					}

					if (found) break;
				}
				if (found) break;
			}
			break;
		}
	} break;
	case 5:
		// The only possibilities are in the format of R5f2+ v Raf2# (these are just examples)
		goto specific_piece_move;
	}
	return Move::Move(moved, dest);
}

// Assumes that the moves are legal
void Engine::Openings::LoadOpening(std::string&& source, Board& board) {
	std::ifstream stream;
	stream.open(source);
	if (!stream.is_open())
		std::cerr << "Could not load opening file " << source << "\n";

	std::vector<Move> opening;

	std::string curr_line;
	std::getline(stream, curr_line);
	bool white_opening = std::stoi(curr_line) == 1;
	while (std::getline(stream, curr_line))
		opening.push_back(Chess::Engine::stom(curr_line, board, white_opening));

	if (white_opening)
		w_openings.push_back(opening);
	else
		b_openings.push_back(opening);
}

std::vector<Move> Engine::getTopMoves(const unsigned int& amount, Board& board, const bool& side) {
	// if i used a vector of std::pair I'd have to convert it
	std::vector<Move> top_moves(amount);  // to a vector of moves before returning it, it would be less efficient
	std::vector<int> top_moves_val(amount); // thus we have two different vectors (a map is out of question since I regularly have to access the second member without the key
	for (unsigned int i = 0; i < amount; ++i)
		top_moves_val[i] = -5000;
	for (Move& c : board.getAllPossibleMoves(side)) {
		double val = evaluate(board, c);
		for (unsigned int i = 0; i < amount; i++) {
			if (val > top_moves_val[i]) {
				top_moves[i] = c;
				top_moves_val[i] = val;
				break;
			}
		}
	}
	return top_moves;
}

// w is the turn, time complexity d^4 (or whatever num first argument getTopMoves)
Move Engine::getBest(Board& board, const unsigned int& w, const unsigned int& d, const bool& t) {
	if (t) {
		b = 0;
		u = w;
	}
	if (d == 0) {
		Move s_best = getSurfaceBest(board, w % 2 != 0);
		double val = evaluate(board, s_best);
		b += (w % 2 == u % 2) ? val : -val;
		return null_move;
	}
	std::vector<Move> m = getTopMoves(5, board, w % 2 != 0);
	Move r = null_move;
	double r_val = -5000;
	for (Move& i : m) {
		Board after_move(board);
		after_move.applyMove(i);
		getBest(after_move, w + 1, d - 1, false); // just needed for balance heuristics calculations
		if (t) {
			if (b > r_val) {
				r = i;
				r_val = b;
			}
			b = 0;
		}
	}
	return r;
}

// OPENINGS STUFF
std::vector<Chess::Move> Engine::Openings::getRandomOpening(const bool& side) {
	if (side)
		return w_openings[rand() % w_openings.size()];
	else
		return b_openings[rand() % b_openings.size()];
}

using namespace Chess::Engine::Openings;

OpeningSequence::OpeningSequence(bool&& side) : curr_move(0) {
	opening = getRandomOpening(side);
}

Move OpeningSequence::getMove() {
	++curr_move;
	return opening[curr_move - 1];
}

bool OpeningSequence::isOver() const {
	return curr_move == opening.size();
}