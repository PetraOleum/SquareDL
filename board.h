#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <stdexcept>

#define SQUARES_X 10
#define SQUARES_Y 10

#define NEXT_PLAYER(A) ((A == Player::ONE) ? (Player::TWO) : (Player::ONE))

/// @brief Enum for players
enum class Player : uint8_t {
	NONE,
	ONE,
	TWO
};

/// @brief Orientation of a line
enum class Orientation : uint8_t {
	HORIZONAL,
	VERTICAL
};

struct Move {
	Orientation orientation;
	int x;
	int y;
};

/// @brief Class for the board object
class Board {
	private:
		/// @brief array of horizontalLines
		bool** horizontalLines;

		/// @brief array of verticalLines
		bool** verticalLines;

		/// @brief Who owns a specific square
		Player** squares;

		Player currentPlayer = Player::ONE;

	public:
		/// @brief Basic constructor
		Board();

		/// @brief Copy constructor
		///
		/// @param other The other object
		Board(const Board& other);

		/// @brief Garbage collection
		~Board();

		/// @brief Assignment operator
		///
		/// @param other Other operator
		///
		/// @return Reference to this
		Board& operator=(const Board& other);

		/// @brief Return the owner of a specific square
		///
		/// @param x X-coord
		/// @param y Y-coord
		///
		/// @return enum value of square
		Player squareState(int x, int y);

		/// @brief Whether or not a specific horizontal line is drawn
		///
		/// @param x X-coord
		/// @param y Y-coord
		///
		/// @return True if has been drawn, false if not
		bool horLineState(int x, int y);

		/// @brief Whether or not a specific vertical line is drawn
		///
		/// @param x X-coord
		/// @param y Y-coord
		///
		/// @return True if has been drawn, false if not
		bool verLineState(int x, int y);

		/// @brief Calculate score
		///
		/// @param player Player being queried
		///
		/// @return Score of player ply
		int score(Player player);

		/// @brief Make a move
		///
		/// @param move Struct containing the move
		///
		/// @return New Board in state of new move
		Board moveResult(Move move);

		inline Player CurrentPlayer() {
			return currentPlayer;
		}

};



#endif
