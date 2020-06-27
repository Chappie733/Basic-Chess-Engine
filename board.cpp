#include "board.h"
using namespace Chess;

Board::Board() {
	reset();
}

bool Board::isLegal(const Move& move) const {
	if (move.dest.x < 1 || move.dest.x>8 || move.dest.y < 1 || move.dest.y>8) return false; // if the destination square is not on the board the move is illegal 
	if (getPiece(move.dest).white_side == move.moved.white_side && getPiece(move.dest).type != NONE) return false;
	int x_dist = move.moved.loc.x - move.dest.x, y_dist = move.moved.loc.y - move.dest.y; // x and y distances of the destination to the square before the move
	if (x_dist == 0 && y_dist == 0) return false;
	switch (move.moved.type) {
	case NONE: return false;
	case PAWN:
		// TODO: implement check
		if (abs(y_dist) == 0) return false; // if the pawn hasn't moved vertically the move is always illegal
		int w_side; // possible move
		w_side = (move.moved.white_side ? 1 : -1);
		bool canEat; // wether the pawn could potentially capture a piece
		canEat = ((getPiece(move.moved.loc.x + 1, move.moved.loc.y + w_side).type != NONE) && (getPiece(move.moved.loc.x + 1, move.moved.loc.y + w_side).white_side != move.moved.white_side))
			|| ((getPiece(move.moved.loc.x - 1, move.moved.loc.y + w_side).type != NONE) && (getPiece(move.moved.loc.x - 1, move.moved.loc.y + w_side).white_side != move.moved.white_side));
		if (x_dist != 0) { // if it has moved horizontally
			if (!canEat) // ... and it can't capture
				return false; // the move is not legal
			else // if it can capture
				return ((getPiece(move.dest).type != NONE) && (getPiece(move.dest).white_side != move.moved.white_side)); // the move is legal if it actually captured
		}
		else { // if it hasn't moved horizontally
			if (move.moved.white_side) { // if the pawn is white
				if (move.moved.loc.y + 1 == move.dest.y // if it moved up by one (square)
					&& getPiece(move.dest).type == NONE) // and the square it moved to was empty
					return true; // the move is legal
				else if (move.dest.y == move.moved.loc.y + 2) // else if it moved up by two (squares)
					if (getPiece(move.dest).type == NONE // if the square it moved to is empty
						&& getPiece(move.moved.loc.x, move.moved.loc.y + 1).type == NONE // and the square it moved trough is also empty
						&& move.moved.loc.y == 2)  // and the pawn is in the starting position
						return true; // the move is legal
			}
			else if (!move.moved.white_side) { // if the pawn is black
				if (move.moved.loc.y - 1 == move.dest.y // if it moved down by one (square)
					&& getPiece(move.dest).type == NONE) { // and the square it moved to was empty
					return true; // the move is legal
				}
				else if (move.dest.y == move.moved.loc.y - 2) // else if it moved down by two (squares)
					if (getPiece(move.dest).type == NONE // if the square it moved to is empty
						&& getPiece(move.moved.loc.x, move.moved.loc.y - 1).type == NONE // and the square it moved trough is also empty
						&& move.moved.loc.y == 7) // and the pawn is in the starting position
						return true; // the move is legal
			}
		}
		break;
	case BISHOP:
		if (abs(x_dist) != abs(y_dist)) // if the absolute x distance isn't equal to the absolute y distance (which means the bishop didn't move diagonally)
			return false; // the move is illegal
		else { // if it did move diagonally
			int x_curr, y_curr; // current x and y position in the diagonal (the program will check each square the bishop moves trought)
			for (int t = 1;; t++) { // let t be 0 and increase it each time, it will signify how far we are from the original square (before the move)
				x_curr = move.moved.loc.x + t * -norm(x_dist); // the current checking position, equal to p + t*l, where p is the initial position, t is the distance from the original position, and l is the direction in which the bishop is moving
				y_curr = move.moved.loc.y + t * -norm(y_dist); // ""
				if (getPiece(x_curr, y_curr).type != NONE)  // if there is a piece in the current square 
					return ((x_curr == move.dest.x && y_curr == move.dest.y) && (getPiece(x_curr, y_curr).white_side != move.moved.white_side)); // if the move led the bishop to this exact square and the piece in that square is of the other player it is legal, else it isn't since it can't go over that piece
				else if (x_curr == move.dest.x && y_curr == move.dest.y) // else if the current square is the one the bishop is moving to, and since no value has been returned there wasn't a piece interrupting it before, and it is in a square inside the board
					return true; // the move is valid
			}
		}
		break;
	case KNIGHT:
		if ((abs(x_dist) == 2 && abs(y_dist) == 1) || (abs(x_dist) == 1 && abs(y_dist) == 2))  // if the knight did an "L" shape move
			return true; // the move is legal
		break;
	case ROOK:
		if (x_dist != 0 && y_dist != 0) // if the rook moved diagonally or didn't move at all
			return false; // the move is not legal

		int x_curr, y_curr; // current x and y position of the square being checked (the program will check each square the bishop moves trought)
		for (int t = 1;; t++) { // let t be 0 and increase it each time, it will signify how far we are from the original square (before the move)
			x_curr = move.moved.loc.x + (t * -norm(x_dist)) * ((x_dist != 0) ? 1 : 0); // the current checking position, equal to p + t*l*d, where p is the initial position, t is the distance from the original position, and l is the direction in which the bishop is moving and d is wether the piece moved horizontally
			y_curr = move.moved.loc.y + (t * -norm(y_dist)) * ((y_dist != 0) ? 1 : 0); // "" ... moved vertically
			if (getPiece(x_curr, y_curr).type != NONE) // if there is a piece in the current square 
				return ((x_curr == move.dest.x && y_curr == move.dest.y) && (getPiece(x_curr, y_curr).white_side != move.moved.white_side)); // if the move led the rook to this exact square it is legal, else it isn't since it can't go over that piece
			else if (x_curr == move.dest.x && y_curr == move.dest.y) // else if the current square is the one the rook is moving to, and since no value has been returned there wasn't a piece interrupting it before, and it is in a square inside the board
				return true; // the move is valid
		}
		break;
	case QUEEN:
		if (abs(x_dist) == abs(y_dist)) { // if the queen moved diagonally
			int x_curr, y_curr; // current x and y position in the diagonal (the program will check each square the bishop moves trought)
			for (int t = 1;; t++) { // let t be 0 and increase it each time, it will signify how far we are from the original square (before the move)
				x_curr = move.moved.loc.x + t * -norm(x_dist); // the current checking position, equal to p + t*l, where p is the initial position, t is the distance from the original position, and l is the direction in which the bishop is moving
				y_curr = move.moved.loc.y + t * -norm(y_dist); // ""
				if (getPiece(x_curr, y_curr).type != NONE)  // if there is a piece in the current square 
					return ((x_curr == move.dest.x && y_curr == move.dest.y) && (getPiece(x_curr, y_curr).white_side != move.moved.white_side)); // if the move led the queen to this exact square and the piece in that square is not in the same side it is legal, else it isn't since it can't go over that piece
				else if (x_curr == move.dest.x && y_curr == move.dest.y) // else if the current square is the one the bishop is moving to, and since no value has been returned there wasn't a piece interrupting it before, and it is in a square inside the board
					return true; // the move is valid
			}
		}
		else if ((x_dist != 0 && y_dist == 0) || (x_dist == 0 && y_dist != 0)) { // if it moved in a straight line
			int x_curr, y_curr; // current x and y position of the square being checked (the program will check each square the bishop moves trought)
			for (int t = 1;; t++) { // let t be 0 and increase it each time, it will signify how far we are from the original square (before the move)
				x_curr = move.moved.loc.x + (t * -norm(x_dist)) * ((x_dist != 0) ? 1 : 0); // the current checking position, equal to p + t*l*d, where p is the initial position, t is the distance from the original position, and l is the direction in which the bishop is moving and d is wether the piece moved horizontally
				y_curr = move.moved.loc.y + (t * -norm(y_dist)) * ((y_dist != 0) ? 1 : 0); // "" ... moved vertically
				if (getPiece(x_curr, y_curr).type != NONE) // if there is a piece in the current square 
					return ((x_curr == move.dest.x && y_curr == move.dest.y) && (getPiece(x_curr, y_curr).white_side != move.moved.white_side)); // if the move led the rook to this exact square it is legal, else it isn't since it can't go over that piece
				else if (x_curr == move.dest.x && y_curr == move.dest.y) // else if the current square is the one the rook is moving to, and since no value has been returned there wasn't a piece interrupting it before, and it is in a square inside the board
					return true; // the move is valid
			}
		}
		break;
	case KING:
		if (abs(x_dist) != 1 && abs(y_dist) != 1) // if it hasn't moved either horizontally by 1 or diagonally by 1
			return false;

		if (getPiece(move.dest).type != NONE) // if the square the king is moving to is occupied by a piece
			return (getPiece(move.dest).white_side != move.moved.white_side); // the move is legal only if the piece is the other player's
		else return true; // else if the square the king moves to is empty the move is legal
		break;
	}
	return false;
}

