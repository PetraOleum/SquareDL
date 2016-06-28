#ifndef SQUAREAPP_H
#define SQUAREAPP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <random>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

/// @brief Application class
class SquareApp {
	private:
		/// @brief True if running - if set to false, will quit at end of next loop
		bool running;

		/// @brief Random number generator
		std::default_random_engine generator;

		/// @brief Pointer to random number distribution
		std::uniform_int_distribution<int>* distribution = NULL;

		/// @brief pointer to the window, set initially to NULL
		SDL_Window* window = NULL;

		/// @brief pointer to the surface, set initially to NULL
		SDL_Surface* screensurface = NULL;

		/// @brief Holds an image
		SDL_Surface* imagesurface = NULL;

		/// @brief For when a key is pressed down
		///
		/// @param keyp representation of the key
		void KeyPress(SDL_Keysym keyp);

		/// @brief Blue part of background colour
		int blueval = 0xFF;

		/// @brief Red part of background colour
		int redval = 0xFF;

		/// @brief Green part of background colour
		int greenval = 0xFF;

		/// @brief Position of image
		SDL_Rect posrect;

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
