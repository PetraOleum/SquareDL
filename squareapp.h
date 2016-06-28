#ifndef SQUAREAPP_H
#define SQUAREAPP_H

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

/// @brief Application class
class SquareApp {
	private:
		/// @brief True if running - if set to false, will quit at end of next loop
		bool running;

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

		int blueval = 0xFF;
		int redval = 0xFF;
		int greenval = 0xFF;

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