/* returns wether the move is valid */
bool Board::processMove(Move& move) {
	if (!isLegal(move))
		return false;

	board[move.moved.loc.x - 1][move.moved.loc.y - 1].type = NONE;
	setPiece(move.dest, move.moved);
	board[move.moved.loc.x - 1][move.moved.loc.y - 1].loc = move.dest;
	return true;
}

void Board::applyMove(Move& move) {
	board[move.moved.loc.x - 1][move.moved.loc.y - 1].type = NONE;
	setPiece(move.dest, move.moved);
	board[move.moved.loc.x - 1][move.moved.loc.y - 1].loc = move.dest;
}

std::vector<Move> Board::getPossibleMoves(const Piece& piece, bool&& c_legals) const {
	std::vector<Move> moves;
	switch (piece.type) {
	case NONE: break;
	case PAWN:
		int s; s = (piece.white_side) ? 1 : -1;
		moves.emplace_back(Move::Move(piece, piece.loc.x + 1, piece.loc.y + s));
		moves.emplace_back(Move::Move(piece, piece.loc.x - 1, piece.loc.y + s));
		moves.emplace_back(Move::Move(piece, piece.loc.x, piece.loc.y + s));
		moves.emplace_back(Move::Move(piece, piece.loc.x, piece.loc.y + s * 2));
		break;
	case BISHOP:
		bool found;
		for (int t = 0;; t++) {
			found = false;
			if (piece.loc.x + t <= 8 && piece.loc.y + t <= 8) {
				found = true;
				moves.emplace_back(Move::Move(piece, piece.loc.x + t, piece.loc.y + t));
			} if (piece.loc.x - t >= 0 && piece.loc.y + t <= 8) {
				found = true;
				moves.emplace_back(Move::Move(piece, piece.loc.x - t, piece.loc.y + t));
			} if (piece.loc.x + t <= 8 && piece.loc.y - t >= 0) {
				found = true;
				moves.emplace_back(Move::Move(piece, piece.loc.x + t, piece.loc.y - t));
			} if (piece.loc.x - t >= 0 && piece.loc.y - t <= 0) {
				found = true;
				moves.emplace_back(Move::Move(piece, piece.loc.x - t, piece.loc.y - t));
			}
			if (!found)
				break;
		}

		break;
	case KNIGHT:
		moves.emplace_back(Move::Move(piece, piece.loc.x + 2, piece.loc.y + 1));
		moves.emplace_back(Move::Move(piece, piece.loc.x + 2, piece.loc.y - 1));
		moves.emplace_back(Move::Move(piece, piece.loc.x - 2, piece.loc.y + 1));
		moves.emplace_back(Move::Move(piece, piece.loc.x - 2, piece.loc.y - 1));

		moves.emplace_back(Move::Move(piece, piece.loc.x + 1, piece.loc.y + 2));
		moves.emplace_back(Move::Move(piece, piece.loc.x + 1, piece.loc.y - 2));
		moves.emplace_back(Move::Move(piece, piece.loc.x - 1, piece.loc.y + 2));
		moves.emplace_back(Move::Move(piece, piece.loc.x - 1, piece.loc.y - 2));
		break;
	case ROOK:
		for (int t = 1; t <= 8; t++) {
			moves.emplace_back(Move::Move(piece, t, piece.loc.y));
			moves.emplace_back(Move::Move(piece, piece.loc.x, t));
		}
		break;
	case QUEEN:
		// ROOK
		for (int t = 1; t <= 8; t++) {
			moves.emplace_back(Move::Move(piece, t, piece.loc.y));
			moves.emplace_back(Move::Move(piece, piece.loc.x, t));
		}
		// BISHOP
		for (int t = 0;; t++) {
			found = false;
			if (piece.loc.x + t <= 8 && piece.loc.y + t <= 8) {
				found = true;
				moves.emplace_back(Move::Move(piece, piece.loc.x + t, piece.loc.y + t));
			} if (piece.loc.x - t >= 0 && piece.loc.y + t <= 8) {
				found = true;
				moves.emplace_back(Move::Move(piece, piece.loc.x - t, piece.loc.y + t));
			} if (piece.loc.x + t <= 8 && piece.loc.y - t >= 0) {
				found = true;
				moves.emplace_back(Move::Move(piece, piece.loc.x + t, piece.loc.y - t));
			} if (piece.loc.x - t >= 0 && piece.loc.y - t <= 0) {
				found = true;
				moves.emplace_back(Move::Move(piece, piece.loc.x - t, piece.loc.y - t));
			}
			if (!found)
				break;
		}

		break;
	case KING:
		for (int y = -1; y <= 1; y++)
			for (int x = -1; x <= 1; x++)
				moves.emplace_back(Move::Move(piece, piece.loc.x + x, piece.loc.y + y));
		break;
	}
	if (c_legals) {
		bool removed;
		while (true) {
			removed = false;
			for (unsigned int i = 0; i < moves.size(); i++) {
				if (!isLegal(moves[i])) {
					removed = true;
					moves.erase(moves.begin() + i);
					break;
				}
			}
			if (!removed)
				break;
		}
	}
	return moves;
}

