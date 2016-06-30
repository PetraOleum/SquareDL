#include "board.h"
#include <iostream>

Board::Board() {
	horizontalLines = new bool*[SQUARES_X];
	for (int x = 0; x < SQUARES_X; x++) {
		horizontalLines[x] = new bool[SQUARES_Y + 1];
		for (int y = 0; y <= SQUARES_Y; y++)
			horizontalLines[x][y] = false;
	}
	verticalLines = new bool*[SQUARES_X + 1];
	for (int x = 0; x <= SQUARES_X; x++) {
		verticalLines[x] = new bool[SQUARES_Y];
		for (int y = 0; y < SQUARES_Y; y++)
			verticalLines[x][y] = false;
	}
	squares = new Player*[SQUARES_X];
	for (int x = 0; x < SQUARES_X; x++) {
		squares[x] = new Player[SQUARES_Y];
		for (int y = 0; y < SQUARES_Y; y++)
			squares[x][y] = Player::NONE;
	}
}

Board::~Board() {
	for (int x = 0; x < SQUARES_X; x++)
		delete [] horizontalLines[x];
	delete [] horizontalLines;
	for (int x = 0; x <= SQUARES_X; x++)
		delete [] verticalLines[x];
	delete [] verticalLines;
	for (int x = 0; x < SQUARES_X; x++)
		delete [] squares[x];
	delete [] squares;
}

Board::Board(const Board& other) {
	horizontalLines = new bool*[SQUARES_X];
	for (int x = 0; x < SQUARES_X; x++) {
		horizontalLines[x] = new bool[SQUARES_Y + 1];
		for (int y = 0; y <= SQUARES_Y; y++)
			horizontalLines[x][y] = other.horizontalLines[x][y];
	}
	verticalLines = new bool*[SQUARES_X + 1];
	for (int x = 0; x <= SQUARES_X; x++) {
		verticalLines[x] = new bool[SQUARES_Y];
		for (int y = 0; y < SQUARES_Y; y++)
			verticalLines[x][y] = other.verticalLines[x][y];
	}
	squares = new Player*[SQUARES_X];
	for (int x = 0; x < SQUARES_X; x++) {
		squares[x] = new Player[SQUARES_Y];
		for (int y = 0; y < SQUARES_Y; y++) {
			squares[x][y] = other.squares[x][y];
		}
	}
	currentPlayer = other.currentPlayer;
	turnNumber = other.turnNumber;
}

Board& Board::operator=(const Board& other) {
	if (this != &other) {
		for (int x = 0; x < SQUARES_X; x++)
			delete [] horizontalLines[x];
		delete [] horizontalLines;
		for (int x = 0; x <= SQUARES_X; x++)
			delete [] verticalLines[x];
		delete [] verticalLines;
		for (int x = 0; x < SQUARES_X; x++)
			delete [] squares[x];
		delete [] squares;
		horizontalLines = new bool*[SQUARES_X];
		for (int x = 0; x < SQUARES_X; x++) {
			horizontalLines[x] = new bool[SQUARES_Y + 1];
			for (int y = 0; y <= SQUARES_Y; y++)
				horizontalLines[x][y] = other.horizontalLines[x][y];
		}
		verticalLines = new bool*[SQUARES_X + 1];
		for (int x = 0; x <= SQUARES_X; x++) {
			verticalLines[x] = new bool[SQUARES_Y];
			for (int y = 0; y < SQUARES_Y; y++)
				verticalLines[x][y] = other.verticalLines[x][y];
		}
		squares = new Player*[SQUARES_X];
		for (int x = 0; x < SQUARES_X; x++) {
			squares[x] = new Player[SQUARES_Y];
			for (int y = 0; y < SQUARES_Y; y++)
				squares[x][y] = other.squares[x][y];
		}
		currentPlayer = other.currentPlayer;
		turnNumber = other.turnNumber;
	}
	return *this;
}

Player Board::squareState(int x, int y) const {
	if ((x >= SQUARES_X) || (x < 0) || (y >= SQUARES_Y) || (y < 0))
		throw std::out_of_range("Not a valid square");
	return squares[x][y];
}

