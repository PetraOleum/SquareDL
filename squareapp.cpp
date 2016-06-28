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
	SDL_FillRect(screensurface, NULL, SDL_MapRGB(screensurface->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(window);

//	SDL_Delay(2000);
	return true;
}

void SquareApp::OnLoop() {

}

void SquareApp::OnRender() {

}

/// @brief Close window etc
void SquareApp::OnCleanup() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

/// @brief Handle events
///
/// @param Event Pointer to event
void SquareApp::OnEvent(SDL_Event* Event) {
	std::cout << Event->type << std::endl;
	if (Event->type == SDL_QUIT) {
		std::cout << "Quitting!" << std::endl;
		running = false;
	}
}