std::vector<Move> Board::getAllPossibleMoves(const bool& side) const {
	std::vector<Move> moves;
	for (int y = 1; y <= 8; y++) {
		for (int x = 1; x <= 8; x++) {
			if (getPiece(x, y).type == NONE || getPiece(x, y).white_side != side)
				continue;

			for (Move& m : getPossibleMoves(getPiece(x, y)))
				moves.emplace_back(m);
		}
	}
	return moves;
}

bool Board::isThreatened(unsigned int& x, unsigned int& y, const bool& side) const {
	std::vector<Move> moves = getAllPossibleMoves(side);
	for (Move& m : moves)
		if (m.dest.x == x && m.dest.y == y)
			return true;
	return false;
}

bool Board::isThreatened(Square& square, const bool& side) const {
	std::vector<Move> moves = getAllPossibleMoves(side);
	for (Move& m : moves)
		if (m.dest == square)
			return true;
	return false;
}

bool Board::isThreatened(Square& square, const bool& side, Piece& ignored) const {
	std::vector<Move> moves = getAllPossibleMoves(side);
	for (Move& m : moves)
		if (m.dest == square)
			if (m.moved != ignored)
				return true;
	return false;
}

bool Board::capturesPiece(const Move& move) const {
	return getPiece(move.dest).type != NONE && getPiece(move.dest).white_side != move.moved.white_side;
}

