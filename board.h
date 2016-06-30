#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <stdexcept>

#define SQUARES_X 10
#define SQUARES_Y 10

#define NEXT_PLAYER(A) ((A == Player::ONE) ? (Player::TWO) : (Player::ONE))

/// @brief Enum for players
enum class Player : uint8_t {
	/// @brief Player "none," for unclaimed squares
	NONE,
	/// @brief Player one - the human
	ONE,
	/// @brief Player two - the computer
	TWO
};

/// @brief Orientation of a line
enum class Orientation : uint8_t {
	/// @brief Horizontal lines
	HORIZONAL,
	/// @brief Vertical lines
	VERTICAL
};

/// @brief Holds a move
struct Move {
	/// @brief The orientation of that move - a horizontal line or a vertical line
	Orientation orientation;
	/// @brief The x-coord of the line
	int x;
	/// @brief The y-coord of the line
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

		/// @brief The player whose turn it is
		Player currentPlayer = Player::ONE;

		/// @brief Stores number of turns
		unsigned int turnNumber = 0;

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
		Player squareState(int x, int y) const;

		/// @brief Whether or not a specific horizontal line is drawn
		///
		/// @param x X-coord
		/// @param y Y-coord
		///
		/// @return True if has been drawn, false if not
		bool horLineState(int x, int y) const;

		/// @brief Whether or not a specific vertical line is drawn
		///
		/// @param x X-coord
		/// @param y Y-coord
		///
		/// @return True if has been drawn, false if not
		bool verLineState(int x, int y) const;

		/// @brief Calculate score
		///
		/// @param player Player being queried
		///
		/// @return Score of player ply
		int score(Player player) const;

		/// @brief Make a move
		///
		/// @param move Struct containing the move
		///
		/// @return New Board in state of new move
		Board moveResult(Move move) const;

		/// @brief Expose the current player without allowing its modification
		///
		/// @return The current player
		inline Player CurrentPlayer() const {
			return currentPlayer;
		}

		/// @brief Expose the current turn number
		///
		/// @return The current number of turns
		inline unsigned int turns() const {
			return turnNumber;
		}

		/// @brief Test if a supplied move is legal in this position
		///
		/// @param move The move to be tested
		///
		/// @return True if legal, false if not
		bool legalMove(Move move) const;

};



#endif
