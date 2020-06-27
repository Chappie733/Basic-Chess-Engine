#ifndef SQUARE_H
#define SQUARE_H

#include <string>
#include <iostream>

namespace Chess {

	struct Square {
		unsigned int x, y;

		Square() = default;
		Square(const unsigned int& xt, const unsigned int& yt) : x(xt), y(yt) {}

		std::string asCoordinate() const;

		std::string asNumericalCoordinate() const;

		bool operator==(const Square& other);

		bool operator!=(const Square& other);

		void operator=(const Square& other);
	};


}

#endif