void Board::reset() {
	setPiece(Square::Square(1, 1), Piece::Piece(ROOK, true));
	setPiece(Square::Square(8, 1), Piece::Piece(ROOK, true));
	setPiece(Square::Square(1, 8), Piece::Piece(ROOK, false));
	setPiece(Square::Square(8, 8), Piece::Piece(ROOK, false));

	setPiece(Square::Square(3, 1), Piece::Piece(BISHOP, true));
	setPiece(Square::Square(6, 1), Piece::Piece(BISHOP, true));
	setPiece(Square::Square(3, 8), Piece::Piece(BISHOP, false));
	setPiece(Square::Square(6, 8), Piece::Piece(BISHOP, false));

	setPiece(Square::Square(2, 1), Piece::Piece(KNIGHT, true));
	setPiece(Square::Square(7, 1), Piece::Piece(KNIGHT, true));
	setPiece(Square::Square(2, 8), Piece::Piece(KNIGHT, false));
	setPiece(Square::Square(7, 8), Piece::Piece(KNIGHT, false));

	setPiece(Square::Square(5, 1), Piece::Piece(KING, true));
	setPiece(Square::Square(5, 8), Piece::Piece(KING, false));

	setPiece(Square::Square(4, 1), Piece::Piece(QUEEN, true));
	setPiece(Square::Square(4, 8), Piece::Piece(QUEEN, false));

	for (int xp = 1; xp <= 8; xp++) {
		setPiece(Square::Square(xp, 2), Piece::Piece(PAWN, true));
		setPiece(Square::Square(xp, 7), Piece::Piece(PAWN, false));
	}

	for (int yp = 3; yp <= 6; yp++) {
		for (int xp = 1; xp <= 8; xp++) {
			setPiece(Square::Square(xp, yp), Piece::Piece(NONE, true));
		}
	}
}

