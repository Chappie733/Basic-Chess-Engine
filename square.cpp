#include "square.h"
using namespace Chess;

std::string Square::asCoordinate() const {
	char letter = (char)(96 + x);
	return (std::string(1, letter) + std::to_string(y));
}

std::string Square::asNumericalCoordinate() const {
	return (std::to_string(x) + ", " + std::to_string(y));
}

bool Square::operator==(const Square& other) {
	return (x == other.x && y == other.y);
}

bool Square::operator!=(const Square& other) {
	return (x != other.x || y != other.y);
}

void Square::operator=(const Square& other) {
	x = other.x;
	y = other.y;
}