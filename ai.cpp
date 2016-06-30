#include "ai.h"
#include <vector>
#include <utility>
#include <random>

AnalysedMove calculateBestMove(const Board& position, int depth) {
	std::vector<AnalysedMove> candidates;
	std::vector<Move> legals;
	Player myPlayer = position.CurrentPlayer();
	Player otherPlayer = NEXT_PLAYER(myPlayer);
	int myScore = position.score(myPlayer);
	int otherScore = position.score(otherPlayer);
	for (int x = 0; x <= SQUARES_X; x++)
		for (int y = 0; y <= SQUARES_Y; y++) {
			Move mv = {Orientation::VERTICAL, x, y};
			if (position.legalMove(mv))
				legals.push_back(mv);
			mv = {Orientation::HORIZONAL, x, y};
			if (position.legalMove(mv))
				legals.push_back(mv);
		}
	if (legals.empty())
		return {{Orientation::VERTICAL, -1, -1}, myScore, otherScore, false};
	bool isMoveThatClaims = false;
	for (Move mv : legals) {
		Board npos = position.moveResult(mv);
		int nms = npos.score(myPlayer);
		AnalysedMove amv;
		if (depth > 0) {
			if (nms == myScore) {
				if (isMoveThatClaims)
					continue;
				amv = calculateBestMove(npos, depth - 1);
				int ts = amv.otherPlayerScore;
				amv.otherPlayerScore = amv.curPlayerScore;
				amv.curPlayerScore = ts;
			}
			else {
				amv = calculateBestMove(npos, depth);
				isMoveThatClaims = true;
			}
			amv.move = mv;
		}
		else {
			amv = {mv, nms, npos.score(otherPlayer), nms > myScore};
		}
		candidates.push_back(amv);
	}
	int numcand = candidates.size();
	if (numcand == 1)
		return candidates[0];
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> randmove(0, numcand);
	AnalysedMove rmove = candidates[randmove(gen)];
	int pdiff = rmove.curPlayerScore - rmove.otherPlayerScore;
	for (AnalysedMove am : candidates) {
		int odiff = am.curPlayerScore - am.otherPlayerScore;
		if (odiff > pdiff) {
			pdiff = odiff;
			rmove = am;
		}
	}
	return rmove;
}
