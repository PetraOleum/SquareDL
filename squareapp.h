#ifndef SQUAREAPP_H
#define SQUAREAPP_H

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class SquareApp {
	private:
		bool running;

		SDL_Window* window = NULL;

		SDL_Surface* screensurface = NULL;

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
