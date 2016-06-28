#include "squareapp.h"
#include <iostream>

/// @brief Constructor - currently only sets running to true
SquareApp::SquareApp() {
	running = true;
}

/// @brief Execute the program, start to finish
///
/// @return Exit status
int SquareApp::OnExecute() {
	if (OnInit() == false)
		return -1;

	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event))
			OnEvent(&event);
		OnLoop();
		OnRender();
		SDL_Delay(200);
	}

	OnCleanup();

	return 0;
}

/// @brief Initialise window
///
/// @return success or fail
bool SquareApp::OnInit() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Error in initialising! SDL error " << SDL_GetError() << std::endl;
		return false;
	}
	window = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "Window could not be created! SDL error " << SDL_GetError() << std::endl;
		return false;
	}
	screensurface = SDL_GetWindowSurface(window);
	if ((imagesurface = SDL_LoadBMP("test.bmp")) == NULL) {
		std::cout << "Image could not be loaded! SDL error " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

void SquareApp::OnLoop() {

}

/// @brief Renderer
void SquareApp::OnRender() {
	SDL_FillRect(screensurface, NULL, SDL_MapRGB(screensurface->format, redval, greenval, blueval));
	SDL_BlitSurface(imagesurface, NULL, screensurface, NULL);
	SDL_UpdateWindowSurface(window);
}

/// @brief Close window etc
void SquareApp::OnCleanup() {
	SDL_FreeSurface(imagesurface);
	imagesurface = NULL;
	SDL_DestroyWindow(window);
	SDL_Quit();
}

/// @brief Handle events
///
/// @param Event Pointer to event
void SquareApp::OnEvent(SDL_Event* Event) {
	switch (Event->type) {
		case SDL_QUIT:
			std::cout << "Quitting!" << std::endl;
			running = false;
			break;
		case 1024:
			break;
		case SDL_KEYDOWN:
			KeyPress(Event->key.keysym);
			break;
		default:
			std::cout << Event->type << std::endl;
			break;
	}
}

void SquareApp::KeyPress(SDL_Keysym keyp) {
	switch (keyp.sym) {
		case SDLK_b:
			if (keyp.mod & KMOD_SHIFT)
				blueval = 0xFF;
			else
				blueval = 0;
			break;
		case SDLK_r:
			if (keyp.mod & KMOD_SHIFT)
				redval = 0xFF;
			else
				redval = 0;
			break;
		case SDLK_g:
			if (keyp.mod & KMOD_SHIFT)
				greenval = 0xFF;
			else
				greenval = 0;
			break;
		case SDLK_q:
			running = false;
			break;
		default:
			break;
	}
}
