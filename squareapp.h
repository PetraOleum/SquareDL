#ifndef SQUAREAPP_H
#define SQUAREAPP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <random>
#include "board.h"

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

#define IM_WIDTH 100
#define IM_HEIGHT 100

#define LOOP_TIME 200

#define LINE_WIDTH 50
#define LINE_HEIGHT 8

/// @brief Application class
class SquareApp {
	private:
		/// @brief True if running - if set to false, will quit at end of next loop
		bool running;

		/// @brief Random number generator
		std::default_random_engine generator;

		/// @brief Pointer to random number distribution
		std::uniform_int_distribution<int> xDistribution;

		std::uniform_int_distribution<int> yDistribution;

		std::uniform_int_distribution<int> horverDistribution;

		/// @brief pointer to the window, set initially to NULL
		SDL_Window* window = NULL;

		/// @brief pointer to the surface, set initially to NULL
		SDL_Surface* screensurface = NULL;

//		SDL_Surface* imagesurface = NULL;

		/// @brief The renderer for the window
		SDL_Renderer* renderer = NULL;

		/// @brief For when a key is pressed down
		///
		/// @param keyp representation of the key
		void KeyPress(SDL_Keysym keyp);

		/// @brief The current board
		Board currentPosition;

		/// @brief Propose a move (computer player)
		///
		/// @return Proposed move
		Move proposeMove();

	public:
		SquareApp();

		int OnExecute();

		bool OnInit();

		void OnEvent(SDL_Event* Event);

		void OnLoop();

		void OnRender();

		void OnCleanup();
};


#endif