void Board::castle(bool& side, bool&& kside) {
	int y = (side) ? 1 : 8;
	Move kmove(getPiece(5, y), (kside) ? 7 : 3, (side) ? 1 : 8); // king move
	Move rmove(getPiece((kside) ? 8 : 1, y), (kside) ? 6 : 4, y);
	applyMove(kmove);
	applyMove(rmove);
}

bool Board::canCastle(bool& side, bool& kside) const {
	unsigned int ky = side ? 1 : 8; // king y
	if (getPiece(5, ky).type != KING || getPiece(kside ? 8 : 1, ky).type != ROOK) // if the two pieces aren't in the right position (rook and king)
		if (getPiece(5, ky).white_side != side || getPiece(kside ? 8 : 1, ky).white_side != side) // or if they are but they are of different sides
			return false; // then there can't be a castling
	int dir = (kside) ? 1 : -1;
	for (unsigned int dx = 5; dx >= 1 && dx <= 8; dx += dir) // for every square from the king to the rook
		if (getPiece(dx, ky).type != NONE || isThreatened(dx, ky, !side)) // if there is a piece or the square is being threatened by an enemy piece
			return false; // the king can't castle
	return isChecked(side);
}

bool Board::isChecked(const bool& side) const {
	for (Move& m : getAllPossibleMoves(!side))
		if (getPiece(m.dest).type == KING)
			return true;
	return false;
}

// no need to flip or do weird stuff
Chess::Piece Board::getPiece(const Square& location) const {
	return board[location.x - 1][location.y - 1];
}

// again, no need to flip or do weird stuff
void Board::setPiece(const Square& location, const Piece& piece) {
	board[location.x - 1][location.y - 1] = piece;
	board[location.x - 1][location.y - 1].loc = location;
}

Piece Board::getPiece(const unsigned int& x, const unsigned int& y) const {
	return board[x - 1][y - 1];
}

std::string Board::asString() {
	std::string result;
	for (int y = 7; y >= 0; y--) {
		for (int x = 0; x < 8; x++) {
			result.append(board[x][y].getType() + " ");
		}
		result += "\n";
	}
	return result;
}