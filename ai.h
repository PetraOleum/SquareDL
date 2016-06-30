#ifndef AI_H
#define AI_H

#include "board.h"

/// @brief An "analysed move", holding the move and also the scores
struct AnalysedMove {
	/// @brief The move that the score corresponds to
	Move move;
	/// @brief The resulting score of the player making the move
	int curPlayerScore;
	/// @brief The resulting score of the other player
	int otherPlayerScore;
	/// @brief Does the player gain something immediately or does the player switch
	bool immediateGain;
};

/// @brief Calculate the best move at a position (recursive)
///
/// @param position The position, as a constant Board
/// @param depth The depth to search
///
/// @return The move that is judged to be "best"
AnalysedMove calculateBestMove(const Board& position, int depth);



#endif