bool Board::horLineState(int x, int y) const {
	if ((x >= SQUARES_X) || (x < 0) || (y > SQUARES_Y) || (y < 0))
		throw std::out_of_range("Not a valid square");
	return horizontalLines[x][y];
}

bool Board::verLineState(int x, int y) const {
	if ((x > SQUARES_X) || (x < 0) || (y >= SQUARES_Y) || (y < 0))
		throw std::out_of_range("Not a valid square");
	return verticalLines[x][y];
}

int Board::score(Player player) const {
	int sc = 0;
	for (int x = 0; x < SQUARES_X; x++)
		for (int y = 0; y < SQUARES_Y; y++)
			if (squares[x][y] == player)
				sc++;
	return sc;
}

Board Board::moveResult(Move move) const {
	Board nb = *this;
	nb.currentPlayer = NEXT_PLAYER(currentPlayer);
	nb.turnNumber++;
	
	try {
		if (move.x < 0 || move.y < 0)
			throw std::out_of_range("Invalid move");
		if (move.orientation == Orientation::HORIZONAL) {
			if (move.x >= SQUARES_X || move.y > SQUARES_Y)
				throw std::out_of_range("Invalid move");
			if (nb.horizontalLines[move.x][move.y])
				throw std::out_of_range("Already occupied");
			nb.horizontalLines[move.x][move.y] = true;
			if (move.y > 0)
				if (nb.horizontalLines[move.x][move.y - 1] && nb.verticalLines[move.x][move.y - 1] && nb.verticalLines[move.x + 1][move.y - 1]) {
					nb.currentPlayer = currentPlayer;
//					std::cout << "Square Claimed" << std::endl;
					nb.squares[move.x][move.y - 1] = currentPlayer;
				}
			if (move.y < SQUARES_Y)
				if (nb.horizontalLines[move.x][move.y + 1] && nb.verticalLines[move.x][move.y] && nb.verticalLines[move.x + 1][move.y]) {
					nb.currentPlayer = currentPlayer;
//					std::cout << "Square Claimed" << std::endl;
					nb.squares[move.x][move.y] = currentPlayer;
				}
		} else {
			if (move.x > SQUARES_X || move.y >= SQUARES_Y)
				throw std::out_of_range("Invalid move");
			if (nb.verticalLines[move.x][move.y])
				throw std::out_of_range("Already occupied");
			nb.verticalLines[move.x][move.y] = true;
			if (move.x > 0)
				if (nb.verticalLines[move.x - 1][move.y] && nb.horizontalLines[move.x - 1][move.y] && nb.horizontalLines[move.x - 1][move.y + 1]) {
					nb.currentPlayer = currentPlayer;
					nb.squares[move.x - 1][move.y] = currentPlayer;
//					std::cout << "Square Claimed" << std::endl;
				}
			if (move.x < SQUARES_X)
				if (nb.verticalLines[move.x + 1][move.y] && nb.horizontalLines[move.x][move.y] && nb.horizontalLines[move.x][move.y + 1]) {
					nb.currentPlayer = currentPlayer;
//					std::cout << "Square Claimed" << std::endl;
					nb.squares[move.x][move.y] = currentPlayer;
				}
		}

	} catch (std::out_of_range e) {
//		std::cout << e.what() << std::endl;
		return *this;
	}
//	std::cout << (int)nb.currentPlayer - (int)currentPlayer << std::endl;
	return nb;
}

bool Board::legalMove(Move move) const {
	// Test if the move is actually in range
	if (move.x < 0 || move.y < 0)
		return false;
	if (move.x > SQUARES_X || move.y > SQUARES_Y)
		return false;
	if (move.orientation != Orientation::VERTICAL && move.x == SQUARES_X)
		return false;
	if (move.orientation != Orientation::HORIZONAL && move.y == SQUARES_Y)
		return false;

	// Test if the orientation is valid
	if (move.orientation != Orientation::VERTICAL && move.orientation != Orientation::HORIZONAL)
		return false;

	// Test if that move has already been made
	if (move.orientation == Orientation::HORIZONAL)
		if (horizontalLines[move.x][move.y])
			return false;
	if (move.orientation == Orientation::VERTICAL)
		if (verticalLines[move.x][move.y])
			return false;

	// If reached this far, must be legal
	return true;
	
}
